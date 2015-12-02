//
//  ExchangeData.h
//
//  Created by Jay Berg on 11/28/15.
//
//
#include "ExchangeData.h"
#include "qdebug.h"
#include <vector>
#include "fbutils.h"
#include "Commissioner.h"
#include "PeerNode.h"

using namespace std;
using namespace fantasybit;

void ExchangeData::init() {
    write_sync.sync = true;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status;

    leveldb::DB *db1;
    status = leveldb::DB::Open(options, filedir("settlestore"), &db1);
    settlestore.reset(db1);
    if ( !status.ok() ) {
        qCritical() << " cant open " + filedir("settlestore");
        //todo emit fatal
        return;
    }
    else {
        auto *it = settlestore->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            GameSettlePos gsp;
            gsp.ParseFromString(it->value().ToString());
            for (auto ha : {"home","away"})
            for ( auto bp : ha == "home" ? gsp.home() : gsp.away()) {
                auto it2 = mLimitBooks.insert(make_pair(bp.playerid(),
                               unique_ptr<MatchingEngine>(new MatchingEngine(true))));
                for (auto p : bp.positions()) {
                    it2.first->second->mPkPos.insert(make_pair(p.pk(),Position{p.qty(),p.price()}));
                }
            }
        }
        delete it;
    }

    Int32Comparator *cmp = new Int32Comparator();
    leveldb::Options optionsInt;
    optionsInt.create_if_missing = true;
    optionsInt.comparator = cmp;

    leveldb::DB *db2;
    status = leveldb::DB::Open(optionsInt, filedir("bookdeltastore"), &db2);
    bookdeltastore.reset(db2);
    if ( !status.ok() ) {
        qCritical() << " cant open " + filedir("bookdeltastore");
        //todo emit fatal
        return;
    }
    else {
        auto *it = bookdeltastore->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            BookDelta bd;
            bd.ParseFromString(it->value().ToString());

            auto it2 = mLimitBooks.insert(make_pair(bd.playerid(),
                           unique_ptr<MatchingEngine>(new MatchingEngine(false))));
            //it2.first->second->mPkPos.insert(make_pair(p.pk(),Position{p.qty(),p.price()}));

            if ( !it2.second ) {
                qWarning() << "unable to insert for" << bd.playerid();
                continue;
            }
            it2.first->second->mLimitBook.reset(new LimitBook());
            LimitBook &lb = *(it2.first->second->mLimitBook);
            InsideBook *ib;
            if ( bd.has_newnew()) {
                ib = lb.getInside(bd.newnew().buyside(),bd.newnew().price());
                Order o;
                o.mutable_core()->CopyFrom(bd.newnew());
                o.set_refnum(bd.seqnum());
                ib->New(o);
            }

            for ( auto can : bd.removes() ) {
                ib = lb.getInside(can.core().buyside(),can.core().price());
                ib->Remove(can);
            }

            Level1 L1;
            for ( auto l1 : bd.level1tic()) {
                switch (l1.type()) {
                case MarketTicker::BID:
                    L1.bidsize = l1.size();
                    L1.bid = l1.price();
                    break;
                case MarketTicker::ASK:
                    L1.asksize = l1.size();
                    L1.ask = l1.price();
                    break;
                case MarketTicker::LAST:
                    L1.lastsize = l1.size();
                    L1.last = l1.price();
                    break;
                }
            }

            lb.updateTopfromCache(L1.bid,L1.ask);
        }
        delete it;
    }
}

void ExchangeData::closeAll() {
    mPositions.clear();
    mLimitBooks.clear();
    settlestore.reset();
}

void ExchangeData::OnNewOrderMsg(const ExchangeOrder& eo,
                                 int32_t seqnum,
                                 shared_ptr<FantasyName> fn) {
    switch(eo.type()) {
    case ExchangeOrder::NEW:
        OnOrderNew(eo,seqnum,fn);
        break;
    case ExchangeOrder::CANCEL:
        OnOrderCancel(eo,seqnum,fn);
        break;
    case ExchangeOrder::REPLACE:
        //OnOrderReplace(eo,uid);
        break;
    default:
        break;
    }
}

void ExchangeData::OnOrderNew(const ExchangeOrder& eo,
                              int32_t seqnum,
                              shared_ptr<FantasyName> fn) {
    bool exitonly = fn->getStakeBalance() > 0;
    auto pos = getPosition(fn->pubkey(),eo.playerid());
    if ( exitonly && (
         (eo.core().buyside() && pos.netqty > 0) ||
         (!eo.core().buyside() && pos.netqty < 0) )) {
        qWarning() << "invalid order, exitonly for" << eo.DebugString();
        return;
    }

    //MatchingEngine &ma;
    auto it = mLimitBooks.find(eo.playerid());
    if ( it == end(mLimitBooks)) {
        //todo: confirm player_id even exists
        auto it2 = mLimitBooks.insert(make_pair(eo.playerid(),
               unique_ptr<MatchingEngine>(new MatchingEngine())));
        if ( !it2.second ) {
            qWarning() << "unbale to insert for" << eo.DebugString();
            return;
        }
        it = it2.first;
        it->second->mLimitBook.reset(new LimitBook());
    }
    if ( it->second->islocked) {
         qWarning() << "invalid order, locked limitbook for" << eo.DebugString();
         return;
    }
    MatchingEngine &ma = *(it->second);
    Order ord;
    ord.mutable_core()->CopyFrom(eo.core());
    ord.set_refnum(seqnum);
    ma.mLimitBook->NewOrder(ord);
    mBookDelta.set_playerid(eo.playerid());
    mBookDelta.set_public_key(Commissioner::pk2str(fn->pubkey()));
    SaveBookDelta();
}

