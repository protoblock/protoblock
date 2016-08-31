#ifndef WEEKLYSCHEDULEMODEL_H
#define WEEKLYSCHEDULEMODEL_H

#include <QObject>
#include <QString>
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
#include "../QmlModels/QQmlObjectListModel.h"
#include "StateData.pb.h"
#include "globals.h"
#include "sortfilterproxymodel.h"

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

    void updateWeeklySchedule(int week, const fantasybit::WeeklySchedule &weekly,QString &filter) {

        qDebug() << " updateWeeklySchedule"  << week << weekly.DebugString().data();

        clear();

        setweek(week);

        for ( auto &gi : weekly.games())
           append(new WeeklyScheduleModelItem(gi,this));
    }
};

Q_DECLARE_METATYPE(WeeklyScheduleModelItem*)
Q_DECLARE_METATYPE(WeeklyScheduleModel*)

class WeeklyScheduleViewFilterProxyModel : public SortFilterProxyModel {
    Q_OBJECT

    QString mygames = "Scheduled";

    bool myIsEnabled = true;
public slots:
    void  gameFilter(QString& filter) {
        mygames = filter;
        invalidate();
    }

public:

    WeeklyScheduleViewFilterProxyModel(//QStringListModel * positionCombobox = NULL,
//                                    WeeklyScheduleModel  * gameModelProxy= NULL,
//                                    QItemSelectionModel * gameSelectionModel=NULL,
                                    QObject *parent = 0)
        : SortFilterProxyModel(parent)
    {
//        myPositionCombobox = positionCombobox;
//        myGameModelPrWeeklyScheduleViewFilterProxyModeloxy = gameModelProxy;
//        if (myGameModelProxy != NULL) mySelectedGames = gameSelectionModel;
//        mySelectedGames = gameSelectionModel;
    }


    bool isEnabled(){
        return myIsEnabled;
    }

    void disable(){
        myIsEnabled = false;
    }

    void enable(){
        myIsEnabled = true;
    }

protected:
    //filtering
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
        if (!myIsEnabled) return true;

//        if ( !QSortFilterProxyModel::filterAcceptsRow(sourceRow,sourceParent)) {
//            qDebug() << " filterAcceptsRow  no parent" << sourceRow;
//            return false;
//        }
        qDebug() << " filterAcceptsRow" << sourceRow;

        WeeklyScheduleModel * model = dynamic_cast<WeeklyScheduleModel *>(sourceModel());
        if (model==NULL) return true;


        if ( mygames != "All")
            if ( model->at(sourceRow)->get_status() != mygames )
                return false;

        return true;
    }
};

#endif // WEEKLYSCHEDULEMODEL_H
