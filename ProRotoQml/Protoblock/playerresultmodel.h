#ifndef PlayerResultMODEL_H
#define PlayerResultMODEL_H

#include <QObject>
#include <QString>
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
#include "../QmlModels/QQmlObjectListModel.h"
#include "../QmlSupermacros/QQmlVarPropertyHelpers.h"

#include "StateData.pb.h"
#include "globals.h"
#include <unordered_map>
#include <QStringListModel>

#include <QtCore/qsortfilterproxymodel.h>

#include "sortfilterproxymodel.h"
#include "fbutils.h"
#include "pbgateways.h"
#include "weeklyschedulemodel.h"

namespace pb {
using namespace fantasybit;

class PlayerResultModelItem : public QObject {
    Q_OBJECT
    QML_READONLY_CSTREF_PROPERTY (QString, fullname)
    QML_READONLY_CSTREF_PROPERTY (QString, lastname)
    QML_READONLY_CSTREF_PROPERTY (QString, firstname)
    QML_READONLY_CSTREF_PROPERTY (QString, pos)
    QML_READONLY_CSTREF_PROPERTY (QString, teamid)
    QML_READONLY_CSTREF_PROPERTY (int, status)
    QML_READONLY_CSTREF_PROPERTY (QString, playerid)
    QML_READONLY_CSTREF_PROPERTY (double, result)
    QML_READONLY_CSTREF_PROPERTY (int, fb)
    QML_READONLY_CSTREF_PROPERTY (QString, gameid)
    QML_READONLY_CSTREF_PROPERTY (int, PassTD)
    QML_READONLY_CSTREF_PROPERTY (int, PassYd)
    QML_READONLY_CSTREF_PROPERTY (int, RushTD)
    QML_READONLY_CSTREF_PROPERTY (int, RushYd)
    QML_READONLY_CSTREF_PROPERTY (int, RecTD)
    QML_READONLY_CSTREF_PROPERTY (int, RecYd)
    QML_READONLY_CSTREF_PROPERTY (int, Rec)
    QML_READONLY_CSTREF_PROPERTY (int, Int)
    QML_READONLY_CSTREF_PROPERTY (int, Fum)
    QML_READONLY_CSTREF_PROPERTY (int, PAT)
    QML_READONLY_CSTREF_PROPERTY (int, _2Pt)
    QML_READONLY_CSTREF_PROPERTY (QString, FG)
    QML_READONLY_CSTREF_PROPERTY (int, D_TD)
    QML_READONLY_CSTREF_PROPERTY (int, Sack)
    QML_READONLY_CSTREF_PROPERTY (int, TA)
    QML_READONLY_CSTREF_PROPERTY (int, SFTY)
    QML_READONLY_CSTREF_PROPERTY (int, D2pt)
    QML_READONLY_CSTREF_PROPERTY (int, PtsA)



public:

    explicit PlayerResultModelItem(const fantasybit::PlayerBase &pd,
                                   const QString &teamid,
                                   const QString &gameid,
                                   const fantasybit::PlayerResult &pr,
                                   QObject *parent = nullptr)
                                            :  QObject(parent) {
        m_fullname =  QString("%1 %2")
                .arg ( pd.first ().data () )
                .arg ( pd.last ().data () );
        m_firstname = pd.first().data();
        m_lastname = pd.last().data();
        m_pos = pd.position().data();
        m_teamid = teamid;
        m_playerid = pr.playerid().data();
        m_gameid = gameid;
        m_result = pr.result();
        m_fb = pr.result() * 100.0;


        const Ostats &os = pr.stats().ostats();
        m_PassTD = os.passtd();
        m_PassYd = os.passyds();
        m_RushTD = os.rushyds();
        m_RushYd = os.rushyds();
        m_RecTD = os.rectd();
        m_RecYd = os.recyds();
        m_Rec   = os.rec();
        m_Int   = os.pint();
        m_Fum   = os.fumble();
        m__2Pt  = os.twopt();

        const Dstats &ds = pr.stats().dstats();
        m_D2pt  = ds.twopt();
        m_D_TD  = ds.deftd();
        m_SFTY  = ds.sfty();
        m_TA    = ds.turnovers();
        m_Sack  = ds.sacks();
        m_PtsA  = ds.ptsa();

        const Kstats &ks = pr.stats().kstats();
        m_PAT   = ks.pa();
        QString fgs;
        auto sz = ks.fg_size();
        if ( sz >0 ) {
            fgs = QString::number(ks.fg(0));
            for (int i=1; i<sz ;i++)
                fgs.append(QString(",%1").arg(ks.fg(i)));
        }


        //qDebug() << " PlayerResultModelItem"  << pd.base.DebugString().data() << teamid.data() << m_playerid.data();
    }
};

class PlayerResultModel : public QQmlObjectListModel<PlayerResultModelItem>{
    Q_OBJECT
//    QML_READONLY_CSTREF_PROPERTY(int, week)

public:
    explicit PlayerResultModel (QObject *          parent      = Q_NULLPTR,
                                  const QByteArray & displayRole = QByteArray (),
                                  const QByteArray & uidRole     = {"playerid"})
        : QQmlObjectListModel (parent,displayRole,uidRole) {}


    void updateRosters(const std::vector<pb::GameResult> &inrosters,
                       pb::IDataService *ds,
                       WeeklyScheduleModel &sched) {

        clear();

        for(const pb::GameResult & game  : inrosters) {
            // add game
            QString gameId = game.gameid().data();
            auto item = sched.getByUid(gameId);
            if ( !item ) continue;

            auto home = item->get_home();
            auto away = item->get_away();

            //add home players
            for(const auto& playerresult : game.home_result()) {
                PlayerBase &pd = ds->GetPlayerBase(playerresult.playerid());
                append(new PlayerResultModelItem(pd,home,gameId,playerresult,this));
            }

            //add away players
            for(const auto& playerresult : game.away_result()) {
                PlayerBase &pd = ds->GetPlayerBase(playerresult.playerid());
                append(new PlayerResultModelItem(pd,away,gameId,playerresult,this));
            }
        }
    }
};

Q_DECLARE_METATYPE(PlayerResultModelItem*)
Q_DECLARE_METATYPE(PlayerResultModel*)

}

#endif // PlayerResultMODEL_H

