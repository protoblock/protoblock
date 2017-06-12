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
#ifdef TIMEAGENTWRITEFILLS
#include "../../../fantasybit-2015/tradingfootball/playerloader.h"
#endif

#if defined(TIMEAGENTWRITEFILLS)
    SqlStuff sql("satoshifantasy","timeagentwrite");
#endif


using namespace std;
using namespace fantasybit;

//Q_GLOBAL_STATIC(ExchangeData, staticExchangeData)
Q_GLOBAL_STATIC(BookDeltaMediator, mBookDelta)
Q_GLOBAL_STATIC(ExchangeDataHolder, pExchangeData)


void ExchangeData::init() {
    std::lock_guard<std::recursive_mutex> lockg{ ex_mutex };
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
            qDebug() << "level2 ExchangeData init GameSettlePos llllll" << gsp.gameid().data();
#endif

            for (auto ha : {QString("home"),QString("away")} )
            for ( auto bp : ha == "home" ? gsp.home() : gsp.away()) {
                auto it2 = mLimitBooks.insert(make_pair(bp.playerid(),
                               unique_ptr<MatchingEngine>(new MatchingEngine(bp.playerid(),true))));
                for (auto p : bp.positions()) {
                    it2.first->second->mPkPos.insert(make_pair(p.pk(),Position{p.qty(),p.price()}));
#ifdef TRACE
            qDebug() << "level2 ExchangeData init selltlepos" << p.DebugString().data();
#endif

                }
            }
        }
        delete it;
    }

/*
    leveldb::DB *db3;
    status = leveldb::DB::Open(options, filedir("orderseqstore"), &db3);
    orderseqstore.reset(db3);
    if ( !status.ok() ) {
        qCritical() << " cant open " + filedir("orderseqstore");
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
*/

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
        MAXSEQ = 0;
        auto *it = bookdeltastore->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            BookDelta bd;
            if ( !bd.ParseFromString(it->value().ToString()) ) {
                qCritical() << "level2 ExchanegData bad read BookDelta";
                continue;
            }

            if ( bd.seqnum() > MAXSEQ)
                MAXSEQ = bd.seqnum();
#ifdef TRACE
            qDebug() << "level2 ExchangeData init BookDelta " << bd.playerid().data() << bd.seqnum();
#endif
/*
            auto &myset = mNameSeqMap[bd.fantasy_name()];
            myset.insert(bd.seqnum());
            mSeqNameMap[bd.seqnum()] = bd.fantasy_name();
*/
            auto it3 = mLimitBooks.find(bd.playerid());
            if ( it3 == end(mLimitBooks)) {
                auto it2 = mLimitBooks.insert(make_pair(bd.playerid(),
                               unique_ptr<MatchingEngine>(new MatchingEngine(bd.playerid(),false))));
                //it2.first->second->mPkPos.insert(make_pair(p.pk(),Position{p.qty(),p.price()}));

                if ( !it2.second ) {
                    qWarning() << "level2 unable to insert for" << bd.playerid();
                    continue;
                }

#ifdef TRACE
                qDebug() << "level2 ExchangeData new init BookDelta for" << bd.playerid().data();
#endif

                it2.first->second->ResetLimitBook();//mLimitBook.reset(new LimitBook());

                it3 = it2.first;
            }
            if ( it3->second->islocked ) continue;
            LimitBook *lb = it3->second->mLimitBook.get();
            InsideBook *ib;
            if ( bd.has_newnew()) {
#ifdef TRACE
                qDebug() << "level2 ExchangeData new llllll" <<bd.fantasy_name().data() << bd.newnew().DebugString().data();
#endif
                ib = lb->getInside(bd.newnew().buyside(),bd.newnew().price());
                Order o;
                o.mutable_core()->CopyFrom(bd.newnew());
                o.set_refnum(bd.seqnum());
                ib->New(o);
            }

            for ( auto can : bd.removes() ) {
#ifdef TRACE
                qDebug() << "level2 ExchangeData remove " << can.DebugString().data();
#endif
                ib = lb->getInside(can.core().buyside(),can.core().price());
                ib->Remove(can);
            }

            MarketQuote &mq = mMarketQuote[bd.playerid()];
            ContractOHLC &ohlc = mContractOHLC[bd.playerid()];

//            int i;
//            if ( bd.playerid() == "1307")
//                i = 0;
            for ( auto l1 : bd.level1tic()) {
#ifdef TRACE
                qDebug() << "level2 ExchangeData Level1 " << l1.DebugString().data();
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
                    setOhlc(ohlc,&l1);
                    break;
                }
            }

            if ( bd.level1tic_size() > 0) {
#ifdef TRACE
            qDebug() << "level2 ExchangeData bba bd.blocknum()" << bd.blocknum() <<
                        it3->second->blocknum;//mq.DebugString().data();
#endif
                if ( bd.blocknum() > it3->second->blocknum )
                    it3->second->blocknum = bd.blocknum();

                //MergeMarketQuote(bd.playerid(),mq);
                lb->updateTopfromCache(mq.b(),mq.a());
            }

            ProcessBookDelta(bd);
        }
        if ( it != NULL ) delete it;
    }


    //MarketTicker *mm = l1.New();
    //mm->CopyFrom(l1);
    //emit NewMarketTicker(mm);


    leveldb::DB *db4;
    status = leveldb::DB::Open(options, filedir("posstore"), &db4);
    posstore.reset(db4);
    if ( !status.ok() ) {
        qCritical() << " cant open " + filedir("posstore");
        //todo emit fatal
        return;
    }
    else {
        auto *it = posstore->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            auto str = it->key().ToString();
            int ii =  str.find_first_of(':');
            auto fname = str.substr(0, ii);
            auto nflplayer = str.substr(ii + 1);

            StorePos sp;
            if ( !sp.ParseFromString(it->value().ToString()) )
                continue;

#ifdef TRACE
            qDebug() << "level2 ExchangeData posstore llllll" << str.data() << sp.DebugString().data() << " |";
#endif
            auto &plist = mPositions[fname];
            Position &pos = plist[nflplayer];
            pos.netprice = sp.price();
            pos.netqty = sp.qty();

            auto it3 = mLimitBooks.find(nflplayer);
            if ( it3 == end(mLimitBooks) ) {

                auto it2 = mLimitBooks.insert(make_pair(nflplayer,
                               unique_ptr<MatchingEngine>(new MatchingEngine(nflplayer,false))));

                if ( !it2.second ) {
                    qWarning() << "level2 unable to insert for" << nflplayer.data();
                    continue;
                }

#ifdef TRACE
                qDebug() << "level2 ExchangeData new for pos" << nflplayer.data();
#endif

                it2.first->second->ResetLimitBook();//mLimitBook.reset(new LimitBook());

                it3 = it2.first;
            }
            it3->second->mPkPos.insert(make_pair(fname,pos));
        }
        delete it;
    }
}

