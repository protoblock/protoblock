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
#include "utils/utils.h"

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

    pb::signature signIt(const pb::sha256 &in);

    ordsnap_t
        GetOrdersPositionsByName(const std::string &fname);

    std::unordered_map<int,pair<bool,string>> getAllKnownPlayerStatus();
    //void getLeaderBoard(int week){}
    //void getTeamPlayers(std::string teamId);
    int GetAvgProjection(const string &playerid);
    std::vector<MarketSnapshot> GetCurrentMarketSnaps();

    std::string GetPidfromSymb(const std::string &symb);
    int GetOpenPnl(const std::string &fname);
    std::map<std::string, std::string> GetTeamSymbols(const std::list<std::string> &teams);

    std::vector<SwapOrder> GetCurrentSwapSnaps();
    SwapBuyer GetSwapBid(const QString &);
    SwapSeller GetSwapAsk(const QString &);
    SwapFill GetSwapFill(const QString &buyer, const QString &seller_ref);


};


#endif // DATASERVICE_H
