#ifndef WEEKLYSCHEDULEMODEL_H
#define WEEKLYSCHEDULEMODEL_H

#include <QObject>
#include <QString>
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
#include "../QmlModels/QQmlObjectListModel.h"
#include "StateData.pb.h"
#include "globals.h"

class WeeklyScheduleModelItem : public QObject {
    Q_OBJECT
    QML_READONLY_CSTREF_PROPERTY (QString, time)
    QML_READONLY_CSTREF_PROPERTY (QString, away)
    QML_READONLY_CSTREF_PROPERTY (QString, home)
    QML_READONLY_CSTREF_PROPERTY (QString, status)
    QML_READONLY_CSTREF_PROPERTY (QString, gameid)

public:

    explicit WeeklyScheduleModelItem(const fantasybit::GameInfo &in, QObject *parent = nullptr) :  QObject(parent) {
        m_time = fromTime_t_toFantasyString(in.time());
        m_away = in.away().data();
        m_home = in.home().data();
        m_status = "Scheduled";
        m_gameid = in.id().data();

        qDebug() << " WeeklyScheduleModelItem"  << m_time << m_away << m_home;
    }
};

class WeeklyScheduleModel : public QQmlObjectListModel<WeeklyScheduleModelItem>{
    Q_OBJECT
    QML_READONLY_CSTREF_PROPERTY(int, week)

public:
    explicit WeeklyScheduleModel (QObject *          parent      = Q_NULLPTR,
                                  const QByteArray & displayRole = QByteArray (),
                                  const QByteArray & uidRole     = "gameid")

            : QQmlObjectListModel (parent,displayRole,uidRole) {}

    void updateWeeklySchedule(int week, const fantasybit::WeeklySchedule &weekly) {

        qDebug() << " updateWeeklySchedule"  << week << weekly.DebugString().data();

        clear();

        setweek(week);

        for ( auto &gi : weekly.games())
           append(new WeeklyScheduleModelItem(gi,this));
    }
};

Q_DECLARE_METATYPE(WeeklyScheduleModelItem*)
Q_DECLARE_METATYPE(WeeklyScheduleModel*)

#endif // WEEKLYSCHEDULEMODEL_H
