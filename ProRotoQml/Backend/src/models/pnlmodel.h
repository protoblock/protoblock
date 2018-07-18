#include <QObject>
#include <QAbstractListModel>
#include <QString>


class PnlPropertys
{
public:
        PnlPropertys(
                const QString &fantasyName,
                const QString &playerId,
                const qint32 &qty,
                const qint32 &price,
                const qint32 &pnl,
                const int &week,
                const QString &gameId
              );
        QString fantasyName()const{return m_fantasyName;}
        QString playerId()const{return m_playerId;}
        qint32 qty()const{return m_qty;}
        qint32 price()const{ return m_price;}
        qint32 pnl()const{ return m_pnl;}
        int week()const{return m_week;}
        QString gameId()const{return m_gameId ;}
private:
        QString m_fantasyName;
        QString m_playerId;
        qint32 m_qty;
        qint32 m_price;
        qint32 m_pnl;
        int m_week;
        QString m_gameId;
};

class PNLModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString gameId READ gameId WRITE setGameId NOTIFY gameIdChanged)
    Q_PROPERTY(int week READ week WRITE setWeek NOTIFY weekChanged)
//    Q_PROPERTY(QString playerId READ playerId WRITE setPlayerId NOTIFY playerIdChanged)

public:
    explicit PNLModel(QObject *parent = 0);

    QString gameId()const;
    void setGameId(const QString &gameId);

    int week()const;
    void setWeek(const int &week);

    enum PnlRoles{
        FantasyName,
        PlayerId,
        Qty,
        Price,
        Pnl,
        Week,
        GameId
    };

    void clearModel();
    void addToModel(const PnlPropertys &pnlPropertys) ;
    int rowCount (const QModelIndex &parent = QModelIndex()) const ;
    QVariant data(const QModelIndex &index , int role = Qt::DisplayRole) const;


signals:
    void gameIdChanged();
    void weekChanged();
    void running();
    void done();

protected:
  QHash<int, QByteArray> roleNames()const ;
protected  slots:
  void initModel();



private:
    QString m_gameId;
    int m_week;


    QList<PnlPropertys> m_pnlPropertys;
};
