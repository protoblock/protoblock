#include "previousweekwidget.h"
#include "ui_previousweekwidget.h"
#include "dataservice.h"
#include "datacache.h"

PreviousWeekWidget::PreviousWeekWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviousWeekWidget)
{
    ui->setupUi(this);

    //set game view model
    ui->myGamesListView->setModel(&myGameTableModel);

    //set game selection model
    myGamesSelectionModel.setModel(&myGameTableModel);
    ui->myGamesListView->setSelectionModel(&myGamesSelectionModel);

    //set projection filter
    myProjectionFilterProxy.reset(new ProjectionsViewFilterProxyModel(ui->myPositionComboBox,NULL,&myGamesSelectionModel));
    myProjectionFilterProxy.data()->setSourceModel(&myProjectionsModel);
    myProjectionFilterProxy.data()->setDynamicSortFilter(true);
    myProjectionFilterProxy.data()->setSortRole(Qt::UserRole);
    ui->myProjectionTableView->setModel(myProjectionFilterProxy.data());

    myCurrentWeek = -1;

    //init projection filters
    myProjectionFilterProxy.data()->bindFilter();

    /*
    int width;
    for ( auto h : myProjectionsModel.horizontalHeaders() )
        width += ui->myProjectionTableView->fontMetrics().boundingRect(h).width();
    QSize qsize = ui->myProjectionTableView->size();
    qsize.setWidth(width);
    ui->myProjectionTableView->setBaseSize(qsize);
*/
    QSize qsize2 = ui->myGamesListView->size();
    QString fake = "XXX @ XXX - Xxx 0:00 AM ";
    int width = ui->myGamesListView->fontMetrics().width(fake);
    //int width = rect.width();
    //qsize2.setWidth(width);
    //qsize2.setHeight(rect.height()*17);
    ui->myGamesListView->setBaseSize(qsize2);
    QString fake2 = "XXXXXXXX";
    width += ui->myGamesListView->fontMetrics().boundingRect(fake2).width();
    //qsize2.setWidth(width);
    ui->scheduleGroupBox->setMaximumWidth(width);
    ui->scheduleGroupBox->adjustSize();

    //ui->myProjectionTableView->setBaseSize(qsize);

}

PreviousWeekWidget::~PreviousWeekWidget()
{
    delete ui;
}