/*
void ExchangeData::MergeMarketQuote(const string &playerid,const MarketQuote &in ) {
    MarketQuote &curr = mMarketQuote[playerid];
    if ( )
}
*/

void ExchangeData::closeAll() {
    std::lock_guard<std::recursive_mutex> lockg{ ex_mutex };
    mPositions.clear();
    mLimitBooks.clear();
//    settlestore.reset();
//    bookdeltastore.reset();
//    posstore.reset();
    mContractOHLC.clear();
    mMarketQuote.clear();
    mOpenOrders.clear();
    mNameSeqMap.clear();
    mSeqNameMap.clear();
    ///snapstore.reset();
    ///
    ///
    {
        auto *it = settlestore->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            settlestore->Delete(write_sync,it->key());
        }
        delete it;
    }

    {
        auto *it = bookdeltastore->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            bookdeltastore->Delete(write_sync,it->key());
        }
        delete it;
    }

    {
        auto *it = posstore->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            posstore->Delete(write_sync,it->key());
        }
        delete it;
    }


}

void ExchangeData::clearNewWeek() {
    closeAll();
//    removeAll();
//    init();
}


void ExchangeData::OnNewOrderMsg(const ExchangeOrder& eo,
                                 int32_t seqnum,
                                 shared_ptr<FantasyName> fn,
                                 int32_t blocknum) {

//    if ( amlive && mSubscribed.find(fn->alias()) != end(mSubscribed))
        //mSubscribed.insert(seqnum);

    //fnames.insert(fn->alias());
    mBookDelta->set_blocknum(blocknum);
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


    qDebug() << eo.playerid().data() << ":newOrder:" << seqnum << " : " << fn->alias().data();

    bool exitonly = fn->getStakeBalance() <= 0;

#ifdef NO_EXITONLY
    exitonly = false;
#endif

    auto pos = getPosition(fn->alias(),eo.playerid());
#ifdef TRACE
    qDebug() << "level2 ExchangeData OnOrderNew exitonly" << exitonly << "pos " << pos.ToString().data() << " stake "
             << fn->getStakeBalance() <<  fn->alias().data() << " fn " << &fn << fn->ToString().data();
#endif
    if ( exitonly &&
         (pos.netqty == 0 || (
         (eo.core().buyside() && pos.netqty > 0) ||
         (!eo.core().buyside() && pos.netqty < 0) ))) {
        qWarning() << "invalid order, exitonly for" << eo.playerid().data();
        return;
    }

    if ( !eo.has_playerid() || eo.playerid() == "") {
#ifdef TRACE
    qDebug() << "level2 ExchangeData OnOrderNew no playerid";
#endif

        return;
    }

#ifdef TRACE
    qDebug() << "level2 ExchangeData OnOrderNew oooooo " << fn->alias().data() << eo.DebugString().data();
#endif

    //MatchingEngine &ma;
    auto it = mLimitBooks.find(eo.playerid());
    if ( it == end(mLimitBooks)) {
        //todo: confirm player_id even exists
#ifdef TRACE
    qDebug() << "level2 ExchangeData OnOrderNew create new book " << eo.playerid().data();
#endif
//todo        if ( !Server::goodPid(eo.playerid())) {
//            qWarning() << "invalid order, bad playerid for" << eo.playerid();
//            return;
//        }

        auto it2 = mLimitBooks.insert(make_pair(eo.playerid(),
               unique_ptr<MatchingEngine>(new MatchingEngine(eo.playerid()))));
        if ( !it2.second ) {
            qWarning() << "unbale to insert for" << eo.DebugString().data();
            return;
        }
        it = it2.first;
        it->second->ResetLimitBook();//mLimitBook.reset(new LimitBook());
    }
    if ( it->second->islocked) {
         qWarning() << "invalid order, locked limitbook for" << eo.playerid().data();
         return;
    }
    MatchingEngine &ma = *(it->second);
    Order ord;
    ord.mutable_core()->CopyFrom(eo.core());
    ord.set_refnum(seqnum);
    mBookDelta->Reset(eo.playerid());

    Position instapos;
    bool haveinstapos = ma.mLimitBook->NewOrder(ord, instapos);

    if ( haveinstapos ) {
#ifdef TRACE
        qDebug() << "level2 OnOrderNew haveinstapos" << instapos.ToString().data();
#endif

        std::lock_guard<std::recursive_mutex> lockg{ ex_mutex };
        Position &mypos = mPositions[fn->alias()][eo.playerid()];
        mypos.netprice += instapos.netprice;
        mypos.netqty += instapos.netqty;
        OnNewPosition(fn->alias(),mypos, eo.playerid());
#ifdef TRACE
        qDebug() << "level2 OnOrderNew haveinstapos" << fn->alias().data() << mypos.ToString().data();
#endif

    }

    //mBookDelta->set_playerid(eo.playerid());
    mBookDelta->set_fantasy_name(fn->alias());
    if ( mBookDelta->level1tic_size() > 0 )
        ma.blocknum = mBookDelta->blocknum();

    SaveBookDelta();

#ifdef TIMEAGENTWRITEFILLS
    if ( !amlive ) return;
    if ( mBookDelta->level1tic_size() > 0 ) {
//        SqlStuff sql("satoshifantasy","md_level1");
        sql.mdlevel1(mBookDelta->playerid(),mMarketQuote[mBookDelta->playerid()]);
    }

#endif
}

