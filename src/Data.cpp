//
//  Data.cpp
//  fantasybit
//
//  Created by Jay Berg on 3/25/14.
//
//

#include "Data.h"
#include "qdebug.h"
#include <vector>
#include "fbutils.h"
#include "Commissioner.h"
#include "pbutils.h"
#include "DataPersist.h"

using namespace std;
using namespace fantasybit;

#if  defined(DATAAGENTWRITENAMES) || defined(WRITESYMBOLS)
#include "../apps/dataagent-qt/src/SqlStuff.h"
#endif


#ifdef CHECKPOINTS_2015
void NFLStateData::InitCheckpoint() {

    leveldb::WriteOptions write_sync;
//    write_sync.sync = true;
    leveldb::Options options{};
    options.create_if_missing = true;
    leveldb::Status status;


    Reader<GlobalState> reader{ GET_ROOT_DIR() + "bootstrap/GlobalState.txt"};
    GlobalState gs;
    reader.ReadNext(gs);
    leveldb::DB *db2;
    leveldb::DB::Open(options, filedir("statusstore"), &db2);
    db2->Put(leveldb::WriteOptions(), "globalstate", gs.SerializeAsString());

    qDebug() << "zxcvbn" << gs.DebugString();

    leveldb::DB *db4;
    leveldb::DB::Open(options, filedir("staticstore"), &db4);

    Reader<ScheduleData> reader5{ GET_ROOT_DIR() + "bootstrap/WeeklySchedule.txt" };
    ScheduleData sd;
    while ( reader5.ReadNext(sd) ) {
        string key = "scheduleweek:" + to_string(sd.week());
        db4->Put(write_sync, key,
                       sd.weekly().SerializeAsString() );
    }

    Reader<GameData> reader2{ GET_ROOT_DIR() + "bootstrap/GameData.txt" };
    GameData gas;
    while ( reader2.ReadNext(gas) ) {
        gas.ByteSize();
        string key = "gamestatus:" + gas.gameid();
        db2->Put(leveldb::WriteOptions(), key, gas.status().SerializeAsString());

    }

    Reader<GameResult> reader22{ GET_ROOT_DIR() + "bootstrap/GameResult.txt" };
    GameResult gr;
    while ( reader22.ReadNext(gr) ) {
        gas.ByteSize();
        string key = "gameresult:" + gr.gameid();
        db4->Put(write_sync, key,
                       gr.SerializeAsString() );

    }



    leveldb::DB *db3;
    leveldb::DB::Open(options, filedir("playerstore"), &db3);
    Reader<PlayerData> reader3{ GET_ROOT_DIR() + "bootstrap/PlayerData.txt"};
    PlayerData pd;
    while ( reader3.ReadNext(pd) ) {
        pd.ByteSize();
        db2->Put(write_sync, pd.playerid(),
                       pd.player_status().SerializeAsString() );
        db3->Put(write_sync, pd.playerid(),
                       pd.player_base().SerializeAsString() );

    }
    delete db2;
    delete db3;
    delete db4;

    leveldb::DB *db5;
    status = leveldb::DB::Open(options, filedir("namestore"), &db5);

    Reader<FantasyNameBal> reader4{ GET_ROOT_DIR() + "bootstrap/FantasyNameBal.txt"};
    FantasyNameBal fnb;
    while ( reader4.ReadNext(fnb) ) {
        auto hash = FantasyName::name_hash(fnb.name());
        leveldb::Slice hkey((char*)&hash, sizeof(hash_t));
        db5->Put(write_sync, hkey, fnb.SerializeAsString());
        qDebug() << "zxcvbn" << fnb.DebugString();
        fnb.Clear();
    }
    delete db5;

}
#endif

