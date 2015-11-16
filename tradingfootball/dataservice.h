#ifndef DATASERVICE_H
#define DATASERVICE_H

#include <QMutex>
#include "genericsingleton.h"
#include "core.h"
#include "LAPIWorker.h"
#include "Data.h"
//#include <QMutexLocker>

class DataService : public GenericSingleton<DataService>  {
    friend class GenericSingleton<DataService>;
    DataService(){}
    //QMutex myMutex;
public:
    ~DataService(){}
    std::vector<fantasybit::GameRoster> GetCurrentWeekGameRosters();

    std::vector<fantasybit::GameResult> GetPrevWeekGameResults(int week);

    fantasybit::WeeklySchedule GetWeeklySchedule(int week);

    fantasybit::PlayerBase GetPlayerBase(std::string playerId);

    std::vector<std::shared_ptr<fantasybit::FantasyName>> GetLeaderBoard();

    std::unordered_map<std::string,int> GetProjByName(const std::string &nm);

    std::unordered_map<std::string,int> GetProjById(const std::string &pid);

    fantasybit::GlobalState GetGlobalState();

    std::unordered_map<std::string,PlayerDetail> GetTeamRoster(const std::string &teamid);
    fantasybit::GameStatus GetGameStatus(string gid);


    //void getLeaderBoard(int week){}
    //void getTeamPlayers(std::string teamId);
};


#endif // DATASERVICE_H
