#include "statsmodel.h"
#include "../tempapi.h"


// Set the propertys for the roles of the model
StatsItem::StatsItem(
        const qint32 &week,
        const QString &gameid,
        const QString &homeName,
        const bool &isHome,
        const QString &awayName,
        const bool &isAway,
        const QString &playerId,
        const QString &playersName,
        const qint32 &passtd,
        const qint32 &passyds,
        const qint32 &rushtd,
        const qint32 &rushyds,
        const qint32 &rectd,
        const qint32 &recyds,
        const qint32 &rec,
        const qint32 &pint,
        const qint32 &fumble,
        const qint32 &twopt,
        const qint32 &fg,
        const qint32 &pa,
        const qint32 &ptsa,
        const qint32 &deftd,
        const qint32 &sacks,
        const qint32 &turnovers,
        const qint32 &sfty,
        const qint32 &dtwopt
        ):
    m_week(week),
    m_gameid(gameid),
    m_homeName(homeName),
    m_isHome(isHome),
    m_awayName(awayName),
    m_isAway(isAway),
    m_playerId(playerId),
    m_playersName(playersName),
    m_passtd(passtd),
    m_passyds (passyds),
    m_rushtd  (rushtd),
    m_rushyds  (rushyds),
    m_rectd  (rectd),
    m_recyds  (recyds),
    m_rec  (rec),
    m_pint  (pint),
    m_fumble (fumble),
    m_twopt (twopt),
    m_fg(fg),
    m_pa(pa),
    m_ptsa(ptsa),
    m_deftd(deftd),
    m_sacks(sacks),
    m_turnovers(turnovers),
    m_sfty(sfty),
    m_dtwopt(dtwopt)
{
}
qint32 StatsItem::week() const { return m_week ;}
QString StatsItem::gameid() const { return m_gameid ;}
QString StatsItem::homeName() const { return m_homeName ;}
bool StatsItem::isHome() const { return m_isHome ;}
QString StatsItem::awayName() const { return m_awayName ;}
bool StatsItem::isAway() const { return m_isAway ;}
QString StatsItem::playerId() const { return m_playerId ;}

QString StatsItem::playersName() const{return m_playersName;}
qint32 StatsItem::passtd() const { return m_passtd ;}
qint32 StatsItem::passyds() const { return m_passyds ;}
qint32 StatsItem::rushtd() const { return m_rushtd ;}
qint32 StatsItem::rushyds() const { return m_rushyds ;}
qint32 StatsItem::rectd() const { return m_rectd ;}
qint32 StatsItem::recyds() const { return m_recyds ;}
qint32 StatsItem::rec() const { return m_rec ;}
qint32 StatsItem::pint() const { return m_pint ;}
qint32 StatsItem::fumble() const { return m_fumble ;}
qint32 StatsItem::twopt() const { return m_twopt ;}
qint32 StatsItem::fg() const { return m_fg ;}
qint32 StatsItem::pa() const { return m_pa ;}
qint32 StatsItem::ptsa() const { return m_ptsa ;}
qint32 StatsItem::deftd() const { return m_deftd ;}
qint32 StatsItem::sacks() const { return m_sacks ;}
qint32 StatsItem::turnovers() const { return m_turnovers ;}
qint32 StatsItem::sfty() const { return m_sfty ;}
qint32 StatsItem::dtwopt() const { return m_dtwopt ;}
//TODO returns

StatsModel::StatsModel(QObject *parent)
    : QAbstractListModel(parent),
      m_viewType(Single)

{
    connect (this,SIGNAL(weekChanged()),this,SLOT(fillModel()));
    connect (this,SIGNAL(viewTypeChanged()),this,SLOT(fillModel()));
    connect (this,SIGNAL(playerIdChanged()),this,SLOT(fillModel()));
    connect (this,SIGNAL(gameIdChanged()),this,SLOT(fillModel()));

}

qint32 StatsModel::week() const
{
    return m_week;
}

void StatsModel::setWeek(const qint32 &week)
{
    if(m_week == week)
        return;
    m_week = week;
    weekChanged();
}

QString StatsModel::playerId() const
{
    return m_playerId;
}

void StatsModel::setPlayerId(const QString &playerId)
{
    if(m_playerId == playerId)
        return;
    m_playerId  == playerId;
    playerIdChanged ();
}

QString StatsModel::gameId() const
{
    return m_gameId;
}

void StatsModel::setGameId(const QString &gameId)
{
    if(m_gameId == gameId)
        return;
    m_gameId = gameId;
    gameIdChanged ();
}

QString StatsModel::getPlayersNameById(const QString id)
{
    QString mName;
    for (auto &it : fantasybit::TempApi::mPlayers)
    {
        if ( it.playerId == id){
            mName.append (it.firstName).append (" ").append (it.lastName);
        }
    }
    return mName;
}

StatsModel::ViewType StatsModel::viewType() const
{
    return m_viewType;
}

void StatsModel::setViewType(const StatsModel::ViewType &viewType)
{
    if(m_viewType == viewType)
        return;
    m_viewType = viewType;
    viewTypeChanged();
}

int StatsModel::currentWeek() const
{
    return fantasybit::TempApi::mCurrentWeek.week;
}

void StatsModel::clearModel()
{
    beginResetModel ();
    m_statsItem.clear ();
    endResetModel ();
}

