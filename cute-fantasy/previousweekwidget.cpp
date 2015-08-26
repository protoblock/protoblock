#include "previousweekwidget.h"
#include "ui_previousweekwidget.h"
#include "dataservice.h"
#include "datacache.h"

PreviousWeekWidget::PreviousWeekWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviousWeekWidget)
{
    ui->setupUi(this);
    ui->myGamesTableView->setModel(&myGameTableModel);
    ui->myLeaderBaordTableView->setModel(&DataCache::instance()->leaderBoardModel());
    ui->myProjectionTableView->setModel(&myProjectionsModel);
    myCurrentWeek = -1;
}

PreviousWeekWidget::~PreviousWeekWidget()
{
    delete ui;
}


void PreviousWeekWidget::setWeekData(int week){
    if (myCurrentWeek == week) return;
    myCurrentWeek = week;
     DataCache::instance()->getGameResult(week,myGamesResults);

    //fill game status
    for(const fantasybit::GameResult & game  : myGamesResults) {
        // add game
        QString gameId = game.gameid().data();
        QString homeTeamId,awayTeamId;
        DataCache::instance()->getGameTeamsIdsByWeek(myCurrentWeek,homeTeamId,awayTeamId);
        myGameTableModel.updateItemProperty<PropertyNames::Game_ID>(gameId,gameId);
        myGameTableModel.updateItemProperty<PropertyNames::Kickoff_Time>(gameId,QDateTime::fromMSecsSinceEpoch(game.kickofftime()));
        myGameTableModel.updateItemProperty<PropertyNames::Home>(gameId,homeTeamId);
        myGameTableModel.updateItemProperty<PropertyNames::Away>(gameId,awayTeamId);

        //add home players results
        for(int i=0;i<game.home_result_size();i++) {
            fantasybit::PlayerResult playerResult = game.home_result(i);
            QString playerId = playerResult.playerid().data();
            QString playerFullName = DataCache::instance()->getPlayerFullName(playerId);;
            QString playerPosition = DataCache::instance()->getPlayerPositionInWeek(playerId,myCurrentWeek);
            myProjectionsModel.updateItemProperty<PropertyNames::Player_ID>(playerId,playerId);
            myProjectionsModel.updateItemProperty<PropertyNames::Team_ID>(playerId,homeTeamId);
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Name>(playerId,playerFullName);
            myProjectionsModel.updateItemProperty<PropertyNames::Position>(playerId,playerPosition);
            myProjectionsModel.updateItemProperty<PropertyNames::Result>(playerId,playerResult.result());
        }

        //add away players reults
        for(int i=0;i<game.away_result_size();i++) {
            fantasybit::PlayerResult playerResult = game.away_result(i);
            QString playerId = playerResult.playerid().data();
            QString playerFullName = DataCache::instance()->getPlayerFullName(playerId);;
            QString playerPosition = DataCache::instance()->getPlayerPositionInWeek(playerId,myCurrentWeek);
            myProjectionsModel.updateItemProperty<PropertyNames::Player_ID>(playerId,playerId);
            myProjectionsModel.updateItemProperty<PropertyNames::Team_ID>(playerId,awayTeamId);
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Name>(playerId,playerFullName);
            myProjectionsModel.updateItemProperty<PropertyNames::Position>(playerId,playerPosition);
            myProjectionsModel.updateItemProperty<PropertyNames::Result>(playerId,playerResult.result());
        }
    }
}
