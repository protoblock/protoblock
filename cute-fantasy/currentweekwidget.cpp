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

    //set game filter
    myGameModelFilter.setGameStatusFilter(GamesFilter::OpenGames);
    myGameModelFilter.setDynamicSortFilter(true);
    myGameModelFilter.setSourceModel(&myGameTableModel);
    ui->myGamesListView->setModel(&myGameModelFilter);

    //set selection model
    myGamesSelectionModel.setModel(&myGameModelFilter);
    ui->myGamesListView->setSelectionModel(&myGamesSelectionModel);

    //set projection filter
    myProjectionFilterProxy.reset(new ProjectionsViewFilterProxyModel(ui->myPositionComboBox,&myGameModelFilter,&myGamesSelectionModel));
    myProjectionFilterProxy.data()->setSourceModel(&myProjectionsModel);
    myProjectionFilterProxy.data()->setDynamicSortFilter(true);
    ui->myProjectionTableView->setModel(myProjectionFilterProxy.data());

    //start with upcoming games filter
    ui->myOpenGamesRb->setChecked(true);

    //init projection filters
    myProjectionFilterProxy.data()->bindFilter();

    myCurrentWeek = -1;
    ui->myProjectionTableView->setItemDelegateForColumn(5,&myProjectionDelegate);
}

CurrentWeekWidget::~CurrentWeekWidget() {
    delete ui;
}



void CurrentWeekWidget::setCurrentWeekData(fantasybit::GlobalState state){

    if (myCurrentWeek == myGlobalState.week()) return;
    myProjectionFilterProxy.data()->disable();
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
        myGameTableModel.updateItemProperty<PropertyNames::Game_Time>(gameId,fromTime_t_toFantasyString(game.info.time()));
        myGameTableModel.updateItemProperty<PropertyNames::Game_Status>(gameId,qVariantFromValue<GameStatus_Status>(game.status));
        //add home players
        for(const auto& player : game.homeroster) {
            QString playerId = player.first.data();
            PlayerDetail playerDetails = player.second;
            QString playerName;
            playerName = QString(playerDetails.base.first().data()) + " " + QString(playerDetails.base.last().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Game_ID>(playerId,gameId);
            myProjectionsModel.updateItemProperty<PropertyNames::Player_ID>(playerId,playerId);
            myProjectionsModel.updateItemProperty<PropertyNames::Team_ID>(playerId,game.info.home().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Name>(playerId,playerName);
            myProjectionsModel.updateItemProperty<PropertyNames::Position>(playerId,playerDetails.base.position().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Status>(playerId,
                                                                                qVariantFromValue<PlayerStatus_Status>(playerDetails.team_status));
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Game_Status>(playerId,
                                                                                     qVariantFromValue<PlayerGameStatus>(playerDetails.game_status));
            myProjectionsModel.updateItemProperty<PropertyNames::Projection>(playerId, 0.0);
            myProjectionsModel.updateItemProperty<PropertyNames::ProjectionStatus>(playerId,(int) ScoreState::NonScored);
        }

        //add away players
        for(const auto& player : game.awayroster) {
            QString playerId = player.first.data();
            PlayerDetail playerDetails = player.second;
            QString playerName;
            playerName = QString(playerDetails.base.first().data()) + " " + QString(playerDetails.base.last().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Game_ID>(playerId,gameId);
            myProjectionsModel.updateItemProperty<PropertyNames::Player_ID>(playerId,playerId);
            myProjectionsModel.updateItemProperty<PropertyNames::Team_ID>(playerId,game.info.away().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Name>(playerId,playerName);
            myProjectionsModel.updateItemProperty<PropertyNames::Position>(playerId,playerDetails.base.position().data());
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Status>(playerId,
                                                                                qVariantFromValue<PlayerStatus_Status>(playerDetails.team_status));
            myProjectionsModel.updateItemProperty<PropertyNames::Player_Game_Status>(playerId,
                                                                                     qVariantFromValue<PlayerGameStatus>(playerDetails.game_status));
            myProjectionsModel.updateItemProperty<PropertyNames::Projection>(playerId,0.0);
            myProjectionsModel.updateItemProperty<PropertyNames::ProjectionStatus>(playerId,(int) ScoreState::NonScored);
        }
    }
    invalidateFilters();
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
            int projection = item->propertyValue<PropertyNames::Projection>().toDouble();
            if (projection == 0) continue;
            FantasyBitProj fProj;
            fProj.set_name(myFantasyName);
            fProj.set_proj(projection);
            fProj.set_playerid(playerId.toStdString());
            item->attachProperty<PropertyNames::ProjectionStatus>((int) ScoreState::Sent);
            emit NewProjection(fProj);
        }
    }
}

void CurrentWeekWidget::onGameOver(string gameId){
    myGameTableModel.updateItemProperty<PropertyNames::Game_Status>
            (gameId.data(),qVariantFromValue<GameStatus_Status>(fantasybit::GameStatus_Status_POSTGAME));
        myGameTableModel.updateItemProperty<PropertyNames::Game_Time>(gameId,fromTime_t_toFantasyString(game.info.time()));
  invalidateFilters();
   qDebug() << "Game over";
}
void CurrentWeekWidget::onGameStart(string gameId){
    myGameTableModel.updateItemProperty<PropertyNames::Game_Status>
            (gameId.data(),qVariantFromValue<GameStatus_Status>(fantasybit::GameStatus_Status_INGAME));
        myGameTableModel.updateItemProperty<PropertyNames::Game_Time>(gameId,fromTime_t_toFantasyString(game.info.time()));
   invalidateFilters();
   qDebug() << "Game Start";
}

void CurrentWeekWidget::on_myLockedGamesRb_toggled(bool checked) {
    if (!checked) return;
    myGameModelFilter.setGameStatusFilter(GamesFilter::LockedGames);
    myProjectionFilterProxy.data()->invalidate();
}

void CurrentWeekWidget::on_myOpenGamesRb_toggled(bool checked)
{
    if (!checked) return;
    myGameModelFilter.setGameStatusFilter(GamesFilter::OpenGames);
     myProjectionFilterProxy.data()->invalidate();
}

void CurrentWeekWidget::invalidateFilters(){
    myGameModelFilter.invalidate();
    myProjectionFilterProxy.data()->enable();
    myProjectionFilterProxy.data()->invalidate();
}
