#include <QObject>
#include <QAbstractListModel>
#include <QString>


class LeaderBoardItems
{
public:
        LeaderBoardItems(
                const QString &fantasyName,
                const QString &publicKey,
            //    qint64 nameHash,
                const quint64 &stakeBalance,
                const qint64 &skillBalance
                );

        QString fantasyName() const;
        QString publicKey() const ;
    //    qint64 nameHash() const ;
        quint64 stakeBalance() const;
        qint64 skillBalance() const ;

private:
        QString m_fantasyName;
        QString m_publicKey;
    //    qint64 nameHash;
        quint64 m_stakeBalance;
        qint64 m_skillBalance;
};

class LeaderBoardModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit LeaderBoardModel(QObject *parent = 0);

    enum LeaderBoardRoles{
        FantasyName,
        PublicKey,
    //  Hash,
        StakeBalance,
        SkillBalance
    };

    void initModel();
    void addToModel(const LeaderBoardItems &leaderBoardItem) ;
    int rowCount (const QModelIndex &parent = QModelIndex()) const ;
    QVariant data(const QModelIndex &index , int role = Qt::DisplayRole) const;


protected:
  QHash<int, QByteArray> roleNames()const ;
private:
    QList<LeaderBoardItems> m_leaderboardItem;
};
