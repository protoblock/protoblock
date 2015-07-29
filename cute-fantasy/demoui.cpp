//
//  DemoUI.cpp
//  cute-fantasy
//
//  Created by Jay Berg on 4/8/14.
//
//
#include "demoui.h"
#include "client.h"
#include <ProtoData.pb.h>
#include "ui_demoui.h"
#include "teamsloader.h"
#include "playerloader.h"

#include <QDateTime>
#include <QClipboard>
#include <QItemEditorFactory>

using namespace fantasybit;

DemoUI::DemoUI(QWidget *parent) : QWidget(parent), ui(new Ui::DemoUI)
{
    ui->setupUi(this);
    ui->textBrowser->append(QCoreApplication::applicationDirPath());
    ui->myFantasyPlayersTableView->setModel(&myFantasyPlayerTableModel);

    TeamLoader teamLoader;
    PlayerLoader playerLoader;
    ScheduleLoader scheduleLoader;

    QString error ;
    bool success = teamLoader.loadTeamsFromJsonFile(myPreloadedTeams,error);
    if (!success)
        QMessageBox::critical(this,"Loading teams from JSon File",error);

    success = playerLoader.loadPlayersFromJsonFile(myPreloadedPlayers,error);
    if (!success)
        QMessageBox::critical(this,"Loading players from JSon File",error);

    qDebug() << " about to start schedule";

    success = scheduleLoader.loadSchedulesFromJsonFile(myPreloadedSchedule,error);
    if (!success)
        QMessageBox::critical(this,"Loading players from JSon File",error);

    PlayerLoader::JsonPlayer jp{};
    jp.FantasyPosition = "DEF";
    jp.DepthPosition = "DEF";
    jp.DepthOrder = 1;
    for ( auto t : myPreloadedTeams) {
        jp.Name = t.myFullName;
        jp.PlayerID = t.myPlayerID;
        jp.Team = t.myKey;

        myPreloadedPlayers.append(jp);
        PlayerLoader::CachePlayer(jp);
    }

    ui->myGamesTabWidget->clear();

    //force load week 1
    this->loadWeekUIElements(1);

    /*
    if (!success)
        QMessageBox::critical(this,"Loading teams from JSon File",error);
    else {
        for(int i=0;i<myPreloadedTeams.size();i++){
            TeamLoader::JsonTeam team = myPreloadedTeams[i];
            ui->myTeamsCmb->addItem(team.myKey +":"+  team.myFullName, QVariant(team.myKey));
        }
    }
    */

}

/*
void DemoUI::flashing()
{
    ui->progressBar->setVisible(true);
}
*/

void DemoUI::fromServer(const DeltaData &in)
{
    if ( first ) {
        first = false;
        ui->textBrowser->append(QDateTime::currentDateTime().toString() + " Connected");

        ui->message->setText("Live: ");
        ui->fantasyname->setEnabled(true);
        ui->generate->setEnabled(true);
    }

    ui->textBrowser->append(QString::fromStdString(in.DebugString()));


    refreshViews(in);
}
/**/

DemoUI::~DemoUI()
{
    delete ui;
}


void DemoUI::on_generate_clicked()
{
    try {

        if ( ui->fantasyname->text() == "") return;

        ui->textBrowser->append(QDateTime::currentDateTime().toString() + " requesting: " + ui->fantasyname->text());

        indata.set_type(InData_Type_NEWNAME);
        indata.set_data(ui->fantasyname->text().toStdString());

        emit fromGUI(indata);
    }
    catch (...) {}
}