#if !defined(NOUSE_GENESIS_BOOT) || !defined(NOCHECK_LOCAL_BOOTSTRAP)
#include "PeerNode.h"
void NFLStateData::InitCheckpoint(bool onlyresult) {

    pb::make_all(filedir(""));

    leveldb::WriteOptions write_sync{};
//    write_sync.sync = true;
    leveldb::Options options{};
    options.create_if_missing = true;
    leveldb::Status status{};

    leveldb::DB *db2{};
    leveldb::DB::Open(options, filedir("statusstore"), &db2);
    leveldb::DB *db4{};
    leveldb::DB::Open(options, filedir("staticstore"), &db4);
    leveldb::DB *db3{};
    leveldb::DB::Open(options, filedir("playerstore"), &db3);
    leveldb::DB *db5{};
    leveldb::DB::Open(options, filedir("namestore"), &db5);
    leveldb::DB *db6{};
    string file = GET_ROOT_DIR();
    file += "trade/posstore";
    leveldb::DB::Open(options, file, &db6);


    Bootstrap head{};
    LdbWriter ldb{};
    ldb.init(Node::bootstrap.get());
    ldb.read(ldb.read(ldb.read("head")),head);

    if ( !onlyresult ) {
        Commissioner::WK.SetSeason(head.season());
        GlobalState gs;
        gs.set_season(head.season());
        gs.set_week(head.week());
        if ( gs.week() >= 1 && gs.week() <= Commissioner::WK.FFC)
            gs.set_state(GlobalState_State_INSEASON);
        else
            gs.set_state(GlobalState_State_OFFSEASON);

        db2->Put(leveldb::WriteOptions(), "globalstate", gs.SerializeAsString());
        qDebug() << "InitCheckpoint wrote GlobalState" << gs.DebugString().data();
        {
            MerkleTree mtree;
            std::vector< std::pair<std::string,  GameStatusMeta> > mapt;
            pb::loadMerkleMap(ldb,head.gamemetaroot(),mtree,mapt);

            std::unordered_map<int, unordered_map<int,WeeklySchedule>> wsm;
            for ( auto p : mapt) {
                GameStatusMeta &gsm = p.second;
                auto its = wsm.find(gsm.season());
                if ( its == end(wsm) )
                    wsm.insert({gsm.season(),unordered_map<int,WeeklySchedule>()});

                auto &mwsm = wsm[gsm.season()];
                auto it = mwsm.find(gsm.week());
                if ( it == end(mwsm) ) {
                    mwsm.insert({gsm.week(),WeeklySchedule::default_instance()});
                }
                WeeklySchedule &ws = mwsm[gsm.week()];
                ws.add_games()->CopyFrom(gsm.gameinfo());
                if ( gsm.has_gamesatus() ) {
                    string key = "gamestatus:" + gsm.id();
                    db2->Put(leveldb::WriteOptions(), key, gsm.gamesatus().SerializeAsString());
                }
            }
            for ( auto wgs : wsm ) {
                int se = wgs.first;
                for ( auto wg : wgs.second ) {
                    string key = to_string(se)  + "scheduleweek:" + to_string(wg.first);
                    db4->Put(write_sync, key,
                                   wg.second.SerializeAsString() );
                }
            }
        }


        {
            MerkleTree mtree;
            std::vector< std::pair<std::string,  PlayerMeta> > mapt;
            pb::loadMerkleMap(ldb,head.playermetaroot(),mtree,mapt);

            for ( auto p : mapt) {
                PlayerMeta &pm = p.second;
                if ( pm.has_player_status() )
                    db2->Put(write_sync, pm.playerid(),
                                   pm.player_status().SerializeAsString() );
                db3->Put(write_sync, pm.playerid(),
                               pm.player_base().SerializeAsString() );
            }
        }


        {
            MerkleTree mtree;
            std::vector< std::pair<std::string,  FantasyNameBalMeta> > mapt;
            pb::loadMerkleMap(ldb,head.fnamemetaroot(),mtree,mapt);

    #if defined(DATAAGENTWRITENAMES_FORCE) && !defined(DATAAGENTWRITENAMES_FORCE_NONAMES)
                SqlStuff sql("satoshifantasy","OnFantasyName");
    #endif
            for ( auto p : mapt) {
                FantasyNameBalMeta &pm = p.second;
                FantasyNameBal fnb;
                fnb.set_name(pm.name());
                fnb.set_public_key(pm.public_key());
                fnb.set_bits(pm.bits());
                fnb.set_stake(pm.stake());
                auto hash = FantasyName::name_hash(fnb.name());
                leveldb::Slice hkey((char*)&hash, sizeof(hash_t));
                db5->Put(write_sync, hkey, fnb.SerializeAsString());
    //            qDebug() << "zxcvbn" << fnb.DebugString();
    #if defined(DATAAGENTWRITENAMES_FORCE) && !defined(DATAAGENTWRITENAMES_FORCE_NONAMES)
                FantasyNameHash fnh{};
                fnh.set_name(fnb.name());
                fnh.set_hash(hash);
                qDebug() << " data write fnh " << fnh.DebugString();
                sql.fantasyname(fnh);
    #endif
                fnb.Clear();

            }
        }


        {
            MerkleTree mtree;
            std::vector< std::pair<std::string,  PosMeta> > mapt;
            pb::loadMerkleMap(ldb,head.posmetaroot(),mtree,mapt);

            for ( auto p : mapt) {
                PosMeta &pm = p.second;
                StorePos sp;
                sp.set_price(pm.price());
                sp.set_qty(pm.qty());

                string key(pm.name() + ":" + pm.playerid());
                db6->Put(write_sync, key, sp.SerializeAsString());
                sp.Clear();
            }
        }
    }

    //if onlyresult
    {
        MerkleTree mtree;
        std::vector< std::pair<std::string,  GameResult> > mapt;
        pb::loadMerkleMap(ldb,head.gameresultroot(),mtree,mapt);

        for ( auto p : mapt) {
            GameResult &gr = p.second;
            string key = "gameresult:" + gr.gameid();
            db4->Put(write_sync, key,
                           gr.SerializeAsString() );
            gr.Clear();
        }
    }


    delete db2;
    delete db3;
    delete db4;
    delete db5;
    delete db6;
}
#endif