void ExchangeData::OnNewPosition(const string &fname,
                                const Position &pos,
                                const string &playerid) {

    StorePos spos;
    spos.set_qty(pos.netqty);
    spos.set_price(pos.netprice);

#ifdef TRACE
        qDebug() << "level2 OnOrderNew OnNewPosition" << fname.data() << pos.ToString().data() << playerid.data();
#endif

    string key(fname + ":" + playerid);
    if (!posstore->Put(write_sync, key, spos.SerializeAsString()).ok())
        qWarning() << " error writing posstore" << fname << playerid;

    mLimitBooks[playerid]->mPkPos[fname] = pos;

    if ( !amlive ) return;

    if ( mSubscribed.find(fname) == end(mSubscribed))
#ifdef TRACE
    {
        qDebug() << "level2 OnOrderNew !subscribed" << fname.data() << pos.ToString().data() << playerid.data();
#endif

         return;

#ifdef TRACE
    }
        qDebug() << "level2 OnOrderNew subscribed emit newPos!! " << fname.data() << pos.ToString().data() << playerid.data();
#endif

    emit NewPos(FullPosition{playerid,fname,pos});
}

void ExchangeData::OnDeltaPos(const string &pid, int32_t seqnum,
                              int32_t deltaqty, int32_t deltapos) {
    std::lock_guard<std::recursive_mutex> lockg{ ex_mutex };

#ifdef TRACE
        qDebug() << "level2 OnDeltaPos subscribed" << pid.data() << seqnum << deltaqty << deltapos;
#endif

    auto it = mSeqNameMap.find(seqnum);
    if ( it == end(mSeqNameMap)) {
        qCritical() << " level2 error expect mSeqNameMap OnDeltaPos" << seqnum;
        return;
    }

    Position &mypos = mPositions[it->second][pid];
#ifdef TRACE
        qDebug() << "level2 OnDeltaPos mypos" << pid.data() << mypos.ToString().data();
#endif

    mypos.netprice += deltapos;
    mypos.netqty += deltaqty;

#ifdef TRACE
        qDebug() << "level2 OnDeltaPos new mypos" << pid.data() << mypos.ToString().data();
#endif

    OnNewPosition(it->second,mypos,pid);

}

void ExchangeData::SaveBookDelta() {
#ifdef TRACE
    qDebug() << "level2 ExchangeData SaveBookDelta" << mBookDelta->DebugString().data();
#endif

    int32_t seqnum = mBookDelta->seqnum();
    leveldb::Slice snum((char*)&seqnum, sizeof(int32_t));
    leveldb::Status st = bookdeltastore->Put(write_sync, snum, mBookDelta->SerializeAsString() );
    if ( !st.ok());
        qWarning() << " error writing bookdeltastore";

    {
        std::lock_guard<std::recursive_mutex> lockg{ ex_mutex };
        ProcessBookDelta(*mBookDelta);
    }
}

void ExchangeData::OnCancelOpenOrder(const OpenOrder &oo,int32_t seqnum) {
    if ( !amlive ) return;

    auto it = mSeqNameMap.find(seqnum);
    if ( it == end(mSeqNameMap)) {
        qCritical() << " level2 error expect mSeqNameMap OnDeltaPos" << seqnum;
        return;
    }

    OnDeltaOpenOrder(it->second,oo,seqnum);

}
void ExchangeData::OnDeltaOpenOrder(const string &fname, const OpenOrder &oo,int32_t seqnum) {
    if ( !amlive ) return;
    if ( mSubscribed.find(fname) == end(mSubscribed))
#ifdef TRACE
    {
        qDebug() << "level2 OnDeltaOpenOrder !subscribed" << fname.data();
#endif

         return;

#ifdef TRACE
    }
    qDebug() << "level2 OnDeltaOpenOrder subscribed emit NewOO" << fname.data() ;
#endif

    Order ord;
    ord.mutable_core()->CopyFrom(oo.livecore);
    ord.set_refnum(seqnum);
    emit NewOO(FullOrderDelta{fname,ord,oo.playerid});
}

void ExchangeData::ProcessBookDelta(const BookDelta &bd) {

#ifdef TRACE
        qDebug() << "level2 ProcessBookDelta add" << bd.seqnum() << bd.fantasy_name().data();
#endif

    auto &myset = mNameSeqMap[bd.fantasy_name()];
    myset.insert(bd.seqnum());
    mSeqNameMap[bd.seqnum()] = bd.fantasy_name();

    if ( bd.has_newnew() ) {
        if ( mOpenOrders.find(bd.seqnum()) != end(mOpenOrders) )
            qCritical() << "level2 ExchangeData already have this NEW order" << bd.newnew().DebugString().data();

        OpenOrder openorder{bd.playerid(), bd.newnew()};
        mOpenOrders.insert(make_pair(static_cast<int32_t>(bd.seqnum()),
                                openorder));

        OnDeltaOpenOrder(bd.fantasy_name(),openorder,bd.seqnum());

#ifdef TRACE
        qDebug() << "level2 ProcessBookDelta add open ord" << bd.seqnum() << bd.playerid().data();
#endif

    }

    for ( auto can : bd.removes()) {
        auto iter = mOpenOrders.find(can.refnum());

        if ( iter != end(mOpenOrders) ) {
            OrderCore &core = iter->second.livecore;

#ifdef TRACE
        qDebug() << "level2 ProcessBookDelta remove" << core.DebugString().data();
#endif

            auto sz = core.size();

            auto newsz = sz - can.core().size();
            core.set_size(newsz);

            OnCancelOpenOrder(iter->second,can.refnum());
            if ( newsz <= 0 )
                mOpenOrders.erase(iter);

        }
        else
            qCritical() << " should already have this REMOVE order" << can.DebugString().data();
    }

}