void DemoUI::refreshViews(const DeltaData &in){
    //QMutexLocker locker(&myMutex);
    //    if (in.type() == DeltaData_Type_SNAPSHOT) {
    //        myTeamsStateTableModel.removeAll();
    //        myPlayerDataTableModel.removeAll();
    //        myTeamDataTableModel.removeAll();
    //        myDemoTableModel.removeAll();
    //    }

    int previousWeek = myCurrentSnapShot.week;
    bool weekChanged = false;
    myCurrentSnapShot.fromDeltaData(in);
    weekChanged = previousWeek != myCurrentSnapShot.week;

    ui->message->setText(QString::fromStdString(
                             "Live: " +
                             GlobalState_State_Name(myCurrentSnapShot.globalStateModel.state()) +
                             " " +
                             std::to_string(myCurrentSnapShot.globalStateModel.season()) +
                             " Week "
                             +
                             std::to_string(myCurrentSnapShot.week)
                             ));


    //ui->mySnapshotTimestamp->setText(QDateTime::currentDateTime().toString(Qt::ISODate));
    //ui->mySeasonLE->setText(myCurrentSnapShot.globalStateModel.seasonString());
    //ui->myGlobalStateLE->setText(myCurrentSnapShot.globalStateModel.stateString());
    //ui->weekBox->setValue(myCurrentSnapShot.week);


    ui->fantasyname->setText(myCurrentSnapShot.fantasyName);
    if ( myCurrentSnapShot.fantasyName != "") {
        //ui->fantasyname->setText(QString::fromStdString(in.myfantasyname().name()));
        ui->fantasynamestatus->setText("Fantasy Name Status: " +
                                       QString::fromStdString(MyNameStatus_Name(myCurrentSnapShot.myNameStatus)));
    }

    if ( myCurrentSnapShot.myNameStatus == MyNameStatus::confirmed) {
        ui->myFantasyNameLE->setText(myCurrentSnapShot.fantasyName);
        ui->myBalance->display(QString::number(myCurrentSnapShot.fantasyNameBalance));
    }


    foreach(QString key,myCurrentSnapShot.teamStates.uniqueKeys())
        myTeamsStateTableModel.setItemValue(key,myCurrentSnapShot.teamStates[key]);


    foreach(QString key,myCurrentSnapShot.players.uniqueKeys())
        myPlayerDataTableModel.setItemValue(key,myCurrentSnapShot.players[key]);


    foreach(QString key,myCurrentSnapShot.teams.uniqueKeys()) {
        myTeamDataTableModel.setItemValue(key,myCurrentSnapShot.teams[key]);
        //ui->myTeamsCmb->addItem(key, QVariant(myCurrentSnapShot.teams[key].teamId()));
    }

    foreach(QString key,myCurrentSnapShot.fantasyPlayers.uniqueKeys()) {
        myFantasyPlayerTableModel.setItemValue(key,myCurrentSnapShot.fantasyPlayers[key]);
    }

    // refresh current games schedule and user projections
    // check 1 : Season Changed  => not worrying about that now.
    // check 2 : Week Changed  => 1- purge all projection ui , reload new schedule with new games and tabs
    //                           2- refresh team-players association and player availability => not to worry about that now)

    // check 3 : player changed team or absent from game => remove current projection
    // check 4 : One or many game states have changed


    // game transition (pregame => ingame) freeze projections of the game.

    if (weekChanged)
      loadWeekUIElements(myCurrentSnapShot.week);
    else {
        //check the games that have transitted and freeze them
        for(int i=0; i< ui->myGamesTabWidget->count();i++){
            QWidget * tabWidget = ui->myGamesTabWidget->widget(i);
            QObjectUserData * data = tabWidget->userData(0);
            TabData * tabTeamData = dynamic_cast<TabData *>(data);
            if (tabTeamData == NULL) continue; //should never happen
            TeamState_State gameState = getCurrentGameState(tabTeamData->myHomeTeam,tabTeamData->myAwayTeam);
            //we need to update ui and freez projection when game state change
            if (gameState == tabTeamData->myGameState) continue;
            tabTeamData->myGameState = gameState;
            QTableView * scoringTableView  = dynamic_cast<QTableView*>(tabWidget);
            if (scoringTableView==NULL) continue; //shouldn't happen
            GameProjectionTableModel * model = dynamic_cast<GameProjectionTableModel *>(scoringTableView->model());
            if (model ==NULL) continue; //shouldn't happen
            if (gameState == TeamState_State_PREGAME) {
               scoringTableView->setItemDelegateForColumn(3,&myDelegate);
               ui->myGamesTabWidget->tabBar()->setTabTextColor(i,QColor(Qt::darkGreen));
            }
            else {
                model->setEditable(false);
                ui->myGamesTabWidget->tabBar()->setTabTextColor(i,QColor(Qt::darkRed));
            }
        }
    }
}

TeamState_State DemoUI::getCurrentGameState(const QString & homeTeam, const QString & awayTeam) {
    int homeTeamState = myTeamsStateTableModel.getTeamState(homeTeam);
    int awayTeamState = myTeamsStateTableModel.getTeamState(awayTeam);
    TeamState_State gameState = TeamState_State_PREGAME;

    if (!(homeTeamState == ::TeamState_State_PREGAME &&
          awayTeamState == ::TeamState_State_PREGAME
         )){
        gameState = TeamState_State_INGAME;
    }

    return gameState;
}


