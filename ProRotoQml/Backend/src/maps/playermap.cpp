#include "playermap.h"
#include "../tempapi.h"

PlayerMap::PlayerMap(QObject *parent) : QObject(parent)
{
    createPlayerNameMap ();
}

void PlayerMap::createPlayerNameMap()
{
    QMap<QString,QString> mapNames;
    QMap<QString,QString >mapTeam;
    QMap<QString, QString>mapPostion;
    QMap<QString, QString>mapPlayerStatus;

    for (auto &it :fantasybit::TempApi::mPlayers) {
            QString fullName = QString("%1 %2").arg (it.firstName).arg (it.lastName);
           mapNames.insert (it.playerId, fullName);
           mapTeam.insert (it.playerId,it.team);
           mapPostion.insert (it.playerId,it.position);
           mapPlayerStatus.insert (it.playerId,it.playerStatus);
    }
    m_playerNameMap = mapNames;
    m_playerTeamMap = mapTeam;
    m_playerPostionMap = mapPostion;
    m_playerStatusMap = mapPlayerStatus;
    clearMaps(mapNames,mapTeam,mapPostion,mapPlayerStatus);
    emit mapsFilled();
}

void PlayerMap::clearMaps(
        QMap<QString, QString> mName,
        QMap<QString, QString> mTeam,
        QMap<QString, QString> mPostion,
        QMap<QString,QString> mStatus
        )
{
    mName.clear ();
    mTeam.clear ();
    mPostion.clear ();
    mStatus.clear ();
}





QString PlayerMap::getPlayerNameById(const QString id)
{
    return m_playerNameMap.value(id);
}

QString PlayerMap::getPlayerTeamById(const QString id)
{
    return m_playerTeamMap.value (id);
}

QString PlayerMap::getPlayerPostionById(const QString id)
{
    return m_playerPostionMap.value (id);
}

QString PlayerMap::getPlayerStatusById(const QString id)
{
    return m_playerStatusMap.value (id);
}
