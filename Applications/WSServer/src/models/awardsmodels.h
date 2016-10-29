#include <QObject>
#include <QAbstractListModel>
#include <QString>
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"


class AwardsProperty
{
public:
        AwardsProperty(
                const QString &fantasyName,
                const QString &playerId,
                const QString &playersName,
                const qint32 &projection,
                const qint32 &award,
                const qint32 &result,
                const int &week,
                const QString &gameId
                );
        QString fantasyName() const;
        QString playerId()const;
        QString playersName()const;
        qint32 projection()const;
        qint32 award()const;
        qint32 result()const;
        int week()const;
        QString gameId()const;
private:
        QString m_fantasyName;
        QString m_playerId;
        QString m_playersName;
        qint32 m_projection;
        qint32 m_award;
        qint32 m_result;
        int m_week;
        QString m_gameId;
};

class AwardsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int week READ week WRITE setWeek NOTIFY weekChanged)
    Q_PROPERTY(QString playerId READ playerId WRITE setPlayerId NOTIFY playerIdChanged)
    Q_PROPERTY(QString gameId READ gameId WRITE setGameId NOTIFY gameIdChanged)
    Q_PROPERTY(QString fname READ fname WRITE setFname NOTIFY fnameChanged)
    Q_PROPERTY(AwardType awardType READ awardType WRITE setType NOTIFY typeChanged)
    Q_ENUMS (AwardType)
public:
    explicit AwardsModel(QObject *parent = 0);

    enum AwardType{
        Game,
        FillWeek,
        Player,
        FName,
        FNameAndWeek,
        All
    };



    AwardType awardType()const;
    void setType(const AwardType awardType);

    int week()const;
    void setWeek(const int &week);

    QString playerId()const;
    void setPlayerId(const QString &playerId);

    QString gameId()const;
    void setGameId(const QString &gameId);

    QString fname()const;
    void setFname(const QString &fname);

    QString getPlayersNameById(const QString id);


    enum AwardsRoles{
        Season  = Qt::UserRole + 1,
        Week,
        FantasyName,
        Projection,
        Award,
        PlayerId,
        PlayersName,
        Result,
        Pos,
        Team,
        GameId,
        Home,
        Away,
        Pnl,
        SettleQty,
        AvgPrice,
    };

    void clearModel();
    void addToModel(const AwardsProperty &awardsProperty) ;
    int rowCount (const QModelIndex &parent = QModelIndex()) const ;
    QVariant data(const QModelIndex &index , int role = Qt::DisplayRole) const;
    Q_INVOKABLE  QVariantMap get(int row);

signals:
    void playerIdChanged();
    void weekChanged();
    void gameIdChanged();
    void running();
    void done();
    void typeChanged();
    void fnameChanged();
protected slots:
    void fillModel();

protected:
  QHash<int, QByteArray> roleNames()const ;
private:
    QList<AwardsProperty> m_awardProperty;
    QString m_playerId;
    int m_week;
    QString m_gameId;
    QString m_fname;
    AwardType m_type ;
};