void DemoUI::loadWeekUIElements(int week){
    this->clearScheduleUI();
     QList<ScheduleLoader::JsonSchedule> games = myPreloadedSchedule.values(week);
     for(int i = 0; i < games.size(); ++i) {
         //check if we have a 2 team states for the teams in the scheduled game read from static resources
          ScheduleLoader::JsonSchedule game =  games[i];

          TeamState_State gameState = getCurrentGameState(game.HomeTeam,game.AwayTeam);

          GameProjectionTableModel * gameProjectionTableModel = new GameProjectionTableModel();
          gameProjectionTableModel->setEditable(true);
         //add players
         QList<QString>  homePlayers = PlayerLoader::getTeamPlayers(game.HomeTeam);
         QList<QString>  awayPlayers = PlayerLoader::getTeamPlayers(game.AwayTeam);

         for(int i=0;i< homePlayers.size();i++){
             GameProjectionModelView * projection = new GameProjectionModelView();
             projection->myPlayerId = homePlayers[i];
             projection->myTeamId = game.HomeTeam;
             projection->myScore = 0;
             gameProjectionTableModel->addItem(projection);
         }

         for(int i=0;i< awayPlayers.size();i++){
             GameProjectionModelView * projection = new GameProjectionModelView();
             projection->myPlayerId = awayPlayers[i];
             projection->myTeamId = game.AwayTeam;
             projection->myScore = 0;
             gameProjectionTableModel->addItem(projection);
         }


         QTableView * scoringTableView = new QTableView(ui->myGamesTabWidget);
         scoringTableView->setUserData(0,new TabData(game.HomeTeam,game.AwayTeam,gameState));
         scoringTableView->setObjectName(game.GameKey);
         //gameProjectionTableModel->setEditable(true);
         scoringTableView->setModel(gameProjectionTableModel);
         int index = ui->myGamesTabWidget->addTab(scoringTableView,
                                                  game.HomeTeam + " vs " + game.AwayTeam);

         if (gameState == TeamState_State_PREGAME) {
            scoringTableView->setItemDelegateForColumn(3,&myDelegate);
            ui->myGamesTabWidget->tabBar()->setTabTextColor(index,QColor(Qt::darkGreen));
         }
         else {
             gameProjectionTableModel->setEditable(false);
             ui->myGamesTabWidget->tabBar()->setTabTextColor(index,QColor(Qt::darkRed));
         }

         myGameProjectionTableModels.append(gameProjectionTableModel);

     }
}

/*
void DemoUI::on_myTeamsCmb_currentIndexChanged(int index)
{
   QString teamKey = ui->myTeamsCmb->itemData(index).toString();
   //reload players combo
   ui->myPlayersCmb->clear();
   foreach(PlayerDataViewModel * player, myPlayerDataTableModel.list()){
       if (player->teamId().trimmed().toUpper()== teamKey.trimmed().toUpper())
        ui->myPlayersCmb->addItem(player->playerId(),QVariant(player->playerId()));
   }
}
*/
/*
void DemoUI::on_myAddScoringLineButton_clicked()
{
    if (ui->myTeamsCmb->currentData().isValid() &&
            ui->myPlayersCmb->currentData().isValid()){
       QString teamId,teamName,playerId;
       teamId = ui->myTeamsCmb->currentData().toString();
       teamName = ui->myTeamsCmb->currentText();
       playerId = ui->myPlayersCmb->currentData().toString();

       //check if the player is already scored
       bool added = false;
       foreach(ScoringModelView * scoring,myScoringTableModel.list() ) {
           if (scoring->myTeamId.trimmed().toUpper() == teamId.trimmed().toUpper()&&
               scoring->myPlayerId.trimmed().toUpper() == playerId.trimmed().toUpper()){
               added = true;
               break;
           }
       }
       if (!added)
           myScoringTableModel.addItem(new ScoringModelView(teamId,teamName,playerId));
       else
           QMessageBox::critical(this,"Cute Fantasy","You've already scored the selected player!");

    }
    else
        QMessageBox::critical(this,"Cute Fantasy","Please select a team and a player before adding a new line!");

}
*/

/*
void DemoUI::on_myAddTeam_clicked()
{
    if (ui->myTeamsCmb->currentData().isValid() )
    {
        ui->teamStartList->addItem(ui->myTeamsCmb->currentData().toString());
        myTeamTransitions.append(ui->myTeamsCmb->currentData().toString());
    }
}

*/

