#ifndef PLAYERQUOTESLICEMODEL_H
#define PLAYERQUOTESLICEMODEL_H

#include <QObject>
#include <QString>
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
#include "../QmlModels/QQmlObjectListModel.h"
#include "StateData.pb.h"
#include "sortfilterproxymodel.h"
#include "fbutils.h"
#include "playerprojmodel.h"
#include <QtCore/qsortfilterproxymodel.h>


namespace pb {
using namespace fantasybit;

//class PlayerProjModelItem;

class PlayerQuoteSliceModelItem : public QObject {
    Q_OBJECT
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, lastprice)
    QML_READONLY_CSTREF_PROPERTY (QString, pos)
    QML_READONLY_CSTREF_PROPERTY (QString, lastname)
    QML_READONLY_CSTREF_PROPERTY (QString, firstname)
    QML_READONLY_CSTREF_PROPERTY (QString, teamid)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, bidsize)
    QML_READONLY_CSTREF_PROPERTY (QString , fullname)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, bid)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, ask)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, asksize)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, volume)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, change)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, updown)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, hi)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, lo)
    QML_READONLY_CSTREF_PROPERTY (QString, playerid)
    QML_READONLY_CSTREF_PROPERTY (QString, symbol)


    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, myposition)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, mypnl)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (double, myavg)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (double, ytd)

    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, PassTD)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, PassYd)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, RushTD)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, RushYd)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, RecTD)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, RecYd)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, Rec)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, Int)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, Fum)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, PAT)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, _2Pt)
    QML_READONLY_CSTREF_PROPERTY (QString, FG)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, D_TD)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, Sack)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, TA)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, SFTY)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, D2pt)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, PtsA)
public:

    explicit PlayerQuoteSliceModelItem(const fantasybit::ROWMarket &in) :  QObject(nullptr) {
        m_lastprice = in.quote().l();
        m_pos = in.playerdata().player_base().position().data();
        m_firstname = in.playerdata().player_base().first().data();
        m_lastname = in.playerdata().player_base().last().data();
        m_teamid = in.playerdata().player_status().teamid().data();
        m_fullname =  QString("%1 %2")
                .arg ( in.playerdata ().player_base ().first ().data () )
                .arg ( in.playerdata ().player_base ().last ().data () );
        m_bidsize = in.quote().bs();
        m_bid = in.quote().b();
        m_ask = in.quote().a();
        m_asksize = in.quote().as();
        m_volume = in.ohlc().volume();
        m_change = in.ohlc().change();
        m_updown = in.quote().udn();
        m_hi = in.ohlc().high();
        m_lo = in.ohlc().low();
        m_playerid = in.pid().data();
        m_symbol = in.pid().data(); //TODO
    }

    void setProperties(const fantasybit::ROWMarket &in)   {
       setplayerid(in.pid().data());
       setsymbol(in.pid().data()); //TODO
       setlastprice(in.quote().l());
       setpos(in.playerdata().player_base().position().data());

       setlastname(in.playerdata().player_base().last().data());
       setfirstname(in.playerdata().player_base().first().data());
       setteamid(in.playerdata().player_status().teamid().data());
       setbidsize(in.quote().bs());
       setfullname(QString("%1 %2")
                   .arg ( in.playerdata ().player_base ().first ().data () )
                   .arg ( in.playerdata ().player_base ().last ().data () ));
       setbid(in.quote().b());
       setask(in.quote().a());
       setasksize(in.quote().as());
       setvolume(in.ohlc().volume());
       setchange(in.ohlc().change());
       setupdown(in.quote().udn());
       sethi(in.ohlc().high());
       setlo(in.ohlc().low());

    }

    explicit PlayerQuoteSliceModelItem(const pb::PlayerProjModelItem &in) :  QObject(nullptr) {
//        m_lastprice = in.quote().l();
        m_pos = in.get_pos();
        m_firstname = in.get_firstname();
        m_lastname = in.get_lastname();
        m_teamid = in.get_teamid();
        m_fullname =  in.get_fullname();
//        m_bidsize = in.quote().bs();
//        m_bid = in.quote().b();
//        m_ask = in.quote().a();
//        m_asksize = in.quote().as();
//        m_volume = in.ohlc().volume();
//        m_change = in.ohlc().change();
//        m_updown = in.quote().udn();
//        m_hi = in.ohlc().high();
//        m_lo = in.ohlc().low();
        m_playerid = in.get_playerid();
        m_symbol = m_playerid;
    }
};


class PlayerQuoteSliceModel : public QQmlObjectListModel<PlayerQuoteSliceModelItem>{
public:
    explicit PlayerQuoteSliceModel (QObject *          parent      = Q_NULLPTR,
                                  const QByteArray & displayRole = QByteArray (),
                                  const QByteArray & uidRole     = QByteArray ())
        : QQmlObjectListModel (parent,displayRole,uidRole) {}
};

