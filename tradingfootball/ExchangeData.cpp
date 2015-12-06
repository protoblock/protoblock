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
#include "leveldb/slice.h"
#include <QGlobalStatic>
#include "dataservice.h"

using namespace std;
using namespace fantasybit;

//Q_GLOBAL_STATIC(ExchangeData, staticExchangeData)
Q_GLOBAL_STATIC(BookDeltaMediator, mBookDelta)
Q_GLOBAL_STATIC(ExchangeDataHolder, pExchangeData)


void ExchangeData::init() {
    pExchangeData->set(this);
    qDebug() << "level2 ExchangeData init";
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
            if ( !gsp.ParseFromString(it->value().ToString()) )
                continue;

#ifdef TRACE
            qDebug() << "level2 ExchangeData init GameSettlePos " << gsp.gameid();
#endif

            for (auto ha : {"home","away"})
            for ( auto bp : ha == "home" ? gsp.home() : gsp.away()) {
                auto it2 = mLimitBooks.insert(make_pair(bp.playerid(),
                               unique_ptr<MatchingEngine>(new MatchingEngine(bp.playerid(),true))));
                for (auto p : bp.positions()) {
                    it2.first->second->mPkPos.insert(make_pair(p.pk(),Position{p.qty(),p.price()}));
#ifdef TRACE
            qDebug() << "level2 ExchangeData init selltlepos" << p.DebugString();
#endif

                }
            }
        }
        delete it;
    }

    leveldb::DB *db3;
    status = leveldb::DB::Open(options, filedir("snapstore"), &db3);
    snapstore.reset(db3);
    if ( !status.ok() ) {
        qCritical() << " cant open " + filedir("snapstore");
        //todo emit fatal
        return;
    }
    else {
        auto *it = snapstore->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            ContractOHLC ohlc;
            if ( !ohlc.ParseFromString(it->value().ToString()) )
                continue;

#ifdef TRACE
            qDebug() << "level2 ExchangeData init ContractOHLC " <<  ohlc.symbol();
#endif

            auto it3 = mLimitBooks.find(ohlc.symbol());
            if ( it3 == end(mLimitBooks)) {
                auto it2 = mLimitBooks.insert(make_pair(ohlc.symbol(),
                               unique_ptr<MatchingEngine>(new MatchingEngine(ohlc.symbol(),false))));

                if ( !it2.second ) {
                    qWarning() << "unable to insert for" << ohlc.symbol();
                    continue;
                }

#ifdef TRACE
            qDebug() << "level2 ExchangeData new init ContractOHLC for" << ohlc.symbol();
#endif
                it2.first->second->ResetLimitBook();
            }

            mContractOHLC[ohlc.symbol()] = ohlc;
        }
        delete it;
    }


    Int32Comparator *cmp = new Int32Comparator();
    leveldb::Options optionsInt;
    optionsInt.create_if_missing = true;
    optionsInt.comparator = cmp;

    leveldb::DB *db2;
    status = leveldb::DB::Open(optionsInt, filedir("bookdeltastore"), &db2);
    if ( !status.ok() ) {
        qCritical() << " cant open " + filedir("bookdeltastore");
        //todo emit fatal
        return;
    }

    bookdeltastore.reset(db2);

    /*
    auto it = bookdeltastore->NewIterator(leveldb::ReadOptions());
    it->SeekToFirst();
    string out;
    bookdeltastore->GetProperty("leveldb.stats",&out);
#ifdef TRACE
            qDebug() << "level2 ExchangeData init bookdeltastore " << out <<  it->Valid() << " is valid?";
#endif
    */
    {
        auto *it = bookdeltastore->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            BookDelta bd;
            if ( !bd.ParseFromString(it->value().ToString()) )
                qCritical() << "level2 ExchanegData bad read BookDelta";
#ifdef TRACE
            qDebug() << "level2 ExchangeData init BookDelta " << bd.playerid() << bd.seqnum();
#endif

            auto it3 = mLimitBooks.find(bd.playerid());
            if ( it3 == end(mLimitBooks)) {
                auto it2 = mLimitBooks.insert(make_pair(bd.playerid(),
                               unique_ptr<MatchingEngine>(new MatchingEngine(bd.playerid(),false))));
                //it2.first->second->mPkPos.insert(make_pair(p.pk(),Position{p.qty(),p.price()}));

                if ( !it2.second ) {
                    qWarning() << "unable to insert for" << bd.playerid();
                    continue;
                }

#ifdef TRACE
            qDebug() << "level2 ExchangeData new init BookDelta for" << bd.playerid();
#endif

                it2.first->second->ResetLimitBook();//mLimitBook.reset(new LimitBook());

                it3 = it2.first;
            }
            LimitBook *lb = it3->second->mLimitBook.get();
            InsideBook *ib;
            if ( bd.has_newnew()) {
#ifdef TRACE
                qDebug() << "level2 ExchangeData new " << bd.newnew().DebugString();
#endif
                ib = lb->getInside(bd.newnew().buyside(),bd.newnew().price());
                Order o;
                o.mutable_core()->CopyFrom(bd.newnew());
                o.set_refnum(bd.seqnum());
                ib->New(o);
            }

            for ( auto can : bd.removes() ) {
#ifdef TRACE
                qDebug() << "level2 ExchangeData remove " << can.DebugString();
#endif
                ib = lb->getInside(can.core().buyside(),can.core().price());
                ib->Remove(can);
            }

            MarketQuote mq;
            for ( auto l1 : bd.level1tic()) {
#ifdef TRACE
                qDebug() << "level2 ExchangeData Level1 " << l1.DebugString();
#endif
                switch (l1.type()) {
                case MarketTicker::BID:
                    mq.set_bs(l1.size());
                    mq.set_b(l1.price());
                    break;
                case MarketTicker::ASK:
                    mq.set_as(l1.size());
                    mq.set_a(l1.price());
                    break;
                case MarketTicker::LAST:
                    mq.set_ls(l1.size());
                    mq.set_l(l1.price());
                    break;
                }
            }

            if ( bd.level1tic_size() > 0) {
#ifdef TRACE
            qDebug() << "level2 ExchangeData bba " << mq.DebugString();
#endif
                lb->updateTopfromCache(mq.b(),mq.a());
            }
            mMarketQuote[bd.playerid()] = mq;
        }
        if ( it != NULL ) delete it;
    }


    //MarketTicker *mm = l1.New();
    //mm->CopyFrom(l1);
    //emit NewMarketTicker(mm);



}