void DemoUI::on_mySendProjectionsButton_clicked()
{

    for(int i=0; i< ui->myGamesTabWidget->count();i++){
        QWidget * tabWidget = ui->myGamesTabWidget->widget(i);
        QObjectUserData * data = tabWidget->userData(0);
        TabData * tabTeamData = dynamic_cast<TabData *>(data);
        if (tabTeamData == NULL) continue; //should never happen
        if (tabTeamData->myGameState != TeamState_State_PREGAME) continue;
        QTableView * scoringTableView  = dynamic_cast<QTableView*>(tabWidget);
        if (scoringTableView==NULL) continue; //shouldn't happen
        GameProjectionTableModel * model = dynamic_cast<GameProjectionTableModel *>(scoringTableView->model());
        if (model ==NULL) continue; //shouldn't happen
        foreach(GameProjectionModelView * scoring,model->list() ) {
            indata.Clear();
            indata.set_type(InData_Type_PROJ);
            indata.set_data2(scoring->myPlayerId.toStdString());
            indata.set_num(scoring->myScore);
            scoring->myScoreState = GameProjectionModelView::Sent;
            scoringTableView->repaint();
            emit fromGUI(indata);
        }

    }




    /*
    foreach(ScoringModelView * scoring,myScoringTableModel.list() ) {
        indata.Clear();
        indata.set_type(InData_Type_PROJ);
        indata.set_data2(scoring->myPlayerId.toStdString());
        indata.set_num(scoring->myScore);
        emit fromGUI(indata);
    }
    */
}

/*
void DemoUI::on_mySendResultsButton_clicked()
{
    DataTransition dt{};

    dt.set_type(static_cast<DataTransition_Type>(ui->dataTransitionCombo->currentData().toInt()));
    dt.set_season(ui->seasonBox->value());
    dt.set_week(ui->weekBox->value());

    for (auto st :  myTeamTransitions) {
        dt.add_teamid(st.toStdString());
    }

    foreach(ScoringModelView * scoring,myScoringTableModel.list() ) {
        Data d{};
        d.set_type(Data::RESULT);
        ::fantasybit::DemoPoints fpp{};
        fpp.set_season(dt.season());
        fpp.set_week(dt.week());
        fpp.set_playerid(scoring->myPlayerId.toStdString());
        fpp.set_points(scoring->myScore);
        ResultData rd{};
        rd.mutable_fpp()->CopyFrom(fpp);

        d.MutableExtension(ResultData::result_data)->CopyFrom(rd);
        Data *d2 = dt.add_data();
        d2->CopyFrom(d);
    }

    if ( ui->allTeams->isChecked() ) {
        Data d{};
        d.set_type(Data::TEAM);
        TeamData td{};
        for ( auto t : myPreloadedTeams ) {
            td.set_teamid(t.myKey.toStdString());
            d.MutableExtension(TeamData::team_data)->CopyFrom(td);
            Data *d2 = dt.add_data();
            d2->CopyFrom(d);
        }
    }

    if ( ui->allPLayers->isChecked() ) {
        Data d{};
        d.set_type(Data::PLAYER);
        PlayerData td{};
        for ( auto t : myPreloadedPlayers ) {
            td.set_teamid(t.Team.toStdString());
            td.set_playerid(t.Name.toStdString());
            d.MutableExtension(PlayerData::player_data)->CopyFrom(td);
            Data *d2 = dt.add_data();
            d2->CopyFrom(d);
        }
    }

    indata.set_type(InData_Type_DATA);
    indata.mutable_trans()->CopyFrom(dt);

    emit fromGUI(indata);
}
*/

void DemoUI::on_myDeleteAllRowsButton_clicked()
{
    /*
    myScoringTableModel.removeAll();
    myTeamTransitions.clear();
    ui->teamStartList->clear();
    */
}


void DemoUI::clearScheduleUI(){
    //safely deconnect models from the view
    for(int i =0;i< ui->myGamesTabWidget->count();i++){
        QTableView * scoringTableView = dynamic_cast<QTableView *>(ui->myGamesTabWidget->widget(i));
        if (scoringTableView != NULL)
            scoringTableView->setModel(NULL);
    }
    //remove tabs
    ui->myGamesTabWidget->clear();

    //delete the game projection models data and emtpy the weekly schedule model
    QMutableListIterator<GameProjectionTableModel *> it(myGameProjectionTableModels);
    while (it.hasNext()){
               GameProjectionTableModel * gameProjectionTableModel = it.next();
               gameProjectionTableModel->setAutoDelete(true);
               gameProjectionTableModel->removeAll();
               it.remove();
               delete gameProjectionTableModel;
    }
}

