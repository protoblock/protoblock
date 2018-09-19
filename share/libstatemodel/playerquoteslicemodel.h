#ifndef PLAYERQUOTESLICEMODEL_H
#define PLAYERQUOTESLICEMODEL_H

#include <QObject>
#include <QString>
#include "QQmlConstRefPropertyHelpers.h"
#include "QQmlObjectListModel.h"
#include "StateData.pb.h"
#include "sortfilterproxymodel.h"
#include "fbutils.h"
#include "playerprojmodel.h"
#include <QtCore/qsortfilterproxymodel.h>
#include "openordersmodel.h"
#include "depthmarketmodel.h"
#include "PlayerSymbolsModel.h"
#include <QTimer>


namespace pb {
using namespace fantasybit;


//class PlayerProjModelItem;
class PlayerQuoteSliceModelItem : public QObject {
    Q_OBJECT

    DepthMarketModel mDepthMarketModel;
//    OpenOrdersModel mOpenOrdersModel;
    QML_READONLY_PTR_PROPERTY(DepthMarketModel, pDepthMarketModel)
//    QML_READONLY_PTR_PROPERTY(PlayerProjModelItem, pPlayerProjItem)
    QML_READONLY_PTR_PROPERTY(PlayerSymbolsModelItem, pPlayerSymbolsModelItem)

    QML_READONLY_CSTREF_PROPERTY (QString, playerid)
    QML_READONLY_CSTREF_PROPERTY (QString, symbol)
    QML_READONLY_CSTREF_PROPERTY (qint32, multiplier)

    QML_CONSTANT_CSTREF_PROPERTY_PROXY (QString, pos, pPlayerSymbolsModelItem)
    QML_CONSTANT_CSTREF_PROPERTY_PROXY (QString, lastname, pPlayerSymbolsModelItem)
    QML_CONSTANT_CSTREF_PROPERTY_PROXY (QString, firstname, pPlayerSymbolsModelItem)
    QML_CONSTANT_CSTREF_PROPERTY_PROXY (QString, teamid, pPlayerSymbolsModelItem)
    QML_CONSTANT_CSTREF_PROPERTY_PROXY (QString , fullname, pPlayerSymbolsModelItem)

    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, lastprice)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, lastsize)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, bidsize)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, bid)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, ask)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, asksize)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, volume)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, change)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, updown)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, hi)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, lo)

    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, oi)

    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, myposition)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (qint32, mypnl)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (double, myavg)


    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, blocknum)

    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, bsdiff)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, bdiff)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, adiff)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, asdiff)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, ldiff)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, lsdiff)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, vdiff)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, hdiff)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, lodiff)
    QML_READONLY_CSTREF_PROPERTY_INIT0 (int, cdiff)

//    QML_OBJMODEL_PROPERTY (OpenOrdersModel, ordersModel)
//    QML_OBJMODEL_PROPERTY (DepthMarketModel, depthModel)





public:

    explicit PlayerQuoteSliceModelItem(pb::PlayerSymbolsModelItem *ppmi,
                                       const QString &insymbol,
                                       const QString &suffix,
                                       int blocknum = 0) :
                            mDepthMarketModel{}
                            ,m_pDepthMarketModel{&mDepthMarketModel}
                            ,m_pPlayerSymbolsModelItem{ppmi}
                            ,QObject(nullptr) {

        m_playerid = m_pPlayerSymbolsModelItem->get_playerid();
        if ( !insymbol.isEmpty())
            m_symbol = insymbol;
        else
            m_symbol = QString("%1%2").arg(m_pPlayerSymbolsModelItem->get_symbol()).arg(suffix);

        m_multiplier = fantasybit::isWeekly(m_symbol) ? 100.0 : 1.0;
        m_lastprice = 0;
        m_blocknum = blocknum;
    }

//    PlayerQuoteSliceModelItem() :  QObject(nullptr) {}
  /*
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

//       m_depthModel = new DepthMarketModel()
    }
    */

//        m_depthModel{&mDepthMarketModel},
//        m_cDepthMarketModel,

//        m_lastprice = in.quote().l();

//        m_pPlayerSymbolsModelItem = in.get_pPlayerSymbolsModelItem();
//        m_pos = in.get_pos();
//        m_firstname = in.get_firstname();
//        m_lastname = in.get_lastname();
//        m_teamid = in.get_teamid();
//        m_fullname =  in.get_fullname();

//        m_bidsize = in.quote().bs();
//        m_bid = in.quote().b();
//        m_ask = in.quote().a();
//        m_asksize = in.quote().as();
//        m_volume = in.ohlc().volume();
//        m_change = in.ohlc().change();
//        m_updown = in.quote().udn();
//        m_hi = in.ohlc().high();
//        m_lo = in.ohlc().low();

