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
#include <QItemSelectionModel>
#include "fantasynamemodel.h"

namespace pb {
using namespace fantasybit;

class FantasyBitAwardModelItem : public QObject {
    Q_OBJECT
    QML_READONLY_CSTREF_PROPERTY (QString, name)
    QML_READONLY_CSTREF_PROPERTY (int, proj)
    QML_READONLY_CSTREF_PROPERTY (int, award)
//    QML_READONLY_CSTREF_PROPERTY (int, position)
//    QML_READONLY_CSTREF_PROPERTY (int, price)
//    QML_READONLY_CSTREF_PROPERTY (int, pnl)


public:

    explicit FantasyBitAwardModelItem(const fantasybit::FantasyBitAward &fba,
                                   QObject *parent = nullptr)
                            :  QObject(parent) {
        m_name = fba.name().data();
        m_proj = fba.proj();
        m_award = fba.award();
    }
};

//class FantasyBitAwardModel : QQmlObjectListModel<FantasyBitAwardModelItem> {};


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
    QML_OBJMODEL_PROPERTY (FantasyBitAwardModelItem, awardsModel)

    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, myproj)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, myaward)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, mypos)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (double, myprice)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, mypnl)

    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, myrowpnl)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (double, myrowprice)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, myrowpos)

public:

    explicit PlayerResultModelItem(const fantasybit::PlayerBase &pd,
                                   const QString &teamid,
                                   const QString &gameid,
                                   const fantasybit::PlayerResult &pr,
                                   const std::string fname = "",
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
        m_RushTD = os.rushtd();
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
        m_FG = fgs;


        m_myproj = 0;
        m_myaward = 0;
        m_awardsModel = new QQmlObjectListModel<FantasyBitAwardModelItem>(this,QByteArray(),{"name"});
        for ( auto it : pr.fantaybitaward()) {
            m_awardsModel->append(new FantasyBitAwardModelItem(it));
            if ( fname == it.name() ) {
                m_myproj = it.proj();
                m_myaward = it.award();
            }
        }

        for ( auto it : pr.fantasybitpnl()) {
            if ( fname == it.name() ) {
                m_mypnl = it.pnl();
                m_mypos = it.spos().qty();
//                m_myprice = abs(it.spos().price());
                if ( m_mypos != 0 ) {
                    m_myprice = double(it.spos().price()) / (m_mypos * -1.0);
                }
            }
        }

        for ( auto it : pr.rowposdividend()) {
            if ( fname == it.name() ) {
                m_myrowpnl = it.pnl();
                m_myrowpos = it.spos().qty();
                if ( m_myrowpos != 0 ) {
                    m_myrowprice = double(it.spos().price()) / (m_myrowpos * -1.0);
                }

            }
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
                       WeeklyScheduleModel &sched,const std::string &usename) {

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
                const PlayerBase &pd = ds->GetPlayerBase(playerresult.playerid());
                append(new PlayerResultModelItem(pd,home,gameId,playerresult,usename,this));

            }

            //add away players
            for(const auto& playerresult : game.away_result()) {
                const PlayerBase &pd = ds->GetPlayerBase(playerresult.playerid());
                append(new PlayerResultModelItem(pd,away,gameId,playerresult,usename,this));
            }
        }
    }
};

class ResultsViewFilterProxyModel : public SortFilterProxyModel
{
    Q_OBJECT

    WeeklyScheduleModel  * myGameModelProxy;
    QItemSelectionModel * mySelectedGames;
//    QStringListModel * myPositionCombobox;
    bool myIsEnabled = true;
    QString myPos = "All";

public:
    Q_PROPERTY(QStringList userRoleNames READ userRoleNames CONSTANT)

    ResultsViewFilterProxyModel(//QStringListModel * positionCombobox = NULL,
                                    WeeklyScheduleModel  * gameModelProxy= NULL,
                                    QItemSelectionModel * gameSelectionModel=NULL,
                                    QObject *parent = 0)
        : SortFilterProxyModel(parent)
    {
//        myPositionCombobox = positionCombobox;
        myGameModelProxy = gameModelProxy;
//        if (myGameModelProxy != NULL) mySelectedGames = gameSelectionModel;
        mySelectedGames = gameSelectionModel;
    }

