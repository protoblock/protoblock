//
//  ExchangeData.h
//
//  Created by Jay Berg on 11/28/15.
//
//
#ifndef __fantasybit__ExchangeData__
#define __fantasybit__ExchangeData__

#include <QObject>

#include "ProtoData.pb.h"
#include "StatusData.pb.h"
#include <unordered_set>
#include <unordered_map>
#include <mutex>
#include <leveldb/db.h>
#include <utility>
#include <memory>
#include "FantasyName.h"
#include "ExData.pb.h"
#include <list>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstrea
#include <set>
#include <leveldb/comparator.h>
#include "pbutils.h"
#include "PeerNode.h"

#ifdef TIMEAGENTWRITEFILLS
#include "SqlStuff.h"
#endif

using namespace std;

namespace fantasybit
{
struct OpenOrder {
    string symbol;
    OrderCore livecore;
};

struct Position {
    int netqty;
    int netprice;
    int openpnl;
    string ToString() const {
        return to_string(netqty) + " " + to_string(netprice);
    }
};

struct FullPosition {
    string symbol;
    string fname;
    Position pos;
};

struct FullOrderDelta {
    string fname;
    Order openorder;
    string symbol;
};


typedef std::unordered_map<std::string,std::pair<Position,std::vector<Order> > > ordsnap_t;


struct InsideBook {
    InsideBook() : totSize(0) {}
    list<Order> mOList;
    std::unordered_map<int32_t,list<Order>::iterator> mOMap;
    int32_t totSize;

    list<Order>::iterator top() {
        return begin(mOList);
    }

    list<Order>::iterator bot() {
        return end(mOList);
    }

    void New(Order& order) {
        totSize += order.core().size();
        mOMap.insert(make_pair(order.refnum(),mOList.insert(mOList.end(),order)));
        return;
    }

    void Cancel(Order &order) {
        auto it = mOMap.find(order.refnum());
        if ( it == end(mOMap) )
            return;

        if ( it->second == end(mOList))
            return;


        auto ret = it->second->core().size();
        totSize -= ret;

        mOList.erase(it->second);
        mOMap.erase(it);

        return;
    }

    bool Fill(int32_t qty,list<Order>::iterator &iter) {
        totSize -= qty;

        if ( iter->core().size() == 0)
        {
            mOMap.erase(iter->refnum());
            iter = mOList.erase(iter);
        }

        return true;
    }

    bool Fill(int32_t qty,list<Order>::reverse_iterator &iter) {
#ifdef TRACE
        qDebug() << "level2 qty " << qty << iter->core().size() << totSize;
#endif
        totSize -= qty;
        if (iter->core().size() == 0) {
            mOMap.erase(iter->refnum());

            list<Order>::iterator tempIter = mOList.erase( --iter.base());
            iter = list<Order>::reverse_iterator(tempIter);
        }

        return true;
    }

    void Remove(Order &order) {
        auto it = mOMap.find(order.refnum());
        if ( it == end(mOMap) )
            return;

        if ( it->second == end(mOList))
            return;

        auto sz = order.core().size();
        totSize -= sz;

        auto newsz = it->second->core().size() - sz;
        if ( newsz > 0 )
            it->second->mutable_core()->set_size(newsz);
        else {
            mOList.erase(it->second);
            mOMap.erase(it);
        }
    }
};

/*
struct Level1 {
   Level1() {
       bidsize = bid = ask = asksize = last = lastsize = 0;
   }

   int32_t bidsize;
   int bid;
   int ask;
   int32_t asksize;
   int last;
   int32_t lastsize;
   std::string ToString() {
       std::stringstream ss;
       ss << "Level1 "
          << bidsize << " "
          << bid << " "
          << ask << " "
          << asksize << " "
          << last << " "
          << lastsize;

        return ss.str();
   }

};
*/
//#define BOOK_SIZE 40
class LimitBook {
    vector<InsideBook> mBids, mAsks;//[BOOK_SIZE], mAsks[BOOK_SIZE];
    int mBb, mBa;
    bool NewBid(Order &order, Position &);
    bool NewAsk(Order &order, Position &);
    void NewTop(int price, int32_t qty, bool isbuy);
    void NewNew(Order &order);

