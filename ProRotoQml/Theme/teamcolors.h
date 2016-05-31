#ifndef TEAMCOLORS_H
#define TEAMCOLORS_H

#include <QObject>
#include <QMap>
#include <QMapIterator>


class TeamColors : public QObject
{
    Q_OBJECT
public:
    explicit TeamColors(QObject *parent = 0);
    Q_INVOKABLE QString getPrimaryAt(const QString &team);
    Q_INVOKABLE QString getSecondaryAt(const QString &team);
    void setPrimaryMap();


protected:

signals:

public slots:


private:
    // the primary map
    QMap<QString,QString>  m_map;
    // the secondary map
    QMap<QString,QString> m_secondMap;
//     The Teams long name map
//    QMap m_longTeamMap;
    //testing map
    QMap<QString , QString> m_testMap;
};

#endif // TEAMCOLORS_H
