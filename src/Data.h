//
//  Data.h
//  fantasybit
//
//  Created by Jay Berg on 3/25/14.
//
//
#ifndef __fantasybit__Data__
#define __fantasybit__Data__

#include <QObject>

#include "StaticData.pb.h"
#include "StatusData.pb.h"
#include <unordered_set>
#include <unordered_map>

#include <leveldb/db.h>

namespace fantasybit
{

struct PlayerDetail {
    PlayerBase base;
    PlayerStatus::Status team_status;
    PlayerGameStatus game_status;
};

struct GameRoster {
    GameInfo info;
    GameStatus::Status  status;
    std::unordered_map<std::string,PlayerDetail> homeroster;
    std::unordered_map<std::string,PlayerDetail> awayroster;

};

class DataData : public QObject {

    Q_OBJECT


    leveldb::DB *staticstore;
    leveldb::DB *statusstore;

    leveldb::WriteOptions write_sync{};

    std::map<std::string, std::unordered_set<std::string>> MyTeamRoster;
    std::map<std::string, PlayerStatus> MyPlayerStatus;

    void removePlayerTeam(const std::string &pid,const std::string &tid) {
        auto tr = MyTeamRoster[tid];
        tr.erase(pid);
    }

    void addPlayerTeam(const std::string &pid,const std::string &tid) {
        auto tr = MyTeamRoster[tid];
        tr.insert(pid);
    }

    bool amlive = false;
public slots:
    void OnSubscribeLive() {
        amlive = true;
    }

public:
    DataData() {}
    void init();
    void AddNewPlayer(const std::string playerid, const PlayerBase &);
    void AddNewWeeklySchedule(int week, const WeeklySchedule &);
    void AddGameResult(const std::string &gameid, const GameResult&);

    //void AddTeamDepth(const TeamDepth &);
    void UpdatePlayerStatus(const std::string &playerid, const PlayerStatus &);
    //void UpdatePlayerStatus(std::string playerid, const PlayerGameStatus &);

    void UpdateGameStatus(const std::string &gameid, const GameStatus &gs);

    void OnGameStart(const std::string &gameid, const GameStatus &gs);

    std::vector<GameRoster> GetWeeklyGameRosters(int week);
    std::unordered_map<std::string,PlayerDetail>
            GetTeamRoster(const std::string &teamid);

    GlobalState GetGlobalState();

    void OnGlobalState(const GlobalState &gs);

    void OnNewPlayer(const std::string &pid);
    void OnPlayerTrade(const std::string &pid, const std::string &tid, const std::string &ntid);
    void OnPlayerRelease(const std::string &pid, const std::string &tid);
    void OnPlayerSign(const std::string &pid, const std::string &tid);
    void OnPlayerStatus(const std::string &pid,PlayerStatus ps);


    std::string filedir(const std::string &in);
};

}

#endif /* defined(__fantasybit__Data__) */
