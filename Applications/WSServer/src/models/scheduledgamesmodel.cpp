#include "scheduledgamesmodel.h"
#include "../tempapi.h"


// Set the propertys for the roles of the model
ScheduledGamesItems::ScheduledGamesItems(
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
QString ScheduledGamesItems::gameid() const{return m_gameid;}
qint32 ScheduledGamesItems::week() const{return m_week;}
QString ScheduledGamesItems::hometeam() const{return m_hometeam;}
QString ScheduledGamesItems::awayteam() const{return m_awayteam;}
QString ScheduledGamesItems::time() const{return m_time;}
QString ScheduledGamesItems::status() const {return m_status;}
qint32 ScheduledGamesItems::eTime() const{return m_eTime;}


ScheduledGamesModel::ScheduledGamesModel(QObject *parent)
    : QAbstractListModel(parent),
//      m_week(fantasybit::TempApi::mCurrentWeek.week),
      m_weeklyFilter (Single),
      m_week(20)
{
    connect (this,SIGNAL(weekChanged ()),this,SLOT(initModel()));
    this->sortTime ();
}

void ScheduledGamesModel::setWeek(const int &week)
{
    if(m_week == week)
        return;
    m_week = week;
    weekChanged ();
}

int ScheduledGamesModel::currentWeek()
{
    return fantasybit::TempApi::mCurrentWeek.week;
}

ScheduledGamesModel::WeeklyFilter ScheduledGamesModel::weeklyFilter() const
{
    return m_weeklyFilter ;
}

void ScheduledGamesModel::clearModel()
{
    beginResetModel ();
    m_scheduledGamesItem.clear ();
    endResetModel ();
}

void ScheduledGamesModel::initModel()
{

    // if the count is larger then one we reset the model
    if(m_scheduledGamesItem.count () > 1 ){
        clearModel ();
    }

    //FIXME
    if (m_weeklyFilter == Single)
    {
        for (auto &it :fantasybit::TempApi::mGameStatusItem)
        {
            if (it.week == m_week)
            {
                addToModel (ScheduledGamesItems(
                                it.gameid,
                                it.week,
                                it.hometeam,
                                it.awayteam,
                                it.time,
                                it.status,
                                it.eTime
                                ) ) ;
            }
        }
    }
    else
    {
        for (auto &it :fantasybit::TempApi::mGameStatusItem)
        {
            addToModel (ScheduledGamesItems(
                           it.gameid,
                            it.week,
                            it.hometeam,
                            it.awayteam,
                            it.time,
                            it.status,
                            it.eTime
                          ) ) ;
        }
    }
}

// maybe we should pass this in from the api utils
void ScheduledGamesModel::addToModel(const ScheduledGamesItems &scheduledGamesItems)
{
    beginInsertRows (QModelIndex() , rowCount (),rowCount () );
    m_scheduledGamesItem << scheduledGamesItems;
    endInsertRows ();
}

int ScheduledGamesModel::rowCount(const QModelIndex &parent) const
{

    Q_UNUSED(parent)
    return m_scheduledGamesItem.count ();

}

QVariant ScheduledGamesModel::data(const QModelIndex &index, int role) const
{
    if (index.row () < 0 || index.row () >= m_scheduledGamesItem.count ())
        return QVariant();
    const ScheduledGamesItems &scheduledGamesItems = m_scheduledGamesItem[index.row()];
    if (role == Gameid)
        return scheduledGamesItems.gameid ();
    else if (role ==Week)
        return scheduledGamesItems.week ();
    else if (role == Hometeam)
        return scheduledGamesItems.hometeam ();
    else if (role == Awayteam)
        return scheduledGamesItems.awayteam ();
    else if (role == Time)
        return scheduledGamesItems.time ();
    else if (role == Status)
        return scheduledGamesItems.status ();
    return QVariant();
}

void ScheduledGamesModel::sortTime()
{
    this->sort (0, Qt::AscendingOrder);
}

void ScheduledGamesModel::handelUpdate(const int week)
{
    initModel ();
}

QHash<int, QByteArray> ScheduledGamesModel::roleNames() const
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