//BookDelta mBookDelta{};

void ExchangeData::OnOrderCancel(const ExchangeOrder& eo, int32_t seqnum,
                                 shared_ptr<FantasyName> fn) {

#ifdef TRACE
        qDebug() << "level2 ProcessBookDelta OnOrderCancel" << fn->ToString().data() << eo.DebugString().data();
#endif

    string playerid;
    Order ord;
    auto mit = mOpenOrders.find(eo.cancel_oref());
    if ( mit == end(mOpenOrders)) {
        playerid = eo.playerid();
        ord.mutable_core()->CopyFrom(eo.core());
        ord.set_refnum(eo.cancel_oref());
    }
    else {
        playerid = mit->second.playerid;
        ord.mutable_core()->CopyFrom(mit->second.livecore);
        ord.set_refnum(eo.cancel_oref());
    }

    auto it = mLimitBooks.find(playerid);
    if ( it == end(mLimitBooks)) {
        qWarning() << "invalid cancel LimitBook not found for" << eo.DebugString().data();
        return;
    }

    if ( it->second->islocked) {
         qWarning() << "invalid order, locked limitbook for" << eo.DebugString().data();
         return;
    }

    mBookDelta->Reset(playerid);

    MatchingEngine &ma = *(it->second);
    //ord.mutable_core()->CopyFrom(eo.core());
    //ord.set_refnum(eo.cancel_oref());
    mBookDelta->set_seqnum(seqnum);
    ma.mLimitBook->CancelOrder(ord);
    mBookDelta->set_playerid(playerid);
    mBookDelta->set_fantasy_name(fn->alias());
    if ( mBookDelta->level1tic_size() > 0 )
        ma.blocknum = mBookDelta->blocknum();
    SaveBookDelta();

}

Position ExchangeData::getPosition(const string &fname,const string &playerid) {
    std::lock_guard<std::recursive_mutex> lockg{ ex_mutex };
    Position ret{0,0};
    auto &pmap = mPositions[fname];
    auto it = pmap.find(playerid);
    if ( it != end(pmap))
       ret = it->second;

    return ret;
}

/*
void ExchangeData::OnGameResult(const GameResult&gs) {

#ifdef TRACE
    qDebug() << "level2 ExchangeData OnGameResult " << gs.gameid();
#endif

    GameSettlePos gsp;
    if ( !GetGameSettlePos(gs.gameid(),gsp) )
       return;

    for (auto ha : {QString("home"),QString("home")}) {
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
*/
bool ExchangeData::GetGameSettlePos(const string &gid,GameSettlePos &gsp) {
    string temp;
    leveldb::Slice sgid(gid);
    if ( !settlestore->Get(leveldb::ReadOptions(), sgid, &temp).ok() ) {
        qWarning() << "cant read GetGameSettlePos " << gid.data();
        return false;
    }
    gsp.Clear();
    if ( !gsp.ParseFromString(temp) ) {
        qWarning() << "error parsing  GameSettlePos " << gid.data();
        return false;
    }

    return true;
}

void ExchangeData::GetRemainingSettlePos(std::unordered_map<string,BookPos>  &mbp) {

    for ( auto &it : mLimitBooks) {
        if ( it.second->islocked == true) continue;

        BookPos bp{};
        bp.set_playerid(it.first);

        for ( auto p : it.second->mPkPos ) {
            SettlePos &sp = *bp.add_positions();
            sp.set_qty(p.second.netqty);
            sp.set_price(p.second.netprice);
            sp.set_pk(p.first);
        }
        if ( bp.positions_size() > 0 )
            mbp[it.first] = bp;
    }

#ifdef TRACE
    qDebug() << "level2 ExchangeData GetRemainingSettlePos " << mbp.size();
#endif
}

void ExchangeData::OnGameStart(const std::string &gid,
              const std::vector<std::string> &home,
              const std::vector<std::string> &away
              ) {

#ifdef TRACE
    qDebug() << "level2 ExchangeData OnGameStart " << gid.data();
#endif

    GameSettlePos gsp{};
    gsp.set_gameid(gid);

    for (auto ha : { QString("home"),QString("away")})
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
    qDebug() << "level2 ExchangeData OnGameStart locking " << it->first.data();
#endif

            for ( auto p : it->second->mPkPos ) {
                SettlePos sp;
                sp.set_qty(p.second.netqty);
                sp.set_price(p.second.netprice);
                sp.set_pk(p.first);
                bp.add_positions()->CopyFrom(sp);
#ifdef TRACE
    qDebug() << "level2 ExchangeData OnGameStart settlepos oooooo" << sp.DebugString().data();
#endif
            }

        }
        if ( ha == "home")
            gsp.add_home()->CopyFrom(bp);
        else
            gsp.add_away()->CopyFrom(bp);
    }

    if (!settlestore->Put(write_sync, gid, gsp.SerializeAsString()).ok())
        qWarning() << "error writing settlestore" << gid.data();
    else
        qDebug() << "ExchangeData OnGameStart " << gid.data();
}

void ExchangeData::OnWeekOver(int week) {
    //set all results to 0 and proess pnl
#ifdef TRACE
    qDebug() << "level2 ExchangeData OnWeekOver" << week;
#endif

    /*
    for (auto &it : mLimitBooks) {
        ProcessResultOver(it.first,0);
    }
    */
    clearNewWeek();
    mWeek = week;
}


