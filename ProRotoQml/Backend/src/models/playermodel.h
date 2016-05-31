#include <QObject>
#include <QAbstractListModel>
#include <QString>

class PlayerItems
{
public:
        PlayerItems(
                const QString &firstName,
                const QString &lastName,
                const QString &position,
                const QString &team,
                const QString &status,
                const QString &playerId
                );
        QString firstName() const;
        QString lastName() const;
        QString position() const;
        QString team() const;
        QString playerStatus() const;
        QString playerId() const;
private:
        QString m_firstName;
        QString m_lastName;
        QString m_postion;
        QString m_team;
        QString m_playerStatus;
        QString m_playerId;
};


class PlayerModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit PlayerModel(QObject *parent = 0);

    enum PlayerRoles{
        FirstName,
        LastName,
        Position,
        Team,
        PlayerStatus,
        PlayerId
    };



    Q_INVOKABLE void initModel();
    void addToModel(const PlayerItems &leaderBoardItem) ;
    int rowCount (const QModelIndex &parent = QModelIndex()) const ;
    QVariant data(const QModelIndex &index , int role = Qt::DisplayRole) const;


protected:
  QHash<int, QByteArray> roleNames()const ;
private:
    QList<PlayerItems> m_playerItem;
};
