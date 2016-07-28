#include "closedgamesmodel.h"
#include "../tempapi.h"


// Set the propertys for the roles of the model
ClosedGamesItems::ClosedGamesItems(
        const QString &gameid,
        const qint32 &week,
        const QString &hometeam,
        const QString &awayteam,
        const QString &time,
        const QString &status,
        const qint32 &eTime
        ):
    m_gameid(gameid),
    m_week(week),
    m_hometeam(hometeam),
    m_awayteam(awayteam),
    m_time(time),
    m_status(status),
    m_eTime(eTime)
{
}
QString ClosedGamesItems::gameid() const{return m_gameid;}
qint32 ClosedGamesItems::week() const{return m_week;}
QString ClosedGamesItems::hometeam() const{return m_hometeam;}
QString ClosedGamesItems::awayteam() const{return m_awayteam;}
QString ClosedGamesItems::time() const{return m_time;}
QString ClosedGamesItems::status() const {return m_status;}
qint32 ClosedGamesItems::eTime() const{return m_eTime;}


ClosedGamesModel::ClosedGamesModel(QObject *parent)
    : QAbstractListModel(parent),
      m_week(fantasybit::TempApi::mCurrentWeek.week),
      m_weeklyFilter (Single)
{
    // Init here
    initModel (fantasybit::TempApi::mCurrentWeek.week);
//    qDebug() << fantasybit::TempApi::mCurrentWeek.week;


    this->sortTime ();

}

void ClosedGamesModel::setWeek(const int &week){
    if(m_week == week)
        return;
    m_week = week;
    emit weekChanged ();
}

ClosedGamesModel::WeeklyFilter ClosedGamesModel::weeklyFilter() const
{
    return m_weeklyFilter ;
}

void ClosedGamesModel::initModel(const int week)
{
    //FIXME



////    set week ?
//      fantasybit::TempApi::mPrevWeeks week;



//    addToModel (ClosedGamesItems){
//        for(auto &it : fantasybit::TempApi::mPrevWeeks)


//    }






}

// maybe we should pass this in from the api utils
void ClosedGamesModel::addToModel(const ClosedGamesItems &ClosedGamesItems)
{
    beginInsertRows (QModelIndex() , rowCount (),rowCount () );
    m_closedGamesItem << ClosedGamesItems;
    endInsertRows ();
}

int ClosedGamesModel::rowCount(const QModelIndex &parent) const
{

    Q_UNUSED(parent)
    return m_closedGamesItem.count ();

}

QVariant ClosedGamesModel::data(const QModelIndex &index, int role) const
{
    if (index.row () < 0 || index.row () >= m_closedGamesItem.count ())
        return QVariant();
    const ClosedGamesItems &closedGamesItems = m_closedGamesItem[index.row()];
    if (role == Gameid)
        return closedGamesItems.gameid ();
    else if (role ==Week)
        return closedGamesItems.week ();
    else if (role == Hometeam)
        return closedGamesItems.hometeam ();
    else if (role == Awayteam)
        return closedGamesItems.awayteam ();
    else if (role == Time)
        return closedGamesItems.time ();
    else if (role == Status)
        return closedGamesItems.status ();
    return QVariant();
}

void ClosedGamesModel::sortTime()
{
    this->sort (6, Qt::AscendingOrder);
}

void ClosedGamesModel::handelUpdate(const int week)
{
    initModel (week);
}

QHash<int, QByteArray> ClosedGamesModel::roleNames() const
{

    QHash<int,QByteArray> roles;
    roles[Gameid] = "gameId";
    roles[Week] = "week";
    roles[Hometeam] = "homeTeam";
    roles[Awayteam] = "awayTeam";
    roles[Time] = "time";
    roles[Status] = "gameStatus";

 return roles;
}
