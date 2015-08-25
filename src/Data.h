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
#include <mutex>
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

class NFLStateData : public QObject {

    Q_OBJECT

    leveldb::DB *staticstore;
    leveldb::DB *statusstore;

    leveldb::WriteOptions write_sync{};
    //int week = 0;
    int amlive = false;

    std::mutex data_mutex{};


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


signals:
    void PlayerStatusChange(std::pair<std::string,fantasybit::PlayerStatus>);
    void PlayerAdd(fantasybit::PlayerBase);
    void GameStart(std::string);
    void GlobalStateChange(GlobalState);

public slots:
    void OnLive(bool subscribe) {
        amlive = true;
    }
    void OnWeekOver(int week);
    void OnWeekStart(int week);

public:
    NFLStateData() {}
    void init();
    void AddNewPlayer(const std::string playerid, const PlayerBase &);
    void AddNewWeeklySchedule(int week, const WeeklySchedule &);
    void AddGameResult(const std::string &gameid, const GameResult&);
    GameResult GetGameResult(const std::string &gameid);

    PlayerBase GetPlayerBase(std::string playerid);

    //void AddTeamDepth(const TeamDepth &);
    void UpdatePlayerStatus(const std::string &playerid, const PlayerStatus &);
    //void UpdatePlayerStatus(std::string playerid, const PlayerGameStatus &);

    void UpdateGameStatus(const std::string &gameid, const GameStatus &gs);

    void OnGameStart(const std::string &gameid, const GameStatus &gs);

    std::vector<GameRoster> GetCurrentWeekGameRosters();
    std::vector<fantasybit::GameResult> GetPrevWeekGameResults(int week);

    std::unordered_map<std::string,PlayerDetail>
            GetTeamRoster(const std::string &teamid);

    GameStatus GetUpdatedGameStatus(std::string id);
    WeeklySchedule GetWeeklySchedule(int week);

    GlobalState GetGlobalState();
    void OnGlobalState(GlobalState &gs);

private:
    void OnNewPlayer(const std::string &pid);
    void OnPlayerTrade(const std::string &pid, const std::string &tid, const std::string &ntid);
    void OnPlayerRelease(const std::string &pid, const std::string &tid);
    void OnPlayerSign(const std::string &pid, const std::string &tid);
    void OnPlayerStatus(const std::string &pid,PlayerStatus ps);

    WeeklySchedule getWeeklyStaticSchedule(int week);
    int week();

    std::string filedir(const std::string &in);
};

}

#endif /* defined(__fantasybit__Data__) */
