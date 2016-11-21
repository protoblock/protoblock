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

using namespace std;

namespace fantasybit
{
struct OpenOrder {
    string playerid;
    OrderCore livecore;
};

struct Position {
    int netqty;
    int netprice;
    string ToString() const {
        return to_string(netqty) + " " + to_string(netprice);
    }
};

struct FullPosition {
    string playerid;
    string fname;
    Position pos;
};

struct FullOrderDelta {
    string fname;
    Order openorder;
    string playerid;
};


typedef std::unordered_map<std::string,std::pair<Position,std::vector<Order> > > ordsnap_t;

/*
struct Order {
    OrderCore   mOrderCore;
    int32_t     orderId;
    //private key
};
*/

struct InsideBook {
    InsideBook() : totSize(0) {}
    list<Order> mOList;
    std::unordered_map<int32_t,list<Order>::iterator> mOMap;
    //static int32_t orderIdCount;
    int32_t totSize;

    /*
    int32_t New(const ExchangeOrder& order) {
        totSize += order.size();
        Order ord {order,++orderIdCount};
        mOMap.insert(make_pair(orderIdCount,mOList.insert(mOList.end(),ord)));
        return orderIdCount;
    }
    */

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

        //orderIdCount++;
        //return ret;
        return;
    }

    bool Fill(int32_t qty,list<Order>::iterator &iter) {
        totSize -= qty;

        /*
        auto newsize = iter->core().size() - qty;
        if ( newsize > 0 ) {
            iter->mutable_core()->set_size(newsize);
        }
        */
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
        /*
        auto newsize = iter->core().size() - qty;
        if ( newsize > 0 ) {
            iter->mutable_core()->set_size(newsize);
        }
        */
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
        //orderIdCount++;
        //return ret;
        //return orderIdCount;
    }
};

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

#define BOOK_SIZE 40
class LimitBook {
    InsideBook mBids[BOOK_SIZE], mAsks[BOOK_SIZE];
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
    LimitBook(const string &playerid) : mPlayerid(playerid) {
        mBb = -1;
        mBa = BOOK_SIZE;
    }

    string mPlayerid;

    InsideBook *getInside(bool bid, int32_t price) {
        return bid ? &mBids[price-1] : &mAsks[price-1];
    }

    void updateTopfromCache(int b, int a) {
        if ( b > 0 && b <= BOOK_SIZE)
            mBb = b-1;
        if ( a > 0 && a <= BOOK_SIZE)
            mBa = a-1;
    }

    bool NewOrder(Order &eo, Position &);

    int32_t CancelOrder(Order &order);
};

struct MatchingEngine {
    MatchingEngine(const string &playerid,bool il = false) : mPlayerid(playerid), islocked(il) {

    }
    unique_ptr<LimitBook> mLimitBook;
    bool islocked;
    std::unordered_map<std::string,Position> mPkPos;
    string mPlayerid;
    MarketSnapshot *makeSnapshot(MarketSnapshot *) ;
    void ResetLimitBook() {
        mLimitBook.reset(new LimitBook(mPlayerid));
    }
};


class ExchangeData : public QObject {

    Q_OBJECT

    std::shared_ptr<leveldb::DB> settlestore;
    std::shared_ptr<leveldb::DB> bookdeltastore;
    std::shared_ptr<leveldb::DB> orderseqstore;
    std::shared_ptr<leveldb::DB> posstore;

    std::unordered_map<string,std::unordered_map<std::string,Position>> mPositions;
    std::unordered_map<string,unique_ptr<MatchingEngine>> mLimitBooks;
    std::unordered_map<string,ContractOHLC> mContractOHLC;
    std::unordered_map<string,MarketQuote> mMarketQuote;

    std::unordered_map<int32_t,OpenOrder> mOpenOrders;


    std::set<std::string> mSubscribed;

    leveldb::WriteOptions write_sync{};
    //void ProcessResultOver(const string &,int32_t);
    //void ProcessResult(const SettlePos &spos,std::unordered_map<string,int32_t>::const_iterator result);

