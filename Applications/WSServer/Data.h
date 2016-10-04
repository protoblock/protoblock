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
#include <utility>
#include <memory>

using namespace std;

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

    std::shared_ptr<leveldb::DB> playerstore;
    std::shared_ptr<leveldb::DB> staticstore;
    std::shared_ptr<leveldb::DB> statusstore;

    leveldb::WriteOptions write_sync{};
    //int week = 0;
    int amlive = false;

    std::recursive_mutex data_mutex{};


    std::map<std::string, std::unordered_set<std::string>> MyTeamRoster;
    std::map<std::string, PlayerStatus> MyPlayerStatus;
    std::map<std::string, GameInfo> MyGameInfo;

    //std::map<std::string, bool> TeamLocked;

    void removePlayerTeam(const std::string &pid,const std::string &tid) {       

        MyTeamRoster[tid].erase(pid);
    }

    void addPlayerTeam(const std::string &pid,const std::string &tid) {

        MyTeamRoster[tid].insert(pid);
    }


signals:
    void PlayerStatusChange(pair<string,fantasybit::PlayerStatus>);
    void PlayerAdd(PlayerBase);
    void GameStart(string);
    void GlobalStateChange(fantasybit::GlobalState);
    void NewGameResult(string);

public slots:
    void OnLive(bool subscribe) {
        amlive = true;
    }
public:
    void OnWeekOver(int week);
    void OnWeekStart(int week);

public:
    NFLStateData() {}

#ifdef CHECKPOINTS
    static void InitCheckpoint();
#endif

    void init();
    void closeAll();

    void AddNewPlayer(const std::string playerid, const PlayerBase &);
    void AddNewWeeklySchedule(int week, const WeeklySchedule &);
    void AddGameResult(const string &gameid, const GameResult&);
    GameResult GetGameResult(const std::string &gameid);

    PlayerStatus GetPlayerStatus(const std::string &playerid);

    PlayerBase GetPlayerBase(std::string playerid);

    //void AddTeamDepth(const TeamDepth &);
    void UpdatePlayerStatus(const std::string &playerid, const PlayerStatus &);
    //void UpdatePlayerStatus(std::string playerid, const PlayerGameStatus &);

    void UpdateGameStatus(const std::string &gameid, const GameStatus &gs);

    void OnGameStart(const std::string &gameid, const GameStatus &gs);
    bool GetGameResult(const std::string &gameid, GameResult &result);

    std::vector<GameRoster> GetCurrentWeekGameRosters();
    std::vector<fantasybit::GameResult> GetPrevWeekGameResults(int week);

    std::unordered_map<std::string,PlayerDetail>
            GetTeamRoster(const std::string &teamid);

    GameStatus GetUpdatedGameStatus(std::string id);
    WeeklySchedule GetWeeklySchedule(int week);

    GlobalState GetGlobalState();
    void OnGlobalState(GlobalState &gs);

    GameInfo GetGameInfo(string gameid);


private:
    void OnNewPlayer(const std::string &pid);
    void OnPlayerTrade(const std::string &pid, const std::string &tid, const std::string &ntid);
    void OnPlayerRelease(const std::string &pid, const std::string &tid);
    void OnPlayerSign(const std::string &pid, const std::string &tid);
    void OnPlayerStatus(const std::string &pid,PlayerStatus ps);

    WeeklySchedule getWeeklyStaticSchedule(int week);
    int week();

    static std::string filedir(const std::string &in);
};

}

#endif /* defined(__fantasybit__Data__) */
