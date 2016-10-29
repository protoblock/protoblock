#ifndef TEAMMAP_H
#define TEAMMAP_H

#include <QObject>
#include <QMap>
class TeamMap : public QObject
{
    Q_OBJECT
public:
    explicit TeamMap(QObject *parent = 0);
    void setupMap();

    Q_INVOKABLE QString getTeamLongName(const QString shortName);

public slots:

private:
        QMap<QString , QString> m_longNameMap;

};

#endif // TEAMMAP_H
