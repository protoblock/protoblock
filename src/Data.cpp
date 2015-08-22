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
        qCritical() << " cant open " + filedir("staticstore");
        //todo emit fatal
        return;
    }

    auto *it = statusstore->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        PlayerStatus ps;
        ps.ParseFromString(it->value().ToString());
        MyPlayerStatus[it->key().ToString()] = ps;
        if (ps.has_teamid()) {
            auto itr = MyTeamRoster.find(ps.teamid());
            if ( itr != end(MyTeamRoster))
                itr->second.insert(it->key().ToString());
        }
    }

}

void NFLStateData::AddNewPlayer(std::string playerid, const PlayerBase &pb) {
    staticstore->Put(write_sync, playerid, pb.SerializeAsString());
    qDebug() << QString::fromStdString(pb.DebugString());
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

void NFLStateData::UpdatePlayerStatus(const std::string &playerid, const PlayerStatus &ps) {
    statusstore->Put(write_sync, playerid, ps.SerializeAsString());
    qDebug() << QString::fromStdString(ps.DebugString());

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
        emit PlayerChange(pid);

}

void NFLStateData::OnPlayerTrade(const std::string &pid, const std::string &tid,
                         const std::string &ntid) {
    removePlayerTeam(pid,tid);
    addPlayerTeam(pid,ntid);
    if ( amlive ) {
        emit PlayerChange(pid);
        emit TeamPlus(ntid, pid);
        emit TeamMinus(tid, pid);
    }
}

void NFLStateData::OnPlayerRelease(const std::string &pid, const std::string &tid) {
    removePlayerTeam(pid,tid);
    if ( amlive ) {
        emit PlayerChange(pid);
        emit TeamMinus(tid, pid);
    }
}

void NFLStateData::OnPlayerSign(const std::string &pid, const std::string &tid) {
    addPlayerTeam(pid,tid);
    if ( amlive ) {
        emit PlayerChange(pid);
        emit TeamPlus(tid, pid);
    }
}

void NFLStateData::OnPlayerStatus(const std::string &pid,PlayerStatus ps) {
    if ( amlive ) {
        emit PlayerChange(pid);
    }
}

void NFLStateData::UpdateGameStatus(const std::string &gameid, const GameStatus &gs) {
    string key = "gamestatus:" + gameid;
    statusstore->Put(write_sync, key, gs.SerializeAsString());
    qDebug() << QString::fromStdString(gs.DebugString());
}

void NFLStateData::OnWeekOver(int in) {
    //get weekly schedule
    //see if we have gameresult
    //update game to closed or scored
    //delete gamestatus
    //update gameresult -
    //with team rosters and player status from players that were not scored
}

void NFLStateData::OnWeekStart(int in) {
    week = in;
}


WeeklySchedule NFLStateData::GetWeeklySchedule(int week,bool updates) {
    WeeklySchedule ws{};
    string temp;
    string key = "scheduleweek:" + to_string(week);
    if ( !staticstore->Get(leveldb::ReadOptions(), key, &temp).ok() ) {
        qWarning() << "cant find schedule " << key.c_str();
        return ws;
    }
    ws.ParseFromString(temp);

    if ( !updates ) return ws;
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
     }
    return gs;
}
vector<GameRoster> NFLStateData::GetLiveWeekGameRosters() {

    vector<GameRoster> retgr{};
    if ( !amlive ) {
        qWarning() << "am not live" ;
    }

    WeeklySchedule ws = GetWeeklySchedule(week,false);

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

std::unordered_map<std::string,PlayerDetail>
        NFLStateData::GetTeamRoster(const std::string &teamid) {
    std::unordered_map<std::string,PlayerDetail> vpb{};
    auto teamroster = MyTeamRoster[teamid];
    string temp;
    for ( auto p : teamroster) {
        auto ps = MyPlayerStatus[p];
        if ( ps.teamid() != teamid  ||
             ps.status() != PlayerStatus::ACTIVE)
            continue;
        if ( !statusstore->Get(leveldb::ReadOptions(), p, &temp).ok() )
            continue;
        PlayerDetail pd;
        pd.base.ParseFromString(temp);
        pd.team_status = ps.status();
        pd.game_status = PlayerGameStatus::IN;
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
        qCritical() << "No GlobalState";
    }

    return gs;
}

void NFLStateData::OnGlobalState(GlobalState &gs) {
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
