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

#include "DataPersist.h"

using namespace std;
using namespace fantasybit;

#ifdef CHECKPOINTS
void NFLStateData::InitCheckpoint() {

    leveldb::WriteOptions write_sync;
    write_sync.sync = true;
    leveldb::Options options;
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

void NFLStateData::init() {
    write_sync.sync = true;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status;

    leveldb::DB *db1;
    status = leveldb::DB::Open(options, filedir("staticstore"), &db1);
    staticstore.reset(db1);
    if ( !status.ok() ) {
        qCritical() << " cant open " + filedir("staticstore");
        //todo emit fatal
        return;
    }

    leveldb::DB *db2;
    status = leveldb::DB::Open(options, filedir("statusstore"), &db2);
    statusstore.reset(db2);
    if ( !status.ok() ) {
        qCritical() << " cant open " + filedir("statusstore");
        //todo emit fatal
        return;
    }

    leveldb::DB *db3;
    status = leveldb::DB::Open(options, filedir("playerstore"), &db3);
    playerstore.reset(db3);
    if ( !status.ok() ) {
        qCritical() << " cant open " + filedir("playerstore");
        //todo emit fatal
        return;
    }

#ifdef ONE_TIME_PROD_SEASON_TRADING
    Reader<PlayerData> reader3{ GET_ROOT_DIR() + "PlayerData.txt"};
    PlayerData pd;
    while ( reader3.ReadNext(pd) ) {
        pd.ByteSize();
        statusstore->Put(write_sync, pd.playerid(),
                       pd.player_status().SerializeAsString() );
        playerstore->Put(write_sync, pd.playerid(),
                       pd.player_base().SerializeAsString() );
    }
#endif

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
            MyPlayerStatus[it->key().ToString()] = ps;
            auto team = ps.has_teamid() ? ps.teamid() : "FA";
            if ( team == "FA")
                qDebug() << it->key().ToString().data() << ps.DebugString().data();
            {
                //qDebug() << ps.teamid();
                auto itr = MyTeamRoster.find(team);
                if ( itr == end(MyTeamRoster))
                    MyTeamRoster[team] =  std::unordered_set<std::string>{};

                MyTeamRoster[team].insert(it->key().ToString());
            }
        }
        delete it;
    }

#ifdef ONE_TIME_PROD_SEASON_TRADING
    for ( auto mtr : MyTeamRoster ) {
        auto tr = GetTeamRoster(mtr.first);

        qDebug() << mtr.first.data();
        for ( auto pd : tr) {
            qDebug() << pd.second.base.DebugString().data();
        }
    }

