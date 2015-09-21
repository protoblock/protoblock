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
        QObject::connect(this,SIGNAL(NewProjection(vector<fantasybit::FantasyBitProj>)),
                         theLAPIWorker,SLOT(OnProjTX(vector<fantasybit::FantasyBitProj>)));

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
    myProjectionDelegate.setTableView(ui->myProjectionTableView);
    ui->myProjectionTableView->setItemDelegateForColumn(4,&myProjectionDelegate);
    //QObject::connect(&myProjectionDelegate,SIGNAL(commitData(QWidget*)),this,SLOT(onProjectionEnterPressed(QWidget *)));

}

CurrentWeekWidget::~CurrentWeekWidget() {
    delete ui;
}

void CurrentWeekWidget::setCurrentWeekData(fantasybit::GlobalState state){

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
            myProjectionsModel.updateItemProperty<PropertyNames::Projection>(playerId, 0);
            myProjectionsModel.updateItemProperty<PropertyNames::ProjectionStatus>(playerId,QVariant::fromValue(ScoreState::NonScored));
            myProjectionsModel.updateItemProperty<PropertyNames::KnownProjection>(playerId,-1);

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
            myProjectionsModel.updateItemProperty<PropertyNames::Projection>(playerId,0);
            myProjectionsModel.updateItemProperty<PropertyNames::ProjectionStatus>(playerId,QVariant::fromValue(ScoreState::NonScored));
            myProjectionsModel.updateItemProperty<PropertyNames::KnownProjection>(playerId,-1);

        }
    }
    invalidateFilters();
    updateCurrentFantasyPlayerProjections();
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
            myProjectionsModel.updateItemProperty<PropertyNames::ProjectionStatus>
                    (playerId,QVariant::fromValue(ScoreState::NonScored));
            myProjectionsModel.updateItemProperty<PropertyNames::Projection>(playerId,0);   
            myProjectionsModel.updateItemProperty<PropertyNames::KnownProjection>(playerId,-1);

        }
        updateCurrentFantasyPlayerProjections();
    }
}

void CurrentWeekWidget::updateCurrentFantasyPlayerProjections(){
    //update to recent projection projection and mark them a sent
    auto  recentProjections = DataService::instance()->GetProjByName(myFantasyName);
    for ( auto it = recentProjections.begin(); it != recentProjections.end(); ++it ){
        myProjectionsModel.updateItemProperty<PropertyNames::Projection>(it->first.data(),it->second);
        myProjectionsModel.updateItemProperty<PropertyNames::ProjectionStatus>
                (it->first.data(),QVariant::fromValue(ScoreState::Scored));
        myProjectionsModel.updateItemProperty<PropertyNames::KnownProjection>(it->first.data(),it->second);

    }
}

void CurrentWeekWidget::on_mySendProjectionButton_clicked() {
    std::unordered_map<string,vector<FantasyBitProj>> projbygame{};
    if (myCurrentWeek == myGlobalState.week()){
        //ProjectionTransBlock  ptb{};
        for( auto  playerId : myProjectionsModel.keys() ){
            ViewModel * item = myProjectionsModel.itemByKey(playerId);
            if (item == NULL)
                continue;

            int projection = item->propertyValue<PropertyNames::Projection>().toInt();
            if (projection == 0)
                continue;

            QString gameId = item->propertyValue<PropertyNames::Game_ID>().toString();

            //check game status
            QVariant vGameStatus;
            bool found = myGameTableModel.itemPropertyValue<PropertyNames::Game_Status>(gameId,vGameStatus);
            if (!found)  continue;
            GameStatus_Status gameStatus = vGameStatus.value<fantasybit::GameStatus_Status>();

            if ( gameStatus != GameStatus_Status_SCHEDULED && gameStatus != GameStatus_Status_PREGAME )
                continue;

            int knownprojection = item->propertyValue<PropertyNames::KnownProjection>().toInt();
            if ( knownprojection == projection)
                continue;


            vector<FantasyBitProj> &vproj = projbygame[gameId.toStdString()];

            FantasyBitProj fproj;
            fproj.set_name(myFantasyName);
            fproj.set_proj(projection);
            fproj.set_playerid(playerId.toStdString());
            vproj.push_back(fproj);

            item->attachProperty<PropertyNames::ProjectionStatus>(QVariant::fromValue(ScoreState::Sent));
        }
    }

    for ( auto &vg : projbygame)
        emit NewProjection(vg.second);

}

void CurrentWeekWidget::onGameOver(string gameId){
    myGameTableModel.updateItemProperty<PropertyNames::Game_Status>
            (gameId.data(),qVariantFromValue<GameStatus_Status>(fantasybit::GameStatus_Status_POSTGAME));
  invalidateFilters();
   qDebug() << "Game over " << gameId;
}
void CurrentWeekWidget::onGameStart(string gameId){
    myGameTableModel.updateItemProperty<PropertyNames::Game_Status>
            (gameId.data(),qVariantFromValue<GameStatus_Status>(fantasybit::GameStatus_Status_INGAME));
   invalidateFilters();
   qDebug() << "Game Start " << gameId;
}

void CurrentWeekWidget::on_myLockedGamesRb_toggled(bool checked) {
    if (!checked) return;
    setProjectionEnabled(false);
    myGameModelFilter.setGameStatusFilter(GamesFilter::LockedGames);
    myProjectionFilterProxy.data()->invalidate();

}

void CurrentWeekWidget::on_myOpenGamesRb_toggled(bool checked)
{
    if (!checked) return;
    setProjectionEnabled(true);
    myGameModelFilter.setGameStatusFilter(GamesFilter::OpenGames);
     myProjectionFilterProxy.data()->invalidate();
}