void  StatsModel::fillModel()
{

    if (m_statsItem.length () >= 1 ){
        clearModel ();
    }
    emit running();

    // single player
    if (m_viewType == Single){
        for (auto &it :fantasybit::TempApi::m_statsItem) {
            if (it.gameid == m_gameId ){
                addToModel ( StatsItem(
                                 it.week,
                                 it.gameid,
                                 it.homeName ,
                                 it.isHome,
                                 it.awayName,
                                 it.isAway,
                                 it.playerId,
                                 getPlayersNameById (it.playerId),
                                 it.passtd,
                                 it.passyds ,
                                 it.rushtd  ,
                                 it.rushyds  ,
                                 it.rectd  ,
                                 it.recyds  ,
                                 it.rec  ,
                                 it.pint  ,
                                 it.fumble ,
                                 it.twopt ,
                                 it.fg,
                                 it.pa,
                                 it.ptsa,
                                 it.deftd,
                                 it.sacks,
                                 it.turnovers,
                                 it.sfty,
                                 it.dtwopt
                                 ) ) ;
            }
        }
    }
    // per game


    // full week

    else if(m_viewType == SortWeek ){
        // full model
        for (auto &it :fantasybit::TempApi::m_statsItem) {
            if (it.week == m_week){
                addToModel ( StatsItem(
                                 it.week, it.gameid, it.homeName , it.isHome, it.awayName, it.isAway, it.playerId, getPlayersNameById (it.playerId),
                                 it.passtd, it.passyds , it.rushtd  , it.rushyds  , it.rectd  , it.recyds  , it.rec  , it.pint  , it.fumble , it.twopt , it.fg,
                                 it.pa, it.ptsa, it.deftd, it.sacks, it.turnovers, it.sfty, it.dtwopt ) ) ;

            }
        }
    }




    // last two weeks


    // last four weeks

    else if(m_viewType == All ){
        // full model
        for (auto &it :fantasybit::TempApi::m_statsItem) {
            addToModel ( StatsItem(
                             it.week, it.gameid, it.homeName , it.isHome, it.awayName, it.isAway, it.playerId, getPlayersNameById (it.playerId),
                             it.passtd, it.passyds , it.rushtd  , it.rushyds  , it.rectd  , it.recyds  , it.rec  , it.pint  , it.fumble , it.twopt , it.fg,
                             it.pa, it.ptsa, it.deftd, it.sacks, it.turnovers, it.sfty, it.dtwopt ) ) ;

        }
    }
    emit done();
}

// maybe we should pass this in from the api utils
void  StatsModel::addToModel(const  StatsItem & statsItem)
{
    beginInsertRows (QModelIndex() , rowCount (),rowCount () );
    m_statsItem <<  statsItem;
    endInsertRows ();
}

int  StatsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_statsItem.count ();
}

QVariant  StatsModel::data(const QModelIndex &index, int role) const
{
    if (index.row () < 0 || index.row () >= m_statsItem.count ())
        return QVariant();
    const StatsItem & statsItem = m_statsItem[index.row()];
    if (role == Week)
        return  statsItem.week ();
    else if ( role == Gameid)
        return statsItem.gameid ();
    else if ( role == HomeName)
        return statsItem.homeName ();
    else if ( role == IsHome)
        return statsItem.isHome ();
    else if ( role == AwayName )
        return statsItem.awayName ();
    else if ( role == IsAway)
        return statsItem.isAway ();
    else if ( role == PlayerId)
        return statsItem.playerId ();
    else if (role == PlayersName)
        return statsItem.playersName ();
    else if ( role == Passtd)
        return statsItem.passtd ();
    else if ( role == Passyds)
        return statsItem.passyds ();
    else if ( role == Rushtd)
        return statsItem.rushtd ();
    else if ( role == Rushyds )
        return statsItem.rushyds ();
    else if ( role == Rectd )
        return statsItem.rectd ();
    else if ( role == Recyds )
        return statsItem.recyds ();
    else if ( role == Rec)
        return statsItem.rec ();
    else if ( role == Pint)
        return statsItem.pint ();
    else if ( role == Fumble)
        return statsItem.fumble ();
    else if ( role == Twopt)
        return statsItem.twopt ();
    else if ( role == Fg)
        return statsItem.fg ();
    else if ( role == Pa)
        return statsItem.pa ();
    else if ( role == Ptsa )
        return statsItem.ptsa ();
    else if ( role == Deftd)
        return statsItem.deftd ();
    else if ( role == Sacks)
        return statsItem.sacks ();
    else if ( role == Turnovers)
        return statsItem.turnovers ();
    else if ( role == Sfty)
        return statsItem.sfty ();
    else if ( role == DTwopt)
        return statsItem.dtwopt ();
    return QVariant();
}

int StatsModel::count(){
    return m_statsItem.length ();
}

QHash<int, QByteArray>  StatsModel::roleNames() const
{
    QHash<int,QByteArray> roles;
    roles[Week] = "week";
    roles[Gameid] = "gameid";
    roles[HomeName] = "homeName";
    roles[IsHome] = "isHome";
    roles[AwayName] = "awayName";
    roles[IsAway] = "isAway";
    roles[PlayerId] = "playerId";
    roles[PlayersName] = "playerName";
    roles[Passtd] = "passtd";
    roles[Passyds] = "passyds";
    roles[Rushtd] = "rushtd";
    roles[Rushyds] = "rushyds";
    roles[Rectd] = "rectd";
    roles[Recyds] = "recyds";
    roles[Rec] = "rec";
    roles[Pint] = "pint";
    roles[Fumble] = "fumble";
    roles[Twopt] = "twopt ";
    roles[Fg] = "fg";
    roles[Pa] = "pa";
    roles[Ptsa] = "ptsa";
    roles[Deftd] = "deftd";
    roles[Sacks] = "sacks";
    roles[Turnovers] = "turnovers";
    roles[Sfty] = "sfty";
    roles[DTwopt] = "dTwopt";
    return roles;
}
