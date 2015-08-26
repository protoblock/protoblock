#include "currentweekwidget.h"
#include "ui_currentweekwidget.h"
#include "dataservice.h"
#include "propertynames.h"
#include "datacache.h"

CurrentWeekWidget::CurrentWeekWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurrentWeekWidget)
{
    ui->setupUi(this);    
    ui->myGamesTableView->setModel(&myGameTableModel);
    myGameModelFilter.setSourceModel(&myGameTableModel);
    ui->myLeaderBaordTableView->setModel(&DataCache::instance()->leaderBoardModel());
    ui->myProjectionTableView->setModel(&myProjectionsModel);
    //start with upcoming games filter
    ui->myUpcomingGamesRb->setChecked(true);
    myCurrentWeekDataLoaded = false;
    myCurrentWeek = -1;
}

CurrentWeekWidget::~CurrentWeekWidget()
{
    delete ui;
}

void CurrentWeekWidget::on_myCompletedGamesRb_toggled(bool checked)
{
    if (checked)
        if (myGameModelFilter.filter()== GamesFilter::Completed)
            return;
        else
            myGameModelFilter.setGameStatusFilter(GamesFilter::Completed);
}

void CurrentWeekWidget::on_myInGamesRb_toggled(bool checked)
{
    if (checked)
        if (myGameModelFilter.filter()== GamesFilter::InGame)
            return;
        else
            myGameModelFilter.setGameStatusFilter(GamesFilter::InGame);
}

void CurrentWeekWidget::on_myUpcomingGamesRb_toggled(bool checked)
{
    if (checked)
        if (myGameModelFilter.filter()== GamesFilter::Upcoming)
            return;
        else
            myGameModelFilter.setGameStatusFilter(GamesFilter::Upcoming);
}


void CurrentWeekWidget::setCurrentWeekData(fantasybit::GlobalState state,const std::string fantasyPlayerId){

  if (myCurrentWeek == myGlobalState.week() &&
      myFantasyPlayerId == fantasyPlayerId) return;
  myGlobalState = state;
  myFantasyPlayerId = fantasyPlayerId;
  myCurrentWeek = myGlobalState.week();

  std::vector<fantasybit::GameRoster> myGameRosters = DataService::instance()->GetCurrentWeekGameRosters();

  //fill game status
  for(const fantasybit::GameRoster & game  : myGameRosters) {
      // add game
      QString gameId = game.info.id().data();
      myGameTableModel.updateItemProperty<PropertyNames::Game_ID>(gameId,gameId);
      myGameTableModel.updateItemProperty<PropertyNames::Away>(gameId,game.info.away().data());
      myGameTableModel.updateItemProperty<PropertyNames::Home>(gameId,game.info.home().data());
      myGameTableModel.updateItemProperty<PropertyNames::Game_Time>(gameId,QDateTime::fromMSecsSinceEpoch(game.info.time()));
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
          myProjectionsModel.updateItemProperty<PropertyNames::ProjectionStatus>(playerId,(int) ScoreState::NonScored);
      }
  }

  //update to recent projection projection and mark them a sent
  auto  recentProjections = DataService::instance()->GetProjById(myFantasyPlayerId);
  for ( auto it = recentProjections.begin(); it != recentProjections.end(); ++it ){
      myProjectionsModel.updateItemProperty<PropertyNames::Projection>(it->first.data(),it->second);
      myProjectionsModel.updateItemProperty<PropertyNames::ProjectionStatus>(it->first.data(),(int) ScoreState::Sent);
  }  
}



