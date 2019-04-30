#include "dataservice.h"
#include "Commissioner.h"

using namespace fantasybit;
using namespace std;

std::unordered_map<std::string,PlayerDetail>
    DataService::GetTeamRoster(const std::string &teamid){
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().GetTeamRoster(teamid);
}

std::vector<MarketSnapshot> DataService::GetCurrentMarketSnaps() {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->ExData().GetCurrentMarketSnaps();
}

std::vector<fantasybit::GameRoster> DataService::GetCurrentWeekGameRosters(){
    //QMutexLocker(&DataService::instance()->myMutex);
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().GetCurrentWeekGameRosters();
}

int DataService::GetAvgProjection(const string &playerid) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NameData().GetAvgProj(playerid);
}

std::vector<fantasybit::GameResult> DataService::GetPrevWeekGameResults(int season,int week){
    //QMutexLocker(&DataService::instance()->myMutex);
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().GetPrevWeekGameResults(season,week);
}

std::map<std::string,std::string> DataService::GetAllSymbols() {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().GetAllSymbols();
}

std::map<std::string,std::string> DataService::GetTeamSymbols(const std::list<std::string> &teams)  {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().GetTeamSymbols(teams);
}


fantasybit::WeeklySchedule DataService::GetWeeklySchedule(int season,int week) {
//QMutexLocker(&DataService::instance()->myMutex);
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().GetWeeklySchedule(season,week);
}

fantasybit::PlayerBase DataService::GetPlayerBase(std::string playerId) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().GetPlayerBase(playerId);
}

fantasybit::PlayerStatus DataService::GetPlayerStatus(std::string playerId) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().GetPlayerStatus(playerId);
}


fantasybit::GameStatus DataService::GetGameStatus(string gid) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().GetUpdatedGameStatus(gid);
}

PlayerDetail DataService::GetPlayerDetail(const std::string &symbol) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().GetPlayerDetail(symbol);
}


std::vector<std::shared_ptr<fantasybit::FantasyName>> DataService::GetLeaderBoard() {
    return Commissioner::GetFantasyNames();

//    std::sort(v.begin(), v.end(),
//        [](shared_ptr<FantasyName> f1, shared_ptr<FantasyName> f2){
//            return (f2->getBalance() == f1->getBalance()) ? (f1->hash() < f2->hash() ) :
//                                                       (f2->getBalance() < f1->getBalance());
//        });

//    return v;
}

// curent week
unordered_map<std::string,int> DataService::GetProjByName(const std::string &fname) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NameData().GetProjByName(fname);
}

//current week
unordered_map<std::string,int> DataService::GetProjById(const std::string &pid) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NameData().GetProjById(pid);
}

fantasybit::GlobalState DataService::GetGlobalState() {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().GetGlobalState();
}


MyFantasyName DataService::importMnemonic(const std::string &in) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->Agent().UseMnemonic(in);
}

string DataService::exportMnemonic(std::string &in) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->Agent().getMnemonic(in);
}

pb::signature DataService::signIt(const pb::sha256 &in) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->Agent().sign_raw(in);
}

ordsnap_t DataService::GetOrdersPositionsByName(const std::string &fname) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->ExData().GetOrdersPositionsByName(fname);
}

int DataService::GetOpenPnl(const std::string &fname) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->ExData().GetOpenPnl(fname);
}

std::unordered_map<int, pair<bool, std::string> > DataService::getAllKnownPlayerStatus(){
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().myKnownPlayerStatus();
}

std::string DataService::GetPidfromSymb(const std::string &symb) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().mSym2Pid[symb];
}

std::vector<SwapOrder> DataService::GetCurrentSwapSnaps() {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->SwapState().GetCurrentSwapSnaps();
}

SwapBuyer DataService::GetSwapBid(const QString &fn) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->SwapState().GetSwapBid(fn);
}

SwapSeller DataService::GetSwapAsk(const QString &fn) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->SwapState().GetSwapAsk(fn);
}

SwapFill DataService::GetSwapFill(const QString &buyer, const QString &seller_ref) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->SwapState().GetSwapFill(buyer, seller_ref);
}