void NFLStateData::init() {
    pb::make_all(filedir(""));

//    write_sync.sync = true;
    leveldb::Options options{};
    options.create_if_missing = true;
    leveldb::Status status;

    leveldb::DB *db1;
    status = leveldb::DB::Open(options, filedir("staticstore"), &db1);
    staticstore.reset(db1);
    if ( !status.ok() ) {
        qCritical() << " cant open " << filedir("staticstore").data ();
        //todo emit fatal
        return;
    }

    leveldb::DB *db2;
    status = leveldb::DB::Open(options, filedir("statusstore"), &db2);
    statusstore.reset(db2);
    if ( !status.ok() ) {
        qCritical() << " cant open " << filedir("statusstore").data ();
        //todo emit fatal
        return;
    }

    leveldb::DB *db3;
    status = leveldb::DB::Open(options, filedir("playerstore"), &db3);
    playerstore.reset(db3);
    if ( !status.ok() ) {
        qCritical() << " cant open " << filedir("playerstore").data ();
        //todo emit fatal
        return;
    }

    leveldb::DB *db4;
    status = leveldb::DB::Open(options, filedir("statsstore"), &db4);
    statsstore.reset(db4);
    if ( !status.ok() ) {
        qCritical() << " cant open " << filedir("statsstore").data ();
        //todo emit fatal
        return;
    }

#ifdef WRITE_BOOTSTRAP
    {
    Writer<GlobalState> writer{ GET_ROOT_DIR() + "bootstrap/GlobalState.txt"};

    GlobalState gs{};
    std::string temp;
    if (statusstore->Get(leveldb::ReadOptions(), "globalstate", &temp).ok()) {
        if (gs.ParseFromString(temp))
            writer(gs);
    }
    }
#endif

    {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };


#ifdef WRITESYMBOLS_FORCE_INITALL
        SqlStuff sql("tfprod","symbolstatausforce");
#endif

#ifdef WRITE_BOOTSTRAP
        //Writer<PlayerStatus> writer{ GET_ROOT_DIR() + "bootstrap/PlayerStatus.txt" };
        Writer<PlayerData> writer3{ GET_ROOT_DIR() + "bootstrap/PlayerData.txt"};
#endif


        auto *it = playerstore->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
#ifdef WRITE_BOOTSTRAP
            PlayerBase pb;
            pb.ParseFromString(it->value().ToString());
            PlayerData pd;
            pd.mutable_player_base()->CopyFrom(pb);
            pd.set_playerid(it->key().ToString());
#endif
            /*
            PlayerBase pb;
            pb.ParseFromString(it->value().ToString());
            PlayerData pd;
            pd.mutable_player_base()->CopyFrom(pb);
            pd.set_playerid(it->key().ToString());
            qDebug() << pd.DebugString().data();
            */
            string temp;
            if ( !statusstore->Get(leveldb::ReadOptions(), it->key(), &temp).ok() )
                continue;
            PlayerStatus ps;
            if (!ps.ParseFromString(temp) ) {
                continue;
            }
#ifdef WRITE_BOOTSTRAP
            pd.mutable_player_status()->CopyFrom(ps);
            writer3(pd);
#endif

            auto pid = it->key().ToString();
            MyPlayerStatus[pid] = ps;
            if (ps.has_teamid()) {
                //qDebug() << ps.teamid();
                auto itr = MyTeamRoster.find(ps.teamid());
                if ( itr == end(MyTeamRoster))
                    MyTeamRoster[ps.teamid()] =  std::unordered_set<std::string>{};

                MyTeamRoster[ps.teamid()].insert(it->key().ToString());
            }
            if ( ps.symbol() != "" ) {
                FromTicker(ps.symbol());
                mSym2Pid[ps.symbol()] = pid;
#ifdef WRITESYMBOLS_FORCE_INITALL
                sql.playerStatus(ps,pid);
#endif
            }
            else
                qWarning() << " no ticker!! " << ps.DebugString().data();
        }
        delete it;
    }

    {
#ifdef WRITE_BOOTSTRAP
        Writer<ScheduleData> writer{ GET_ROOT_DIR() + "bootstrap/WeeklySchedule.txt" };

        Writer<GameData> writer2{ GET_ROOT_DIR() + "bootstrap/GameData.txt" };
        Writer<GameResult> writer3{ GET_ROOT_DIR() + "bootstrap/GameResult.txt" };
#endif
        auto gs = GetGlobalState();
        Commissioner::WK.SetSeason(gs.season());

        for (int i=1; i<=Commissioner::WK.NFL;i++) {
            string key = to_string(gs.season()) + "scheduleweek:" + to_string(i);
            string temp;
            if ( !staticstore->Get(leveldb::ReadOptions(), key, &temp).ok() ) {
                qWarning() << "NFLStateData::init cant find schedule " << key.data();
                qDebug() << gs.DebugString().data();
                break;
            }
            WeeklySchedule ws;
            if ( !ws.ParseFromString(temp) ) {
                qCritical() << "bad read WeeklySchedule ";
                continue;
            }


#ifdef WRITE_BOOTSTRAP
            ScheduleData sd;
            sd.set_week(i);
            sd.mutable_weekly()->CopyFrom(ws);
            writer(sd);
#endif
            for ( auto game : ws.games()) {
                MyGameInfo[game.id()] = game;
#ifdef WRITE_BOOTSTRAP
                string key = "gamestatus:" + game.id();
                string temp;
                if ( statusstore->Get(leveldb::ReadOptions(), key, &temp).ok() ) {
                    GameStatus gs;
                    if (gs.ParseFromString(temp) ) {
                        GameData gd;
                        gd.set_gameid(game.id());
                        gd.mutable_status()->CopyFrom(gs);
                        writer2(gd);
                    }
                }
                GameResult gamer;
                if ( GetGameResult(game.id(),gamer))
                    writer3(gamer);
#endif

            }
        }

    }

}

