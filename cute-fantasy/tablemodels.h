#ifndef TABLEMODELS_H
#define TABLEMODELS_H

#include <QDateTime>
#include "viewmodels.h"
#include "propertynames.h"
#include "spinboxdelegate.h"
using namespace PropertyNames;

enum class WeekDisplayType : int {
    CurrentWeek,
    PreviousWeek,
    UpcomingWeek
};
Q_DECLARE_METATYPE(WeekDisplayType)

enum class ScoreState  : int {
    NonScored, //score not set by the user (initial state).
    Scored, //user sets a score but didn't marked for sending
    Sent, //score sent
    Result //score is a result
};
Q_DECLARE_METATYPE(ScoreState)



enum class GamesFilter {
    LockedGames,
    OpenGames,
    All
};

class   GameTableModel :
        public TKeyedListModel<QString,ViewModel> {

public:    

    GameTableModel(WeekDisplayType gameTableType) : TKeyedListModel<QString,ViewModel>(NULL){
        myGameTableType = gameTableType;
        initialize();
    }
    ~GameTableModel() {}    

protected:

    QVariant getColumnDisplayData(quint32 column,ViewModel * data) {
        if (data==NULL) return QVariant();
        if (column !=0) return QVariant();
        QString text = QString("%1 @ %2 - %3");
        QString home,away;
        QDateTime gametime;
        home =data->propertyValue<PropertyNames::Home>().toString();
        away =data->propertyValue<PropertyNames::Away>().toString();
        switch (myGameTableType) {
        case WeekDisplayType::CurrentWeek:
        case WeekDisplayType::UpcomingWeek :
            gametime = data->propertyValue<PropertyNames::Game_Time>().toDateTime();
            break;
        case WeekDisplayType::PreviousWeek:
            gametime = data->propertyValue<PropertyNames::Kickoff_Time>().toDateTime();
            break;
        default:
            return QVariant();
        }

        return text.arg(home).arg(away).arg(gametime.toString("ddd HH:MM"));
    }

    int getColumnCount() {
        return 1;
    }

private:
    void initialize() {
        QStringList  headers;
        switch (myGameTableType) {
        case WeekDisplayType::CurrentWeek:  headers << "Time" << "Home" << "Away"; break;
        case WeekDisplayType::PreviousWeek: headers << naturalName<Kickoff_Time>() << "Home" << "Away"; break;
        case WeekDisplayType::UpcomingWeek: headers << "Time" << "Home" << "Away"; break;
        }
        setHorizontalHeaders(headers);
    }

    WeekDisplayType myGameTableType;
};

class   LeaderBoardTableModel :
        public TKeyedListModel<QString,ViewModel> {

public:

    LeaderBoardTableModel(): TKeyedListModel<QString,ViewModel>(NULL){
        initialize();
    }
    ~LeaderBoardTableModel() {}

protected:

    QVariant getColumnDisplayData(quint32 column,ViewModel * data) {

        if (data==NULL) return QVariant();
        if( column ==0)
            return data->propertyValue<PropertyNames::Fantasy_Name>();
        if( column ==1)
            return data->propertyValue<PropertyNames::Balance>();
        return QVariant();
    }

    int getColumnCount() { return 2; }

private:
    void initialize() {
        QStringList headers;
        headers << "Player" << "Balance";
        setHorizontalHeaders(headers);
    }
};