void CurrentWeekWidget::invalidateFilters(){
    myGameModelFilter.invalidate();
    myProjectionFilterProxy.data()->enable();
    myProjectionFilterProxy.data()->invalidate();
}

void CurrentWeekWidget::OnPlayerStatusChange(pair<string,PlayerStatus>   in){
 QString playerId = in.first.data(); 
 myProjectionsModel.updateItemProperty<PropertyNames::Player_Status>(playerId, QVariant::fromValue(in.second.status()));
 myProjectionFilterProxy.data()->invalidate();
}

void CurrentWeekWidget::OnProjAck(fantasybit::FantasyBitProj projection){
    if ( myFantasyName != projection.name() ) {
        qDebug() << "Received OnProjAck != myFantasyName" << myFantasyName;
    }

    QString playerId = projection.playerid().data();
    //bug what if the player change the projection he have sent before the ack gets to us??

    //QVariant known;
    //myProjectionsModel.itemPropertyValue<PropertyNames::KnownProjection>(playerId,known);
    //int knownproj = known.toInt();
    myProjectionsModel.updateItemProperty<PropertyNames::KnownProjection>(playerId,projection.proj());
    myProjectionsModel.updateItemProperty<PropertyNames::Projection>(playerId,projection.proj());
    myProjectionsModel.updateItemProperty<PropertyNames::ProjectionStatus>(playerId, QVariant::fromValue(ScoreState::Scored));
}


void CurrentWeekWidget::setProjectionEnabled(bool on){
    ui->mySendProjectionButton->setEnabled(on);
    myProjectionDelegate.setEnabled(on);
}

void CurrentWeekWidget::toggleFantasyNameColumn(const QString & fantasyName){
    if (!myProjectionsModel.hasColumn(fantasyName))
    {
        //refresheing is done when leaderboard model refresh
        std::unordered_map<std::string,int> theOtherGuyProjection = DataService::instance()->GetProjByName(fantasyName.toStdString());
        for (std::pair<std::string,int> elem : theOtherGuyProjection){
            std::string playerId = elem.first;
            int projection = elem.second;
            if (projection == 0)  continue;
            myProjectionsModel.updateItemProperty(fantasyName,QString(playerId.data()),projection);
        }
    }

    if (myProjectionsModel.hasColumn(fantasyName)){
        bool isVisible = myProjectionsModel.isColumnVisible(fantasyName);
        myProjectionsModel.setColumnVisible(fantasyName,!isVisible);
    }
    else {        
        myProjectionsModel.appendColumn(fantasyName,true);
    }
}

void CurrentWeekWidget::onSendFantasyNameProjection(const std::string & fantasyName){
    std::unordered_map<std::string,int> theOtherGuyProjection = DataService::instance()->GetProjByName(fantasyName);
    std::unordered_map<string,vector<FantasyBitProj>> projbygame{};
    int projectionsCount =0;
    for (std::pair<std::string,int> elem : theOtherGuyProjection){
        std::string playerId = elem.first;
        int projection = elem.second;
        if (projection == 0)  continue;
        QVariant vGameId;
        bool found =myProjectionsModel.itemPropertyValue<PropertyNames::Game_ID>(QString(playerId.data()),vGameId);
        if (!found) continue;
        QString gameId = vGameId.toString();
        if (gameId.isEmpty()) continue;


        //check game status
        QVariant vGameStatus;
        found = myGameTableModel.itemPropertyValue<PropertyNames::Game_Status>(gameId,vGameStatus);
        if (!found)  continue;
        GameStatus_Status gameStatus = vGameStatus.value<fantasybit::GameStatus_Status>();

        if ( gameStatus != GameStatus_Status_SCHEDULED && gameStatus != GameStatus_Status_PREGAME )
            continue;

        QVariant vMyProjection;
        found = myProjectionsModel.itemPropertyValue<PropertyNames::KnownProjection>(QString(playerId.data()),vMyProjection);
         if (found){
            int knownprojection = vMyProjection.toInt();
            if ( knownprojection == projection)
                continue;
         }

        vector<FantasyBitProj> &vproj = projbygame[gameId.toStdString()];

        projectionsCount++;
        FantasyBitProj fproj;
        fproj.set_name(myFantasyName);
        fproj.set_proj(projection);
        fproj.set_playerid(playerId);
        vproj.push_back(fproj);
    }
    for ( auto &vg : projbygame)
        emit NewProjection(vg.second);

     QMessageBox::information(this,APPLICATION_NAME,QString("%1 projections have been copied and sent.").arg(projectionsCount));
}


void CurrentWeekWidget::refreshFantasyNamesProjections(const QString & fantasyName){
    if (!myProjectionsModel.hasColumn(fantasyName)) return;
    //refresheing is done when leaderboard model refresh
    std::unordered_map<std::string,int> theOtherGuyProjection = DataService::instance()->GetProjByName(fantasyName.toStdString());
    for (std::pair<std::string,int> elem : theOtherGuyProjection){
        std::string playerId = elem.first;
        int projection = elem.second;
        if (projection == 0)  continue;
        myProjectionsModel.updateItemProperty(fantasyName,QString(playerId.data()),projection);
    }
}

void CurrentWeekWidget::onControlMessage(QString message){
    ui->myControlMessageLabel->setText(message);
}

#ifdef MIKECLAYIMPORT
#include "playerloader.h"

void CurrentWeekWidget::on_importmike_clicked()
{
    MikeClayLoader ml{};
    auto vpp = ml.loadProjFromFile();

    for ( auto pp : vpp )
    myProjectionsModel.updateItemProperty<PropertyNames::Projection>(pp.playerid().data(),pp.points());
}
#endif