    QStringList ret;
    QStringList userRoleNames() // Return ordered List of user-defined roles
    {


        return ret;
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

    void bindFilter(){
//        if (myPositionCombobox!=NULL){
//            QObject::connect(myPositionCombobox,
//                             SIGNAL(currentTextChanged(QString)),
//                             this,SLOT(invalidate()));
//        }

        if (myGameModelProxy!=NULL){
            QObject::connect(myGameModelProxy,
                             SIGNAL(modelReset()),
                             this,SLOT(invalidate()));
        }

        if (mySelectedGames!=NULL){
            QObject::connect(mySelectedGames,
                             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                             this,SLOT(invalidate()));
        }
    }

    Q_INVOKABLE void setPos(const QString &pos) {
        if ( pos != myPos ) {
            myPos = pos;
            invalidate();
        }
    }

    Q_INVOKABLE QString getAwardsModelUid(int rowIndex) {
        auto sindex = mapToSource(index(rowIndex,0));

        PlayerResultModel * model = dynamic_cast<PlayerResultModel *>(sourceModel());
        if (model==NULL) return nullptr;

//        qDebug() << " jay fullname " << model->at(sindex.row())->get_fullname();

        QQmlObjectListModel<FantasyBitAwardModelItem> *mymodel = model->at(sindex.row())->get_awardsModel();
//        qDebug() << " mymodel " << mymodel->count();
        return model->at(sindex.row())->get_playerid();
    }

    Q_INVOKABLE virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) Q_DECL_OVERRIDE {
//        qDebug() << " sort called" << column;
        QSortFilterProxyModel::sort(column, order);


//                qDebug() << " << cc " << columnCount();
//        QSortFilterProxyModel::setSortRole(column);
//        QSortFilterProxyModel::sort(0, order);
    }

protected:
    //filtering
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
        if (!myIsEnabled) {
            qDebug() << " ! enabled";
            return true;
        }

//        qDebug() << " << cc " << " ResultsViewFilterProxyModel " << sourceRow;
//        if ( !QSortFilterProxyModel::filterAcceptsRow(sourceRow,sourceParent)) {
//            qDebug() << " filterAcceptsRow  no parent" << sourceRow;
//            return false;
//        }
//        qDebug() << " filterAcceptsRow" << sourceRow;

        PlayerResultModel * model = dynamic_cast<PlayerResultModel *>(sourceModel());
        if (model==NULL) return true;

//        qDebug() << " !null " << myPos;

        if ( myPos != "All")
            if ( model->at(sourceRow)->get_pos() != myPos )
                return false;


//        QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        auto gameid = model->at(sourceRow)->get_gameid();

//        qDebug() << " filterAcceptsRow gameid" << gameid;
//        if (item == NULL) return true;
//        QString gameId = item->propertyValue<PropertyNames::Game_ID>().toString();

        if (gameid=="")  return false;
        if (mySelectedGames!=NULL && myGameModelProxy != NULL){
            if ( !mySelectedGames->hasSelection() ) return true;

//            qDebug() << " filterAcceptsRow hasselection";

            int i = myGameModelProxy->indexOf(myGameModelProxy->getByUid(gameid));
            return mySelectedGames->isSelected(myGameModelProxy->index(i));
        }

        return true;
    }

//    bool setData (const QModelIndex & index, const QVariant & value, int role) {
//        if ( index.row() < 0 )
//            return true;

//        qDebug() << "setDatasetDatasetDatasetData setting data" << index.row() << index.column();

//        auto myindex = mapToSource(index);

//        qDebug() << "setDatasetDatasetDatasetData after map" << myindex.row() << myindex.column();

//        PlayerResultModel * model = dynamic_cast<PlayerResultModel *>(sourceModel());
//        if (model==NULL) return true;

//        qDebug() << " index model->at(index.row())->get_firstname() " << model->at(myindex.row())->get_firstname();

//        model->at(myindex.row())- set_projection(value.toInt());
//        return true;
//    }

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE {
        return SortFilterProxyModel::data(index, role);
//        qDebug() <<role << " << cc " << columnCount();
    }

//        if (myPositionCombobox!=NULL){
//            //get current postion from position combo box
//            QString position= myPositionCombobox->currentText().trimmed().toUpper();
//            if (position != "ALL"){
//                QString playerPosition=item->propertyValue<PropertyNames::Position>().toString();
//                if (playerPosition != position)
//                    return false;
//            }
//        }

//        if (myGameModelProxy!=NULL){
//            GamesFilter gameFilter = myGameModelProxy->filter();
//            if (gameFilter != GamesFilter::All) {
//            GameTableModel * allGames = dynamic_cast<GameTableModel *>(myGameModelProxy->sourceModel());
//            if (allGames==NULL) return false;
//            QVariant vvalue;
//            GameStatus_Status gameStatus;
//            bool propertyFound = allGames->itemPropertyValue<PropertyNames::Game_Status>(gameId,vvalue);
//            if (!propertyFound) return false;
//            gameStatus = qvariant_cast<GameStatus_Status>(vvalue);
//            if (!GameViewFilterProxyModel::testGameStatus(gameFilter,gameStatus))
//                return false;
//            }
//        }


//    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{


//    }

//    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const{
//        ProjectionSheetTableModel * model = dynamic_cast<ProjectionSheetTableModel *>(sourceModel());
//        if (model==NULL) return false;
//        QVariant lefData = model->columnData(source_left.column(),source_left);
//        QVariant rightData = model->columnData(source_right.column(),source_right);
//        if (lefData.isNull())  return true;
//        if (rightData.isNull())  return false;
//        if (lefData.type()!=rightData.type()) return false;
//        return lefData < rightData;
//    }

};


}
using namespace pb;
Q_DECLARE_METATYPE(PlayerResultModelItem*)
Q_DECLARE_METATYPE(PlayerResultModel*)
Q_DECLARE_METATYPE(FantasyBitAwardModelItem *)
Q_DECLARE_METATYPE(QQmlObjectListModel<FantasyBitAwardModelItem> *)



#endif // PlayerResultMODEL_H