//        mDepthMarketModel.append(new DepthMarketModelItem(100,2,30,50));
//        mDepthMarketModel.append(new DepthMarketModelItem(200,1,31,1));

//        m_BackgroundColor = "transparent";
//    }

    /*
    explicit PlayerQuoteSliceModelItem(const QString &symb) :
        m_symbol(symb.data()),
        mDepthMarketModel{},
        m_pDepthMarketModel{&mDepthMarketModel},
        QObject(nullptr) {
        m_multiplier = (m_symbol[m_symbol.length()-1] == 's') ? 1.0 : 100.0;
    }
    */
/*
    void setProperties(const fantasybit::PlayerDetail &in,
                       PlayerSymbolsModelItem *p ,
                       int blocknum) {
        m_pPlayerSymbolsModelItem = p;
        setpos(in.base.position().data());
        setfirstname( in.base.first().data());
        m_lastname = in.base.last().data();
        m_teamid = in.team.data();
        setfullname( QString("%1, %2")
                .arg ( in.base.last().data() )
                .arg ( in.base.first().data()));
        if ( p == nullptr )
            setplayerid(in.symbol.data());
        else
            setplayerid(p->get_playerid());

        if ( m_symbol == "" ) qDebug() << "bad";//should not
        m_lastprice = 0;
        m_blocknum = blocknum;
    }
*/

    void setProperties(PlayerSymbolsModelItem *p ,
                       int blocknum,
                       const QString &insymbol) {
        if ( p == nullptr ) return;

        m_pPlayerSymbolsModelItem = p;
        setplayerid(p->get_playerid());
        m_symbol = insymbol;
        if ( m_symbol == "" ) qDebug() << "bad";//should not
        m_blocknum = blocknum;
        m_multiplier = fantasybit::isWeekly(m_symbol) ? 100.0 : 1.0;
    }

    /*
    explicit PlayerQuoteSliceModelItem(const fantasybit::MarketSnapshot  &ms) :
                        mDepthMarketModel{},
                        m_pDepthMarketModel{&mDepthMarketModel},
                        QObject(nullptr) {

        setplayerid(ms.symbol().data());
        setsymbol(ms.symbol().data());
        m_multiplier = (m_symbol[m_symbol.length()-1] == 's') ? 1.0 : 100.0;

        Update(ms);
    }

    explicit PlayerQuoteSliceModelItem(PlayerProjModelItem* it) :
                        mDepthMarketModel{},
                        m_pDepthMarketModel{&mDepthMarketModel},
                        QObject(nullptr) {

        setplayerid(it->get_playerid());
        setsymbol(it->get_symbol());
        m_multiplier = (m_symbol[m_symbol.length()-1] == 's') ? 1.0 : 100.0;
//        qDebug() << " PlayerQuoteSliceModelItem new " << it->get_playerid();
        Update(it);
    }

    */
    void Update(const MarketSnapshot &rms) {
        setblocknum(rms.blocknum());
        const MarketSnapshot *ms = &rms;
        if ( ms->has_quote()) {
            const MarketQuote &mq = ms->quote();
            setbidsize(mq.bs());
            setbid(mq.b());
            setask(mq.a());
            setasksize(mq.as());
            setlastprice(mq.l());
            setlastsize(mq.ls());
        }

        if ( ms->has_ohlc()) {
            const ContractOHLC &ohlc = ms->ohlc();
            sethi(ohlc.high());
            setlo(ohlc.low());
            setvolume(ohlc.volume());
            setchange(ohlc.change());
        }

        m_pDepthMarketModel->snapDepth(ms);

    }

    void Update(const TradeTic *tt) {
        UpdateLast(tt->price(),tt->size());
        if ( tt->size() > 0 ) {
            setvolume(m_volume + tt->size());
            setvdiff(1);
        }
        if ( tt->ishigh() ) {
            sethi(tt->price());
            if ( !tt->islow())
                sethdiff(1);
        }
        if ( tt->islow() ) {
            setlo(tt->price());
            if ( !tt->ishigh())
                setlodiff(1);
        }

        if ( tt->change() != m_change ) {
            setcdiff( tt->change() > m_change ? 1 : -1);
            setchange(tt->change());
        }

        QTimer::singleShot(3000, this, SLOT(resetTic()));
    }

    void Update(const MarketTicker *mt,int32_t blocknum) {
        setblocknum(blocknum);
        switch ( mt->type() ) {
        case MarketTicker_Type_BID:
            UpdateBid(mt->price(),mt->size());
            break;
        case MarketTicker_Type_ASK:
            UpdateAsk(mt->price(),mt->size());
            break;
        default:
            qWarning() << " base case price slice " << mt->DebugString().data();
            break;
        }

    }

    void UpdateBid(qint32 bid, qint32 size) {
        bool dotime = false;
        if ( bid != get_bid()) {
            dotime = true;
            setbdiff(bid > get_bid() ? 1 : -1);
            setbsdiff(m_bdiff);
            setbid(bid);
            setbidsize(size);
        }
        else if ( size != get_bidsize()) {
            dotime = true;
            setbsdiff(size > get_bidsize() ? 1 : -1 );
            setbidsize(size);
        }

        if ( dotime )
            QTimer::singleShot(3000, this, SLOT(resetBid()));
    }

    void UpdateAsk(qint32 ask, qint32 size) {
        bool dotime = false;
        if ( ask != get_ask()) {
            dotime = true;
            if ( (ask > get_ask() && get_ask() != 0) || ask == 0 )
                setadiff(1);
            else
                setadiff(-1);
            setasdiff(m_adiff);
            setask(ask);
            setasksize(size);
        }
        else if ( size != get_asksize()) {
            dotime = true;
            setasdiff(size > get_asksize() ? -1 : 1 );
            setasksize(size);
        }

        if ( dotime )
            QTimer::singleShot(3000, this, SLOT(resetAsk()));
    }

    void UpdateLast(qint32 last, qint32 size) {
        bool dotime = false;
        if ( last != get_lastprice() ) {
            dotime = true;
            setldiff(last > get_lastprice() ? 1 : -1);
            setlsdiff(m_ldiff);
            setlastprice(last);
            setlastsize(size);
        }
        else {
            dotime = true;
            setlsdiff(2);
            setlastsize(size);
        }

        if ( dotime )
            QTimer::singleShot(3000, this, SLOT(resetLast()));
    }