/*
void ExchangeData::ProcessResult(const SettlePos &spos,
                                 std::unordered_map<string,int32_t>::const_iterator result) {
    //set pnl delte position
}

void ExchangeData::ProcessResultOver(const string &key, int32_t result) {
    //set pnl delte position
    //mLimitBooks.erase(key);
}
*/

void ExchangeData::OnMarketTicker(const string &playerid, fantasybit::MarketTicker &mt) {
    //MarketQuote & mq = ;
    mt.set_symbol(playerid);
    MarketQuote &mquote = mMarketQuote[playerid];
    if ( mt.type() == MarketTicker::BID ) {
        mquote.set_b(mt.price());
        mquote.set_bs(mt.size());
    }
    else if ( mt.type() == MarketTicker::ASK){
        mquote.set_a(mt.price());
        mquote.set_as(mt.size());
    }

    if ( amlive ) {
        emit NewMarketTicker(mt,mBookDelta->blocknum());
    }
}

void ExchangeData::OnTrade(const string &playerid, fantasybit::TradeTic *tt) {
    //MarketQuote & mq = ;
    tt->set_symbol(playerid);
    MarketQuote &mquote = mMarketQuote[playerid];
    if ( tt->price() > mquote.l())
        mquote.set_udn(1);
    else if ( tt->price() < mquote.l() )
        mquote.set_udn(-1);
    else
        mquote.set_udn(0);

    tt->set_tic(mquote.udn());
    mquote.set_l(tt->price());
    mquote.set_ls(tt->size());

    auto it = mContractOHLC.find(playerid);
    if ( it == mContractOHLC.end() ) {
        ContractOHLC oh;
        oh.set_symbol(playerid);
        mContractOHLC[playerid] = oh;
    }

    ContractOHLC &myphlc = mContractOHLC[playerid];

    if ( !myphlc.has_high() || tt->price() > myphlc.high()) {
        myphlc.set_high(tt->price());
        tt->set_ishigh(true);
    }
    if ( !myphlc.has_low() || myphlc.low() <= 0 || tt->price() < myphlc.low()) {
        myphlc.set_low(tt->price());
        tt->set_islow(true);
    }
    if ( !myphlc.has_open() || myphlc.open() == 0)
        myphlc.set_open(tt->price());
    myphlc.set_close(tt->price());
    myphlc.set_volume(myphlc.volume()+tt->size());
    auto chg  = myphlc.close() - myphlc.open();
//    if ( !myphlc.has_change() || chg != myphlc.change())
    {
        myphlc.set_change(chg);
        tt->set_change(chg);
    }

    //StoreOhlc(playerid);

    if ( amlive ) {
       emit NewTradeTic(tt);
    }

#ifdef TIMEAGENTWRITEFILLS
    if ( amlive ) {
    SqlStuff sql("satoshifantasy","playerquotes");
    sql.quote(playerid,myphlc);
    }

#endif
}


void LimitBook::SaveRemove(Order &o,int32_t fillqty) {

#ifdef TRACE
        qDebug() << "level2 SaveRemove " << o.DebugString().data() << "fillqty" << fillqty;
#endif

    auto no = mBookDelta->add_removes();
    no->CopyFrom(o);
    no->mutable_core()->set_size(fillqty);

#ifdef TRACE
        qDebug() << "level2 SaveRemove muted" << no->DebugString().data() << "fillqty" << fillqty;
#endif

}

void LimitBook::SaveNew(const OrderCore &oc) {
#ifdef TRACE
    string s = oc.buyside() ? "Bid" : "Ask";
    qDebug() << "level2 New SaveNew " << s.data()  << oc.price() << oc.size();
#endif

    mBookDelta->mutable_newnew()->CopyFrom(oc);
}

void LimitBook::NewTop(int price, int32_t qty, bool isbuy) {
//    MarketTicker *pmt = MarketTicker::default_instance().New();
    MarketTicker mt;// = *pmt;
    mt.set_type(isbuy ? MarketTicker::BID : MarketTicker::ASK);
    mt.set_price(price);
    mt.set_size(qty);
    mBookDelta->add_level1tic()->CopyFrom(mt);
    //emit pExchangeData->get()->NewMarketTicker(pmt);
    pExchangeData->get()->OnMarketTicker(mPlayerid,mt);


#ifdef TRACE
    string s = isbuy ? "Bid" : "Ask";
    qDebug() << "level2 NewTop " <<  s.data() << price << qty;
#endif

}

void LimitBook::NewDepth(bool isbuy,int price) {

#ifdef TRACE
        qDebug() << "level2 NewDepth " << isbuy << "price" << price;
#endif

    DepthFeedDelta *df = DepthFeedDelta::default_instance().New();
//    DepthFeedDelta *df = &dfd;
    df->set_isbid(isbuy);
    df->set_price(price+1);
    df->set_size(isbuy ? mBids[price].totSize : mAsks[price].totSize);


#ifdef TRACE
        qDebug() << "level2 NewDepth " << df->DebugString().data();
#endif

    pExchangeData->get()->OnNewDepth(mPlayerid,df);

}

bool LimitBook::NewOrder(Order &o, Position &posdelta) {
    bool haspos = false;
    //mBookDelta->Clear();
    mBookDelta->set_seqnum(o.refnum());
    //mBookDelta->set_allocated_newnew(o.mutable_core());
    if ( o.core().buyside() ) {
        if ( o.core().price() > 0 && o.core().size() > 0 && o.core().size() <= 100000 )
            haspos = NewBid(o,posdelta);
        else
            qWarning() << "level2 invalid order";
    }
    else {
        if ( o.core().price() <= BOOK_SIZE && o.core().size() > 0 && o.core().size() <= 100000 )
            haspos = NewAsk(o,posdelta);
        else
            qWarning() << "level2 invalid order";
    }

    return haspos;
}

