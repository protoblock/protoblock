#ifndef PLAYERPROJMODEL_H
#define PLAYERPROJMODEL_H

#include <QObject>
#include <QString>
#include "QQmlConstRefPropertyHelpers.h"
#include "QQmlObjectListModel.h"
#include "QQmlVarPropertyHelpers.h"

#include "StateData.pb.h"
#include "globals.h"
#include <unordered_map>
#include <QStringListModel>

#include <QtCore/qsortfilterproxymodel.h>

#include "sortfilterproxymodel.h"
#include "fbutils.h"
#include "pbgateways.h"
#include "weeklyschedulemodel.h"
#include "PlayerSymbolsModel.h"
#include "QQmlConstRefPropertyHelpers.h"

#include <QItemSelectionModel>
#include "QQmlPtrPropertyHelpers.h"
#include "QQmlConstRefPropertyHelpers.h"



namespace pb {
using namespace fantasybit;



class PlayerProjModelItem : public QObject {
    Q_OBJECT

    QML_READONLY_PTR_PROPERTY(PlayerSymbolsModelItem, pPlayerSymbolsModelItem)
    QML_CONSTANT_CSTREF_PROPERTY_PROXY (QString, fullname, pPlayerSymbolsModelItem)
    QML_CONSTANT_CSTREF_PROPERTY_PROXY (QString, lastname, pPlayerSymbolsModelItem)
    QML_CONSTANT_CSTREF_PROPERTY_PROXY (QString, firstname, pPlayerSymbolsModelItem)
    QML_CONSTANT_CSTREF_PROPERTY_PROXY (QString, pos, pPlayerSymbolsModelItem)
    QML_CONSTANT_CSTREF_PROPERTY_PROXY (QString, teamid, pPlayerSymbolsModelItem)
    QML_READONLY_CSTREF_PROPERTY (int, status)
    QML_READONLY_CSTREF_PROPERTY (QString, playerid)
    QML_READONLY_CSTREF_PROPERTY (QString, symbol)
    QML_WRITABLE_VAR_PROPERTY(int, projection)
    QML_WRITABLE_VAR_PROPERTY (int, knownProjection)
    QML_WRITABLE_VAR_PROPERTY (int, projectionStatus)
    QML_READONLY_CSTREF_PROPERTY (QString, gameid)
    QML_READONLY_CSTREF_PROPERTY ( bool, isopen)
    QML_WRITABLE_VAR_PROPERTY (int, avg)
    QML_WRITABLE_VAR_PROPERTY (int, fname1)
    QML_WRITABLE_VAR_PROPERTY (int, fname2)
    QML_WRITABLE_VAR_PROPERTY (int, fname3)
    QML_WRITABLE_VAR_PROPERTY (int, fname4)
    QML_WRITABLE_VAR_PROPERTY (int, fname5)

public:

    explicit PlayerProjModelItem(PlayerSymbolsModelItem *psmi,
                                 const fantasybit::PlayerDetail &pd,
                                 const std::string &playerid,
                                 const QString &gameid,
                                 int avg,
                                 bool gameopen,
                                 QObject *parent = nullptr) :  QObject(parent) {
        m_pPlayerSymbolsModelItem = psmi;
        m_status = pd.team_status;
        m_playerid = playerid.data();
        m_symbol = pd.symbol.data();
        if ( m_playerid != psmi->get_playerid() || m_symbol != psmi->get_symbol()) {
            qDebug() << "PlayerProjModelItem Criticle error" << m_playerid << m_symbol;
            return;
        }
        m_gameid = gameid;
        m_projection = 0;
        m_knownProjection = 0;
        m_fname1 = 0;
        m_fname2 = 0;
        m_fname3 = 0;
        m_fname4 = 0;
        m_fname5 = 0;
        m_isopen = gameopen;
        set_avg(avg);
//        qDebug() << " PlayerProjModelItem"  << pd.base.DebugString().data() << teamid.data() << m_playerid.data();
    }
};

class PlayerProjModel : public QQmlObjectListModel<PlayerProjModelItem>{
    Q_OBJECT
//    QML_READONLY_CSTREF_PROPERTY(int, week)

public:
    explicit PlayerProjModel (QObject *          parent      = Q_NULLPTR,
                                  const QByteArray & displayRole = QByteArray (),
                                  const QByteArray & uidRole     = {"playerid"})
        : QQmlObjectListModel (parent,displayRole,uidRole) {}


