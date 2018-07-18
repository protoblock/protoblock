#include "awardsmodels.h"
#include "../tempapi.h"


AwardsProperty::AwardsProperty(
        const QString &fantasyName,
        const QString &playerId,
        const QString &playersName,
        const qint32 &projection,
        const qint32 &award,
        const qint32 &result,
        const int &week,
        const QString &gameId
        ):
    m_fantasyName(fantasyName),
    m_playerId(playerId),
    m_playersName(playersName ),
    m_projection(projection),
    m_award(award),
    m_result(result),
    m_week(week),
    m_gameId(gameId)
{}
QString AwardsProperty::fantasyName() const{return m_fantasyName;}
QString AwardsProperty::playerId() const{return m_playerId;}
QString AwardsProperty::playersName() const{return m_playersName;}
qint32 AwardsProperty::projection() const{ return m_projection ;}
qint32 AwardsProperty::award() const{ return m_award ;}
qint32 AwardsProperty::result() const{ return m_result ;}
int AwardsProperty::week() const{ return m_week ;}
QString AwardsProperty::gameId() const{ return m_gameId ;}


//Model Side of things
AwardsModel::AwardsModel(QObject *parent)
    : QAbstractListModel(parent),
      m_gameId("NULL"),
      m_type(Game)
{
    connect (this,SIGNAL(gameIdChanged()),this,SLOT(fillModel()));
    connect (this,SIGNAL(weekChanged()),this,SLOT(fillModel()));
    connect (this,SIGNAL(playerIdChanged()),this,SLOT(fillModel()));
    connect (this,SIGNAL (typeChanged()),this,SLOT(fillModel()));
    connect (this,SIGNAL (fnameChanged()),this,SLOT(fillModel()));
}

AwardsModel::AwardType AwardsModel::awardType() const
{
    return  m_type;
}

void AwardsModel::setType(const AwardsModel::AwardType awardType)
{
    if ( m_type == awardType)
        return;
    m_type = awardType;
    emit typeChanged();
}

void AwardsModel::fillModel()
{
    emit running();
    if(m_awardProperty.count () >= 1 ){
        clearModel ();
    }


    if (m_type == Game){
        for (const auto &it : fantasybit::TempApi::m_awards) {
            if( it.gameId == m_gameId ){
                addToModel ( AwardsProperty ( it.fantasyName, it.playerId, getPlayersNameById (it.playerId), it.projection,
                                it.award, it.result, it.week, it.gameId ) ) ;
            }
        }
    }
    else if (m_type == All)
    {
        for (auto &it : fantasybit::TempApi::m_awards) {
            addToModel ( AwardsProperty (  it.fantasyName, it.playerId, getPlayersNameById (it.playerId), it.projection,
                                           it.award, it.result, it.week, it.gameId ) ) ;
        }
    }
    else if (m_type == FillWeek){
        for (auto &it : fantasybit::TempApi::m_awards) {
            if( it.week == m_week ){
                addToModel ( AwardsProperty ( it.fantasyName, it.playerId, getPlayersNameById (it.playerId), it.projection,
                                it.award, it.result, it.week, it.gameId ) ) ;
            }
        }
    }
   else if (m_type == Player){
        for (auto &it : fantasybit::TempApi::m_awards) {
            if( it.playerId == m_playerId ){
                addToModel ( AwardsProperty ( it.fantasyName, it.playerId, getPlayersNameById (it.playerId), it.projection,
                                it.award, it.result, it.week, it.gameId ) ) ;
            }
        }
    }

    else if (m_type == FName){
         for (auto &it : fantasybit::TempApi::m_awards) {
             if( it.fantasyName == m_fname ){
                 addToModel ( AwardsProperty ( it.fantasyName, it.playerId, getPlayersNameById (it.playerId), it.projection,
                                 it.award, it.result, it.week, it.gameId ) ) ;
             }
         }
     }
    else if (m_type == FNameAndWeek){
         for (auto &it : fantasybit::TempApi::m_awards) {
             if( it.fantasyName == m_fname && it.week == m_week ){
                 addToModel ( AwardsProperty ( it.fantasyName, it.playerId, getPlayersNameById (it.playerId), it.projection,
                                 it.award, it.result, it.week, it.gameId ) ) ;
             }
         }
     }

    emit done();
}