void NFLStateData::closeAll() {
    std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
    amlive = false;
    MyTeamRoster.clear();
    MyPlayerStatus.clear();
    MyGameInfo.clear();
    playerstore.reset();
    staticstore.reset();
    statusstore.reset();
    statsstore.reset();
}

GameInfo NFLStateData::GetGameInfo(string gameid) {
    std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
    return MyGameInfo[gameid];
}

void NFLStateData::AddNewPlayer(std::string playerid, const PlayerBase &pb) {
    playerstore->Put(write_sync, playerid, pb.SerializeAsString());
#ifdef TRACE
    qDebug() << pb.DebugString().data();
#endif
}

PlayerBase NFLStateData::GetPlayerBase(std::string playerid) {
    PlayerBase pb{};
    string temp;
    if ( playerstore->Get(leveldb::ReadOptions(), playerid, &temp).ok() )
        pb.ParseFromString(temp);

    return pb;
}

PlayerStatus NFLStateData::GetPlayerStatus(const std::string &playerid) {
    auto it = MyPlayerStatus.find(playerid);
    if ( it != end(MyPlayerStatus))
        return it->second;
    else
        return PlayerStatus::default_instance();
}

PlayerResult NFLStateData::GetPlayerStats(const std::string &playerid) {
    PlayerResult pr{};
    string temp;
    if ( statsstore->Get(leveldb::ReadOptions(), playerid, &temp).ok() )
        pr.ParseFromString(temp);

    return pr;
}

void NFLStateData::TeamNameChange(const std::string &playerid, const PlayerBase &pb,
                                  const PlayerStatus &ps) {
    AddNewPlayer(playerid,pb);
    string temp;
    if ( !statusstore->Get(leveldb::ReadOptions(), playerid, &temp).ok() ) {
        qDebug() << " error TeamNameChange " << pb.DebugString().data();
        return;
    }
    PlayerStatus mps;
    mps.ParseFromString(temp);
    if ( mps.teamid() == ps.teamid() || ps.teamid() == "" ) {
        qDebug() << " error TeamNameChange mps.teamid() == ps.teamid()";
        return;
    }

    statusstore->Put(write_sync, playerid, ps.SerializeAsString());
    MyPlayerStatus[playerid] = ps;
    auto it = MyTeamRoster.find(mps.teamid());
    if ( it == end(MyTeamRoster)) {
        qDebug() << "error cant find  it" << mps.teamid().data();
    }

    std::swap(MyTeamRoster[ps.teamid()], it->second);


    PlayerStatus newps;
    newps.set_teamid(ps.teamid());
    for ( auto &p : it->second) {
        auto it2 = MyPlayerStatus.find(playerid);
        if ( it2 == end(MyPlayerStatus)) {
            newps.set_status(PlayerStatus_Status_OTHER);
        }
        else {
            newps.set_status(it2->second.status());
        }

        MyPlayerStatus[playerid] = newps;
    }

    MyTeamRoster.erase(mps.teamid());
}