    void StoreOhlc(string playerid);
    void ProcessBookDelta(const BookDelta &bd);

public:
    ExchangeData() {}

    int64_t MAXSEQ;
    void init();
    void closeAll();

    void removeAll() {
       pb::remove_all(GET_ROOT_DIR() + "trade/");
    }

    bool amlive = false;
    std::unordered_map<std::string,std::set<int32_t>> mNameSeqMap;
    std::unordered_map<int32_t,std::string> mSeqNameMap;
    std::unordered_set<string> fnames;
    void OnTradeSessionStart(int week);

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
    std::unordered_map<string,BookPos> GetRemainingSettlePos();

    void OnNewOrderMsg(const ExchangeOrder&, int32_t seqnum,
                       shared_ptr<fantasybit::FantasyName> fn);

    void OnOrderNew(const ExchangeOrder&, int32_t seqnum,
                    shared_ptr<fantasybit::FantasyName> fn);

    void OnOrderCancel(const ExchangeOrder&, int32_t seqnum,
                       shared_ptr<fantasybit::FantasyName> fn);

    //void OnOrderReplace(const ExchangeOrder&, const string &uid) {}

    Position getPosition(const string &pk,const string &playerid);
    void OnDeltaPos(const string &pid, int32_t seqnum, int32_t pqty, int price);

    void SaveBookDelta();
    void OnGameResult(const GameResult&gs);

    void OnGameStart(std::string gid,
                     std::vector<std::string> &home,
                     std::vector<std::string> &away
                     );
    void clearNewWeek();

    int mWeek;

    GlobalState mGlobalState;

    void OnWeekOver(int week);
    void OnWeekStart(int week) {
        mWeek = week;
        if (amlive)
            doEmitSnap();
    }

    std::string filedir(const std::string &in) {
        return GET_ROOT_DIR() + "trade/" + in;
    }

//    void removeAll() {
//       fc::remove_all(GET_ROOT_DIR() + "trade/");
//    }

    //void MergeMarketQuote(const string &playerid,const MarketQuote & );
    void OnTrade(const string &playerid, fantasybit::TradeTic *tt);
    void OnMarketTicker(const string &playerid, fantasybit::MarketTicker *mt);
    void OnNewDepth(const string &playerid, fantasybit::DepthFeedDelta *df) {
        if ( amlive ) {
            df->set_symbol(playerid);
            emit NewDepthDelta(df);
        }
    }

    void doEmitSnap() {
        /*
        emit StartMarketSnapShot(mWeek);
        for (auto &it : mLimitBooks) {
            #ifdef TRACE
                qDebug() << "level2 ExchangeData onlive snapshot emit" << it.first;
            #endif
            auto *ms = MarketSnapshot::default_instance().New();
            ms->set_week(mWeek);
            //auto it2 = mMarketQuote.find(it.first);
            ms->mutable_quote()->CopyFrom(mMarketQuote[it.first]);
            ms->mutable_ohlc()->CopyFrom(mContractOHLC[it.first]);
            ms->set_symbol(it.first);

            if ( ms->depth_size() > 0 || ms->ohlc().volume() > 0 ||
                 (ms->quote().l() + ms->quote().b() + ms->quote().a()) > 0)
                emit NewMarketSnapShot(it.second->makeSnapshot(ms));
        }
        emit FinishMarketSnapShot(mWeek);
        */
    }

    std::vector<MarketSnapshot> GetCurrentMarketSnaps() {
        std::lock_guard<std::recursive_mutex> lockg{ ex_mutex };

        qDebug() << "get GetCurrentMarketSnaps" ;
        std::vector<MarketSnapshot> vms{};

        for (auto &it : mLimitBooks) {
            #ifdef TRACE
                qDebug() << "level2 ExchangeData GetCurrentMarketSnaps" << it.first;
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


signals:
    void NewMarketTicker(fantasybit::MarketTicker*);
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

    void Reset(const string &ticker) {
        Clear();
        set_playerid(ticker);
        mutable_ohlc()->set_symbol(ticker);
    }

};


}

#endif /* defined(__fantasybit__ExchangeData__) */
