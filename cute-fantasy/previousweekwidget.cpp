#include "previousweekwidget.h"
#include "ui_previousweekwidget.h"
#include "dataservice.h"
#include "datacache.h"

PreviousWeekWidget::PreviousWeekWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviousWeekWidget)
{
    ui->setupUi(this);
    ui->myGamesListView->setModel(&myGameTableModel);
    ui->myProjectionTableView->setModel(&myProjectionsModel);
    myCurrentWeek = -1;
}

PreviousWeekWidget::~PreviousWeekWidget()
{
    delete ui;
}


void PreviousWeekWidget::setWeekData(int week){
    qDebug() << myCurrentWeek << week;

    if (myCurrentWeek == week) return;
    std::unordered_map<std::string,GameInfo> myGameInfos{};

    myCurrentWeek = week;
    DataCache::instance()->getWeeklySchedule(week,myWeeklySchedule);

    myGameTableModel.removeAll();
    for(int i=0;i< myWeeklySchedule.games_size();i++) {
        // add game
        fantasybit::GameInfo game  = myWeeklySchedule.games(i);
        myGameInfos[game.id()] = game;

        QString gameId = game.id().data();
        QString homeTeamId = game.home().data();
        QString awayTeamId= game.away().data();

        myGameTableModel.updateItemProperty<PropertyNames::Game_ID>(gameId,gameId);
        myGameTableModel.updateItemProperty<PropertyNames::Game_Time>(gameId,fromTime_t_toFantasyString(game.time()));
        myGameTableModel.updateItemProperty<PropertyNames::Home>(gameId,homeTeamId);
        myGameTableModel.updateItemProperty<PropertyNames::Away>(gameId,awayTeamId);
    }

    DataCache::instance()->getGameResult(week,myGamesResults);
    myProjectionsModel.removeAll();

    //fill game status
    for(const fantasybit::GameResult & game  : myGamesResults) {
        // add game

        GameInfo mygamei = myGameInfos[game.gameid()];
        QString homeTeamId = mygamei.home().data();
        QString  awayTeamId= mygamei.away().data();

        //add home players results
        for(int i=0;i<game.home_result_size();i++) {
            fantasybit::PlayerResult playerResult = game.home_result(i);

            PlayerBase pb = DataService::instance()->GetPlayerBase(playerResult.playerid());
            if ( !pb.has_position() )
                continue;

            QString playerFullName = QString(pb.first().data()) + " " + QString(pb.last().data());

            QString playerId = playerResult.playerid().data();
            myProjectionsModel.updateItemProperty<PropertyNames::Position>(playerId,pb.position().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_ID>(playerId,playerId);
            myProjectionsModel.updateItemProperty<PropertyNames::Team_ID>(playerId,homeTeamId);
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Name>(playerId,playerFullName);
            myProjectionsModel.updateItemProperty<PropertyNames::Result>(playerId,playerResult.result());
        }

        //add away players reults
        for(int i=0;i<game.away_result_size();i++) {
            fantasybit::PlayerResult playerResult = game.away_result(i);


            PlayerBase pb = DataService::instance()->GetPlayerBase(playerResult.playerid());
            QString playerFullName = QString(pb.first().data()) + " " + QString(pb.last().data());

            QString playerId = playerResult.playerid().data();
            myProjectionsModel.updateItemProperty<PropertyNames::Position>(playerId,pb.position().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_ID>(playerId,playerId);
            myProjectionsModel.updateItemProperty<PropertyNames::Team_ID>(playerId,awayTeamId);
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Name>(playerId,playerFullName);
            myProjectionsModel.updateItemProperty<PropertyNames::Result>(playerId,playerResult.result());
        }
    }
}