void NFLStateData::OnSeasonEnd(int season) {
    for ( int i=0;i<Commissioner::WK.FFC;i++) {
        auto ws = GetWeeklySchedule(season,i+1);

        string key = to_string(season) + "scheduleweek:" + to_string(i+1);
        if ( !staticstore->Put(write_sync, key, ws.SerializeAsString()).ok()) {
            qWarning() << " error writing schecule";
        }

        {
            std::lock_guard<std::recursive_mutex> lockg{ data_mutex };

            for ( auto game : ws.games()) {
                string key = "gamestatus:" + game.id();
                if (!statusstore->Delete(write_sync, key).ok())
                    qWarning() << " error deleting gamestatus";
            }
        }
    }

    {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
        MyGameInfo.clear();
    }
    //        makeBootStrap(season,18,0);


    {

    SeasonResult sr;
    sr.set_season(season);
    PlayerResult pr;
    auto *it = statsstore->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        if ( !pr.ParseFromArray(it->value().data(),it->value().size()) ) {
            qWarning() << " error parsing PlayerResult "  << season;
            continue;
        }
        sr.add_player_results()->CopyFrom(pr);
        sr.set_total(sr.total() + pr.result());
        statsstore->Delete(write_sync,it->key());
    }
    delete it;

    qInfo() << season << "Season total" << sr.total();//pr.DebugString().data();

    if ( !statsstore->Put(write_sync, to_string(season), sr.SerializeAsString()).ok())
        qWarning() << " error writing season "  << season;
    else
        ;//qDebug() << sr.DebugString().data();

    }

}

void NFLStateData::AddNewWeeklySchedule(int season,int week, const WeeklySchedule &ws) {
    string key = to_string(season) + "scheduleweek:" + to_string(week);
    if ( !staticstore->Put(write_sync, key, ws.SerializeAsString()).ok()) {
        qWarning() << " error writing schecule";
        return;
    }
    else {
        qDebug() << "AddNewWeeklySchedule " << key.data();
    }
#ifdef TRACE
    qDebug() << ws.DebugString().data();
#endif
    GameStatus gs{};
    gs.set_status(GameStatus::SCHEDULED);

    {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };

        for ( auto game : ws.games()) {
            MyGameInfo[game.id()] = game;
            UpdateGameStatus(game.id(),gs);
        }
    }

}

void NFLStateData::ProcessAddGameResult(const std::string &gameid, const GameResult&gs) {
    AddGameResult(gameid,gs);
}

void NFLStateData::AddGameResult(const std::string &gameid, const GameResult&gs) {
    string key = "gameresult:" + gameid;
    if (!staticstore->Put(write_sync, key, gs.SerializeAsString()).ok()) {
        qWarning() << "cant add gameresult" << gameid;
    }
//    qDebug() << key.data ()<< gs.DebugString().data ();
    if ( amlive )
        emit NewGameResult(gameid);

    OnGameClosed(gameid);
}

bool NFLStateData::GetGameResult(const std::string &gameid, GameResult &result) {
    string key = "gameresult:" + gameid;
    string temp;
    if (!staticstore->Get(leveldb::ReadOptions(), key, &temp).ok())
        qWarning() << gameid.data() << " not found";
    else {
        if (!result.ParseFromString(temp) )
            qWarning() << " cant parse game result";
        else {
#ifdef TRACE4
            qDebug() << result.DebugString().data();
#endif
            return true;
        }
    }
    return false;
}