int32_t LimitBook::CancelOrder(Order &order) {
    //mBookDelta->Clear();
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
                NewTop(myprice+1, mBids[myprice].totSize, true);
            else
                GetTop(true);
        }

        NewDepth(true, myprice);
        //} else
        //Send(new BookFeedData(ExecType.CancelReject, order));
    }
    else {
        mAsks[myprice].Cancel(order);
        //Send(new BookFeedData(ExecType.Canceled, order));
        SaveRemove(order,order.core().size());
        if (mBa == myprice) {
            if (mAsks[myprice].totSize > 0)
                NewTop(myprice+1, mAsks[myprice].totSize, false);
            else
                GetTop(false);
        }

        NewDepth(false, myprice);
        //} else
        //    //mFeed.Execution(order, BookFeed.OrdStatus.CancelReject, 0);
        //    Send(new BookFeedData(ExecType.CancelReject, order));
    }
}

MarketSnapshot* MatchingEngine::makeSnapshot(MarketSnapshot *ms) {
    //MarketSnapshot *ms = MarketSnapshot::default_instance().New();
    //ms->set_symbol(symbol);
#ifdef TRACE
    qDebug() << "level2 makeSnapshot";// << symbol;
#endif
    if ( islocked ) return ms;
    ms->set_blocknum(blocknum);
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
    qDebug() << "level2 makeSnapshot" << mPlayerid.data() << "depthsize " << ms->depth_size();
#endif

    //if ( ms->depth_size() > 0 )
    return ms;
}

bool LimitBook::NewBid(Order &order, Position &deltapos) {
#ifdef TRACE
qDebug() << "level2 New Bid " << order.core().price() << order.core().size();
#endif


    auto myprice = order.core().price()-1;
    if ( (myprice < 0) || (myprice >= BOOK_SIZE) )
        return false;

    bool havepos = false;

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
     }
    else {
         int fillqty = order.core().size();
#ifdef TRACE
        qDebug() << " order.core().size() " << order.core().size() << " before";
#endif

         int32_t pos =
                 SweepAsks(order); //will modify size
#ifdef TRACE
        qDebug() << " order.core().size() " << order.core().size() << " after";
#endif

         GetTop(false);
         fillqty = fillqty - order.core().size();

         deltapos.netprice = -pos;
         deltapos.netqty = fillqty;
         havepos = true;

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

    return havepos;
}

bool LimitBook::NewAsk(Order &order, Position &deltapos) {
#ifdef TRACE
    qDebug() << order.refnum() << "level2 New Ask " << order.core().price() << order.core().size();
#endif

    auto myprice = order.core().price()-1;
    if ( (myprice < 0) || (myprice >= BOOK_SIZE) )
       return false;

    bool havepos = false;

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

#ifdef TRACE
        qDebug() << order.refnum() << " order.core().size() " << order.core().size() << " before";
#endif
        int fillqty = order.core().size();
        int32_t pos =
                SweepBids(order); //will modify size
#ifdef TRACE
        qDebug() << order.refnum() << " order.core().size() " << order.core().size() << " after";
#endif

        GetTop(true);
        fillqty = fillqty - order.core().size();

        deltapos.netprice = pos;
        deltapos.netqty = -fillqty;
        havepos = true;

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

    return havepos;
}

int32_t LimitBook::SweepAsks( Order &order) {
    int price = order.core().price()-1;
    int left = order.core().size();
    int32_t pos = 0;
    for (; mBa <= price; ++mBa) {
        InsideBook &curr = mAsks[mBa];
#ifdef TRACE
qDebug() << order.refnum() << "level2 SweepAsks  curr " << mBa << curr.totSize << " ord core " << order.core().size();
#endif
        int fillqty = min(curr.totSize, left);
        if (fillqty <= 0)
            continue;
#ifdef TRACE
qDebug() << order.refnum() << "level2 SweepAsks  fillqty continue " << fillqty;
#endif
        pos += (mBa+1) * fillqty; //for instafill

        SendFill(order, fillqty, mBa, false);
        //left -= fillqty;
        //NewTrade(mBa, fillqty, Side.BID);

        for (auto iiter = curr.top();
             iiter != curr.bot() && left > 0;) {
            Order &ord = *iiter;
#ifdef TRACE
qDebug() << order.refnum() << "level2 SweepAsks  curr ord iityer" << mBa << ord.DebugString().data() << " left " << left;
#endif
            fillqty = min(ord.core().size(), left);
            if (fillqty <= 0)
                continue;

            left -= fillqty;

            SendFill(ord, fillqty, mBa, true);
            //ord.mutable_core()->set_size(ord.core().size()-fillqty);
            SaveRemove(ord,fillqty);
            if (curr.Fill(fillqty, iiter))
                ;//Send(new BookFeedData(ExecType.Done, ord));
        }
#ifdef TRACE
qDebug() << order.refnum() << "level2 SweepAsks  curr bottom" << mBa << curr.totSize;
#endif
        NewDepth(false, mBa);

        if (left <= 0)
            break;
    }

    return pos;
}

