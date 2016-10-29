#include "dataservice.h"
#include "Commissioner.h"

using namespace fantasybit;
using namespace std;

std::unordered_map<std::string,PlayerDetail>
    DataService::GetTeamRoster(const std::string &teamid){
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().GetTeamRoster(teamid);
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

std::vector<fantasybit::GameResult> DataService::GetPrevWeekGameResults(int week){
    //QMutexLocker(&DataService::instance()->myMutex);
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().GetPrevWeekGameResults(week);
}

fantasybit::WeeklySchedule DataService::GetWeeklySchedule(int week) {
//QMutexLocker(&DataService::instance()->myMutex);
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().GetWeeklySchedule(week);
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


MyFantasyName DataService::importMnemonic(std::string &in) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->Agent().UseMnemonic(in);
}

string DataService::exportMnemonic(std::string &in) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->Agent().getMnemonic(in);
}

ordsnap_t DataService::GetOrdersPositionsByName(const std::string &fname) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->ExData().GetOrdersPositionsByName(fname);
}


std::unordered_map<int, pair<bool, std::string> > DataService::getAllKnownPlayerStatus(){
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().myKnownPlayerStatus();
}
