#include <QObject>
#include <QAbstractListModel>
#include <QString>

class  StatsItem
{
public:
         StatsItem(
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
                );
         qint32 week () const;
         QString gameid () const;
         QString homeName () const;
         bool isHome () const;
         QString awayName () const;
         bool isAway () const;
         QString playerId () const;
         QString playersName()const;
         qint32 passtd () const;
         qint32 passyds  () const;
         qint32 rushtd   () const;
         qint32 rushyds   () const;
         qint32 rectd   () const;
         qint32 recyds   () const;
         qint32 rec   () const;
         qint32 pint   () const;
         qint32 fumble  () const;
         qint32 twopt  () const;
         qint32 fg () const;
         qint32 pa () const;
         qint32 ptsa () const;
         qint32 deftd () const;
         qint32 sacks () const;
         qint32 turnovers () const;
         qint32 sfty () const;
         qint32 dtwopt () const;

private:
        qint32 m_week;
        QString m_gameid;
        QString m_homeName;
        bool m_isHome;
        QString m_awayName;
        bool m_isAway;
        QString m_playerId;
        QString m_playersName;
        qint32 m_passtd;
        qint32 m_passyds ;
        qint32 m_rushtd  ;
        qint32 m_rushyds  ;
        qint32 m_rectd  ;
        qint32 m_recyds  ;
        qint32 m_rec  ;
        qint32 m_pint  ;
        qint32 m_fumble ;
        qint32 m_twopt ;
        qint32 m_fg;
        qint32 m_pa;
        qint32 m_ptsa;
        qint32 m_deftd;
        qint32 m_sacks;
        qint32 m_turnovers;
        qint32 m_sfty;
        qint32 m_dtwopt;
};


class StatsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(qint32 week READ week WRITE setWeek NOTIFY weekChanged)
    Q_PROPERTY(QString playerId READ playerId WRITE setPlayerId NOTIFY playerIdChanged)
    Q_PROPERTY(QString gameId READ gameId WRITE setGameId NOTIFY gameIdChanged)

    Q_PROPERTY(ViewType viewType READ viewType WRITE setViewType NOTIFY viewTypeChanged)
    Q_PROPERTY(int currentWeek READ currentWeek)
    Q_ENUMS(ViewType)
public:
    explicit StatsModel(QObject *parent = 0);

    qint32 week()const;
    void setWeek(const qint32 &week);
    QString playerId()const;
    void setPlayerId(const QString &playerId);
    QString gameId()const;
    void setGameId(const QString &gameId);


    QString getPlayersNameById(const QString id);


    enum ViewType{
        Single,
        SortWeek,
        Two,
        Four,
        All
    };
    ViewType viewType()const;
    void setViewType(const ViewType &viewType );
    int currentWeek()const;

    enum StatsRoles{
        Week, Gameid, HomeName, IsHome, AwayName, IsAway, PlayerId,PlayersName, Passtd, Passyds , Rushtd  ,
        Rushyds  , Rectd  , Recyds  , Rec  , Pint  , Fumble , Twopt , Fg, Pa, Ptsa, Deftd, Sacks, Turnovers, Sfty, DTwopt
    };


    void clearModel();
    void addToModel(const  StatsItem &statsItem) ;
    int rowCount (const QModelIndex &parent = QModelIndex()) const ;
    QVariant data(const QModelIndex &index , int role = Qt::DisplayRole) const;
    Q_INVOKABLE int count();

signals:
    void updateModel();
    void weekChanged();
    void viewTypeChanged ();
    void playerIdChanged();
    void running();
    void done();
    void gameIdChanged();
protected:
  QHash<int, QByteArray> roleNames()const ;

protected slots:
     void fillModel();

private:
     QString m_gameId;
    QList< StatsItem> m_statsItem;
    QString m_playerId;
    qint32 m_week;
    ViewType m_viewType;

};