int32_t LimitBook::SweepBids( Order &order) {
    int price = order.core().price()-1;
    int left = order.core().size();
    #ifdef TRACE
    qDebug() << order.refnum() << "level2 sweepbids  top ord core" << order.core().size();
    #endif
    int32_t pos = 0;
    for (; mBb >= price; --mBb) {
        InsideBook &curr = mBids[mBb];

#ifdef TRACE
qDebug() << order.refnum() << "level2 sweepbids  curr " << mBb << curr.totSize << " ord core " << order.core().size();
#endif

        int fillqty = min(curr.totSize, left);
        if (fillqty <= 0)
            continue;

#ifdef TRACE
qDebug() << order.refnum() << "level2 sweepbids  fillqty continue " << fillqty;
#endif

        pos += (mBb+1) * fillqty; //for instafill

        SendFill(order, fillqty, mBb, false);
        //NewTrade(mBb, fillqty, Side.ASK);

        for (auto iiter = curr.top();
             iiter != curr.bot() && left > 0;) {
            Order &ord = *iiter;
#ifdef TRACE
qDebug() << order.refnum() << "level2 sweepbids  curr ord iityer" << mBb << ord.DebugString().data() << " left " << left;
#endif

            fillqty = min(ord.core().size(), left);
            if (fillqty <= 0)
                continue;

            left -= fillqty;

            SendFill(ord, fillqty, mBb, true);
            //ord.mutable_core()->set_size(ord.core().size()-fillqty);
            SaveRemove(ord,fillqty);
            if (curr.Fill(fillqty, iiter))
                ;//Send(new BookFeedData(ExecType.Done, ord));
        }

#ifdef TRACE
qDebug() << order.refnum() << "level2 sweepbids  curr bottom" << mBb << curr.totSize;
#endif

        NewDepth(true, mBb);

        if (left <= 0)
            break;
    }

    return pos;
}

void LimitBook::SendFill(Order &o, int32_t q, int price, bool ispassive ) {

#ifdef TRACE
        qDebug() << o.refnum() << "level2 SendFill " << q << price << ispassive << o.DebugString().data();
#endif

    price += 1;
    o.mutable_core()->set_size(o.core().size()-q);

    if ( ispassive ) {
        int32_t deltaqty;
        int32_t deltapos;
        if (o.core().buyside() ) {
            deltaqty = q;
            deltapos = -q * price;
        }
        else {
            deltaqty = -q;
            deltapos = q * price;
        }


        pExchangeData->get()->
                OnDeltaPos(mPlayerid, o.refnum(),deltaqty, deltapos);
    }
    /*
    auto it = pExchangeData->get()->mSeqNameMap.find(o.refnum());
    if ( it == end(pExchangeData->get()->mSeqNameMap))
        qCritical() << "level2 cant find order for fill" << o.DebugString();
    else {

        auto &mypos = mPkPos[it->second];
        auto size = q * (o.core().buyside() ? 1 : -1);
        mypos.netqty += size;
        mypos.netprice += size * (price+1) * -1;
    //fo.refnum(),q,price+1
        pExchangeData->get()->OnOrderPositionChange(o,mypos,this->mPlayerid,it->second);
    }
    */
    if ( ispassive )
        return;

    MarketTicker *lst = mBookDelta->add_level1tic();
    lst->set_type(MarketTicker::LAST);
    lst->set_price(price);
    lst->set_size(q);

    TradeTic *tt = TradeTic::default_instance().New();
    tt->set_price(price);
    tt->set_size(q);

    uint64_t timestamp = (std::chrono::duration_cast<std::chrono::milliseconds>
                      (std::chrono::system_clock::now().time_since_epoch()).count());

    auto dt = currentNewYorkTime();
    pExchangeData->get()->OnTrade(mPlayerid,tt);
#ifdef TIMEAGENTWRITEFILLS
#ifndef TIMEAGENTWRITEFILLS_FORCE
    if ( !pExchangeData->get()->amlive )
        return;

#else
#endif

    qDebug() << " sql epoch " << timestamp;

//    SqlStuff sql("satoshifantasy","ticker");
    sql.fill(mPlayerid,tt,pExchangeData->get()->mWeek,timestamp,o.refnum());

#endif
}

void ExchangeData::OnLive(bool subscribe) {
    amlive = true;
#ifdef TRACE4
qDebug() << "level2 ExchangeData OnLive qqqqqq" << fnames.size();
    for (  auto n : fnames ) {
        qDebug() << "qqqqqq" << n;
    }
#endif

    auto &st = DataService::instance()->GetGlobalState();
    mWeek = st.week();

    doEmitSnap();

}

/*
void ExchangeData::StoreOhlc(string playerid) {
//        if (!snapstore->Put(write_sync, "xxxx",
//                            "xxxxxx").ok())

    if (!snapstore->Put(write_sync, playerid, mContractOHLC[playerid].SerializeAsString()).ok())
        qWarning() << "error writing ohlc";// << ohlc.DebugString();
    else
        if ( amlive )
            qDebug() << "snapstore stored" << mContractOHLC[playerid].DebugString();
        else
            qDebug() << "snapstore stored";
}
*/
void LimitBook::NewNew(Order &order) {
    //pExchangeData->get()->OnNewOrder(order);
    SaveNew(order.core());
    NewDepth(order.core().buyside(), order.core().price()-1);

#ifdef TRACE
        qDebug() << order.refnum() << "level2 NewNew " << order.DebugString().data();
#endif

}

ordsnap_t  ExchangeData::GetOrdersPositionsByName(const std::string &fname) {
    std::lock_guard<std::recursive_mutex> lockg{ ex_mutex };

    mSubscribed.insert(fname);
    ordsnap_t ret{};
//        ordsnap_t::value_type mypair

//      ordsnap_t::value_type::second_type

    auto &mypos = mPositions[fname];
    for ( auto p : mypos) {
        auto &mypair = ret[p.first];
        mypair.first = p.second;
    }
    auto it = mNameSeqMap.find(fname);
    if ( it != end(mNameSeqMap)) {
        for ( auto oid : it->second) {
#ifdef TRACE
            qDebug() << "level2 GetOrdersPositionsByName" << oid;
#endif
            auto it2 = mOpenOrders.find(oid);
            if ( it2 == end(mOpenOrders)) {
#ifdef TRACE
                qDebug() << "level2 GetOrdersPositionsByName not cound" << oid;
#endif
                continue;
            }
            OpenOrder &ord = mOpenOrders.at(oid);
#ifdef TRACE
            qDebug() << "level2 GetOrdersPositionsByName ord.playerid" << ord.playerid.data() << ord.livecore.DebugString().data();
#endif
            //continue;
            auto &mypair = ret[ord.playerid];
            Order o{};
            o.set_refnum(oid);
            o.mutable_core()->CopyFrom(ord.livecore);
            mypair.second.push_back(o);
        }
    }

#ifdef TRACE
    qDebug() << "level2 GetOrdersPositionsByName ret.size() " << ret.size();
#endif
    //ret.clear();

    return ret;
}

