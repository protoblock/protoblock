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

using namespace std;

namespace fantasybit
{

struct Position {
    int netqty;
    int netprice;
};

/*
struct Order {
    OrderCore   mOrderCore;
    int32_t     orderId;
    //private key
};
*/

struct InsideBook {
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

    bool Fill(int32_t qty,list<Order>::iterator iter) {
        totSize -= qty;
        auto newsize = iter->core().size() - qty;
        if ( newsize > 0 ) {
            iter->mutable_core()->set_size(newsize);
        }
        else {
            mOMap.erase(iter->refnum());
            mOList.erase(iter);
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
   int32_t bidsize;
   int bid;
   int ask;
   int32_t asksize;
   int last;
   int32_t lastsize;
};

#define BOOK_SIZE 40
class LimitBook {
    InsideBook mBids[BOOK_SIZE], mAsks[BOOK_SIZE];
    int mBb, mBa;
    void NewBid(Order &order) {
        auto myprice = order.core().price()-1;
        if ( (myprice < 0) || (myprice >= BOOK_SIZE) )
            return;

        if (myprice < mBa) {
            //if (order.constraint == Constraint.IOC)
            //    Send(new BookFeedData(ExecType.Canceled, order));
            // else
            //{
             mBids[myprice].New(order);
             if (myprice >= mBb) {
                 mBb = myprice;
                 NewTop(myprice, mBids[myprice].totSize, true);
             }
             NewNew(order);
             //}
         }
         else {
             SweepAsks(order); //will modify size
             GetTop(false);

             if (order.core().size() <= 0)
                 ;//Send(new BookFeedData(ExecType.Done, order));
             else {
                 //if (order.constraint == Constraint.IOC)
                 //    Send(new BookFeedData(ExecType.Canceled, order));
                 //else {
                 mBids[myprice].New(order);
                 mBb = myprice;
                 NewTop(myprice, mBids[myprice].totSize, true);
                 NewNew(order);

                 //mFeed.Execution(order, BookFeed.OrdStatus.New, 0);
                 //Send(new BookFeedData(ExecType.New,order));
                 //}
             }
         }
    }
    int32_t NewAsk(Order &eo) { return 0; }

    void NewTop(int price, int32_t qty, bool isbuy) {}
    void NewNew(Order &order) {
        SaveNew(order.core());
    }

    void SweepAsks( Order &order) {
        int price = order.core().price();
        int left = order.core().size();
        for (; mBa <= price; ++mBa) {
            InsideBook curr = mAsks[mBa];

            int fillqty = min(curr.totSize, left);
            if (fillqty <= 0)
                continue;

            //SendFill(order, fillqty, mBa);
            //NewTrade(mBa, fillqty, Side.BID);

            for (auto iiter = curr.top();
                 iiter != curr.bot() && left > 0; ++iiter) {
                Order &ord = *iiter;
                fillqty = min(ord.core().size(), left);
                if (fillqty <= 0)
                    continue;

                left -= fillqty;

                //SendFill(ord, fillqty, mBa);
                //ord.mutable_core()->set_size(ord.core().size()-fillqty);
                SaveRemove(ord,fillqty);
                if (curr.Fill(fillqty, iiter))
                    ;//Send(new BookFeedData(ExecType.Done, ord));
            }

            //NewDepth(Side.ASK, mBa);

            if (left <= 0)
                break;
        }
    }

    void GetTop(bool isbuy) {
        if (isbuy) {
            for (; mBb > 0 && mBids[mBb].totSize == 0; --mBb) ;
            if (mBb <= 0)
                NewTop(0,0,isbuy);
            else
                NewTop(mBb, mBids[mBb].totSize, isbuy);
        }
        else {
            for (; mBa < BOOK_SIZE && mAsks[mBa].totSize == 0; ++mBa) ;
            if (mBa > BOOK_SIZE)
                NewTop(0,0,isbuy);
            else
                NewTop(mBa, mAsks[mBa].totSize, isbuy);
        }
    }

    void SaveRemove(Order &o,int32_t fillqty);
    void SaveNew(const OrderCore &oc);

public:
    LimitBook() {
        mBb = 0;
        mBa = BOOK_SIZE-1;
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
};

class ExchangeData {

    std::shared_ptr<leveldb::DB> settlestore;
    std::shared_ptr<leveldb::DB> bookdeltastore;

    std::unordered_map<pubkey_t,std::unordered_map<std::string,Position>> mPositions;
    std::unordered_map<string,unique_ptr<MatchingEngine>> mLimitBooks;
    leveldb::WriteOptions write_sync{};

public:
    ExchangeData() {}
    void init();
    void closeAll();
    static BookDelta mBookDelta;

    void OnNewOrderMsg(const ExchangeOrder&, int32_t seqnum,
                       shared_ptr<fantasybit::FantasyName> fn);

    void OnOrderNew(const ExchangeOrder&, int32_t seqnum,
                    shared_ptr<fantasybit::FantasyName> fn);

    void OnOrderCancel(const ExchangeOrder&, int32_t seqnum,
                       shared_ptr<fantasybit::FantasyName> fn);

    //void OnOrderReplace(const ExchangeOrder&, const string &uid) {}

    Position getPosition(const pubkey_t &pk,const string &playerid);

    void ExchangeData::SaveBookDelta();

    void OnNewOrderMsg(const ExchangeOrder&);

    void OnGameStart(std::string gid,
                     std::vector<std::string> &home,
                     std::vector<std::string> &away
                     );

    std::string filedir(const std::string &in) {
        return GET_ROOT_DIR() + "trade/" + in;
    }

};


}

#endif /* defined(__fantasybit__ExchangeData__) */
