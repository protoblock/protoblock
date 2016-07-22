#include <QObject>
#include <QAbstractListModel>
#include <QString>

class ClosedGamesItems
{
public:
        ClosedGamesItems(
                const QString &gameid,
                const qint32 &week,
                const QString &hometeam,
                const QString &awayteam,
                const QString &time,
                const QString &status,
                const qint32 &eTime
                );
        QString gameid()const;
        qint32 week()const;
        QString hometeam()const;
        QString awayteam()const;
        QString time()const;
        QString status()const;
        qint32 eTime()const;
private:
        QString m_gameid;
        qint32  m_week;
        QString m_hometeam;
        QString m_awayteam;
        QString  m_time;
        QString m_status;
        qint32 m_eTime;
};







class ClosedGamesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int week READ week WRITE setWeek NOTIFY weekChanged)
    Q_PROPERTY(WeeklyFilter weeklyFilter READ weeklyFilter WRITE setWeeklyFilter NOTIFY weeklyFilterChanged)
    Q_ENUMS(WeeklyFilter)

public:
    explicit ClosedGamesModel(QObject *parent = 0);
    int week(){return m_week;}
    void setWeek(const int &week);

    enum WeeklyFilter{
        All,
        Single
    };
    WeeklyFilter weeklyFilter()const;
    void setWeeklyFilter(const WeeklyFilter weeklyFilter)
    {
        switch (weeklyFilter) {
        case All:
            m_weeklyFilter = All;
            break;
         case Single:
            m_weeklyFilter = Single;
            break;
        }
        emit weeklyFilterChanged();
    }


    enum ScheduledGamesRoles{
        Gameid,
        Week,
        Hometeam,
        Awayteam,
        Time,
        Status
    };

    Q_INVOKABLE void initModel(const int week);
    void addToModel(const ClosedGamesItems &ClosedGamesItem) ;
    int rowCount (const QModelIndex &parent = QModelIndex()) const ;
    QVariant data(const QModelIndex &index , int role = Qt::DisplayRole) const;
    void sortTime ();
signals:
    void weekChanged();
    void weekUpdate(int);
    void weeklyFilterChanged();


protected slots:
    void handelUpdate(const int week);

protected:
  QHash<int, QByteArray> roleNames()const ;
private:
    QList<ClosedGamesItems> m_closedGamesItem;
    WeeklyFilter m_weeklyFilter;
    int m_week;
};