    void NewDepth(bool isbuy,int price);
    int32_t SweepAsks( Order &order);
    int32_t SweepBids( Order &order);
    void GetTop(bool isbuy) {
        if (isbuy) {
            for (; mBb >= 0 && mBids[mBb].totSize == 0; --mBb) ;
            if (mBb < 0)
                NewTop(0,0,isbuy);
            else
                NewTop(mBb+1, mBids[mBb].totSize, isbuy);
        }
        else {
            for (; mBa < BOOK_SIZE && mAsks[mBa].totSize == 0; ++mBa) ;
            if (mBa >= BOOK_SIZE)
                NewTop(0,0,isbuy);
            else
                NewTop(mBa+1, mAsks[mBa].totSize, isbuy);
        }
    }

    void SaveRemove(Order &o,int32_t fillqty);
    void SaveNew(const OrderCore &oc);
    void SendFill(Order &o, int32_t q, int price, bool );

public:
    LimitBook(const string &symbol, int book_size = 40) :
            BOOK_SIZE(book_size),
            mSymbol(symbol),
            mBids(book_size),
            mAsks(book_size) {
        mBb = -1;
        mBa = BOOK_SIZE;
    }

    int BOOK_SIZE;
    string mSymbol;

    InsideBook *getInside(bool bid, int32_t price) {
        return bid ? &mBids[price-1] : &mAsks[price-1];
    }

    void updateTopfromCache(int b, int a) {
        if ( b > 0 && b <= BOOK_SIZE)
            mBb = b-1;
        else
            mBb = -1;
        if ( a > 0 && a <= BOOK_SIZE)
            mBa = a-1;
        else
            mBa = BOOK_SIZE;
    }

    bool NewOrder(Order &eo, Position &);

    int32_t CancelOrder(Order &order);
};

struct MatchingEngine {
    MatchingEngine(const string &symbol,bool il = false)
        : mSymbol(symbol), islocked(il), blocknum(0)
    {

    }
    unique_ptr<LimitBook> mLimitBook;
    bool islocked;
    std::unordered_map<std::string,Position> mPkPos;
    string mSymbol;
    int32_t blocknum;
    MarketSnapshot *makeSnapshot(MarketSnapshot *) ;
    void ResetLimitBook(int booksize = 40) {
        mLimitBook.reset(new LimitBook(mSymbol,booksize));
    }
};


class ExchangeData : public QObject {

    Q_OBJECT

    std::shared_ptr<leveldb::DB> settlestore;
    std::shared_ptr<leveldb::DB> bookdeltastore;
    std::shared_ptr<leveldb::DB> orderseqstore;
    std::shared_ptr<leveldb::DB> posstore;

    /*
    struct cmaps {
        std::unordered_map<string,std::unordered_map<std::string,Position>> mPositions;
        std::unordered_map<string,unique_ptr<MatchingEngine>> mLimitBooks;
        std::unordered_map<string,ContractOHLC> mContractOHLC;
        std::unordered_map<string,MarketQuote> mMarketQuote;
        std::unordered_map<int32_t,OpenOrder> mOpenOrders;
        std::unordered_map<std::string,std::set<int32_t>> mNameSeqMap;
        std::unordered_map<int32_t,std::string> mSeqNameMap;
    };

    cmaps mWk;
    cmaps mRow;
    cmaps mS;
    */

    std::unordered_map<string,std::unordered_map<std::string,Position>> mPositions;
    std::map<string,unique_ptr<MatchingEngine>> mLimitBooks;
    std::unordered_map<string,ContractOHLC> mContractOHLC;
    std::unordered_map<string,MarketQuote> mMarketQuote;
    std::unordered_map<int32_t,OpenOrder> mOpenOrders;
    std::unordered_map<std::string,std::set<int32_t>> mNameSeqMap;
    std::unordered_map<int32_t,std::string> mSeqNameMap;
    std::unordered_map<string,int> mTotOpenPnl;


    std::set<std::string> mLockedSymb;
    std::set<std::string> mSubscribed;

    leveldb::WriteOptions write_sync{};
    //void ProcessResultOver(const string &,int32_t);
    //void ProcessResult(const SettlePos &spos,std::unordered_map<string,int32_t>::const_iterator result);