void ExchangeData::closeAll() {
    mPositions.clear();
    mLimitBooks.clear();
    settlestore.reset();
    bookdeltastore.reset();
    mContractOHLC.clear();
    snapstore.reset();
}

void ExchangeData::clearNewWeek() {
    closeAll();
    removeAll();
    init();
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
    bool exitonly = fn->getStakeBalance() < 0;
    auto pos = getPosition(fn->pubkey(),eo.playerid());
#ifdef TRACE
    qDebug() << "level2 ExchangeData OnOrderNew exitonly" << exitonly << "pos " << pos;
#endif
    if ( exitonly && (
         (eo.core().buyside() && pos.netqty > 0) ||
         (!eo.core().buyside() && pos.netqty < 0) )) {
        qWarning() << "invalid order, exitonly for" << eo.DebugString();
        return;
    }

    if ( !eo.has_playerid() || eo.playerid() == "") {
#ifdef TRACE
    qDebug() << "level2 ExchangeData OnOrderNew no playerid";
#endif

        return;
    }
    //MatchingEngine &ma;
    auto it = mLimitBooks.find(eo.playerid());
    if ( it == end(mLimitBooks)) {
        //todo: confirm player_id even exists
#ifdef TRACE
    qDebug() << "level2 ExchangeData OnOrderNew create new book " << eo.playerid();
#endif

        auto it2 = mLimitBooks.insert(make_pair(eo.playerid(),
               unique_ptr<MatchingEngine>(new MatchingEngine(eo.playerid()))));
        if ( !it2.second ) {
            qWarning() << "unbale to insert for" << eo.DebugString();
            return;
        }
        it = it2.first;
        it->second->ResetLimitBook();//mLimitBook.reset(new LimitBook());
    }
    if ( it->second->islocked) {
         qWarning() << "invalid order, locked limitbook for" << eo.DebugString();
         return;
    }
    MatchingEngine &ma = *(it->second);
    Order ord;
    ord.mutable_core()->CopyFrom(eo.core());
    ord.set_refnum(seqnum);
    mBookDelta->Reset(eo.playerid());
    ma.mLimitBook->NewOrder(ord);
    //mBookDelta->set_playerid(eo.playerid());
    mBookDelta->set_public_key(Commissioner::pk2str(fn->pubkey()));
    SaveBookDelta();
}

