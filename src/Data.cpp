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

using namespace std;
using namespace fantasybit;

void NFLStateData::init() {
    write_sync.sync = true;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status;

    status = leveldb::DB::Open(options, filedir("staticstore"), &staticstore);
    if ( !status.ok() ) {
        qCritical() << " cant open " + filedir("staticstore");
        //todo emit fatal
        return;
    }

    status = leveldb::DB::Open(options, filedir("statusstore"), &statusstore);
    if ( !status.ok() ) {
        qCritical() << " cant open " + filedir("statusstore");
        //todo emit fatal
        return;
    }

    status = leveldb::DB::Open(options, filedir("playerstore"), &playerstore);
    if ( !status.ok() ) {
        qCritical() << " cant open " + filedir("playerstore");
        //todo emit fatal
        return;
    }

    {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };

        auto *it = playerstore->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            string temp;
            if ( !statusstore->Get(leveldb::ReadOptions(), it->key(), &temp).ok() )
                continue;
            PlayerStatus ps;
            if (!ps.ParseFromString(temp) )
                continue;

            MyPlayerStatus[it->key().ToString()] = ps;
            if (ps.has_teamid()) {
                //qDebug() << ps.teamid();
                auto itr = MyTeamRoster.find(ps.teamid());
                if ( itr == end(MyTeamRoster))
                    MyTeamRoster[ps.teamid()] =  std::unordered_set<std::string>{};

                MyTeamRoster[ps.teamid()].insert(it->key().ToString());
            }
        }
    }

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


void NFLStateData::AddNewWeeklySchedule(int week, const WeeklySchedule &ws) {
    string key = "scheduleweek:" + to_string(week);
    staticstore->Put(write_sync, key, ws.SerializeAsString());
    qDebug() << QString::fromStdString(ws.DebugString());
}

void NFLStateData::AddGameResult(const std::string &gameid, const GameResult&gs) {
    string key = "gameresult:" + gameid;
    staticstore->Put(write_sync, key, gs.SerializeAsString());
    qDebug() << QString::fromStdString(gs.DebugString());
    if ( amlive )
        emit GameResult(gameid);
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
            qDebug() << QString::fromStdString(result.DebugString());
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
    string key = "gamestatus:" + gameid;
    statusstore->Put(write_sync, key, gs.SerializeAsString());
    qDebug() << QString::fromStdString(gs.DebugString());
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
    for (auto g : ws.games()) {
        GameStatus gs = GetUpdatedGameStatus(g.id());
        if ( gs.datetime() != -1)
        {
            if ( gs.has_datetime() )
                g.set_time(gs.datetime());
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
        qDebug() << temp;
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
    if  (s.week() <= week)
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

void NFLStateData:: OnGlobalState(GlobalState &gs) {
    statusstore->Put(leveldb::WriteOptions(), "globalstate", gs.SerializeAsString());
    qDebug() << gs.DebugString();
    if ( amlive )
        emit GlobalStateChange(gs);
}

void NFLStateData::OnGameStart(const std::string &gameid, const GameStatus &gs) {
    auto mygs = gs;
    mygs.set_status(GameStatus::INGAME);
    UpdateGameStatus(gameid,gs);
    if ( amlive )
        emit GameStart(gameid);
}

std::string NFLStateData::filedir(const std::string &in) {
    return GET_ROOT_DIR() + "index/" + in;
}
