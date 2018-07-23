#ifndef WEEKLYSCHEDULEMODEL_H
#define WEEKLYSCHEDULEMODEL_H

#include <QObject>
#include <QString>
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
#include "../QmlModels/QQmlObjectListModel.h"
#include "../QmlSupermacros/QQmlVarPropertyHelpers.h"
#include "StateData.pb.h"
#include "globals.h"
#include "sortfilterproxymodel.h"
#include "StatusData.pb.h"

namespace pb {

using namespace  fantasybit;

static QString GameStatusToString(fantasybit::GameStatus_Status gs) {
    switch (gs) {
    case GameStatus_Status_SCHEDULED:
        return "Scheduled";
        break;
    case GameStatus_Status_PREGAME:
        return "Pregame";
        break;
    case GameStatus_Status_INGAME:
        return "Ingame";
        break;
    case GameStatus_Status_POSTGAME:
        return "Postgame";
        break;
    case GameStatus_Status_CLOSED:
        return "Closed";
        break;

    default:
        break;
    }
}

class WeeklyScheduleModelItem : public QObject {
    Q_OBJECT
    QML_READONLY_CSTREF_PROPERTY (QString, time)
    QML_READONLY_CSTREF_PROPERTY (QString, away)
    QML_READONLY_CSTREF_PROPERTY (QString, home)
    QML_WRITABLE_VAR_PROPERTY (QString, status)
    QML_READONLY_CSTREF_PROPERTY (QString, gameid)

public:

    explicit WeeklyScheduleModelItem(const fantasybit::GameInfo &in, QObject *parent = nullptr) :  QObject(parent) {
        m_time = fromTime_t_toFantasyString(in.time());
        m_away = in.away().data();
        m_home = in.home().data();
        set_status("Scheduled");
        m_gameid = in.id().data();


//        qDebug() << " WeeklyScheduleModelItem"  << m_time << m_away << m_home;
    }

    explicit WeeklyScheduleModelItem(const fantasybit::GameInfo &in,fantasybit::GameStatus gs, QObject *parent = nullptr) :  QObject(parent) {
       if ( gs.datetime () > 0 )
           m_time = fromTime_t_toFantasyString( gs.datetime ());
       else
            m_time = fromTime_t_toFantasyString( in.time() );

       if ( gs.status () == GameStatus_Status_INGAME)
           m_time.append ('k');

        m_away = in.away().data();
        m_home = in.home().data();
        set_status(GameStatusToString(gs.status ()));
        m_gameid = in.id().data();


//        qDebug() << " WeeklyScheduleModelItem"  << m_time << m_away << m_home;
    }

    explicit WeeklyScheduleModelItem(WeeklyScheduleModelItem *in,fantasybit::GameStatus::Status gs, QObject *parent = nullptr)
                                                :  QObject(parent) {
        m_time = in->get_time();
        m_away = in->get_away();
        m_home = in->get_home();
        set_status(GameStatusToString(gs));
        m_gameid = in->get_gameid();
    }
};

class WeeklyScheduleModel : public QQmlObjectListModel<WeeklyScheduleModelItem>{
    Q_OBJECT
    QML_READONLY_CSTREF_PROPERTY(int, week)
    QML_READONLY_CSTREF_PROPERTY(int, season)

public:
    explicit WeeklyScheduleModel (QObject *          parent      = Q_NULLPTR,
                                  const QByteArray & displayRole = QByteArray (),
                                  const QByteArray & uidRole     = "gameid")

            : QQmlObjectListModel (parent,displayRole,uidRole) {}

    void updateWeeklySchedule(int season,int week, const fantasybit::WeeklySchedule &weekly) {

        #ifdef TRACE2
                qDebug() << " updateWeeklySchedule"  << week << weekly.DebugString().data();
        #endif
        clear();

        setweek(week);
        setseason(season);

        std::map<int,std::vector<fantasybit::GameInfo>> sorted;
        for ( auto &gi : weekly.games()) {
            auto &vec = sorted[gi.time()];
            vec.push_back(gi);
        }

        for ( auto tvec : sorted ) {
            for ( auto p : tvec.second ) {
                append(new WeeklyScheduleModelItem(p,this));
            }
        }
    }

    bool UpdateStatus(std::string gameid,fantasybit::GameStatus_Status gs, bool reverse = false) {
        auto *it = this->getByUid(gameid.data());
        if ( !it ) return false;

        if (GameStatusToString(gs) == it->get_status())
            return false;

        bool ret = false;
        if ( gs == GameStatus_Status_CLOSED && !reverse) {
            remove(it);
            ret = true;
        }
        else if (gs == GameStatus_Status_CLOSED && reverse ) {
            this->getByUid(gameid.data())->set_status(GameStatusToString(gs));
            ret = true;
        }
        else if ( !reverse ) {
            this->getByUid(gameid.data())->set_status(GameStatusToString(gs));
            ret = false;
        }
        else if ( reverse ) {
            ret = false;
            remove(this->getByUid(gameid.data()));
        }
    }


};


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
//        qDebug() << " filterAcceptsRow" << sourceRow;

        WeeklyScheduleModel * model = dynamic_cast<WeeklyScheduleModel *>(sourceModel());
        if (model==NULL) return true;


        if ( mygames != "All")
            if ( model->at(sourceRow)->get_status() != mygames )
                return false;

        return true;
    }
};
}

using namespace pb;
Q_DECLARE_METATYPE(WeeklyScheduleModelItem*)
Q_DECLARE_METATYPE(WeeklyScheduleModel*)

#endif // WEEKLYSCHEDULEMODEL_H
