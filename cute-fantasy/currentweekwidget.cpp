#include "currentweekwidget.h"
#include "ui_currentweekwidget.h"
#include "dataservice.h"
#include "propertynames.h"
#include "datacache.h"
#include "viewmodels.h"
#include "core.h"

CurrentWeekWidget::CurrentWeekWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurrentWeekWidget) {
    ui->setupUi(this);
    MainLAPIWorker * theLAPIWorker = Core::resolveByName<MainLAPIWorker>("coreapi");

    if (theLAPIWorker!=NULL)
        QObject::connect(this,SIGNAL(NewProjection(fantasybit::FantasyBitProj)),
                         theLAPIWorker,SLOT(OnProjTX(fantasybit::FantasyBitProj)));

    myGameModelFilter.setGameStatusFilter(GamesFilter::Upcoming);
    myGameModelFilter.setDynamicSortFilter(true);
    myGameModelFilter.setSourceModel(&myGameTableModel);
    ui->myGamesListView->setModel(&myGameModelFilter);
    ui->myProjectionTableView->setModel(&myProjectionsModel);
    //start with upcoming games filter
    ui->myUpcomingGamesRb->setChecked(true);

    myCurrentWeek = -1;
    ui->myProjectionTableView->setItemDelegateForColumn(5,&myProjectionDelegate);
}

CurrentWeekWidget::~CurrentWeekWidget() {
    delete ui;
}

void CurrentWeekWidget::on_myCompletedGamesRb_toggled(bool checked) {
    if (checked)
        if (myGameModelFilter.filter()== GamesFilter::Completed)
            return;
        else {
            myGameModelFilter.setGameStatusFilter(GamesFilter::Completed);
            ui->myGamesListView->repaint();
        }
}

void CurrentWeekWidget::on_myInGamesRb_toggled(bool checked) {
    if (checked)
        if (myGameModelFilter.filter()== GamesFilter::InGame)
            return;
        else {
            myGameModelFilter.setGameStatusFilter(GamesFilter::InGame);
            ui->myGamesListView->repaint();
        }
}

void CurrentWeekWidget::on_myUpcomingGamesRb_toggled(bool checked) {
    if (checked)
        if (myGameModelFilter.filter()== GamesFilter::Upcoming)
            return;
        else {
            myGameModelFilter.setGameStatusFilter(GamesFilter::Upcoming);
            ui->myGamesListView->repaint();
        }
}

void CurrentWeekWidget::setCurrentWeekData(fantasybit::GlobalState state){

    if (myCurrentWeek == myGlobalState.week()) return;
    myGlobalState = state;
    myCurrentWeek = myGlobalState.week();
    myGameTableModel.removeAll();
    myProjectionsModel.removeAll();
    myGameRosters = DataService::instance()->GetCurrentWeekGameRosters();

    //fill game status
    for(const fantasybit::GameRoster & game  : myGameRosters) {
        // add game
        QString gameId = game.info.id().data();
        myGameTableModel.updateItemProperty<PropertyNames::Game_ID>(gameId,gameId);
        myGameTableModel.updateItemProperty<PropertyNames::Away>(gameId,game.info.away().data());
        myGameTableModel.updateItemProperty<PropertyNames::Home>(gameId,game.info.home().data());
        myGameTableModel.updateItemProperty<PropertyNames::Game_Time>(gameId,QDateTime::fromTime_t(game.info.time()));
        myGameTableModel.updateItemProperty<PropertyNames::Game_Status>(gameId,game.status);
        //add home players
        for(const auto& player : game.homeroster) {
            QString playerId = player.first.data();
            PlayerDetail playerDetails = player.second;
            QString playerName;
            playerName = QString(playerDetails.base.first().data()) + " " + QString(playerDetails.base.last().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_ID>(playerId,playerId);
            myProjectionsModel.updateItemProperty<PropertyNames::Team_ID>(playerId,game.info.home().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Name>(playerId,playerName);
            myProjectionsModel.updateItemProperty<PropertyNames::Position>(playerId,playerDetails.base.position().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Status>(playerId,playerDetails.team_status);
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Game_Status>(playerId,playerDetails.game_status);
            myProjectionsModel.updateItemProperty<PropertyNames::Projection>(playerId,0);
            myProjectionsModel.updateItemProperty<PropertyNames::ProjectionStatus>(playerId,(int) ScoreState::NonScored);
        }

        //add away players
        for(const auto& player : game.awayroster) {
            QString playerId = player.first.data();
            PlayerDetail playerDetails = player.second;
            QString playerName;
            playerName = QString(playerDetails.base.first().data()) + " " + QString(playerDetails.base.last().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_ID>(playerId,playerId);
            myProjectionsModel.updateItemProperty<PropertyNames::Team_ID>(playerId,game.info.away().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Name>(playerId,playerName);
            myProjectionsModel.updateItemProperty<PropertyNames::Position>(playerId,playerDetails.base.position().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Status>(playerId,playerDetails.team_status);
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Game_Status>(playerId,playerDetails.game_status);
            myProjectionsModel.updateItemProperty<PropertyNames::Projection>(playerId,0);
            myProjectionsModel.updateItemProperty<PropertyNames::ProjectionStatus>(playerId,(int) ScoreState::NonScored);
        }
    }
}

void CurrentWeekWidget::onUserSwitchFantasyName(const std::string fantasyPlayerId){
    //do nothing if the received name is the old one
    if (myFantasyName == fantasyPlayerId) return;

    //case when we received a name in the app start
    if (myFantasyName=="" && fantasyPlayerId != "" ){
        myFantasyName = fantasyPlayerId;
        updateCurrentFantasyPlayerProjections();
    }

    //case when we switched fantasy names
    if (myFantasyName!="" && myFantasyName != fantasyPlayerId){
        myFantasyName = fantasyPlayerId;
        //reset all projections to 0
        foreach(QString playerId,myProjectionsModel.keys()){
            myProjectionsModel.updateItemProperty<PropertyNames::ProjectionStatus>(playerId,(int) ScoreState::NonScored);
            myProjectionsModel.updateItemProperty<PropertyNames::Projection>(playerId,0);
        }
        updateCurrentFantasyPlayerProjections();
    }
}

void CurrentWeekWidget::updateCurrentFantasyPlayerProjections(){
    //update to recent projection projection and mark them a sent
    auto  recentProjections = DataService::instance()->GetProjByName(myFantasyName);
    for ( auto it = recentProjections.begin(); it != recentProjections.end(); ++it ){
        myProjectionsModel.updateItemProperty<PropertyNames::Projection>(it->first.data(),it->second);
        myProjectionsModel.updateItemProperty<PropertyNames::ProjectionStatus>(it->first.data(),(int) ScoreState::Sent);
    }
}

void CurrentWeekWidget::on_mySendProjectionButton_clicked() {
    if (myCurrentWeek == myGlobalState.week()){
        for( auto  playerId : myProjectionsModel.keys() ){
            ViewModel * item = myProjectionsModel.itemByKey(playerId);
            if (item == NULL) continue;
            int projection = item->propertyValue<PropertyNames::Projection>().toInt();
            if (projection == 0) continue;
            FantasyBitProj fProj;
            fProj.set_name(myFantasyName);
            fProj.set_proj(projection);
            fProj.set_playerid(playerId.toStdString());
            emit NewProjection(fProj);
        }
    }
}