void NFLStateData::UpdatePlayerStatus(const std::string &playerid, const PlayerStatus &ps) {
    std::lock_guard<std::recursive_mutex> lockg{ data_mutex };

    auto it = MyPlayerStatus.find(playerid);
    if ( it == end(MyPlayerStatus)) {
        PlayerStatus ps2(ps);
        ps2.set_symbol(GenerateTicker(playerid,ps));

#ifdef WRITESYMBOLS
#ifndef WRITESYMBOLS_FORCE
        if ( amlive )
#endif
        {
            qDebug() << " try write status " << ps2.DebugString().data();
            SqlStuff sql("tfprod","symbolstataus");
            sql.playerStatus(ps2,playerid);
        }
#endif
        if ( !statusstore->Put(write_sync, playerid, ps2.SerializeAsString()).ok())
            qWarning() << "bad write statusstore";
#ifdef TRACE
        qDebug() << "UpdatePlayerStatus" << ps2.DebugString().data();
#endif
        mSym2Pid[ps2.symbol()] = playerid;
        MyPlayerStatus[playerid] = ps2;
        OnNewPlayer(playerid);
        if ( ps.has_teamid())
            OnPlayerSign(playerid,ps2.teamid());
    }
    else {
        PlayerStatus old = it->second;
        PlayerStatus ps2(ps);

        if ( ps2.has_symbol() && ps2.symbol() != "");
        else
            ps2.set_symbol(old.symbol());

#ifdef WRITESYMBOLS_FORCE_SPECIAL
#ifndef WRITESYMBOLS_FORCE
        if ( amlive )
#endif
        {
            qDebug() << " special try write status " << ps2.DebugString().data();
            SqlStuff sql("tfprod","symbolstatus");
            sql.playerStatus(ps2,playerid);
        }
#endif
        if ( !statusstore->Put(write_sync, playerid, ps2.SerializeAsString()).ok())
            qWarning() << "bad write statusstore";

        MyPlayerStatus[playerid] = ps2;

        if ( old.has_teamid() ) {
            if ( ps2.has_teamid() ) {
                if ( old.teamid() != ps2.teamid()) {
                    OnPlayerTrade(playerid,old.teamid(),ps2.teamid());
                }
                if ( ps2.has_status() ) {
                    if ( !old.has_status() )
                        OnPlayerStatus(playerid,ps2);
                    else if ( old.status() != ps.status())
                        OnPlayerStatus(playerid,ps2);
                }
            }
            else {
                OnPlayerRelease(playerid,old.teamid());
            }
        }
        else if ( ps2.has_teamid() )
            OnPlayerSign(playerid,ps2.teamid());
    }
}

void NFLStateData::UpdatePlayerStats(const PlayerResult &pr) {
    const PlayerResult &curr = GetPlayerStats(pr.playerid());
    PlayerResult next;
    next.set_playerid(pr.playerid());

    if ( curr.result() > 0 ) {
        next.set_result(curr.result() + pr.result());

        Stats *ns = next.mutable_stats();

        //Ostats
        if ( curr.stats().has_ostats() && pr.stats().has_ostats()) {
            Ostats *n = ns->mutable_ostats();
            const Ostats &c = curr.stats().ostats();
            const Ostats &p = pr.stats().ostats();

            n->set_passtd(c.passtd() + p.passtd());
            n->set_passyds(c.passyds() + p.passyds());
            n->set_rushyds(c.rushyds() + p.rushyds());
            n->set_rushtd(c.rushtd() + p.rushtd());
            n->set_recyds(c.recyds() + p.recyds());
            n->set_rectd(c.rectd() + p.rectd());
            n->set_rec(c.rec() + p.rec());
            n->set_pint(c.pint() + p.pint());
            n->set_fumble(c.fumble() + p.fumble());
            n->set_twopt(c.twopt() + p.twopt());
        }
        else if ( curr.stats().has_ostats() ) {
            if ( pr.stats().has_ostats() )
                ns->mutable_ostats()->CopyFrom(pr.stats().ostats());
        }
        else if ( pr.stats().has_ostats() ) {
            if ( curr.stats().has_ostats() )
                ns->mutable_ostats()->CopyFrom(curr.stats().ostats());
        }

        //Dstats
        if ( curr.stats().has_dstats() && pr.stats().has_dstats()) {

        }
        else if ( curr.stats().has_dstats() ) {
            if ( pr.stats().has_dstats() )
                ns->mutable_dstats()->CopyFrom(pr.stats().dstats());
        }
        else if ( pr.stats().has_dstats() ) {
            if ( curr.stats().has_dstats() )
                ns->mutable_ostats()->CopyFrom(curr.stats().dstats());
        }

        //kstats
        if ( curr.stats().has_kstats() && pr.stats().has_kstats()) {

        }
        else if ( curr.stats().has_kstats() ) {
            if ( pr.stats().has_kstats() )
                ns->mutable_kstats()->CopyFrom(pr.stats().kstats());
        }
        else if ( pr.stats().has_kstats() ) {
            if ( curr.stats().has_kstats() )
                ns->mutable_ostats()->CopyFrom(curr.stats().kstats());
        }

    }
    else
        next.CopyFrom(pr);

    statsstore->Put(write_sync, next.playerid(), next.SerializeAsString());
    //qDebug() << QString::fromStdString(ps.DebugString());
}