// maybe we should pass this in from the api utils
void AwardsModel::addToModel(const AwardsProperty &awardProperty)
{
    beginInsertRows (QModelIndex() , rowCount (),rowCount () );
    m_awardProperty << awardProperty;
    endInsertRows ();
}

int AwardsModel::week() const
{
    return m_week;
}

void AwardsModel::setWeek(const int &week)
{
    if(m_week == week)
        return;
    m_week = week;
    weekChanged ();
}

QString AwardsModel::playerId() const
{
    return m_playerId;
}

void AwardsModel::setPlayerId(const QString &playerId)
{
    if(m_playerId == playerId)
        return;
    m_playerId = playerId;
    playerIdChanged ();
}

QString AwardsModel::gameId() const
{
    return m_gameId;
}

void AwardsModel::setGameId(const QString &gameId)
{
    if(m_gameId == gameId)
        return;
    m_gameId = gameId;
    emit gameIdChanged();
}

QString AwardsModel::fname() const
{
    return m_fname;
}

void AwardsModel::setFname(const QString &fname)
{
    if ( m_fname == fname)
        return;
    m_fname = fname;
    emit fnameChanged();
}

QString AwardsModel::getPlayersNameById(const QString id)
{
//    QString mName;
//    for (auto &it : fantasybit::TempApi::mPlayers)
//    {
//        if ( it.playerId == id){
//            mName.append (it.firstName).append (" ").append (it.lastName);
//        }
//    }
    auto &pi =  fantasybit::TempApi::pid2Playeritem[id] ;
    return QString("%1 %2").arg(pi.firstName, pi.lastName);
}

void AwardsModel::clearModel()
{
    beginResetModel ();
    m_awardProperty.clear ();
    endResetModel ();
}

int AwardsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_awardProperty.count ();
}

QVariant AwardsModel::data(const QModelIndex &index, int role) const
{
    if (index.row () < 0 || index.row () >= m_awardProperty.count ())
        return QVariant();
    const AwardsProperty &awardProperty = m_awardProperty[index.row()];
    if (role == FantasyName)
        return awardProperty.fantasyName ();
    else if (role == PlayerId)
        return awardProperty.playerId ();
    else if (role == PlayersName)
        return awardProperty.playersName ();
    else if (role == Projection)
        return awardProperty.projection ();
    else if (role == Award)
        return awardProperty.award ();
    else if (role == Week)
        return awardProperty.week ();
    else if (role == Season)
        return "2015";
    else if (role == Team)
        return fantasybit::TempApi::pid2Playeritem[awardProperty.playerId()].team;
    else if (role == Pos)
        return fantasybit::TempApi::pid2Playeritem[awardProperty.playerId()].position;
    else if (role == Home)
        return fantasybit::TempApi::gid2Game[awardProperty.gameId()].hometeam;
    else if (role == Away)
        return fantasybit::TempApi::gid2Game[awardProperty.gameId()].awayteam;
    else if (role == Pnl)
        return int(0);
    else if (role == SettleQty)
        return 0;
    else if (role == AvgPrice)
        return 0;
    else if (role == Result)
        return awardProperty.result();

    return QVariant();
}

QHash<int, QByteArray> AwardsModel::roleNames() const
{
    QHash<int,QByteArray> roles;
    roles[FantasyName] = "fantasyName";
    roles[PlayerId] = "playerId";
    roles[PlayersName] = "playersName";
    roles[Projection] = "projection";
    roles[Award] = "award";
    roles[Week] = "week";
    roles[Season] = "season";
    roles[Result] = "result";
    roles[Pos] = "pos";
    roles[Team] = "team";
    roles[GameId] = "gameId";
    roles[Home] = "home";
    roles[Away] = "away";
    roles[Pnl] = "pnl";
    roles[SettleQty] = "settleQty";
    roles[AvgPrice] = "avgPrice";

    return roles;
}


QVariantMap AwardsModel::get(int row) {
    QHash<int,QByteArray> names = roleNames();
    QHashIterator<int, QByteArray> i(names);
    QVariantMap res;
    while (i.hasNext()) {
        i.next();
        QModelIndex idx = index(row, 0);
        QVariant data = idx.data(i.key());
        res[i.value()] = data;
        //qDebug() << i.key() << ": " << i.value() << endl;
    }
    return res;
}