#endif
    {
#ifdef WRITE_BOOTSTRAP
        Writer<ScheduleData> writer{ GET_ROOT_DIR() + "bootstrap/WeeklySchedule.txt" };

        Writer<GameData> writer2{ GET_ROOT_DIR() + "bootstrap/GameData.txt" };
        Writer<GameResult> writer3{ GET_ROOT_DIR() + "bootstrap/GameResult.txt" };
#endif
        for (int i=1; i<=17;i++) {
            string key = "scheduleweek:" + to_string(i);
            string temp;
            if ( !staticstore->Get(leveldb::ReadOptions(), key, &temp).ok() ) {
                qWarning() << "cant find schedule " << key.c_str();
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
}

GameInfo NFLStateData::GetGameInfo(string gameid) {
    std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
    return MyGameInfo[gameid];
}

void NFLStateData::AddNewPlayer(std::string playerid, const PlayerBase &pb) {
    playerstore->Put(write_sync, playerid, pb.SerializeAsString());
    qDebug() << QString::fromStdString(pb.DebugString());
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

void NFLStateData::AddNewWeeklySchedule(int week, const WeeklySchedule &ws) {
    string key = "scheduleweek:" + to_string(week);
    if ( !staticstore->Put(write_sync, key, ws.SerializeAsString()).ok()) {
        qWarning() << " error writing schecule";
        return;
    }
    qDebug() << QString::fromStdString(ws.DebugString());
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

void NFLStateData::AddGameResult(const std::string &gameid, const GameResult&gs) {
    string key = "gameresult:" + gameid;
    if (!staticstore->Put(write_sync, key, gs.SerializeAsString()).ok()) {
        qWarning() << "cant add gameresult" << gameid;
    }
    qDebug() << key << QString::fromStdString(gs.DebugString());
    if ( amlive )
        emit NewGameResult(gameid);
}

bool NFLStateData::GetGameResult(const std::string &gameid, GameResult &result) {
    string key = "gameresult:" + gameid;
    string temp;
    if (!staticstore->Get(leveldb::ReadOptions(), key, &temp).ok())
        qWarning() << gameid << " not found";
    else {
        if (!result.ParseFromString(temp) )
            qWarning() << " cant parse game result";
        else {
            qDebug() << result.DebugString();
            return true;
        }
    }
    return false;
}

void NFLStateData::UpdatePlayerStatus(const std::string &playerid, const PlayerStatus &ps) {
    statusstore->Put(write_sync, playerid, ps.SerializeAsString());
    qDebug() << QString::fromStdString(ps.DebugString());

    std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
    auto it = MyPlayerStatus.find(playerid);
    if ( it == end(MyPlayerStatus)) {
        MyPlayerStatus[playerid] = ps;
        OnNewPlayer(playerid);
        if ( ps.has_teamid())
            OnPlayerSign(playerid,ps.teamid());
    }
    else {
        auto old = it->second;
        MyPlayerStatus[playerid] = ps;

        if ( old.has_teamid() ) {
            if ( ps.has_teamid() ) {
                if ( old.teamid() != ps.teamid()) {
                    OnPlayerTrade(playerid,old.teamid(),ps.teamid());
                }
                if ( ps.has_status() ) {
                    if ( !old.has_status() )
                        OnPlayerStatus(playerid,ps);
                    else if ( old.status() != ps.status())
                        OnPlayerStatus(playerid,ps);
                }
            }
            else {
                OnPlayerRelease(playerid,old.teamid());
            }
        }
        else if ( ps.has_teamid() )
            OnPlayerSign(playerid,ps.teamid());
    }
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

void NFLStateData::UpdateGameStatus(const std::string &gameid, const GameStatus &gs) {
    GameStatus use;
    if ( gs.status() == GameStatus_Status_SCHEDULED ) {
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
    qDebug() << key << QString::fromStdString(use.DebugString());
}

void NFLStateData::OnWeekOver(int in) {
    std::lock_guard<std::recursive_mutex> lockg{ data_mutex };

    auto ws = GetWeeklySchedule(in);
    for (auto game : ws.games() ) {
        GameResult gs{};
        if ( !GetGameResult(game.id(),gs) ) {
            qWarning() << " no result " << game.id();
            continue;
        }
        else
            qDebug() << "week over" << in << " commit result " << game.id();

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

WeeklySchedule NFLStateData::GetWeeklySchedule(int week) {

    auto ws = getWeeklyStaticSchedule(week);
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

WeeklySchedule NFLStateData::getWeeklyStaticSchedule(int week) {
    WeeklySchedule ws{};
    string temp;
    string key = "scheduleweek:" + to_string(week);
    if ( !staticstore->Get(leveldb::ReadOptions(), key, &temp).ok() ) {
        qWarning() << "cant find schedule " << key.c_str();
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
        qDebug() << key << gs.DebugString();
     }
    return gs;
}

vector<GameRoster> NFLStateData::GetCurrentWeekGameRosters() {

    vector<GameRoster> retgr{};
    if ( !amlive ) {
        qWarning() << "am not live" ;
    }

    qDebug() << week();

    WeeklySchedule ws = getWeeklyStaticSchedule(week());

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
        retgr.push_back(gr);
    }

    return retgr;
}

std::vector<fantasybit::GameResult> NFLStateData::GetPrevWeekGameResults(int week) {
    std::vector<fantasybit::GameResult> ret{};
    auto s = GetGlobalState();
    if  (s.week() <= week && s.week() != 0)
        return ret;

    auto ws = GetWeeklySchedule(week);
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

    qDebug() << "get team roster" << teamid;
    std::unordered_map<std::string,PlayerDetail> vpb{};

    auto it = MyTeamRoster.find(teamid);
    if ( it != end(MyTeamRoster))
        qDebug() << " found it" << it->second.size();

    auto teamroster = MyTeamRoster[teamid];
    qDebug() << teamroster.size();
    string temp;
    for ( auto p : teamroster) {
        auto ps = MyPlayerStatus[p];
        if ( ps.teamid() != teamid  ||
             false)//ps.status() != PlayerStatus::ACTIVE)
            continue;

        PlayerDetail pd;
        pd.base = GetPlayerBase(p);
        pd.team_status = ps.status();
        pd.game_status = PlayerGameStatus::NA;
        vpb[p] = pd;
    }
    return vpb;
}

GlobalState NFLStateData::GetGlobalState() {
    GlobalState gs{};
    std::string temp;
    if (statusstore->Get(leveldb::ReadOptions(), "globalstate", &temp).ok()) {
        gs.ParseFromString(temp);
    }
    else {
        return Commissioner::InitialGlobalState();
        qWarning() << "No GlobalState";
    }

    return gs;
}

int NFLStateData::week() {
    auto gs = GetGlobalState();
    return gs.week();
}

void NFLStateData:: OnGlobalState(fantasybit::GlobalState &gs) {
    statusstore->Put(write_sync, "globalstate", gs.SerializeAsString());
    qDebug() << gs.DebugString();
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
