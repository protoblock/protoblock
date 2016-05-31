#ifndef PLAYERMAP_H
#define PLAYERMAP_H

#include <QObject>
#include <QMap>
class PlayerMap : public QObject
{
    Q_OBJECT
public:
    explicit PlayerMap(QObject *parent = 0);
    void createPlayerNameMap();

    void clearMaps(
            QMap<QString, QString> mName,
            QMap<QString, QString> mTeam,
            QMap<QString, QString> mPostion,
            QMap<QString,QString> mStatus
            );

    Q_INVOKABLE QString getPlayerNameById(const QString id);
    Q_INVOKABLE QString getPlayerTeamById(const QString id);
    Q_INVOKABLE QString getPlayerPostionById(const QString id);
    Q_INVOKABLE QString getPlayerStatusById(const QString id);



signals:
    void mapsFilled();


private:
    QMap<QString,QString>  m_playerNameMap;
    QMap<QString,QString>  m_playerPostionMap;
    QMap<QString,QString>  m_playerTeamMap;
    QMap<QString,QString>  m_playerStatusMap;
};

#endif // PLAYERMAP_H