void NFLStateData::OnNewPlayer(const std::string &pid) {
    if ( amlive )
        emit PlayerAdd(GetPlayerBase(pid));
}

void NFLStateData::OnPlayerTrade(const std::string &pid, const std::string &tid,
                         const std::string &ntid) {
    removePlayerTeam(pid,tid);
    addPlayerTeam(pid,ntid);
    if ( amlive ) {
        emit PlayerStatusChange(*MyPlayerStatus.find(pid));
    }
}

void NFLStateData::OnPlayerRelease(const std::string &pid, const std::string &tid) {
    removePlayerTeam(pid,tid);
    if ( amlive ) {
        emit PlayerStatusChange(*MyPlayerStatus.find(pid));
    }
}

void NFLStateData::OnPlayerSign(const std::string &pid, const std::string &tid) {
    addPlayerTeam(pid,tid);
    if ( amlive ) {
        emit PlayerStatusChange(*MyPlayerStatus.find(pid));
    }
}

void NFLStateData::OnPlayerStatus(const std::string &pid,PlayerStatus ps) {
    if ( amlive ) {
        emit PlayerStatusChange(*MyPlayerStatus.find(pid));
    }
}

void NFLStateData::UpdateGameStatus(const std::string &gameid, const GameStatus &gs, bool force) {
    GameStatus use;
    if ( gs.status() == GameStatus_Status_SCHEDULED || force) {
        use.CopyFrom(gs);
    }
    else {
        use = GetUpdatedGameStatus(gameid);
        if ( use.status() == GameStatus_Status_SCHEDULED ) {
            use.set_status(gs.status());
        }
    }

    string key = "gamestatus:" + gameid;
    if (!statusstore->Put(write_sync, key, use.SerializeAsString()).ok())
        qWarning() << "!ok" << "cant update status";
}

void NFLStateData::OnWeekOver(int in) {
    std::lock_guard<std::recursive_mutex> lockg{ data_mutex };

    qDebug() << "week over" << in << " commit result";

    auto ws = GetWeeklySchedule(theSeason(),in);
    for (auto game : ws.games() ) {
        GameResult gs{};
        if ( !GetGameResult(game.id(),gs) ) {
            qWarning() << " no result " << game.id().data();
            continue;
        }

        auto tr = GetTeamRoster(game.home());
        for ( auto pr : gs.home_result() ) {
            tr.erase(pr.playerid());
        }

        //set players who plaid to 0.0
        for ( auto p : tr ) {
            PlayerResult pr{};
            pr.set_playerid(p.first);
            pr.set_result(0.0);
            gs.add_home_result()->CopyFrom(pr);
        }

        tr = GetTeamRoster(game.away());
        for ( auto pr : gs.away_result() ) {
            tr.erase(pr.playerid());
        }

        //set players who plaid to 0.0
        for ( auto p : tr ) {
            PlayerResult pr{};
            pr.set_playerid(p.first);
            pr.set_result(0.0);
            gs.add_away_result()->CopyFrom(pr);
        }

        AddGameResult(game.id(),gs);
        GameStatus games{};
        games.set_datetime(gs.kickofftime());
        games.set_status(GameStatus::CLOSED);
        UpdateGameStatus(game.id(),games);
    }
}

void NFLStateData::OnWeekStart(int in) {
    //week = in;
}

WeeklySchedule NFLStateData::GetWeeklySchedule(int season,int week) {

    auto ws = getWeeklyStaticSchedule(season,week);
    if ( season != theSeason() )
        return ws;

    auto sz = ws.games().size();
    for ( int i = 0; i<sz; i++) {
        GameInfo *g = ws.mutable_games(i);
        GameStatus gs = GetUpdatedGameStatus(g->id());
        if ( gs.datetime() != -1)
        {
            if ( gs.has_datetime() )
                g->set_time(gs.datetime());
        }
    }

    return ws;
}

WeeklySchedule NFLStateData::getWeeklyStaticSchedule(int season,int week) {
    WeeklySchedule ws{};
    string temp;
    string key = to_string(season) + "scheduleweek:" + to_string(week);
    if ( !staticstore->Get(leveldb::ReadOptions(), key, &temp).ok() ) {
        qWarning() << "getWeeklyStaticSchedule cant find schedule " << key.data();
        return ws;
    }
    ws.ParseFromString(temp);

    return ws;
}

