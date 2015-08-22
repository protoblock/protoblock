#ifndef DATASERVICE_H
#define DATASERVICE_H

#include <QMutex>
#include "genericsingleton.h"
#include "core.h"
#include "LAPIWorker.h"
#include "Data.h"
#include <QMutexLocker>

class DataService : GenericSingleton<DataService>  {
    friend class GenericSingleton<DataService>;
    DataService(){}
    QMutex myMutex;
public:
    ~DataService(){}
    static std::vector<fantasybit::GameRoster> getGameScheduleForWeek(int week);
    static void getLeaderBoard(int week){}
    static void getPlayerInfo(std::string playerId);
    static void getTeamPlayers(std::string teamId);
};


#endif // DATASERVICE_H
