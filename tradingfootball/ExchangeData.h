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
#include <fc/filesystem.hpp>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstrea

using namespace std;

namespace fantasybit
{

struct Position {
    int netqty;
    int netprice;
    operator string() {
        return to_string(netqty) + " " + to_string(netprice);
    }
};

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

    list<Order>::reverse_iterator rtop() {
        return rbegin(mOList);
    }

    list<Order>::reverse_iterator rbot() {
        return rend(mOList);
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
        auto newsize = iter->core().size() - qty;
        if ( newsize > 0 ) {
            iter->mutable_core()->set_size(newsize);
        }
        else {
            mOMap.erase(iter->refnum());
            iter = mOList.erase(iter);
        }

        return true;
    }

    bool Fill(int32_t qty,list<Order>::reverse_iterator &iter) {
        totSize -= qty;
        auto newsize = iter->core().size() - qty;
        if ( newsize > 0 ) {
            iter->mutable_core()->set_size(newsize);
        }
        else {
            mOMap.erase(iter->refnum());

            mOList.erase(std::next(iter).base());
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
    void NewBid(Order &order);
    void NewAsk(Order &order);
    void NewTop(int price, int32_t qty, bool isbuy);
    void NewNew(Order &order) {
        SaveNew(order.core());
    }


    void SweepAsks( Order &order);
    void SweepBids( Order &order);
    void GetTop(bool isbuy) {
        if (isbuy) {
            for (; mBb >= 0 && mBids[mBb].totSize == 0; --mBb) ;
            if (mBb < 0)
                NewTop(0,0,isbuy);
            else
                NewTop(mBb, mBids[mBb].totSize, isbuy);
        }
        else {
            for (; mBa < BOOK_SIZE && mAsks[mBa].totSize == 0; ++mBa) ;
            if (mBa >= BOOK_SIZE)
                NewTop(0,0,isbuy);
            else
                NewTop(mBa, mAsks[mBa].totSize, isbuy);
        }
    }

    void SaveRemove(Order &o,int32_t fillqty);
    void SaveNew(const OrderCore &oc);
    void SendFill(Order &o, int32_t q, int price ) {
        o.mutable_core()->set_size(o.core().size()-q);
        //send fill
    }

public:
    LimitBook() {
        mBb = -1;
        mBa = BOOK_SIZE;
    }

    InsideBook *getInside(bool bid, int32_t price) {
        return bid ? &mBids[price-1] : &mAsks[price-1];
    }

    void updateTopfromCache(int b, int a) {
        if ( b > 0 && b <= BOOK_SIZE)
            mBb = b-1;
        if ( a > 0 && a <= BOOK_SIZE)
            mBa = a-1;
    }

    void NewOrder(Order &eo);

    int32_t CancelOrder(Order &order);
};

struct MatchingEngine {
    MatchingEngine(bool il = false) : islocked(il) {}
    unique_ptr<LimitBook> mLimitBook;
    bool islocked;
    std::unordered_map<std::string,Position> mPkPos;

    MarketSnapshot *makeSnapshot(const string &symbol) ;
};


class ExchangeData : public QObject {

    Q_OBJECT

    std::shared_ptr<leveldb::DB> settlestore;
    std::shared_ptr<leveldb::DB> bookdeltastore;

    std::unordered_map<pubkey_t,std::unordered_map<std::string,Position>> mPositions;
    std::unordered_map<string,unique_ptr<MatchingEngine>> mLimitBooks;
    leveldb::WriteOptions write_sync{};
    void ProcessResultOver(const string &,int32_t);
    void ProcessResult(const SettlePos &spos,std::unordered_map<string,int32_t>::const_iterator result);
    bool GetGameSettlePos(const string &gid,GameSettlePos &gsp);
    bool amlive = false;
public:
    ExchangeData() {}
    void init();
    void closeAll();

    void OnNewOrderMsg(const ExchangeOrder&, int32_t seqnum,
                       shared_ptr<fantasybit::FantasyName> fn);

    void OnOrderNew(const ExchangeOrder&, int32_t seqnum,
                    shared_ptr<fantasybit::FantasyName> fn);

    void OnOrderCancel(const ExchangeOrder&, int32_t seqnum,
                       shared_ptr<fantasybit::FantasyName> fn);

    //void OnOrderReplace(const ExchangeOrder&, const string &uid) {}

    Position getPosition(const pubkey_t &pk,const string &playerid);

    void SaveBookDelta();
    void OnGameResult(const GameResult&gs);

    void OnNewOrderMsg(const ExchangeOrder&);

    void OnGameStart(std::string gid,
                     std::vector<std::string> &home,
                     std::vector<std::string> &away
                     );
    void clearNewWeek();

    void OnWeekOver(int week);
    std::string filedir(const std::string &in) {
        return GET_ROOT_DIR() + "trade/" + in;
    }

    void removeAll() {
       fc::remove_all(GET_ROOT_DIR() + "trade/");
    }

signals:
    void NewMarketTicker(fantasybit::MarketTicker*);
    void NewMarketSnapShot(fantasybit::MarketSnapshot*);
public slots:
    void OnLive(bool subscribe) {
        amlive = true;
#ifdef TRACE
    qDebug() << "level2 ExchangeData OnLive";
#endif

        for (auto &it : mLimitBooks) {
#ifdef TRACE
    qDebug() << "level2 ExchangeData onlive snapshot emit" << it.first;
#endif
            emit NewMarketSnapShot(it.second->makeSnapshot(it.first));
        }
    }

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


}

#endif /* defined(__fantasybit__ExchangeData__) */