GameStatus NFLStateData::GetUpdatedGameStatus(string id) {
    GameStatus gs{};
    gs.set_datetime(-1);

    string key = "gamestatus:" + id;
    string temp;
    if ( !statusstore->Get(leveldb::ReadOptions(), key, &temp).ok() ) {
        qWarning() << "cant find game status " << key;
    }
    else {
        gs.ParseFromString(temp);
        if ( gs.has_datetime() && gs.datetime() < 1)
            gs.set_datetime(-1);
//        qDebug() << key.data() << gs.status().DebugString().data();
     }
    return gs;
}

vector<GameRoster> NFLStateData::GetCurrentWeekGameRosters() {

    vector<GameRoster> retgr{};
    std::map<GameStatus_Status,vector<GameRoster>> mretgr;

//    if ( !amlive ) {
//        qWarning() << "am not live" ;
//    }

    WeeklySchedule ws = getWeeklyStaticSchedule(theSeason(),week());

    for (const auto g : ws.games()) {
        GameRoster gr{};
        gr.info = g;

        GameStatus gs = GetUpdatedGameStatus(g.id());
        if ( gs.datetime() != -1)
        {
            if ( gs.has_datetime() )
                gr.info.set_time(gs.datetime());

            if ( gs.has_status())
               gr.status = gs.status();
        }

        gr.homeroster = GetTeamRoster(g.home());
        gr.awayroster = GetTeamRoster(g.away());

        mretgr[gr.status].push_back (gr);
    }

    for(auto &vecs : mretgr)
        retgr.insert(retgr.end(),
                     std::make_move_iterator(vecs.second.begin()),
                     std::make_move_iterator(vecs.second.end()));

    return retgr;
}

std::vector<fantasybit::GameResult> NFLStateData::GetPrevWeekGameResults(int season,int week) {
    std::vector<fantasybit::GameResult> ret{};
    auto s = GetGlobalState();
    if  (s.season() == season && s.week() < week && s.week() != 0)
        return ret;

    auto ws = GetWeeklySchedule(season,week);
    for (const auto g : ws.games()) {
        GameResult gr{};
        if (GetGameResult(g.id(),gr) )
            ret.push_back(gr);
    }

    return ret;
}

std::unordered_map<std::string,PlayerDetail>
        NFLStateData::GetTeamRoster(const std::string &teamid) {
    std::lock_guard<std::recursive_mutex> lockg{ data_mutex };

    std::unordered_map<std::string,PlayerDetail> vpb{};

    for ( auto p : MyTeamRoster.at (teamid)) {
        vpb[p] = GetPlayerDetail(p,true);
    }
    return vpb;
}

PlayerDetail NFLStateData::GetPlayerDetail(const std::string &symbol,bool ispid) {
    auto p = ispid ? symbol : mSym2Pid[symbol];
    auto ps = MyPlayerStatus[p];

    PlayerDetail pdt;
    pdt.base = GetPlayerBase(p);
    pdt.team_status = ps.status();
    pdt.game_status = PlayerGameStatus::NA;
    pdt.symbol = ps.symbol();
    pdt.team = ps.teamid();
    return pdt;
}

GlobalState NFLStateData::GetGlobalState() {
    GlobalState gs{};
    std::string temp;
    if (statusstore->Get(leveldb::ReadOptions(), "globalstate", &temp).ok()) {
        gs.ParseFromString(temp);
    }
    else {
        qWarning() << "No GlobalState";
        return Commissioner::InitialGlobalState();
    }

    return gs;
}

int NFLStateData::week() {
    auto gs = GetGlobalState();
    return gs.week();
}

int NFLStateData::theSeason() {
    auto gs = GetGlobalState();
    return gs.season();
}


void NFLStateData:: OnGlobalState(fantasybit::GlobalState &gs) {
    statusstore->Put(write_sync, "globalstate", gs.SerializeAsString());
    qDebug() << "OnGlobalState" << gs.DebugString().data();
    if ( amlive )
        emit GlobalStateChange(gs);
}

void NFLStateData::OnGameStart(const std::string &gameid, const GameStatus &gs) {
    auto mygs = gs;
    mygs.set_status(GameStatus::INGAME);
    UpdateGameStatus(gameid,gs);

    //TeamLocked[]
    if ( amlive )
        emit GameStart(gameid);
}

std::string NFLStateData::filedir(const std::string &in) {
    return GET_ROOT_DIR() + "index/" + in;
}

std::map<std::string,int> NFLStateData::PosIndexMap = {
    {"QB",0} ,
    {"RB",1} ,
    {"WR",2} ,
    {"TE",3} ,
    {"K",4}
};