class   ProjectionSheetTableModel :
        public TKeyedListModel<QString,ViewModel> {

public:
    ProjectionSheetTableModel(WeekDisplayType displayType) : TKeyedListModel<QString,ViewModel>(NULL){
        myDisplayType = displayType;
        initialize();
    }
    ~ProjectionSheetTableModel() {}

protected:

    QVariant getColumnDecorateData(uint column, ViewModel *value){
      if (myDisplayType== WeekDisplayType::CurrentWeek && column== 5){
          int proj = value->propertyValue<PropertyNames::Projection>().toInt();

          ScoreState scoreState = qvariant_cast<ScoreState>(value->propertyValue<PropertyNames::ProjectionStatus>());
          if (scoreState== ScoreState::Scored)
                  return QPixmap(":/icons/scored.png");
          if (scoreState== ScoreState::Sent)
              return QPixmap(":/icons/scoresent.png");
      }
      return QVariant();
    }

    QVariant getColumnDisplayData(quint32 column,ViewModel * data) {

        switch (myDisplayType) {
        case WeekDisplayType::CurrentWeek: {
            if (data==NULL) return QVariant();
            if( column ==0)
                return data->propertyValue<PropertyNames::Player_Name>();
            if( column ==1)
                return data->propertyValue<PropertyNames::Position>();
            if( column ==2)
                return data->propertyValue<PropertyNames::Team_ID>();
            if( column ==3)
                return tr(data->propertyValue<fantasybit::PlayerStatus_Status,PropertyNames::Player_Status>());
            if( column ==4)
                return tr(data->propertyValue<fantasybit::PlayerGameStatus,PropertyNames::Player_Game_Status>());
            if( column ==5)
                return data->propertyValue<PropertyNames::Projection>();
            return QVariant();

        }
        case WeekDisplayType::PreviousWeek: {
            if( column ==0)
                return data->propertyValue<PropertyNames::Player_Name>();
            if( column ==1)
                return data->propertyValue<PropertyNames::Position>();
            if( column ==2)
                return data->propertyValue<PropertyNames::Team_ID>();
            if( column ==3)
                return data->propertyValue<PropertyNames::Result>();
            return QVariant();

        }
        case WeekDisplayType::UpcomingWeek: {
            if (data==NULL) return QVariant();
            if( column ==0)
                return data->propertyValue<PropertyNames::Player_Name>();
            if( column ==1)
                return data->propertyValue<PropertyNames::Position>();
            if( column ==2)
                return data->propertyValue<PropertyNames::Team_ID>();
            if( column ==3)
                return data->propertyValue<PropertyNames::Projection>();
            return QVariant();

        }
        default:
            return QVariant();
        }
    }

    int getColumnCount() {
        switch (myDisplayType) {
        case WeekDisplayType::CurrentWeek:  return 6;
        case WeekDisplayType::PreviousWeek: return 4;
        case WeekDisplayType::UpcomingWeek: return 4;
        default: return 0;
        }
    }

    void setDataFromColumn(ViewModel* data, const QModelIndex & index,const QVariant & vvalue,int /*role*/){
        if (index.column()==5)
            data->attachProperty<PropertyNames::Projection>(vvalue);
    }

private:
    void initialize() {
        QStringList  headers;
        switch (myDisplayType) {
        case WeekDisplayType::CurrentWeek:  headers << "Player Name" << "Pos"
                                                    <<"Team"<<"Game Status"<< "Team Status"
                                                   << "Projection"  ;
            setEditable(5,true);
            break;
        case WeekDisplayType::PreviousWeek: headers << "Player Name" << "Pos"
                                                    <<"Team" << "Result"; break;
        case WeekDisplayType::UpcomingWeek: headers << "Player Name" << "Pos"
                                                    <<"Team" << "Projection";

            break;
        }
        setHorizontalHeaders(headers);
    }

    QString tr(fantasybit::PlayerGameStatus playerGameStatus){
        switch (playerGameStatus) {
        case OUT : return "OUT";
        case IN : return "IN";
        case NA : return "NA";
        default: return "?";
        }
    }

    QString tr(fantasybit::PlayerStatus_Status playerStatus){
        switch(playerStatus){
        case fantasybit::PlayerStatus_Status_ACTIVE : return "ACTIVE";
        case fantasybit::PlayerStatus_Status_INACTIVE : return "INACTIVE";
        case fantasybit::PlayerStatus_Status_OTHER : return "OTHER";
        case fantasybit::PlayerStatus_Status_FA : return "FA";
        default: return "?";
        }
    }


    WeekDisplayType myDisplayType;
};

#include <QSortFilterProxyModel>

class GameViewFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:

    GameViewFilterProxyModel(QObject *parent = 0)
        : QSortFilterProxyModel(parent)
    {
        myGamesFilter = GamesFilter::All;
    }
    void setGameStatusFilter(GamesFilter gamesFilter){
        myGamesFilter = gamesFilter;
        invalidateFilter();
    }

    GamesFilter  filter(){
        return myGamesFilter;
    }


    static bool testGameStatus(GamesFilter filter,GameStatus_Status gameStatus){
        if (filter == GamesFilter::All) return true;
        if (filter == GamesFilter::LockedGames)
            return (gameStatus== GameStatus_Status_CLOSED) ||
                    (gameStatus== GameStatus_Status_POSTGAME) ||
                    (gameStatus== GameStatus_Status_INGAME);

        if (filter == GamesFilter::OpenGames)
            return (gameStatus== GameStatus_Status_SCHEDULED)||
                    (gameStatus== GameStatus_Status_PREGAME);
        return false;
    }