void PreviousWeekWidget::setWeekData(int week){
    qDebug() << myCurrentWeek << week;

    if (myCurrentWeek == week) return;
    std::unordered_map<std::string,GameInfo> myGameInfos{};

    //myGameTableModel.removeAll();
    //myProjectionsModel.removeAll();

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
            myProjectionsModel.updateItemProperty<PropertyNames::Game_ID>(playerId,game.gameid().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Position>(playerId,pb.position().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_ID>(playerId,playerId);
            myProjectionsModel.updateItemProperty<PropertyNames::Team_ID>(playerId,homeTeamId);
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Name>(playerId,playerFullName);
            myProjectionsModel.updateItemProperty<PropertyNames::Result>(playerId,playerResult.result());
            if ( playerId,playerResult.stats().ostats().has_passtd() )
            myProjectionsModel.updateItemProperty<PropertyNames::PassTD>(playerId,playerResult.stats().ostats().passtd());
            if ( playerId,playerResult.stats().ostats().passyds() )
            myProjectionsModel.updateItemProperty<PropertyNames::PassYd>(playerId,playerResult.stats().ostats().passyds());
            if ( playerId,playerResult.stats().ostats().has_rushtd() )
            myProjectionsModel.updateItemProperty<PropertyNames::RushTD>(playerId,playerResult.stats().ostats().rushtd());
            if ( playerId,playerResult.stats().ostats().has_rushyds() )
            myProjectionsModel.updateItemProperty<PropertyNames::RushYd>(playerId,playerResult.stats().ostats().rushyds());
            if ( playerId,playerResult.stats().ostats().has_rectd() )
            myProjectionsModel.updateItemProperty<PropertyNames::RecTD>(playerId,playerResult.stats().ostats().rectd());
            if ( playerId,playerResult.stats().ostats().has_recyds() )
            myProjectionsModel.updateItemProperty<PropertyNames::RecYd>(playerId,playerResult.stats().ostats().recyds());
            if ( playerId,playerResult.stats().ostats().has_rec() )
            myProjectionsModel.updateItemProperty<PropertyNames::Rec>(playerId,playerResult.stats().ostats().rec());
            if ( playerId,playerResult.stats().ostats().has_pint() )
            myProjectionsModel.updateItemProperty<PropertyNames::Int>(playerId,playerResult.stats().ostats().pint());
            if ( playerId,playerResult.stats().ostats().has_fumble() )
            myProjectionsModel.updateItemProperty<PropertyNames::Fum>(playerId,playerResult.stats().ostats().fumble());
            if ( playerId,playerResult.stats().ostats().has_twopt() )
            myProjectionsModel.updateItemProperty<PropertyNames::_2Pt>(playerId,playerResult.stats().ostats().twopt());
            //kicker
            QString fgs;
            auto sz = playerResult.stats().kstats().fg_size();
            if ( sz >0 ) {
                fgs = QString::number(playerResult.stats().kstats().fg(0));
                for (int i=1; i<sz ;i++)
                    fgs.append(QString(",%1").arg(playerResult.stats().kstats().fg(i)));
            }

            myProjectionsModel.updateItemProperty<PropertyNames::FG>(playerId,fgs);
            if ( playerId,playerResult.stats().kstats().has_pa() )
            myProjectionsModel.updateItemProperty<PropertyNames::PAT>(playerId,playerResult.stats().kstats().pa());

            //defense
            if ( playerId,playerResult.stats().dstats().has_ptsa() )
            myProjectionsModel.updateItemProperty<PropertyNames::PtsA>(playerId,playerResult.stats().dstats().ptsa());
            if ( playerId,playerResult.stats().dstats().has_deftd() )
            myProjectionsModel.updateItemProperty<PropertyNames::D_TD>(playerId,playerResult.stats().dstats().deftd());
            if ( playerId,playerResult.stats().dstats().has_sacks() )
            myProjectionsModel.updateItemProperty<PropertyNames::Sack>(playerId,playerResult.stats().dstats().sacks());
            if ( playerId,playerResult.stats().dstats().has_turnovers() )
            myProjectionsModel.updateItemProperty<PropertyNames::TA>(playerId,playerResult.stats().dstats().turnovers());
            if ( playerId,playerResult.stats().dstats().has_sfty() )
            myProjectionsModel.updateItemProperty<PropertyNames::SFTY>(playerId,playerResult.stats().dstats().sfty());
            if ( playerId,playerResult.stats().dstats().has_twopt() )
            myProjectionsModel.updateItemProperty<PropertyNames::D2pt>(playerId,playerResult.stats().dstats().twopt());

        }

        //add away players reults
        for(int i=0;i<game.away_result_size();i++) {
            fantasybit::PlayerResult playerResult = game.away_result(i);


            PlayerBase pb = DataService::instance()->GetPlayerBase(playerResult.playerid());
            QString playerFullName = QString(pb.first().data()) + " " + QString(pb.last().data());

            QString playerId = playerResult.playerid().data();
            myProjectionsModel.updateItemProperty<PropertyNames::Game_ID>(playerId,game.gameid().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Position>(playerId,pb.position().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_ID>(playerId,playerId);
            myProjectionsModel.updateItemProperty<PropertyNames::Team_ID>(playerId,awayTeamId);
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Name>(playerId,playerFullName);
            myProjectionsModel.updateItemProperty<PropertyNames::Result>(playerId,playerResult.result());
            if ( playerId,playerResult.stats().ostats().has_passtd() )
            myProjectionsModel.updateItemProperty<PropertyNames::PassTD>(playerId,playerResult.stats().ostats().passtd());
            if ( playerId,playerResult.stats().ostats().passyds() )
            myProjectionsModel.updateItemProperty<PropertyNames::PassYd>(playerId,playerResult.stats().ostats().passyds());
            if ( playerId,playerResult.stats().ostats().has_rushtd() )
            myProjectionsModel.updateItemProperty<PropertyNames::RushTD>(playerId,playerResult.stats().ostats().rushtd());
            if ( playerId,playerResult.stats().ostats().has_rushyds() )
            myProjectionsModel.updateItemProperty<PropertyNames::RushYd>(playerId,playerResult.stats().ostats().rushyds());
            if ( playerId,playerResult.stats().ostats().has_rectd() )
            myProjectionsModel.updateItemProperty<PropertyNames::RecTD>(playerId,playerResult.stats().ostats().rectd());
            if ( playerId,playerResult.stats().ostats().has_recyds() )
            myProjectionsModel.updateItemProperty<PropertyNames::RecYd>(playerId,playerResult.stats().ostats().recyds());
            if ( playerId,playerResult.stats().ostats().has_rec() )
            myProjectionsModel.updateItemProperty<PropertyNames::Rec>(playerId,playerResult.stats().ostats().rec());
            if ( playerId,playerResult.stats().ostats().has_pint() )
            myProjectionsModel.updateItemProperty<PropertyNames::Int>(playerId,playerResult.stats().ostats().pint());
            if ( playerId,playerResult.stats().ostats().has_fumble() )
            myProjectionsModel.updateItemProperty<PropertyNames::Fum>(playerId,playerResult.stats().ostats().fumble());
            if ( playerId,playerResult.stats().ostats().has_twopt() )
            myProjectionsModel.updateItemProperty<PropertyNames::_2Pt>(playerId,playerResult.stats().ostats().twopt());
            //kicker
            QString fgs;
            auto sz = playerResult.stats().kstats().fg_size();
            if ( sz >0 ) {
                fgs = QString::number(playerResult.stats().kstats().fg(0));
                for (int i=1; i<sz ;i++)
                    fgs.append(QString(",%1").arg(playerResult.stats().kstats().fg(i)));
            }

            myProjectionsModel.updateItemProperty<PropertyNames::FG>(playerId,fgs);
            if ( playerId,playerResult.stats().kstats().has_pa() )
            myProjectionsModel.updateItemProperty<PropertyNames::PAT>(playerId,playerResult.stats().kstats().pa());

            //defense
            if ( playerId,playerResult.stats().dstats().has_ptsa() )
            myProjectionsModel.updateItemProperty<PropertyNames::PtsA>(playerId,playerResult.stats().dstats().ptsa());
            if ( playerId,playerResult.stats().dstats().has_deftd() )
            myProjectionsModel.updateItemProperty<PropertyNames::D_TD>(playerId,playerResult.stats().dstats().deftd());
            if ( playerId,playerResult.stats().dstats().has_sacks() )
            myProjectionsModel.updateItemProperty<PropertyNames::Sack>(playerId,playerResult.stats().dstats().sacks());
            if ( playerId,playerResult.stats().dstats().has_turnovers() )
            myProjectionsModel.updateItemProperty<PropertyNames::TA>(playerId,playerResult.stats().dstats().turnovers());
            if ( playerId,playerResult.stats().dstats().has_sfty() )
            myProjectionsModel.updateItemProperty<PropertyNames::SFTY>(playerId,playerResult.stats().dstats().sfty());
            if ( playerId,playerResult.stats().dstats().has_twopt() )
            myProjectionsModel.updateItemProperty<PropertyNames::D2pt>(playerId,playerResult.stats().dstats().twopt());

        }
    }
    //invalidateFilters();
    ui->myProjectionTableView->resizeColumnsToContents();
    ui->myProjectionTableView->selectColumn(4);

}

