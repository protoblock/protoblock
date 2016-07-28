#include "playermodel.h"
#include "../tempapi.h"


// Set the propertys for the roles of the model
PlayerItems::PlayerItems(
        const QString &firstName,
        const QString &lastName,
        const QString &position,
        const QString &team,
        const QString &playerStatus,
        const QString &playerId
        ):
    m_firstName(firstName),
    m_lastName(lastName),
    m_postion(position),
    m_team(team),
    m_playerStatus(playerStatus),
    m_playerId(playerId)
{
}
QString PlayerItems::firstName() const{return m_firstName;}
QString PlayerItems::lastName() const{return m_lastName;}
QString PlayerItems::position() const{return m_postion;}
QString PlayerItems::team() const{return m_team;}
QString PlayerItems::playerStatus() const {return m_playerStatus;}
QString PlayerItems::playerId() const {return m_playerId;}


PlayerModel::PlayerModel(QObject *parent)
    : QAbstractListModel(parent)

{
    // Init here
    initModel ();
}

void PlayerModel::initModel()
{


    for (auto &it :fantasybit::TempApi::mPlayers) {
        addToModel (PlayerItems(
                      it.firstName,
                      it.lastName,
                      it.position,
                      it.team,
                      it.playerStatus,
                      it.playerId
                      ) ) ;
    }
}

// maybe we should pass this in from the api utils
void PlayerModel::addToModel(const PlayerItems &playerItem)
{
    beginInsertRows (QModelIndex() , rowCount (),rowCount () );
    m_playerItem << playerItem;
    endInsertRows ();
}

int PlayerModel::rowCount(const QModelIndex &parent) const
{

    Q_UNUSED(parent)
    return m_playerItem.count ();

}

QVariant PlayerModel::data(const QModelIndex &index, int role) const
{
    if (index.row () < 0 || index.row () >= m_playerItem.count ())
        return QVariant();
    const PlayerItems &playerItem = m_playerItem[index.row()];
    if (role == FirstName)
        return playerItem.firstName ();
    else if (role == LastName)
        return playerItem.lastName ();
    else if (role == Position)
        return playerItem.position ();
    else if (role == Team)
        return playerItem.team ();
    else if (role == PlayerStatus)
        return playerItem.playerStatus ();
    else if ( role == PlayerId )
        return playerItem.playerId ();
    return QVariant();
}

QHash<int, QByteArray> PlayerModel::roleNames() const
{

    QHash<int,QByteArray> roles;
    roles[FirstName] = "firstName";
    roles[LastName] = "lastName";
    roles[Position] = "position";
    roles[Team] = "team";
    roles[PlayerStatus] = "playerStatus";
    roles[PlayerId] =  "playerId";
 return roles;

}