    void updateRosters(const std::vector<pb::GameRoster> &inrosters,
                       pb::IDataService *ds,
                       const PlayerSymbolsModel &inplayersymbol) {

        clear();

        for(const pb::GameRoster & game  : inrosters) {
            if ( game.status > GameStatus_Status_INGAME)
                continue;

//            std::multimap<int,PlayerProjModelItem*,std::greater<int>> sorted;
            // add game
            QString gameId = game.info.id().data();
            //add home players
            for (auto ha : {QString("home"),QString("away")}) {
                for(const auto& player : (ha == QString("home")) ? game.homeroster : game.awayroster) {

                    auto it = inplayersymbol.getByUid(player.second.symbol.data());
                    if ( !it ) {
                        qDebug() << "PlayerProjModel::updateRosters sont have symbol error " << player.second.symbol.data() << player.first.data();
                        continue;
                    }
                    append(new PlayerProjModelItem(it,
                                                   player.second,
                                                   player.first,
                                                   gameId,
                                                   ds->GetAvgProjection(player.first),
                                                   game.status < GameStatus_Status_INGAME,
                                                   this));
                }
            }

        }
    }

    void ongameStatusChange(std::string gameid,fantasybit::GameStatus_Status gs) {
        if ( gs == GameStatus_Status_CLOSED ) {
            for ( auto it : toList()) {
                if ( it->get_gameid() == gameid.data())
                    remove(it);
            }
        }
    }
};


class ProjectionsViewFilterProxyModel : public SortFilterProxyModel
{
    Q_OBJECT

    WeeklyScheduleModel  * myGameModelProxy;
    QItemSelectionModel * mySelectedGames;
//    QStringListModel * myPositionCombobox;
    bool myIsEnabled = true;
    QString myPos = "All";

public:
    Q_PROPERTY(QStringList userRoleNames READ userRoleNames CONSTANT)

    ProjectionsViewFilterProxyModel(//QStringListModel * positionCombobox = NULL,
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
//            qDebug() << " ! enabled";
            return true;
        }

//                qDebug() << " << cc " << columnCount();
//        if ( !QSortFilterProxyModel::filterAcceptsRow(sourceRow,sourceParent)) {
//            qDebug() << " filterAcceptsRow  no parent" << sourceRow;
//            return false;
//        }
//        qDebug() << " filterAcceptsRow" << sourceRow;

        PlayerProjModel * model = dynamic_cast<PlayerProjModel *>(sourceModel());
        if (model==NULL) return true;


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

    bool setData (const QModelIndex & index, const QVariant & value, int role) {
        if ( index.row() < 0 )
            return true;

//        qDebug() << "setDatasetDatasetDatasetData setting data" << index.row() << index.column();

        auto myindex = mapToSource(index);

//        qDebug() << "setDatasetDatasetDatasetData after map" << myindex.row() << myindex.column();

        PlayerProjModel * model = dynamic_cast<PlayerProjModel *>(sourceModel());
        if (model==NULL) return true;

//        qDebug() << " index model->at(index.row())->get_firstname() " << model->at(myindex.row())->get_firstname();

        model->at(myindex.row())->set_projection(value.toInt());
//        if ( model->at(index.row())->get_firstname() )
//            return false;

        return true;
    }

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




class LeaderBaordFantasyNameModelItem : public QObject {
    Q_OBJECT
    QML_CONSTANT_CSTREF_PROPERTY (QString, name)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, skill)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, lastseason)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, thisseason)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, thisweek)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, lastweek)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, lastupdate)
    QML_CONSTANT_CSTREF_PROPERTY (qint32, numcomplete)
    QML_CONSTANT_CSTREF_PROPERTY (quint64, hash)
    QML_CONSTANT_CSTREF_PROPERTY (QString, pk)

public:

    explicit LeaderBaordFantasyNameModelItem(const fantasybit::FantasyNameBal &in) :  QObject(nullptr) {
        m_name = in.name().data();
        m_pk = in.public_key().data();
        m_skill = in.stake();
        m_hash = in.chash();
    }
};


class LeaderBaordFantasyNameModel : public QQmlObjectListModel<LeaderBaordFantasyNameModelItem> {};

}
using namespace pb;

Q_DECLARE_METATYPE(LeaderBaordFantasyNameModel *)
Q_DECLARE_METATYPE(PlayerProjModelItem*)
Q_DECLARE_METATYPE(PlayerProjModel*)



#endif // PLAYERPROJMODEL_H














//struct PlayerDetail {
//    PlayerBase base;
//    PlayerStatus::Status team_status;
//    PlayerGameStatus game_status;
//};

//struct GameRoster {
//    GameInfo info;
//    GameStatus::Status  status;
//    std::unordered_map<std::string,PlayerDetail> homeroster;
//    std::unordered_map<std::string,PlayerDetail> awayroster;

//};

//unordered_map<std::string,int> DataService::GetProjByName(const std::string &fname) {

//unordered_map<std::string,int> DataService::GetProjById(const std::string &pid) {

//std::vector<fantasybit::GameRoster> DataService::GetCurrentWeekGameRosters()


