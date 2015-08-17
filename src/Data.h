//
//  Data.h
//  fantasybit
//
//  Created by Jay Berg on 3/25/14.
//
//
#ifndef __fantasybit__Data__
#define __fantasybit__Data__

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

class Data {
    leveldb::DB *staticstore;
    leveldb::DB *statusstore;

    leveldb::WriteOptions write_sync{};

    std::map<std::string, std::unordered_set<std::string>> MyTeamRoster;
    std::map<std::string, PlayerStatus> MyPlayerStatus;

public:
    Data();
    void init();
    void AddNewPlayer(std::string playerid, const PlayerBase &);
    void AddNewWeeklySchedule(int week, const WeeklySchedule &);
    void AddGameResult(std::string &gameid, const GameResult&);

    //void AddTeamDepth(const TeamDepth &);
    void UpdatePlayerStatus(std::string &playerid, const PlayerStatus &);
    //void UpdatePlayerStatus(std::string playerid, const PlayerGameStatus &);

    void UpdateGameStatus(std::string &gameid, const GameStatus &gs);

    std::vector<GameRoster> GetWeeklyGameRosters(int week);
    std::unordered_map<std::string,PlayerDetail>
            GetTeamRoster(const std::string &teamid);

    std::string filedir(const std::string &in);

};

}

#endif /* defined(__fantasybit__Data__) */
