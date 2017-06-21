#ifndef DATASERVICE_H
#define DATASERVICE_H

#include <QMutex>
#include "genericsingleton.h"
#include "core.h"
//#include "LAPIWorker.h"
#include "Data.h"
//#include <QMutexLocker>
#include "pbgateways.h"
#include "ExchangeData.h"

class DataService : public GenericSingleton<DataService>
        , public pb::IDataService
{
    friend class GenericSingleton<DataService>;
    DataService(){}
    //QMutex myMutex;
public:
    ~DataService(){}
    std::vector<fantasybit::GameRoster> GetCurrentWeekGameRosters();

    std::vector<fantasybit::GameResult> GetPrevWeekGameResults(int season,int week);

    std::map<std::string,std::string> GetAllSymbols();

    fantasybit::WeeklySchedule GetWeeklySchedule(int season,int week);

    fantasybit::PlayerBase GetPlayerBase(std::string playerId);

    fantasybit::PlayerStatus GetPlayerStatus(std::string playerId);

    std::vector<std::shared_ptr<fantasybit::FantasyName>> GetLeaderBoard();

    std::unordered_map<std::string,int> GetProjByName(const std::string &nm);

    std::unordered_map<std::string,int> GetProjById(const std::string &pid);

    fantasybit::GlobalState GetGlobalState();

    std::unordered_map<std::string,fantasybit::PlayerDetail> GetTeamRoster(const std::string &teamid);
    fantasybit::GameStatus GetGameStatus(string gid);

    PlayerDetail GetPlayerDetail(const std::string &symbol);

    fantasybit::MyFantasyName importMnemonic(const std::string &in);
    std::string exportMnemonic(std::string &in);

    ordsnap_t
        GetOrdersPositionsByName(const std::string &fname);

    std::unordered_map<int,pair<bool,string>> getAllKnownPlayerStatus();
    //void getLeaderBoard(int week){}
    //void getTeamPlayers(std::string teamId);
    int GetAvgProjection(const string &playerid);
    std::vector<MarketSnapshot> GetCurrentMarketSnaps();
};


#endif // DATASERVICE_H
