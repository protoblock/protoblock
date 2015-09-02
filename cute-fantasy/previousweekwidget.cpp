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
    ui->myProjectionTableView->resizeColumnsToContents();
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
            myProjectionsModel.updateItemProperty<PropertyNames::PassTD>(playerId,playerResult.stats().ostats().passtd());
            myProjectionsModel.updateItemProperty<PropertyNames::PassYd>(playerId,playerResult.stats().ostats().passyds());
            myProjectionsModel.updateItemProperty<PropertyNames::RushTD>(playerId,playerResult.stats().ostats().rushtd());
            myProjectionsModel.updateItemProperty<PropertyNames::RushYd>(playerId,playerResult.stats().ostats().rushyds());
            myProjectionsModel.updateItemProperty<PropertyNames::RecTD>(playerId,playerResult.stats().ostats().rectd());
            myProjectionsModel.updateItemProperty<PropertyNames::RecYd>(playerId,playerResult.stats().ostats().recyds());
            myProjectionsModel.updateItemProperty<PropertyNames::Rec>(playerId,playerResult.stats().ostats().rec());
            myProjectionsModel.updateItemProperty<PropertyNames::Int>(playerId,playerResult.stats().ostats().pint());
            myProjectionsModel.updateItemProperty<PropertyNames::Fum>(playerId,playerResult.stats().ostats().fumble());
            myProjectionsModel.updateItemProperty<PropertyNames::_2Pt>(playerId,playerResult.stats().ostats().twopt());
            //kicker
            QString fgs;
            auto sz = playerResult.stats().kstats().fg_size();
            if ( sz >0 ) {
                fgs = QString::number(playerResult.stats().kstats().fg(1));
                for (int i=1; i<sz ;i++)
                    fgs.append(",%1").arg(playerResult.stats().kstats().fg(i));
            }

            myProjectionsModel.updateItemProperty<PropertyNames::FG>(playerId,fgs);
            myProjectionsModel.updateItemProperty<PropertyNames::PAT>(playerId,playerResult.stats().kstats().pa());

            //defense
            myProjectionsModel.updateItemProperty<PropertyNames::PtsA>(playerId,playerResult.stats().dstats().ptsa());
            myProjectionsModel.updateItemProperty<PropertyNames::D_TD>(playerId,playerResult.stats().dstats().deftd());
            myProjectionsModel.updateItemProperty<PropertyNames::Sack>(playerId,playerResult.stats().dstats().sacks());
            myProjectionsModel.updateItemProperty<PropertyNames::TA>(playerId,playerResult.stats().dstats().turnovers());
            myProjectionsModel.updateItemProperty<PropertyNames::SFTY>(playerId,playerResult.stats().dstats().sfty());
            myProjectionsModel.updateItemProperty<PropertyNames::D2pt>(playerId,playerResult.stats().dstats().twopt());
            myProjectionsModel.updateItemProperty<PropertyNames::D1pt>(playerId,playerResult.stats().dstats().onept());

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
            myProjectionsModel.updateItemProperty<PropertyNames::PassTD>(playerId,playerResult.stats().ostats().passtd());
            myProjectionsModel.updateItemProperty<PropertyNames::PassYd>(playerId,playerResult.stats().ostats().passyds());
            myProjectionsModel.updateItemProperty<PropertyNames::RushTD>(playerId,playerResult.stats().ostats().rushtd());
            myProjectionsModel.updateItemProperty<PropertyNames::RushYd>(playerId,playerResult.stats().ostats().rushyds());
            myProjectionsModel.updateItemProperty<PropertyNames::RecTD>(playerId,playerResult.stats().ostats().rectd());
            myProjectionsModel.updateItemProperty<PropertyNames::RecYd>(playerId,playerResult.stats().ostats().recyds());
            myProjectionsModel.updateItemProperty<PropertyNames::Rec>(playerId,playerResult.stats().ostats().rec());
            myProjectionsModel.updateItemProperty<PropertyNames::Int>(playerId,playerResult.stats().ostats().pint());
            myProjectionsModel.updateItemProperty<PropertyNames::Fum>(playerId,playerResult.stats().ostats().fumble());
            myProjectionsModel.updateItemProperty<PropertyNames::_2Pt>(playerId,playerResult.stats().ostats().twopt());
            //kicker
            QString fgs;
            auto sz = playerResult.stats().kstats().fg_size();
            if ( sz >0 ) {
                fgs = QString::number(playerResult.stats().kstats().fg(1));
                for (int i=1; i<sz ;i++)
                    fgs.append(",%1").arg(playerResult.stats().kstats().fg(i));
            }

            myProjectionsModel.updateItemProperty<PropertyNames::FG>(playerId,fgs);
            myProjectionsModel.updateItemProperty<PropertyNames::PAT>(playerId,playerResult.stats().kstats().pa());

            //defense
            myProjectionsModel.updateItemProperty<PropertyNames::PtsA>(playerId,playerResult.stats().dstats().ptsa());
            myProjectionsModel.updateItemProperty<PropertyNames::D_TD>(playerId,playerResult.stats().dstats().deftd());
            myProjectionsModel.updateItemProperty<PropertyNames::Sack>(playerId,playerResult.stats().dstats().sacks());
            myProjectionsModel.updateItemProperty<PropertyNames::TA>(playerId,playerResult.stats().dstats().turnovers());
            myProjectionsModel.updateItemProperty<PropertyNames::SFTY>(playerId,playerResult.stats().dstats().sfty());
            myProjectionsModel.updateItemProperty<PropertyNames::D2pt>(playerId,playerResult.stats().dstats().twopt());
            myProjectionsModel.updateItemProperty<PropertyNames::D1pt>(playerId,playerResult.stats().dstats().onept());

        }
    }
}
