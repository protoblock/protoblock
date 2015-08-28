#include "nextweekwidget.h"
#include "ui_nextweekwidget.h"
#include "datacache.h"


NextWeekWidget::NextWeekWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NextWeekWidget)
{
    ui->setupUi(this);
    ui->myGamesListView->setModel(&myGameTableModel);    
    myCurrentWeek = -1;
}

NextWeekWidget::~NextWeekWidget()
{
    delete ui;
}

void NextWeekWidget::setWeekData(int week){
    if (myCurrentWeek == week) return;
    myCurrentWeek = week;
     DataCache::instance()->getWeeklySchedule(week,myWeeklySchedule);
     myGameTableModel.removeAll();
    //fill games
    for(int i=0;i< myWeeklySchedule.games_size();i++) {
        // add game
        fantasybit::GameInfo game  = myWeeklySchedule.games(i);
        QString gameId = game.id().data();
        QString homeTeamId = game.home().data();
        QString  awayTeamId= game.away().data();

        myGameTableModel.updateItemProperty<PropertyNames::Game_ID>(gameId,gameId);
        myGameTableModel.updateItemProperty<PropertyNames::Game_Time>(gameId,QDateTime::fromTime_t(game.time()));
        myGameTableModel.updateItemProperty<PropertyNames::Home>(gameId,homeTeamId);
        myGameTableModel.updateItemProperty<PropertyNames::Away>(gameId,awayTeamId);

//        //add home players results
//        for(int i=0;i<myWeeklySchedule.games_size();i++) {

//            fantasybit::PlayerResult playerResult = game.home_result(i);
//            QString playerId = playerResult.playerid().data();
//            QString playerFullName = DataCache::instance()->getPlayerFullName(playerId);;
//            QString playerPosition = DataCache::instance()->getPlayerPositionInWeek(playerId,myCurrentWeek);;
//            myProjectionsModel.updateItemProperty<PropertyNames::Player_ID>(playerId,playerId);
//            myProjectionsModel.updateItemProperty<PropertyNames::Team_ID>(playerId,homeTeamId);
//            myProjectionsModel.updateItemProperty<PropertyNames::Player_Name>(playerId,playerFullName);
//            myProjectionsModel.updateItemProperty<PropertyNames::Position>(playerId,playerPosition);
//            myProjectionsModel.updateItemProperty<PropertyNames::Result>(playerId,playerResult.result());
//        }

//        //add away players reults
//        for(int i=0;i<game.away_result_size();i++) {
//            fantasybit::PlayerResult playerResult = game.away_result(i);
//            QString playerId = playerResult.playerid().data();
//            QString playerFullName = DataCache::instance()->getPlayerFullName(playerId);;
//            QString playerPosition = DataCache::instance()->getPlayerPosition(playerId,myCurrentWeek);;
//            myProjectionsModel.updateItemProperty<PropertyNames::Player_ID>(playerId,playerId);
//            myProjectionsModel.updateItemProperty<PropertyNames::Team_ID>(playerId,awayTeamId);
//            myProjectionsModel.updateItemProperty<PropertyNames::Player_Name>(playerId,playerFullName);
//            myProjectionsModel.updateItemProperty<PropertyNames::Position>(playerId,playerPosition);
//            myProjectionsModel.updateItemProperty<PropertyNames::Result>(playerId,playerResult.result());
//        }
    }
}