/*
    void Update(PlayerProjModelItem *it) {
//        qDebug() << " PlayerQuoteSliceModelItem update " << it->get_playerid();

        m_pPlayerProjItem = it;
//        setfirstname(it->get_firstname());
//        setlastname(it->get_lastname());
//        setteamid(it->get_teamid());
//        setpos(it->get_pos());
//        setfullname(it->get_fullname());
    }
*/
public slots:
    void resetBid() {
        setbdiff(0);
        setbsdiff(0);
    }

    void resetAsk() {
        setadiff(0);
        setasdiff(0);
    }

    void resetLast() {
        setldiff(0);
        setlsdiff(0);
    }

    void resetTic() {
        setvdiff(0);
        setcdiff(0);
        sethdiff(0);
        setlodiff(0);
    }

};


class PlayerQuoteSliceModel : public QQmlObjectListModel<PlayerQuoteSliceModelItem>{
    Q_OBJECT

public:
    explicit PlayerQuoteSliceModel (QObject *          parent      = Q_NULLPTR,
                                  const QByteArray & displayRole = QByteArray (),
                                  const QByteArray & uidRole     = {"symbol"})
        : QQmlObjectListModel (parent,displayRole,uidRole) {}


    int initedweek = 0;
    int filledweek = 0;
    void initWeek(int week) {
        if ( week > initedweek ) {
            initedweek = week;
            clear();
        }
    }

    bool filledWeekRoster(int week) {
        if ( week <= filledweek )
            return true;

        initWeek(week);
        filledweek = week;
        return false;
    }

    /*
    void Update(const std::vector<MarketSnapshot> &vms,
                const PlayerProjModel &ppm,const string &suffix = "17s") {
       for ( auto ppmit : ppm ) {
           Update(ppmit,suffix);
       }
       for ( const auto &it : vms) {
           Update(it);
       }
    }
    */

    bool Update(const MarketSnapshot &ms) {
        auto *it = getByUid(ms.symbol().data());
        if ( it == nullptr )
            return false;
        else {
            it->Update(ms);
            return true;
        }
    }

    bool Update(MarketTicker *ms,int32_t blocknum) {
        auto *it = getByUid(ms->symbol().data());
        if ( it == nullptr ) {
            qDebug() << "Update(MarketTicker *ms dont have this symbol" << ms->symbol().data();
            return false;
        }
        else {
            it->Update(ms,blocknum);
            if ( it->get_myposition() != 0 || it->get_myavg() != 0.0)
                emit MyPosPriceChange(it);
        }

        return true;
    }