    void StoreOhlc(string mSymbol);
    void ProcessBookDelta(const BookDelta &bd);



    void UpdateOpenPnl(MatchingEngine &ma);
public:
    ExchangeData() : mMaxSeason(2017), mMinSeason(2017)
#if defined(TIMEAGENTWRITEFILLS)
      , mSql{"protoblock","timeagentwrite"}
#endif
    {

    }

#if defined(TIMEAGENTWRITEFILLS)
    fantasybit::SqlStuff mSql;
#endif

    int64_t MAXSEQ;
    void init(const fantasybit::GlobalState &);
    void closeAll(bool saverow = false);

    void removeAll() {
        settlestore.reset();
        bookdeltastore.reset();
        posstore.reset();
        pb::remove_all(GET_ROOT_DIR() + "trade/");
    }

    bool amlive = false;

    std::unordered_set<string> fnames;
    void OnTradeSessionStart(int season, int week);

    void OnNewPosition(const string &,const Position &, const string &);

    void OnDeltaOpenOrder(const string &fname, const OpenOrder &oo,int32_t seqnum);
    void OnCancelOpenOrder(const OpenOrder &oo,int32_t seqnum);


    void Subscribe(std::string in) {
#ifdef TRACE
        qDebug() << "level2 ExchangeData Subscribe" << in.data();
#endif

        mSubscribed.insert(in);
    }

    void UnSubscribe(std::string in) {
#ifdef TRACE
        qDebug() << "level2 ExchangeData UnSubscribe" << in.data();
#endif

        mSubscribed.erase(in);
    }

    void setOhlc(ContractOHLC &myphlc, const MarketTicker *mt) {
        if ( !myphlc.has_high() || mt->price() > myphlc.high())
            myphlc.set_high(mt->price());
        if ( !myphlc.has_low() || myphlc.low() <= 0 || mt->price() < myphlc.low())
            myphlc.set_low(mt->price());
        if ( !myphlc.has_open() || myphlc.open() == 0)
            myphlc.set_open(mt->price());
        myphlc.set_close(mt->price());
        myphlc.set_volume(myphlc.volume()+mt->size());
        myphlc.set_change(myphlc.close() - myphlc.open());

    }

    bool GetGameSettlePos(const string &gid,GameSettlePos &gsp);
    void GetRemainingSettlePos(std::unordered_map<string,BookPos>&);

    void OnNewOrderMsg(const ExchangeOrder&, int32_t seqnum,
                       shared_ptr<fantasybit::FantasyName> fn,
                       int32_t blocknum,
                       const FutContract *,
                       const string &symbol);

    void OnCancelOrderMsg(const ExchangeOrder&, int32_t seqnum,
                       shared_ptr<fantasybit::FantasyName> fn,
                       int32_t blocknum);

    void OnOrderNew(const ExchangeOrder&, int32_t seqnum,
                    shared_ptr<fantasybit::FantasyName> fn);

    void OnOrderCancel(const ExchangeOrder&, int32_t seqnum,
                       shared_ptr<fantasybit::FantasyName> fn);

    //void OnOrderReplace(const ExchangeOrder&, const string &uid) {}

    Position getPosition(const string &pk,const string &mSymbol);
    void OnDeltaPos(const string &sym, int32_t seqnum, int32_t pqty, int price);

    void SaveBookDelta();
    void OnGameResult(const GameResult&gs);

    void OnGameStart(const std::string &gid,
                  const std::unordered_map<std::string,PlayerDetail> &home,
                  const std::unordered_map<std::string,PlayerDetail> &away
                     );
    void lockSymbols(const std::string &gid,
                  const std::unordered_map<std::string,PlayerDetail> &home,
                  const std::unordered_map<std::string,PlayerDetail> &away
                     );

    void clearNewWeek(bool = false);

    void OnSeasonStart(int season) {
        mSeason = season;
        mMinSeason = season;
        if ( mMaxSeason < mMinSeason)
            mMaxSeason = mMinSeason;
    }

    void OnSeasonEnd(int season) {
        mMinSeason = season+1;
        if ( mMaxSeason < mMinSeason)
            mMaxSeason = mMinSeason;
    }

    int mWeek;
    int mMinSeason, mMaxSeason;
    int mSeason;