void ExchangeData::SaveBookDelta() {
#ifdef TRACE
    qDebug() << "level2 ExchangeData SaveBookDelta" << mBookDelta->DebugString();
#endif

    int32_t seqnum = mBookDelta->seqnum();
    leveldb::Slice snum((char*)&seqnum, sizeof(int32_t));
    //string temp = "fuck you ass hole";//
    //mBookDelta->SerializeAsString();
    leveldb::Status st = bookdeltastore->Put(write_sync, snum, mBookDelta->SerializeAsString() );
    if ( !st.ok());
        qWarning() << " error writing bookdeltastore";
}

//BookDelta mBookDelta{};

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
    mBookDelta->set_playerid(eo.playerid());
    mBookDelta->set_public_key(Commissioner::pk2str(fn->pubkey()));
    SaveBookDelta();

}

Position ExchangeData::getPosition(const pubkey_t &pk,const string &playerid) {
    Position ret{0,0};
    auto &pmap = mPositions[pk];
    auto it = pmap.find(playerid);
    if ( it != end(pmap))
       ret = it->second;

    return ret;
}

void ExchangeData::OnGameResult(const GameResult&gs) {

#ifdef TRACE
    qDebug() << "level2 ExchangeData OnGameResult " << gs.gameid();
#endif

    GameSettlePos gsp;
    if ( !GetGameSettlePos(gs.gameid(),gsp) )
       return;

    for (auto ha : {"home","away"}) {
        std::unordered_map<string,int32_t> idtoresult ;

        for ( auto pr : ha == "home" ? gs.home_result() : gs.away_result()) {
            float fresult = pr.result() * 100.0;
            int n = floor(fresult + 0.5);
            idtoresult[pr.playerid()] = n;
        }

        for ( auto pp : ha == "home" ? gsp.home() : gsp.away()) {
            auto it = idtoresult.find(pp.playerid());
            if ( it == end(idtoresult))
                continue;

            for ( auto spos : pp.positions() )
                ProcessResult(spos,it);
        }
    }

}

bool ExchangeData::GetGameSettlePos(const string &gid,GameSettlePos &gsp) {
    string temp;
    leveldb::Slice sgid(gid);
    if ( !settlestore->Get(leveldb::ReadOptions(), sgid, &temp).ok() ) {
        qWarning() << "cant read GetGameSettlePos " << gid;
        return false;
    }
    gsp.Clear();
    if ( !gsp.ParseFromString(temp) ) {
        qWarning() << "error parsing  GameSettlePos " << gid;
        return false;
    }

    return true;
}