class PlayerQuoteSliceViewFilterProxyModel : public SortFilterProxyModel
{
    Q_OBJECT

//    WeeklyScheduleModel  * myGameModelProxy;
//    QItemSelectionModel * mySelectedGames;
//    QStringListModel * myPositionCombobox;
    bool myIsEnabled = true;
    QString myPos = "All";

public:
    Q_PROPERTY(QStringList userRoleNames READ userRoleNames CONSTANT)

    PlayerQuoteSliceViewFilterProxyModel(//QStringListModel * positionCombobox = NULL,
//                                    WeeklyScheduleModel  * gameModelProxy= NULL,
//                                    QItemSelectionModel * gameSelectionModel=NULL,
                                    QObject *parent = 0)
        : SortFilterProxyModel(parent)
    {
//        myPositionCombobox = positionCombobox;
//        myGameModelProxy = gameModelProxy;
//        if (myGameModelProxy != NULL) mySelectedGames = gameSelectionModel;
//        mySelectedGames = gameSelectionModel;
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

//        if (myGameModelProxy!=NULL){
//            QObject::connect(myGameModelProxy,
//                             SIGNAL(modelReset()),
//                             this,SLOT(invalidate()));
//        }

//        if (mySelectedGames!=NULL){
//            QObject::connect(mySelectedGames,
//                             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
//                             this,SLOT(invalidate()));
//        }
    }

    Q_INVOKABLE void setPos(const QString &pos) {
        if ( pos != myPos ) {
            myPos = pos;
            invalidate();
        }
    }

    Q_INVOKABLE QString getPlayerSliceModelUid(int rowIndex) {
        auto sindex = mapToSource(index(rowIndex,0));

        PlayerQuoteSliceModel * model = dynamic_cast<PlayerQuoteSliceModel *>(sourceModel());
        if (model==NULL) return nullptr;

//        qDebug() << " jay fullname " << model->at(sindex.row())->get_fullname();

//        QQmlObjectListModel<PlayerQuoteSliceModelItem> *mymodel = model->at(sindex.row())->get_awardsModel();
//        qDebug() << " mymodel " << mymodel->count();
        return model->at(sindex.row())->get_symbol();
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

        PlayerQuoteSliceModel * model = dynamic_cast<PlayerQuoteSliceModel *>(sourceModel());
        if (model==NULL) return true;

//        qDebug() << " !null " << myPos;

        if ( myPos != "All")
            if ( model->at(sourceRow)->get_pos() != myPos )
                return false;


//        QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
//        auto gameid = model->at(sourceRow)->get_gameid();

//        qDebug() << " filterAcceptsRow gameid" << gameid;
//        if (item == NULL) return true;
//        QString gameId = item->propertyValue<PropertyNames::Game_ID>().toString();

//        if (gameid=="")  return false;
//        if (mySelectedGames!=NULL && myGameModelProxy != NULL){
//            if ( !mySelectedGames->hasSelection() ) return true;

////            qDebug() << " filterAcceptsRow hasselection";

//            int i = myGameModelProxy->indexOf(myGameModelProxy->getByUid(gameid));
//            return mySelectedGames->isSelected(myGameModelProxy->index(i));
//        }

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


Q_DECLARE_METATYPE(PlayerQuoteSliceModel*)
Q_DECLARE_METATYPE(PlayerQuoteSliceModelItem*)

}

//https://github.com/mkawserm/ModelsTest/blob/master/main.cpp


//message ContractOHLC {
//    optional string symbol = 10;
//    optional int32 open = 20;
//    optional int32 high = 30;
//    optional int32 low = 40;
//    optional int32 close = 50;
//    optional int32 volume = 60;
//    optional int32 change = 70;
//}


//message MarketQuote {
//    optional int32 bs = 10;
//    optional int32 b = 20;
//    optional int32 a = 30;
//    optional int32 as = 40;
//    optional int32 l = 50;
//    optional int32 ls = 60;
//    optional int32 udn = 70;
//}



//if( column ==i++)
//    return data->propertyValue<PropertyNames::LAST>();
//if( column ==  i++)
//    return data->propertyValue<PropertyNames::Position>();
//if( column == i++)
//    return data->propertyValue<PropertyNames::Player_Name>();
//if( column == i++)
//    return data->propertyValue<PropertyNames::Team_ID>();
//if( column ==i++)
//    return data->propertyValue<PropertyNames::BIDSIZE>();
//if( column ==i++)
//    return data->propertyValue<PropertyNames::BID>();
//if( column ==i++)
//    return data->propertyValue<PropertyNames::ASK>();
//if( column ==i++)
//    return data->propertyValue<PropertyNames::ASKSIZE>();
//if( column ==i++)
//    return data->propertyValue<PropertyNames::VOLUME>();
//if( column == i++)
//    return data->propertyValue<PropertyNames::CHANGE>();


#endif // PLAYERQUOTESLICEMODEL_H