    GlobalState mGlobalState;

    void OnWeekOver(int week);
    void OnWeekStart(int week) {
        mWeek = week;
//        if (amlive)
//            doEmitSnap();
        auto *it = posstore->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            auto str = it->key().ToString();
            int ii =  str.find_first_of(':');
            auto fname = str.substr(0, ii);
            auto tickersymbol = str.substr(ii + 1);

            StorePos sp;
            if ( !sp.ParseFromString(it->value().ToString()) )
                continue;

#ifdef TRACE
            qDebug() << "OnWeekStart level2 ExchangeData posstore " << str.data() << sp.DebugString().data() << " |";
#endif
            auto &plist = mPositions[fname];
            Position &pos = plist[tickersymbol];
            pos.netprice = sp.price();
            pos.netqty = sp.qty();

            auto it3 = mLimitBooks.find(tickersymbol);
            if ( it3 == end(mLimitBooks) ) {

                auto it2 = mLimitBooks.insert(make_pair(tickersymbol,
                               unique_ptr<MatchingEngine>(new MatchingEngine(tickersymbol,false))));

                if ( !it2.second ) {
                    qWarning() << "level2 unable to insert for" << tickersymbol.data();
                    continue;
                }

#ifdef TRACE
                qDebug() << "level2 ExchangeData new for pos" << tickersymbol.data();
#endif

                it2.first->second->ResetLimitBook(fantasybit::isWeekly(tickersymbol) ? 40 : 400);//mLimitBook.reset(new LimitBook());

                it3 = it2.first;
            }
            //ToDO already have this from settlepos?
            it3->second->mPkPos.insert(make_pair(fname,pos));
        }
        delete it;

        //update total pnl
        for ( auto &it : mLimitBooks) {
            UpdateOpenPnl(*(it.second));
        }
    }

    std::string filedir(const std::string &in) {
        return GET_ROOT_DIR() + "trade/" + in;
    }


    //void MergeMarketQuote(const string &playerid,const MarketQuote & );
    void OnTrade(const string &symbol, fantasybit::TradeTic *tt);
    void OnMarketTicker(const string &symbol, fantasybit::MarketTicker &mt);
    void OnNewDepth(const string &symbol, fantasybit::DepthFeedDelta *df) {
#ifndef FORCE_LIVE
    if ( amlive )
#endif
        {
            df->set_symbol(symbol);
            emit NewDepthDelta(df);
        }
    }


    std::vector<MarketSnapshot> GetCurrentMarketSnaps() {
        std::lock_guard<std::recursive_mutex> lockg{ ex_mutex };

        qDebug() << "get GetCurrentMarketSnaps" ;
        std::vector<MarketSnapshot> vms{};

        for (auto &it : mLimitBooks) {
            #ifdef TRACE
                qDebug() << "level2 ExchangeData GetCurrentMarketSnaps" << it.first.data();
            #endif
            MarketSnapshot ms;
            ms.set_week(mWeek);
            ms.mutable_quote()->CopyFrom(mMarketQuote[it.first]);
            ms.mutable_ohlc()->CopyFrom(mContractOHLC[it.first]);
            ms.set_symbol(it.first);
            it.second->makeSnapshot(&ms);
            vms.emplace_back(ms);
        }

        return vms;
    }

    /*
    void OnNewOrder(Order &neworder) {
        if ( !amlive )  return;
#ifdef TRACE
            qDebug() << "level2 ExchangeData OnNewOrder emit NewFantasyNameOrder";
#endif

        emit NewFantasyNameOrder(neworder);
    }
    */

    std::unordered_map<std::string,Position> GetPositionsByName(const std::string &fname, bool subscribe = true) {
        std::lock_guard<std::recursive_mutex> lockg{ ex_mutex };
        mSubscribed.insert(fname);
        return mPositions[fname];
    }

    std::unordered_map<std::string,Position> GetOrdersByName(const std::string &fname) {
        std::lock_guard<std::recursive_mutex> lockg{ ex_mutex };
        return mPositions[fname];
    }

    ordsnap_t  GetOrdersPositionsByName(const std::string &fname);
    std::recursive_mutex ex_mutex{};