void ExchangeData::OnGameStart(std::string gid,
              std::vector<std::string> &home,
              std::vector<std::string> &away
              ) {

#ifdef TRACE
    qDebug() << "level2 ExchangeData OnGameStart " << gid;
#endif

    GameSettlePos gsp{};
    gsp.set_gameid(gid);

    for (auto ha : {"home","away"})
    for ( auto pid : ha == "home" ? home : away) {
        BookPos bp{};
        bp.set_playerid(pid);
        auto it = mLimitBooks.find(pid);
        if ( it == end(mLimitBooks)) {
            mLimitBooks.insert(make_pair(pid,
                   unique_ptr<MatchingEngine>(new MatchingEngine(pid,true))));
        }
        else {
            it->second->islocked = true;
#ifdef TRACE
    qDebug() << "level2 ExchangeData OnGameStart locking " << it->first;
#endif

            for ( auto p : it->second->mPkPos ) {
                SettlePos sp;
                sp.set_qty(p.second.netqty);
                sp.set_price(p.second.netprice);
                sp.set_pk(p.first);
                bp.add_positions()->CopyFrom(sp);
#ifdef TRACE
    qDebug() << "level2 ExchangeData OnGameStart settlepos " << sp.DebugString();
#endif

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

void ExchangeData::OnWeekOver(int week) {
    //set all results to 0 and proess pnl
#ifdef TRACE
    qDebug() << "level2 ExchangeData OnWeekOver" << week;
#endif

    for (auto &it : mLimitBooks) {
        ProcessResultOver(it.first,0);
    }
    clearNewWeek();
}

void ExchangeData::ProcessResult(const SettlePos &spos,std::unordered_map<string,int32_t>::const_iterator result) {
    //set pnl delte position
}

void ExchangeData::ProcessResultOver(const string &key, int32_t result) {
    //set pnl delte position
    //mLimitBooks.erase(key);
}

void ExchangeData::OnMarketTicker(const string &playerid, fantasybit::MarketTicker *mt) {
    //MarketQuote & mq = ;
    MarketQuote &mquote = mMarketQuote[playerid];
    if ( mt->type() == MarketTicker::LAST ) {
        if ( mt->price() > mquote.l())
            mquote.set_udn(1);
        else if ( mt->price() < mquote.l() )
            mquote.set_udn(-1);
        else
            mquote.set_udn(0);

        mquote.set_l(mt->price());
        mquote.set_ls(mt->size());

        auto it = mContractOHLC.find(playerid);
        if ( it == mContractOHLC.end() ) {
            ContractOHLC oh;
            oh.set_symbol(playerid);
            mContractOHLC[playerid] = oh;
        }

        ContractOHLC &myphlc = mContractOHLC[playerid];


        if ( !myphlc.has_high() || mt->price() > myphlc.high())
            myphlc.set_high(mt->price());
        if ( !myphlc.has_low() || myphlc.low() <= 0 || mt->price() < myphlc.low())
            myphlc.set_low(mt->price());
        if ( !myphlc.has_open() || myphlc.open() == 0)
            myphlc.set_open(mt->price());
        myphlc.set_close(mt->price());
        myphlc.set_volume(myphlc.volume()+mt->size());
        myphlc.set_change(myphlc.close() - myphlc.open());

        StoreOhlc(playerid);
    }
    else if ( mt->type() == MarketTicker::BID ) {
        mquote.set_b(mt->price());
        mquote.set_bs(mt->size());
    }
    else {
        mquote.set_a(mt->price());
        mquote.set_as(mt->size());
    }

    emit NewMarketTicker(mt);
}


void LimitBook::SaveRemove(Order &o,int32_t fillqty) {
    auto no = mBookDelta->add_removes();
    no->CopyFrom(o);
    no->mutable_core()->set_size(fillqty);
}

void LimitBook::SaveNew(const OrderCore &oc) {
#ifdef TRACE
    string s = oc.buyside() ? "Bid" : "Ask";
    qDebug() << "level2 New SaveNew " << s  << oc.price() << oc.size();
#endif

    mBookDelta->mutable_newnew()->CopyFrom(oc);
}

void LimitBook::NewTop(int price, int32_t qty, bool isbuy) {
    MarketTicker *pmt = MarketTicker::default_instance().New();
    MarketTicker &mt = *pmt;
    mt.set_type(isbuy ? MarketTicker::BID : MarketTicker::ASK);
    mt.set_price(price);
    mt.set_size(qty);
    mBookDelta->add_level1tic()->CopyFrom(mt);
    //emit pExchangeData->get()->NewMarketTicker(pmt);
    pExchangeData->get()->OnMarketTicker(mPlayerid,pmt);


#ifdef TRACE
    string s = isbuy ? "Bid" : "Ask";
    qDebug() << "level2 NewTop " <<  s << price << qty;
#endif

}

void LimitBook::NewOrder(Order &eo) {
    //mBookDelta->Clear();
    mBookDelta->set_seqnum(eo.refnum());
    //mBookDelta->set_allocated_newnew(eo.mutable_core());
    if ( eo.core().buyside() ) {
        if ( eo.core().price() > 0 && eo.core().size() > 0 && eo.core().size() <= 100000 )
            NewBid(eo);
        else
            qWarning() << " invalid order";
    }
    else {
        if ( eo.core().price() <= 40 && eo.core().size() > 0 && eo.core().size() <= 100000 )
            NewAsk(eo);
        else
            qWarning() << " invalid order";
    }
}

int32_t LimitBook::CancelOrder(Order &order) {
    mBookDelta->Clear();
    auto myprice = order.core().price()-1;
    if ( (myprice < 0) || (myprice >= BOOK_SIZE) )
        return -1;

    if (order.core().buyside()) {
        mBids[myprice].Cancel(order);
        SaveRemove(order,order.core().size());
        //mBookDelta->add_removes()->CopyFrom(order);
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

MarketSnapshot* MatchingEngine::makeSnapshot(MarketSnapshot *ms) {
    //MarketSnapshot *ms = MarketSnapshot::default_instance().New();
    //ms->set_symbol(symbol);
#ifdef TRACE
    qDebug() << "level2 makeSnapshot";// << symbol;
#endif

    int a = 1;
    int b = BOOK_SIZE;
    int nexta = 0;
    int nextb = 0;
    InsideBook *next;
    DepthItem *di;
    do {
        if ( b >= 1) {
            if ( nullptr != (next = mLimitBook->getInside(true,b)))
                if ( next->totSize > 0) {
#ifdef TRACE
                    qDebug() << "level2 makeSnapshot bid" << nextb << ":" << b << next->totSize;
#endif

                    if ( nextb == ms->depth_size())
                        di = ms->add_depth();
                    else if ( nextb < ms->depth_size() )//nextbid < depth_size
                        di = ms->mutable_depth(nextb);
                    else {
                        qCritical() << " should not be here nexta > depth_size";
                        continue;
                    }


                    di->set_bs(next->totSize);
                    di->set_b(b);
                    nextb++;
                }
            b--;
        }

        if ( a <= BOOK_SIZE) {
            if ( nullptr != (next = mLimitBook->getInside(false,a)))
                if ( next->totSize > 0) {
#ifdef TRACE
                    qDebug() << "level2 makeSnapshot ask" << nexta << ":" << a << next->totSize;
#endif

                    if ( nexta == ms->depth_size())
                        di = ms->add_depth();
                    else if ( nexta < ms->depth_size() )//nexta < depth_size
                        di = ms->mutable_depth(nexta);
                    else {
                        qCritical() << " should not be here nexta > depth_size";
                        continue;
                    }

                    di->set_as(next->totSize);
                    di->set_a(a);
                    nexta++;
                }
            a++;
        }
    } while(b >= 1 && a <= BOOK_SIZE);

#ifdef TRACE
    qDebug() << "level2 makeSnapshot" << mPlayerid << "depthsize " << ms->depth_size();
#endif

    //if ( ms->depth_size() > 0 )
    return ms;
}

void LimitBook::NewBid(Order &order) {
#ifdef TRACE
qDebug() << "level2 New Bid " << order.core().price() << order.core().size();
#endif

    auto myprice = order.core().price()-1;
    if ( (myprice < 0) || (myprice >= BOOK_SIZE) )
        return;

    if (myprice < mBa) {
        //if (order.constraint == Constraint.IOC)
        //    Send(new BookFeedData(ExecType.Canceled, order));
        // else
        //{
         mBids[myprice].New(order);
#ifdef TRACE
    qDebug() << "level2 NewBid 1 mBb" <<  mBb;
#endif

         if (myprice >= mBb) {
             mBb = myprice;
             NewTop(myprice+1, mBids[myprice].totSize, true);
         }
         NewNew(order);
#ifdef TRACE
    qDebug() << "level2 NewBid 2 mBb" <<  mBb;
#endif

         //}
     }
     else {
#ifdef TRACE
    qDebug() << "level2 NewBid 3 mBb" <<  mBb;
#endif

         SweepAsks(order); //will modify size
#ifdef TRACE
    qDebug() << "level2 NewBid 4 mBb" <<  mBb;
#endif

         GetTop(false);
#ifdef TRACE
    qDebug() << "level2 NewBid 5 mBb" <<  mBb;
#endif

         if (order.core().size() <= 0)
             ;//Send(new BookFeedData(ExecType.Done, order));
         else {
             //if (order.constraint == Constraint.IOC)
             //    Send(new BookFeedData(ExecType.Canceled, order));
             //else {
#ifdef TRACE
    qDebug() << "level2 NewBid 6 mBb" <<  mBb;
#endif

             mBids[myprice].New(order);
             mBb = myprice;
#ifdef TRACE
    qDebug() << "level2 NewBid 7 mBb" <<  mBb;
#endif

             NewTop(myprice+1, mBids[myprice].totSize, true);
             NewNew(order);


             //mFeed.Executi    on(order, BookFeed.OrdStatus.New, 0);
             //Send(new BookFeedData(ExecType.New,order));
             //}
         }
     }
}

void LimitBook::NewAsk(Order &order){
#ifdef TRACE
   qDebug() << "level2 New Ask " << order.core().price() << order.core().size();
#endif

       auto myprice = order.core().price()-1;
       if ( (myprice < 0) || (myprice >= BOOK_SIZE) )
           return;

       if (myprice > mBb) {
           //if (order.constraint == Constraint.IOC)
           //    Send(new BookFeedData(ExecType.Canceled, order));
           // else
           //{
            mAsks[myprice].New(order);
            if (myprice <= mBa) {
                mBa = myprice;
                NewTop(myprice+1, mAsks[myprice].totSize, false);
            }
            NewNew(order);
            //}
        }
        else {
            SweepBids(order); //will modify size
            GetTop(true);

            if (order.core().size() <= 0)
                ;//Send(new BookFeedData(ExecType.Done, order));
            else {
                //if (order.constraint == Constraint.IOC)
                //    Send(new BookFeedData(ExecType.Canceled, order));
                //else {
                mAsks[myprice].New(order);
                mBa = myprice;
                NewTop(myprice+1, mAsks[myprice].totSize, false);
                NewNew(order);

                //mFeed.Execution(order, BookFeed.OrdStatus.New, 0);
                //Send(new BookFeedData(ExecType.New,order));
                //}
            }
        }
   }

void LimitBook::SweepAsks( Order &order) {
    int price = order.core().price()-1;
    int left = order.core().size();
    for (; mBa <= price; ++mBa) {
        InsideBook &curr = mAsks[mBa];

        int fillqty = min(curr.totSize, left);
        if (fillqty <= 0)
            continue;


        SendFill(order, fillqty, mBa);
        //NewTrade(mBa, fillqty, Side.BID);

        for (auto iiter = curr.top();
             iiter != curr.bot() && left > 0;) {
            Order &ord = *iiter;
            fillqty = min(ord.core().size(), left);
            if (fillqty <= 0)
                continue;

            left -= fillqty;

            SendFill(ord, fillqty, mBa);
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
void LimitBook::SweepBids( Order &order) {
    int price = order.core().price()-1;
    int left = order.core().size();
    for (; mBb >= price; --mBb) {
        InsideBook &curr = mBids[mBb];

        int fillqty = min(curr.totSize, left);
        if (fillqty <= 0)
            continue;

        SendFill(order, fillqty, mBb);
        //NewTrade(mBb, fillqty, Side.ASK);

        for (auto iiter = curr.rtop();
             iiter != curr.rbot() && left > 0;) {
            Order &ord = *iiter;
            fillqty = min(ord.core().size(), left);
            if (fillqty <= 0)
                continue;

            left -= fillqty;

            SendFill(ord, fillqty, mBa);
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

void LimitBook::SendFill(Order &o, int32_t q, int price ) {
    price += 1;
    o.mutable_core()->set_size(o.core().size()-q);
    MarketTicker *lst = mBookDelta->add_level1tic();
    lst->set_type(MarketTicker::LAST);
    lst->set_price(price);
    lst->set_size(q);

    pExchangeData->get()->OnMarketTicker(mPlayerid,lst);
}

void ExchangeData::OnLive(bool subscribe) {
    amlive = true;
#ifdef TRACE
qDebug() << "level2 ExchangeData OnLive";
#endif

    auto st = DataService::instance()->GetGlobalState();

    for (auto &it : mLimitBooks) {
#ifdef TRACE
qDebug() << "level2 ExchangeData onlive snapshot emit" << it.first;
#endif`
        auto *ms = MarketSnapshot::default_instance().New();
        ms->set_week(st.week());
        //auto it2 = mMarketQuote.find(it.first);
        ms->mutable_quote()->CopyFrom(mMarketQuote[it.first]);
        ms->mutable_ohlc()->CopyFrom(mContractOHLC[it.first]);
        ms->set_symbol(it.first);
        emit NewMarketSnapShot(it.second->makeSnapshot(ms));
    }
}