    bool Update(TradeTic *ms) {
        auto *it = getByUid(ms->symbol().data());
        if ( it == nullptr ) {
            qDebug() << "Update(TradeTic *ms) dont have this symbol" << ms->symbol().data();
            return false;
        }
        else {
            it->Update(ms);
            if ( it->get_myposition() != 0 || it->get_myavg() != 0.0)
                emit MyPosPriceChange(it);
        }
        return true;
    }
    /*
    void Update(PlayerProjModelItem *item, const string &suffix) {
        auto *it = getByUid(QString("%1%2").arg(item->get_symbol()).arg(suffix.data()));
        if ( it == nullptr )
            append(new PlayerQuoteSliceModelItem(*item,suffix));
        else
            it->Update(item);
    }
    */
    bool Update(fantasybit::DepthFeedDelta* dfd) {
        auto *it = getByUid(dfd->symbol().data());
        if ( it == nullptr ) {
            qDebug() << " playquote update Depth delys symbol not found" << dfd->symbol();
            return false;
        }
        else
            it->get_pDepthMarketModel()->Update(dfd);

        return true;
    }

    void UpdateSymbols(PlayerSymbolsModelItem *p, int blocknum = 0, const string &suffix = "17s") {
        QString symb = QString("%1%2").arg(p->get_symbol()).arg(suffix.data());
        auto *item = getByUid(symb);
        if ( !item ) {
            item = new PlayerQuoteSliceModelItem(p,symb,suffix.data(),blocknum);
            this->append(item);
        }
        else
            item->setProperties(p,blocknum,symb);
    }

    void UpdateSymbol(PlayerSymbolsModelItem *p, int blocknum, const string &symbol) {
        QString symb = symbol.data();
        auto *item = getByUid(symb);
        if ( !item ) {
            item = new PlayerQuoteSliceModelItem(p,symb,"",blocknum);
            this->append(item);
        }
        else
            item->setProperties(p,blocknum,symb);
    }

signals:
    void MyPosPriceChange(PlayerQuoteSliceModelItem*);
};

class PlayerQuoteSliceViewFilterProxyModel : public SortFilterProxyModel {
    Q_OBJECT

//    WeeklyScheduleModel  * myGameModelProxy;
//    QItemSelectionModel * mySelectedGames;
//    QStringListModel * myPositionCombobox;
    bool myIsEnabled = true;
    QString myPos = "All";
        QString myTeam = "All";

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

    Q_INVOKABLE void setTeam(const QString &team) {
        if ( team != myTeam ) {
            myTeam = team;
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

    Q_INVOKABLE int getViewIndexFromSymbol(QString  symbol) {
        qDebug() <<"getViewIndexFromSymbol " << symbol;
        PlayerQuoteSliceModel * model = dynamic_cast<PlayerQuoteSliceModel *>(sourceModel());
        auto sindex = model->indexOf (model->getByUid (symbol));
        qDebug() << sindex << mapFromSource (index(sindex,0)).row ();
        return mapFromSource (index(sindex,0)).row ();
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
    const char* LIGHTGREEN = "#c8ffc8";
    const char* LIGHTRED = "#ffc8c8";

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

        if ( myPos != "ALL")
            if ( model->at(sourceRow)->get_pos() != myPos )
                return false;

        if ( myTeam != "ALL")
            if ( model->at(sourceRow)->get_teamid() != myTeam )
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



}

using namespace pb;
Q_DECLARE_METATYPE(PlayerQuoteSliceModel*)
Q_DECLARE_METATYPE(PlayerQuoteSliceModelItem*)

#endif // PLAYERQUOTESLICEMODEL_H







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



//QML_READONLY_CSTREF_PROPERTY_INIT0 (double, ytd)

//QML_READONLY_CSTREF_PROPERTY_INIT0 (int, PassTD)
//QML_READONLY_CSTREF_PROPERTY_INIT0 (int, PassYd)
//QML_READONLY_CSTREF_PROPERTY_INIT0 (int, RushTD)
//QML_READONLY_CSTREF_PROPERTY_INIT0 (int, RushYd)
//QML_READONLY_CSTREF_PROPERTY_INIT0 (int, RecTD)
//QML_READONLY_CSTREF_PROPERTY_INIT0 (int, RecYd)
//QML_READONLY_CSTREF_PROPERTY_INIT0 (int, Rec)
//QML_READONLY_CSTREF_PROPERTY_INIT0 (int, Int)
//QML_READONLY_CSTREF_PROPERTY_INIT0 (int, Fum)
//QML_READONLY_CSTREF_PROPERTY_INIT0 (int, PAT)
//QML_READONLY_CSTREF_PROPERTY_INIT0 (int, _2Pt)
//QML_READONLY_CSTREF_PROPERTY (QString, FG)
//QML_READONLY_CSTREF_PROPERTY_INIT0 (int, D_TD)
//QML_READONLY_CSTREF_PROPERTY_INIT0 (int, Sack)
//QML_READONLY_CSTREF_PROPERTY_INIT0 (int, TA)
//QML_READONLY_CSTREF_PROPERTY_INIT0 (int, SFTY)
//QML_READONLY_CSTREF_PROPERTY_INIT0 (int, D2pt)
