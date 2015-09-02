#include "dataservice.h"
#include "Commissioner.h"

using namespace fantasybit;
using namespace std;

std::vector<fantasybit::GameRoster> DataService::GetCurrentWeekGameRosters(){
    //QMutexLocker(&DataService::instance()->myMutex);
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->NFLState().GetCurrentWeekGameRosters();
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

std::vector<std::shared_ptr<fantasybit::FantasyName>> DataService::GetLeaderBoard() {
    auto v = Commissioner::GetFantasyNames();

    std::sort(v.begin(), v.end(),
        [](shared_ptr<FantasyName> f1, shared_ptr<FantasyName> f2){
            return (f2->getBalance() == f2->getBalance()) ? (f1->hash() < f2->hash() ) :
                                                       (f2->getBalance() < f1->getBalance());
        });

    return v;
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
