#ifndef DATACACHE_H
#define DATACACHE_H

#include "genericsingleton.h"
#include "dataservice.h"
#include "tablemodels.h"
#include <unordered_map>

using namespace std;

class DataCache : public GenericSingleton<DataCache>
{
    friend class GenericSingleton<DataCache>;
    DataCache(){}
    std::unordered_map<uint,std::vector<fantasybit::GameResult>> myGamesResultCache;
    std::unordered_map<uint,fantasybit::WeeklySchedule> myWeeklySchedulesCache;
    std::vector<std::shared_ptr<fantasybit::FantasyName>> myLeaderBoardData;
    LeaderBoardTableModel myLeaderBoardTableModel;
    bool myLeaderboardDataUpdated = false;

public:
    ~DataCache(){}
    void getGameResult(uint week, std::vector<GameResult> &weekResults){
        unordered_map<uint, vector<fantasybit::GameResult> >::const_iterator found =
                myGamesResultCache.find(week);
        //not found in cache get them from dataservice
        if (found == myGamesResultCache.end()){
            vector<fantasybit::GameResult>results = DataService::instance()->GetPrevWeekGameResults(week);
            pair<uint,vector<fantasybit::GameResult>> pair(week,results);
            myGamesResultCache.insert(pair);
            for(const fantasybit::GameResult & result : results)
                weekResults.push_back(result);
        }
        else {   //found add them to referenced vector
            weekResults.clear();
            for(const fantasybit::GameResult & result : found->second)
                weekResults.push_back(result);
        }

    }

    void refreshLeaderboard(){
        if (myLeaderboardDataUpdated) return;
        myLeaderBoardData = DataService::instance()->GetLeaderBoard();
        myLeaderBoardTableModel.removeAll();
        for(std::shared_ptr<fantasybit::FantasyName> & fPlayer  : myLeaderBoardData) {
            QString fantasyPlayerName = fPlayer->alias().data();
            myLeaderBoardTableModel.updateItemProperty<PropertyNames::Fantasy_Name>(fantasyPlayerName,fantasyPlayerName);
            myLeaderBoardTableModel.updateItemProperty<PropertyNames::Balance>(fantasyPlayerName,fPlayer->getBalance());
        }
        myLeaderboardDataUpdated = true;
    }
    LeaderBoardTableModel & leaderBoardModel() { return myLeaderBoardTableModel; }

    void getWeeklySchedule(int week,fantasybit::WeeklySchedule & schedule){
        std::unordered_map<uint,fantasybit::WeeklySchedule>::const_iterator found =
                myWeeklySchedulesCache.find(week);
        //not found
        if (found  == myWeeklySchedulesCache.end()){
            schedule = DataService::instance()->GetWeeklySchedule(week);
            std::pair<uint,fantasybit::WeeklySchedule> pair(week,schedule);
            myWeeklySchedulesCache.insert(pair);
        }
        else {   //found
            schedule.CopyFrom(found->second);
        }
    }

    void getGameTeamsIdsByWeek(uint currentWeek,QString & homeTeamId,QString & awayTeamId){
        Q_UNUSED(currentWeek);
        homeTeamId = "??";
        awayTeamId = "??";
    }

    QString getPlayerFullName(const QString playerId) const {
        Q_UNUSED(playerId);
        return "??";
    }

    QString getPlayerPositionInWeek(const QString playerId,uint week) const {
        Q_UNUSED(week);
        Q_UNUSED(playerId);
        return "??";
    }
};

#endif // DATACACHE_H
