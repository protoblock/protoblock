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
    Completed ,
    InGame,
    Upcoming,
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
        QString home,away,gametime,strISODateTime;
        home =data->propertyValue<PropertyNames::Home>().toString();
        away =data->propertyValue<PropertyNames::Away>().toString();
        switch (myGameTableType) {
        case WeekDisplayType::CurrentWeek:
        case WeekDisplayType::UpcomingWeek :
            strISODateTime = data->propertyValue<PropertyNames::Game_Time>().toString();
            break;
        case WeekDisplayType::PreviousWeek:
            strISODateTime = data->propertyValue<PropertyNames::Kickoff_Time>().toString();
            break;
        default:
            return QVariant();
        }
        QDateTime  dt = QDateTime::fromString(strISODateTime,Qt::ISODate);
        gametime = dt.toString("ddd HH:MM");
        return text.arg(home).arg(away).arg(gametime);
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
                return data->propertyValue<fantasybit::PlayerStatus_Status,PropertyNames::Player_Status>();
            if( column ==4)
                return data->propertyValue<fantasybit::PlayerGameStatus,PropertyNames::Player_Game_Status>();
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
        QString str;

        GameStatus_Status gameStatus = (GameStatus_Status) item->propertyValue<PropertyNames::Game_Status>().toInt();


        qDebug() << item->propertyValue<PropertyNames::Home>().toString()
                 << item->propertyValue<PropertyNames::Away>().toString()
                 << "gameStatus :" <<str;

        if (myGamesFilter == GamesFilter::Completed)
            return gameStatus== GameStatus_Status_CLOSED;

        if (myGamesFilter == GamesFilter::Upcoming)
            return (gameStatus== GameStatus_Status_SCHEDULED)||
                    (gameStatus== GameStatus_Status_PREGAME);

        if (myGamesFilter == GamesFilter::InGame)
            return gameStatus == GameStatus_Status_INGAME;
    }
    //    bool lessThan(const QModelIndex &left, const QModelIndex &right) const {
    //        GameTableModel * model  = dynamic_cast<GameTableModel *>(sourceModel());
    //        if (model==NULL) return true;
    //        ViewModel * leftItem = model->getItemByIndex(left);
    //        if (leftItem == NULL)  return true;
    //        ViewModel * rightItem = model->getItemByIndex(right);
    //        if (rightItem == NULL)  return true;
    //        return leftItem->propertyValue("Game_Time")
    //                < rightItem->propertyValue("Game_Time");
    //    }

private:
    GamesFilter myGamesFilter;

};

#endif // TABLEMODELS_H