protected:
    //filtering
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
        if (myGamesFilter == GamesFilter::All)
            return true;
        GameTableModel * model = dynamic_cast<GameTableModel *>(sourceModel());
        if (model==NULL) return true;
        QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        ViewModel * item = model->getItemByIndex(index);
        if (item == NULL) return true;
        if (!item->hasProperty<PropertyNames::Game_Status>()) return true;
        GameStatus_Status gameStatus = (GameStatus_Status) item->propertyValue<PropertyNames::Game_Status>().toInt();        
        bool result = testGameStatus(myGamesFilter,gameStatus);
        return result;
    }    

private:
    GamesFilter myGamesFilter;
};

#include <QComboBox>
#include <QItemSelectionModel>
class ProjectionsViewFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    QComboBox * myPositionCombobox;
    GameViewFilterProxyModel  * myGameModelProxy;
    QItemSelectionModel * mySelectedGames;
    bool myIsEnabled = true;

public:

    ProjectionsViewFilterProxyModel(QComboBox * positionCombobox = NULL,
                                    GameViewFilterProxyModel  * gameModelProxy= NULL,
                                    QItemSelectionModel * gameSelectionModel=NULL,
                                    QObject *parent = 0)
        : QSortFilterProxyModel(parent)
    {
        myPositionCombobox = positionCombobox;
        myGameModelProxy = gameModelProxy;
        if (myGameModelProxy != NULL) mySelectedGames = gameSelectionModel;
    }

    bool isEnabled(){
        return myIsEnabled;
    }

    void disable(){
        myIsEnabled = false;
    }

    void enable(){
        myIsEnabled = true;
    }

    void bindFilter(){
        if (myPositionCombobox!=NULL){
            QObject::connect(myPositionCombobox,
                             SIGNAL(currentTextChanged(QString)),
                             this,SLOT(invalidate()));
        }

        if (myGameModelProxy!=NULL){
            QObject::connect(myGameModelProxy,
                             SIGNAL(modelReset()),
                             this,SLOT(invalidate()));
        }

        if (mySelectedGames!=NULL){
            QObject::connect(mySelectedGames,
                             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                             this,SLOT(invalidate()));
        }
    }

protected:
    //filtering
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
        if (!myIsEnabled) return true;
        ProjectionSheetTableModel * model = dynamic_cast<ProjectionSheetTableModel *>(sourceModel());
        if (model==NULL) return true;
        QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        ViewModel * item = model->getItemByIndex(index);
        if (item == NULL) return true;
        QString gameId = item->propertyValue<PropertyNames::Game_ID>().toString();

        if (gameId=="")  return false;


        if (myPositionCombobox!=NULL){
            //get current postion from position combo box
            QString position= myPositionCombobox->currentText().trimmed().toUpper();
            if (position != "ALL"){
                QString playerPosition=item->propertyValue<PropertyNames::Position>().toString();
                if (playerPosition != position)
                    return false;
            }
        }

        if (myGameModelProxy!=NULL){
            GamesFilter gameFilter = myGameModelProxy->filter();
            if (gameFilter != GamesFilter::All) {
            GameTableModel * allGames = dynamic_cast<GameTableModel *>(myGameModelProxy->sourceModel());
            if (allGames==NULL) return false;
            QVariant vvalue;
            GameStatus_Status gameStatus;
            bool propertyFound = allGames->itemPropertyValue<PropertyNames::Game_Status>(gameId,vvalue);
            if (!propertyFound) return false;
            gameStatus = qvariant_cast<GameStatus_Status>(vvalue);
            if (!GameViewFilterProxyModel::testGameStatus(gameFilter,gameStatus))
                return false;
            }
        }

        if (mySelectedGames!=NULL){
              QItemSelection filteredSelection = mySelectedGames->selection();
              QItemSelection sourceSelection = myGameModelProxy->mapSelectionToSource(filteredSelection);

              QModelIndexList selectionList = sourceSelection.indexes();
              if (selectionList.count()> 0){
                  GameTableModel * allGames = dynamic_cast<GameTableModel *>(myGameModelProxy->sourceModel());
                  if (allGames==NULL) return false;                  
              for(int i=0;i<selectionList.count();i++){
                  QModelIndex sourceIndex = selectionList.at(i);
                  ViewModel * game = allGames->getItemByIndex(sourceIndex);
                  if (game == NULL) return false;
                  QString selectedGameId = game->propertyValue<PropertyNames::Game_ID>().toString();
                  if (selectedGameId != gameId)
                      return false;
                  else
                      return true;
              }
        }
         return true;
        }
        return true;
    }
};


#endif // TABLEMODELS_H