    void processGameResult(const GameResult&gs, const std::string &symbolsuffix) {
        std::lock_guard<std::recursive_mutex> lockg{ ex_mutex };
        for (auto ha : { QString("home"),QString("away")})
        for (auto &pr : (ha == QString("home")) ? gs.home_result() : gs.away_result()) {
            std::string tradesymbol = pr.symbol() + symbolsuffix; //CKQB + 17s
            for ( auto &rowdiv : pr.rowposdividend() ) {
                // pr.result; // the dividend
                // rowdiv.name; //fantasyname
                Position &mypos = mPositions[rowdiv.name()][tradesymbol];

                int divamount = mypos.netqty * pr.result();
                if ( mypos.netqty > 0 ) {
                    if ( divamount + mypos.netprice > 0 ) {
                        if ( mypos.netprice < 0 )
                            mypos.netprice = 0;
                    }
                    else  mypos.netprice += divamount;

                }
                else if ( mypos.netqty < 0 ) {
                    if ( divamount + mypos.netprice < 0 ) {
                        if ( mypos.netprice > 0 )
                            mypos.netprice = 0;
                    }
                    else  mypos.netprice += divamount;
                }
                OnNewPosition(rowdiv.name(),mypos,tradesymbol);
            }
        }
    }

    void addBootStrap(Bootstrap *in) {
        LdbWriter ldb;
        ldb.init(Node::bootstrap.get());
        Bootstrap &bs = *in;

        bs.set_posmetaroot(BootStrapRowPositions(ldb));

        return;
    }

    std::string BootStrapRowPositions(LdbWriter &ldb) {
        auto *it = posstore->NewIterator(leveldb::ReadOptions());
        PosMeta pm;
        MerkleTree tree;

        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            StorePos sp;
            auto str = it->key().ToString();
            int ii =  str.find_first_of(':');
            auto fname = str.substr(0, ii);
            auto tickersymbol = str.substr(ii + 1);

            sp.ParseFromArray(it->value().data(),it->value().size());
            pm.set_name(fname);
            pm.set_playerid(tickersymbol);
            pm.set_qty(sp.qty());
            pm.set_price(sp.price());
            tree.add_leaves(ldb.write(pm));
        }
        delete it;

        tree.set_root(pb::makeMerkleRoot(tree.leaves()));
        return ldb.write(tree.root(),tree.SerializeAsString());
    }

    int GetOpenPnl(const std::string &fname);
signals:
    void NewMarketTicker(fantasybit::MarketTicker,int32_t);
    void NewMarketSnapShot(fantasybit::MarketSnapshot*);
    void FinishMarketSnapShot(int);
    void StartMarketSnapShot(int);
    void NewDepthDelta(fantasybit::DepthFeedDelta*);
    void NewTradeTic(fantasybit::TradeTic *);
    void NewFantasyNameOrder(fantasybit::Order&);
    void NewPos(fantasybit::FullPosition);
    void NewOO(fantasybit::FullOrderDelta);

public slots:
    void OnLive(bool subscribe);
};

class ExchangeDataHolder {
    ExchangeData *instance;
public:
    void set(ExchangeData *that) {
        instance = that;
    }

    ExchangeData *get() {
        return instance;
    }
};

class BookDeltaMediator : public BookDelta {
public:
    void OnFill(MarketTicker *mt) {
        auto &myphlc = *mutable_ohlc();

        if ( myphlc.has_high() || mt->price() > myphlc.high())
            myphlc.set_high(mt->price());
        if ( myphlc.has_low() || myphlc.low() <= 0 || mt->price() < myphlc.low())
            myphlc.set_low(mt->price());
        if ( !myphlc.has_open() || myphlc.open() == 0)
            myphlc.set_open(mt->price());
        myphlc.set_close(mt->price());
        myphlc.set_volume(myphlc.volume()+mt->size());
        myphlc.set_change(myphlc.close() - myphlc.open());
    }

    void Reset(const string &symbol) {
        auto holdit = blocknum();
        Clear();
        set_blocknum(holdit);
        set_symbol(symbol);
        mutable_ohlc()->set_symbol(symbol);
    }

    std::string mSerialized;
    leveldb::WriteOptions write_sync{};

};


}

#endif /* defined(__fantasybit__ExchangeData__) */
