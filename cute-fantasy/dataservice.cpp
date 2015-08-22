#include "dataservice.h"

std::vector<fantasybit::GameRoster> DataService::getGameScheduleForWeek(int week){
    QMutexLocker(&DataService::instance()->myMutex);
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->getWeekGameRosters(week);
}

