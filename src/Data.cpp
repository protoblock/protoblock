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

void DataData::init() {
    write_sync.sync = true;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status;

    status = leveldb::DB::Open(options, filedir("staticstore"), &staticstore);
    status = leveldb::DB::Open(options, filedir("statusstore"), &statusstore);

}

void DataData::AddNewPlayer(std::string playerid, const PlayerBase &pb) {
    staticstore->Put(write_sync, playerid, pb.SerializeAsString());
    qDebug() << QString::fromStdString(pb.DebugString());
}

void DataData::AddNewWeeklySchedule(int week, const WeeklySchedule &ws) {
    string key = "scheduleweek:" + to_string(week);
    staticstore->Put(write_sync, key, ws.SerializeAsString());
    qDebug() << QString::fromStdString(ws.DebugString());
}

void DataData::AddGameResult(const std::string &gameid, const GameResult&gs) {
    string key = "gameresult:" + gameid;
    staticstore->Put(write_sync, key, gs.SerializeAsString());
    qDebug() << QString::fromStdString(gs.DebugString());
}

void DataData::UpdatePlayerStatus(const std::string &playerid, const PlayerStatus &ps) {
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

void DataData::OnNewPlayer(const std::string &pid) {}

void DataData::OnPlayerTrade(const std::string &pid, const std::string &tid,
                         const std::string &ntid) {
    removePlayerTeam(pid,tid);
    addPlayerTeam(pid,ntid);
}

void DataData::OnPlayerRelease(const std::string &pid, const std::string &tid) {
    removePlayerTeam(pid,tid);
}

void DataData::OnPlayerSign(const std::string &pid, const std::string &tid) {
    addPlayerTeam(pid,tid);
}

void DataData::OnPlayerStatus(const std::string &pid,PlayerStatus ps) {}

void DataData::UpdateGameStatus(const std::string &gameid, const GameStatus &gs) {
    string key = "gamestatus:" + gameid;
    statusstore->Put(write_sync, key, gs.SerializeAsString());
    qDebug() << QString::fromStdString(gs.DebugString());
}

vector<GameRoster> DataData::GetWeeklyGameRosters(int week) {
    vector<GameRoster> gr{};
    WeeklySchedule ws{};
    string temp;
    string key = "scheduleweek:" + to_string(week);
    if ( !staticstore->Get(leveldb::ReadOptions(), key, &temp).ok() ) {
        qWarning() << "cant find schedule " << key.c_str();
        return gr;
    }
    ws.ParseFromString(temp);

    for (const auto g : ws.games()) {
        string key = "gamestatus:" + g.id();
        if ( !statusstore->Get(leveldb::ReadOptions(), key, &temp).ok() ) {
            qWarning() << "cant find game status " << key;
        }
        else {
            GameStatus gs;
            gs.ParseFromString(temp);
            GameRoster gr{};
            gr.info = g;
            if ( gs.has_datetime() ) gr.info.set_time(gs.datetime());
            gr.status = gs.status();

            gr.homeroster = GetTeamRoster(g.home());
            gr.awayroster = GetTeamRoster(g.away());
        }
    }
}

std::unordered_map<std::string,PlayerDetail>
        DataData::GetTeamRoster(const std::string &teamid) {
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

GlobalState DataData::GetGlobalState() {
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

void DataData::OnGlobalState(const GlobalState &gs) {
    statusstore->Put(leveldb::WriteOptions(), "globalstate", gs.SerializeAsString());
    qDebug() << gs.DebugString();
}

void DataData::OnGameStart(const std::string &gameid, const GameStatus &gs) {}

std::string DataData::filedir(const std::string &in) {
    return GET_ROOT_DIR() + "index/" + in;
}