void ExchangeData::OnTradeSessionStart(int week) {
//    if ( week == 0 )
    #ifdef TIMEAGENTWRITEFILLS
        if ( !amlive ) return;

        std::lock_guard<std::recursive_mutex> lockg{ ex_mutex };
        //SessionOpen Tics sots;
//        SqlStuff sql("satoshifantasy","openprice");
        for ( auto mdl1 : mMarketQuote)  {
            if ( mdl1.second.has_l() ) continue;

            if ( !mdl1.second.has_b() || !mdl1.second.has_a())
                continue;
            auto &ohlc = mContractOHLC[mdl1.first];
            if ( !ohlc.has_symbol() || ohlc.symbol() == "")
                ohlc.set_symbol(mdl1.first);

            if ( ohlc.has_close() && ohlc.close() > 0)
                continue;

            int price = (mdl1.second.b() * min(1,mdl1.second.bs()) +
                         mdl1.second.a() * min(1,mdl1.second.as()) );
            price = price/2;

            ohlc.set_open(price);
            {
                sql.openprice(mdl1.first,price,week);
            }

  //          SessionOpenTic *sot = sots.add_opentic();
    //        sot->set_playerid(mdl1.first);
      //      sot->set_price(price);

          }
          //settlestore->put(write_sync,"sessionopentic",sots.SerializeAsString());
    #endif
}

#ifdef TIMEAGENTWRITETWEETSX
#include "o1.h"
#include "o2globals.h"
#include "RestFullCall.h"
#include "o1twitter.h"

void ExchangeData::TweetIt(fantasybit::TradeTic *tt) {

    auto sincelast = (std::chrono::duration_cast<std::chrono::minutes>
                       (std::chrono::system_clock::now()).count());


    std::chrono::steady_clock::time_point last_tweet = std::chrono::steady_clock::now();
    //return;
    auto MY_CLIENT_ID = "Kkuh8WBi5O2sTIlRd7XALiNjQ";
    auto MY_CLIENT_SECRET = "7OCwZV9zd58mioyESDewf9TDlsJGUu8niuJYfZRM2dTro51RI5";

    /*
    auto MY_CLIENT_ID = "Kkuh8WBi5O2sTIlRd7XALiNjQ";
    auto MY_CLIENT_SECRET = "7OCwZV9zd58mioyESDewf9TDlsJGUu8niuJYfZRM2dTro51RI5";
    O1Twitter *o1 = new O1Twitter(this);
    o1->setClientId(MY_CLIENT_ID);
    o1->setClientSecret(MY_CLIENT_SECRET);
    o1->link();

    return;
    */
    string mytweet("My Tweet ");

    auto timestamp = QString::number(QDateTime::currentDateTimeUtc().toTime_t()).toLatin1();
    auto nonce = O1::nonce();
    QList<O1RequestParameter> headers;
    headers.append(O1RequestParameter(O2_OAUTH_VERSION, "1.0"));
    headers.append(O1RequestParameter(O2_OAUTH_SIGNATURE_METHOD, "HMAC-SHA1"));
    headers.append(O1RequestParameter(O2_OAUTH_CONSUMER_KEY, "Kkuh8WBi5O2sTIlRd7XALiNjQ"));
    headers.append(O1RequestParameter(O2_OAUTH_NONCE, nonce));
    headers.append(O1RequestParameter(O2_OAUTH_TOKEN, "3305320862-vktlFL7JYwH8bqsIeQdPyV0FGdRdExTyRcvxyqr"));
    headers.append(O1RequestParameter(O2_OAUTH_TIMESTAMP,timestamp ));
    //headers.append(O1RequestParameter("Status",mytweet.data()));

    QString token_secret("HoSKMd5ikQTag1C7drMu72SXoENg3BA9tyBrqrNtFiVxr");
    QString client_secret("7OCwZV9zd58mioyESDewf9TDlsJGUu8niuJYfZRM2dTro51RI5");

    QUrl url("https://api.twitter.com/1.1/statuses/update.json");
    QList<O1RequestParameter> other;
    other.append(O1RequestParameter("Status",mytweet.data()));


    auto signature = O1::sign(headers, other, url,
                              QNetworkAccessManager::PostOperation,
                              client_secret, token_secret);

    headers.append(O1RequestParameter(O2_OAUTH_SIGNATURE,signature));

    auto head = O1::buildAuthorizationHeader(headers);

    qDebug() << " level2 tweet " << head;
    QMap<QString,QString> headersMap;
    QMap<QString,QString> paramsMap;

    headersMap[O2_HTTP_AUTHORIZATION_HEADER] = head;
    paramsMap["Status"] = mytweet.data();
    RestfullClient rest(url);
    rest.postTData("",paramsMap,headersMap);
    auto resp = rest.lastReply();

    qDebug() << resp;

    //RestService
/*
    headers.append(O1RequestParameter(O2_OAUTH_SIGNATURE,
                  generateSignature(headers, request, QList<O1RequestParameter>(),
                                    QNetworkAccessManager::PostOperation)));

    std::string dst("OAuth ");
    dst += "oauth_version=\"1.0\",";
    dst += "oauth_signature_method=\"HMAC-SHA1\",";
    dst += "oauth_consumer_key=\"Kkuh8WBi5O2sTIlRd7XALiNjQ\",";
    dst += "oauth_token=\"3305320862-vktlFL7JYwH8bqsIeQdPyV0FGdRdExTyRcvxyqr\",";
    //oauth_signature
*/
}
#endif