void ExchangeData::SaveBookDelta() {
    int32_t seqnum = mBookDelta.seqnum();
    leveldb::Slice snum((char*)&seqnum, sizeof(int32_t));
    if ( !bookdeltastore->Put(write_sync, snum, mBookDelta.SerializeAsString()).ok());
        qWarning() << " error writing bookdeltastore";
}

BookDelta ExchangeData::mBookDelta{};

void ExchangeData::OnOrderCancel(const ExchangeOrder& eo, int32_t seqnum,
                                 shared_ptr<FantasyName> fn) {
    auto it = mLimitBooks.find(eo.playerid());
    if ( it == end(mLimitBooks)) {
        qWarning() << "invalid cancel LimitBook not found for" << eo.DebugString();
        return;
    }

    if ( it->second->islocked) {
         qWarning() << "invalid order, locked limitbook for" << eo.DebugString();
         return;
    }

    MatchingEngine &ma = *(it->second);
    Order ord;
    ord.mutable_core()->CopyFrom(eo.core());
    ord.set_refnum(eo.cancel_oref());
    ma.mLimitBook->CancelOrder(ord);
    mBookDelta.set_playerid(eo.playerid());
    mBookDelta.set_public_key(Commissioner::pk2str(fn->pubkey()));
    SaveBookDelta();

}

Position ExchangeData::getPosition(const pubkey_t &pk,const string &playerid) {
    Position ret;
    auto &pmap = mPositions[pk];
    auto it = pmap.find(playerid);
    if ( it != end(pmap))
       ret = it->second;

    return ret;
}

void ExchangeData::OnGameStart(std::string gid,
              std::vector<std::string> &home,
              std::vector<std::string> &away
              ) {

    GameSettlePos gsp{};
    gsp.set_gameid(gid);

    for (auto ha : {"home","away"})
    for ( auto pid : ha == "home" ? home : away) {
        BookPos bp{};
        bp.set_playerid(pid);
        auto it = mLimitBooks.find(pid);
        if ( it == end(mLimitBooks)) {
            mLimitBooks.insert(make_pair(pid,
                   unique_ptr<MatchingEngine>(new MatchingEngine(true))));
        }
        else {
            it->second->islocked = true;
            for ( auto p : it->second->mPkPos ) {
                SettlePos sp;
                sp.set_qty(p.second.netqty);
                sp.set_price(p.second.netprice);
                sp.set_pk(p.first);
                bp.add_positions()->CopyFrom(sp);
            }
        }
        if ( ha == "home")
            gsp.add_home()->CopyFrom(bp);
        else
            gsp.add_away()->CopyFrom(bp);
    }

    if (!settlestore->Put(write_sync, gid, gsp.SerializeAsString()).ok())
        qWarning() << "error writing settlestore" << gid;
    else
        qInfo() << "OnGameStart " << gid;

}

void LimitBook::SaveRemove(Order &o,int32_t fillqty) {
    auto no = ExchangeData::mBookDelta.add_removes();
    no->CopyFrom(o);
    no->mutable_core()->set_size(fillqty);
}

void LimitBook::SaveNew(const OrderCore &oc) {
    ExchangeData::mBookDelta.mutable_newnew()->CopyFrom(oc);
}

void LimitBook::NewOrder(Order &eo) {
    ExchangeData::mBookDelta.Clear();
    ExchangeData::mBookDelta.set_seqnum(eo.refnum());
    //ExchangeData::mBookDelta.set_allocated_newnew(eo.mutable_core());
    if ( eo.core().buyside() )
        NewBid(eo);
    else
        NewAsk(eo);
}

int32_t LimitBook::CancelOrder(Order &order) {
    ExchangeData::mBookDelta.Clear();
    auto myprice = order.core().price()-1;
    if ( (myprice < 0) || (myprice >= BOOK_SIZE) )
        return -1;

    if (order.core().buyside()) {
        mBids[myprice].Cancel(order);
        SaveRemove(order,order.core().size());
        //ExchangeData::mBookDelta.add_removes()->CopyFrom(order);
        //{
        //Send(new BookFeedData(ExecType.Canceled, order));

        if (mBb == myprice) {
            if (mBids[myprice].totSize > 0)
                NewTop(myprice, mBids[myprice].totSize, true);
            else
                GetTop(true);
        }

        //NewDepth(Side.BID, order.tickPrice);
        //} else
        //Send(new BookFeedData(ExecType.CancelReject, order));
    }
    else { // Side.Ask
        /*
        if (mAsks[order.tickPrice].Remove(order)) {
            Send(new BookFeedData(ExecType.Canceled, order));
            if (mBa == order.tickPrice) {
                if (mAsks[order.tickPrice].totSize > 0)
                    NewTop(order.tickPrice, mAsks[order.tickPrice].totSize, Side.ASK);
                else
                    GetTop(Side.ASK);
            }

            NewDepth(Side.ASK, order.tickPrice);
        } else
            //mFeed.Execution(order, BookFeed.OrdStatus.CancelReject, 0);
            Send(new BookFeedData(ExecType.CancelReject, order));
        */
    }
}



