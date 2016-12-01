#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "socketclient.h"
#include <QObject>

#include "QQmlPtrPropertyHelpers.h"
#include "QQmlConstRefPropertyHelpers.h"
#include "QQmlAutoPropertyHelpers.h"
#include "QQmlEnumClassHelper.h"
#include "QMap"
#include "StateData.pb.h"
//#include "FantasyAgent.h"
#include "QQmlListPropertyHelper.h"
#include <QTimer>
#include "fbutils.h"
#include "playerquoteslicemodel.h"
#include "depthmarketmodel.h"
#include "fantasynamemodel.h"
#include "openordersmodel.h"
#include "weeklyschedulemodel.h"
#include <QItemSelectionModel>
#include "playerprojmodel.h"
#include <QStringListModel>
#include <vector>
#include "pbgateways.h"
#include "RestFullCall.h"
#include "playerresultmodel.h"
#include "ExchangeData.h"

using namespace std;

//QML_ENUM_CLASS (nameStatus, none=1, notavil, requested, confirmed )
namespace pb {


class Mediator : public QObject {
    Q_OBJECT

    explicit Mediator(QObject *parent = 0);
    //    QML_READONLY_CSTREF_PROPERTY (QString, namefrompk)
    //    QML_READONLY_CSTREF_PROPERTY (QString, encyptPath)
    //    QML_READONLY_CSTREF_PROPERTY (bool, engineStatus)
    //    QML_READONLY_CSTREF_PROPERTY (QString, currentPidContext)


    //    Q_PROPERTY(QString playersName READ playersName  NOTIFY playersNameChanged)
    //    Q_PROPERTY(QString  playersStatus READ playersStatus  NOTIFY playersStatusChanged)

    //    Q_PROPERTY(MyNameStatus myNameStatus READ myNameStatus NOTIFY myNameStatusChanged)
    //    Q_ENUMS (MyNameStatus)

    //    QML_CONSTANT_CSTREF_PROPERTY (QString, chatServerAddr)

    QML_READONLY_PTR_PROPERTY(FantasyNameBalModel, pFantasyNameBalModel)


    //Trading
    QML_READONLY_PTR_PROPERTY(PlayerQuoteSliceModelItem, pPlayerQuoteSliceModelItem)
//    QML_READONLY_PTR_PROPERTY(PlayerQuoteSliceModel, pPlayerQuoteSliceModel)
    QML_READONLY_PTR_PROPERTY(PlayerQuoteSliceViewFilterProxyModel, pPlayerQuoteSliceViewFilterProxyModel)
//    QML_READONLY_PTR_PROPERTY(QStringListModel, pPrevPosFilter)

    QML_READONLY_PTR_PROPERTY(DepthMarketModel, pDepthMarketModel)
    QML_READONLY_PTR_PROPERTY(OpenOrdersModel, pGlobalOpenOrdersModel)
//    //QML_WRITABLE_PTR_PROPERTY(TradingPositionsModel, pTradingPositionsModel)
    QML_READONLY_PTR_PROPERTY(TradingPositionsModel, pTradingPositionsModel)
//    std::unordered_map<std::string,TradingPositionsModel *> modelMap;



    //fantasyname
    QML_READONLY_PTR_PROPERTY(FantasyNameBalModel, pGoodNameBalModel)
    QML_READONLY_PTR_PROPERTY(FantasyNameBalModelItem, pMyFantasyNameBalance)


    //leaderboard
    QML_READONLY_PTR_PROPERTY(SortFilterProxyModel, pLeaderBoardSortModel)


    //schedule
    QML_READONLY_PTR_PROPERTY(WeeklyScheduleModel, pWeeklyScheduleModel)
    QML_READONLY_PTR_PROPERTY(QItemSelectionModel, pQItemSelectionModel)
    QML_READONLY_PTR_PROPERTY(WeeklyScheduleModel, pNextWeekScheduleModel)
    QML_READONLY_CSTREF_PROPERTY (qint32, theNextWeek)

    //projections
    QML_READONLY_PTR_PROPERTY(ProjectionsViewFilterProxyModel, pProjectionsViewFilterProxyModel)
    QML_READONLY_PTR_PROPERTY(QStringListModel, pPosFilter)
    QML_READONLY_CSTREF_PROPERTY (QString, gameFilter)

    QML_READONLY_CSTREF_PROPERTY (qint32, theWeek)
    QML_READONLY_CSTREF_PROPERTY (qint32, season)
    QML_READONLY_CSTREF_PROPERTY (QString, liveSync)
    QML_READONLY_CSTREF_PROPERTY (QString, seasonString)
    QML_READONLY_CSTREF_PROPERTY (QString, prevSelectedPlayerDisplay)

    QML_WRITABLE_CSTREF_PROPERTY(bool,useSelected)
    QML_WRITABLE_CSTREF_PROPERTY(bool,thisWeekPrev)

    QML_READONLY_CSTREF_PROPERTY (qint32, height)
    QML_READONLY_CSTREF_PROPERTY (qint32, blocknum)


    QML_READONLY_CSTREF_PROPERTY (QString, controlMessage)
    QML_WRITABLE_CSTREF_PROPERTY(bool,busySend)

    //    QML_READONLY_PTR_PROPERTY(PlayerQuoteSliceModelItem, pPlayerQuoteSliceModel)
    //    QML_LIST_PROPERTY(Mediator,goodFname,QString)
    //    QML_READONLY_CSTREF_PROPERTY (QStringList, allNames2)
    //    Q_PROPERTY(QQmlListProperty<QString> goodFnames READ goodFnames NOTIFY goodFnamesChanged)



    PlayerProjModel mPlayerProjModel;
    QItemSelectionModel myGamesSelectionModel;
    pb::IPBGateway *mGateway = nullptr;
    QObject *mOGateway;
    void setupConnection(pb::IPBGateway *ingateway);

    //previous week stuff
    QML_READONLY_CSTREF_PROPERTY (qint32, thePrevWeek)

    PlayerResultModel mPlayerResultModel;
    QItemSelectionModel myPrevGamesSelectionModel;

        //schedule
    QML_READONLY_PTR_PROPERTY(WeeklyScheduleModel, pWeekClosedScheduleModel)
    QML_READONLY_PTR_PROPERTY(WeeklyScheduleModel, pPreviousWeekScheduleModel)
    QML_READONLY_PTR_PROPERTY(QItemSelectionModel, pPrevQItemSelectionModel)

        //results
    QML_READONLY_PTR_PROPERTY(ResultsViewFilterProxyModel, pResultsViewFilterProxyModel)
    QML_READONLY_PTR_PROPERTY(QStringListModel, pPrevPosFilter)

    //awards
    QML_READONLY_PTR_PROPERTY(SortFilterProxyModel, pResultSelectedModel)
//    QML_READONLY_PTR_PROPERTY (QQmlObjectListModel<FantasyBitAwardModelItem>, pResultSelectedModel)
    QQmlObjectListModel<FantasyBitAwardModelItem> dummyResultSelectedModel;

    //trading
    PlayerQuoteSliceModelItem dummyPlayerQuoteSliceModelItem;
    OpenOrdersModel dummyOpenOrdersModel;
    FantasyNameBalModelItem dummyFantasyNameBalModelItem;

    static Mediator *myInstance;
public:
    static Mediator *instance();

    void CopyTheseProjections(std::vector<fantasybit::PlayerPoints> &these) {
        for ( auto t : these) {
            auto *item = mPlayerProjModel.getByUid(t.playerid().data());
            if ( !item ) continue;
            if ( !item->get_isopen() ) continue;
            if ( t.points() <= 0 ) continue;
            item->set_projection(t.points());
        }
        m_pProjectionsViewFilterProxyModel->invalidate();
    }

    void setContext(pb::IPBGateway *ingateway) {
        mGateway = ingateway;
        setupConnection(mGateway);
    }

    Q_INVOKABLE QString init();

    Q_INVOKABLE bool isTestNet() { return fantasybit::IS_TEST_NET; }

    QStringList m_goodList;
    QStringList m_allNamesList;
    QStringList m_allROWList;

//    Q_INVOKABLE void doDepth() {
//        depthCount = 0;
//        if ( polldepth.interval() >= 3000 )
//            getDepthRep();
//    }

    Q_INVOKABLE void startDepth(const QString& symbol) {
        qDebug() << " startDepth " << symbol;
        auto *it = mPlayerQuoteSliceModel.getByUid(symbol);
        if ( it != nullptr ) {
            qDebug() << " startDepth good" << symbol;

//          m_pPlayerQuoteSliceModelItem = it;
            update_pPlayerQuoteSliceModelItem(it);
        }

        auto tit = mTradingPositionsModel.getOrCreate(symbol);
        if ( tit != nullptr ) {
            update_pGlobalOpenOrdersModel(tit->get_pOpenOrdersModel());
        }
        else {
            qDebug() << " startDepth !good getOrCreate" << symbol;
            update_pGlobalOpenOrdersModel(&dummyOpenOrdersModel);
        }
//        depthBackup--;
//        if ( depthBackup <= 0 ) {
//            depthBackup = 0;
////            depthInterval = 1000;
//        }
////        else
////            depthInterval = 1000 * (depthBackup / 5);

//        depthCount = 0;
////        depthInterval = 1000 * (depthBackup / 5);
////        if ( depthInterval < 1000 )
////           depthInterval = 1000;

//        changeDepthContext(symbol);
//        getDepthRep();
////        qDebug() << "startDepth depthInterval " << depthInterval << " bu " << depthBackup;

//        if ( !polldepth.isActive() )
//            polldepth.start(depthInterval);
//        11
//        getOrderReq(FantasyName::name_hash(m_fantasy_agent.currentClient()));
//        getOrderPos();
    }

//    Q_INVOKABLE void stopDepth(const QString& symbol) {
//        polldepth.stop();
//        depthBackup -= 5;
//        if ( depthBackup < 0 ) depthBackup = 0;
//        depthCount = 0;
//    }
//    Q_INVOKABLE void changeDepthContext(const QString& context) {
//        if ( mGetDepthReq.GetExtension(GetDepthReq::req).pid().data() != context )
//            mGetDepthReq.MutableExtension(GetDepthReq::req)->set_pid(context.toStdString());

////            m_currentPidContext = context;
//    }
    Q_INVOKABLE void doCancel(qint32 id);
    Q_INVOKABLE void doTrade(QString symbol, bool isbuy, const qint32 price, qint32 size);

//    QString playersStatus()const;
//    void setPlayersStatus(const QString &playersStatus);

//    QString playersName();
//    void setPlayersName(const QString &playersName);

    // Q_INVOKABLE void newOrder(const QString &id, int qty, int price);


//    Q_INVOKABLE void allNamesGet();

    /*trading
    Q_INVOKABLE void rowMarketGet();
    Q_INVOKABLE void getOrderPos(const QString&);
    Q_INVOKABLE void getOrderPos();
    Q_INVOKABLE void setOrderModel(const QString& symbol) {
        if  ( amLive )
            return;

        auto model = m_pTradingPositionsModel->getByUid(symbol);
        if ( model == nullptr ) {
//            auto model2 = m_pPlayerQuoteSliceModel->getByUid(uid);
//            if ( model2 != nullptr) {
//                AllOdersFname allf{};
//                allf.set_fname(m_fantasy_agent);
//                m_pTradingPositionsModel->append(new TradingPositionsMode);
//            }
            qDebug() << " bad data for m_pTradingPositionsModel " << symbol;
        }
        else
            m_pGlobalOpenOrdersModel = model->get_pOpenOrdersModel();
    }
    */
    Q_INVOKABLE QString getOrderModelSymbol() {
        return m_pGlobalOpenOrdersModel->get_pidsymbol();
    }

    //projections
    Q_INVOKABLE void select(int row, int command) {
        qDebug() << " meiator selected" << row << " commsnd " << command;
        m_pQItemSelectionModel->select(m_pWeeklyScheduleModel->index(row),QItemSelectionModel::Select);
    }

    Q_INVOKABLE void toggle(int row, int command) {
        qDebug() << " meiator selected" << row << " commsnd " << command;
        m_pQItemSelectionModel->select(m_pWeeklyScheduleModel->index(row),QItemSelectionModel::Toggle);
    }

    Q_INVOKABLE void deselect(int row, int command) {
        qDebug() << " meiator deselected" << row << " commsnd " << command;
        m_pQItemSelectionModel->select(m_pWeeklyScheduleModel->index(row),QItemSelectionModel::Deselect);
    }

    Q_INVOKABLE void undoProj() {
        for ( auto it : mPlayerProjModel) {
            int projection = it->get_projection();
            int knownprojection = it->get_knownProjection();
            if ( knownprojection == projection)
                continue;

            it->set_projection(it->get_knownProjection());
        }
        m_pProjectionsViewFilterProxyModel->invalidate();
        set_busySend(false);
    }

    Q_INVOKABLE void sendProjections() {
        std::unordered_map<string,vector<FantasyBitProj>> projbygame{};
        for ( auto it : mPlayerProjModel) {
            int projection = it->get_projection();
            if (projection == 0)
                continue;

            auto gameid = it->get_gameid();

            if ( !it->get_isopen()) continue;

            int knownprojection = it->get_knownProjection();
            if ( knownprojection == projection)
                continue;

            vector<FantasyBitProj> &vproj = projbygame[gameid.toStdString()];

            FantasyBitProj fproj;
            fproj.set_name(myFantasyName);
            fproj.set_proj(projection);
            fproj.set_playerid(it->get_playerid().toStdString());
            vproj.push_back(fproj);

        }

        if ( projbygame.size() > 0 )
            set_busySend(true);

        for ( auto &vg : projbygame)
            emit NewProjection(vg.second);
    }

    std::unordered_map<int,std::string> coltorole;
    Q_INVOKABLE QString addFnameColumn(QString fname,int col) {
//        mPlayerProjModel.AddColumn(fname.toUtf8());
//        m_pProjectionsViewFilterProxyModel->ret.append(fname);
//        qDebug() << " ret " << m_pProjectionsViewFilterProxyModel->ret;
//        m_pProjectionsViewFilterProxyModel->ret.append("pos");
//        m_pProjectionsViewFilterProxyModel->invalidate();
//        m_pProjectionsViewFilterProxyModel->insertColumn(5);
//        for (int i =0;i<5;i++)
//            if ( fnametorole)
        QString ret = fnames[fnameindex].data();
        if ( ++fnameindex >= 5 )
            fnameindex = 0;

        coltorole[col] = ret.toStdString();
        toggleFantasyNameColumn(fname,ret);
        return ret;
    }

    Q_INVOKABLE bool isMyName(QString fname) {
        return m_pGoodNameBalModel->getByUid(fname) != nullptr;
    }


    void toggleFantasyNameColumn(const QString & fantasyName, QString &column){
        //refresheing is done when leaderboard model refresh
//        m_pProjectionsViewFilterProxyModel->beginResetModel();
        std::unordered_map<std::string,int> theOtherGuyProjection = mGateway->dataService->GetProjByName(fantasyName.toStdString());
        qDebug() << "toggleFantasyNameColumn " << fantasyName << column << theOtherGuyProjection.size();

        //        bool(PlayerProjModelItem::*fnamefunc)(int &)
        for ( auto it = theOtherGuyProjection.begin(); it != theOtherGuyProjection.end(); ++it ){
            auto *item = mPlayerProjModel.getByUid(it->first.data());
            if ( !item ) continue;
            if ( column == "fname1")
                item->set_fname1(it->second);
            else if ( column == "fname2")
                item->set_fname2(it->second);
            else if ( column == "fname3")
                item->set_fname3(it->second);
            else if ( column == "fname4")
                item->set_fname4(it->second);
            else if ( column == "fname5")
                item->set_fname5(it->second);
        }
        m_pProjectionsViewFilterProxyModel->invalidate();
//        m_pProjectionsViewFilterProxyModel->endResetModel();


    }

    Q_INVOKABLE void copyProj(int column, QString value, bool clone, bool random = false) {
        qDebug() << "CopyProj " << column << value << clone;

        if ( value == "Average") {
            for ( auto *it : mPlayerProjModel) {
                if ( !it->get_isopen() )
                    continue;
                if ( m_useSelected && m_pQItemSelectionModel->hasSelection()) {
                    auto gameid = it->get_gameid();
                    int i = m_pWeeklyScheduleModel->indexOf(m_pWeeklyScheduleModel->getByUid(gameid));
                    if ( !m_pQItemSelectionModel->isSelected(m_pWeeklyScheduleModel->index(i)) )
                        continue;
                }
                if ( it->get_avg() > 0 )
                    if ( clone || it->get_projection () == 0)
                        if  ( it->get_projection() != it->get_avg())
                            it->set_projection(it->get_avg());

            }
            m_pProjectionsViewFilterProxyModel->invalidate();
            return;
        }

        std::unordered_map<std::string,int> theOtherGuyProjection = mGateway->dataService->GetProjByName(value.toStdString());
//        qDebug() << "toggleFantasyNameColumn " << fantasyName << column << theOtherGuyProjection.size();

        //        bool(PlayerProjModelItem::*fnamefunc)(int &)
        for ( auto it = theOtherGuyProjection.begin(); it != theOtherGuyProjection.end(); ++it ){
            auto *item = mPlayerProjModel.getByUid(it->first.data());
            if ( !item ) continue;

            if ( !item->get_isopen() )
                continue;

            if ( it->second == 0 ) continue;
            if ( clone || item->get_projection() == 0 ) {

                 if ( m_useSelected && m_pQItemSelectionModel->hasSelection()) {
                    auto gameid = item->get_gameid();
                    int i = m_pWeeklyScheduleModel->indexOf(m_pWeeklyScheduleModel->getByUid(gameid));
                    if ( !m_pQItemSelectionModel->isSelected(m_pWeeklyScheduleModel->index(i)) )
                        continue;
                }

                int projection = it->second;
                if ( random ) {
                     int num = qrand() % ((200 + 1) - 1) + 1;
                     double percent = num / 100.0;
                     projection = (double)projection * percent;
                 }

                 if  ( item->get_projection() != projection)
                    item->set_projection(projection);

                 qDebug() << " set " << it->first.data() << " to " << projection;
            }

        }
        m_pProjectionsViewFilterProxyModel->invalidate();

    }

//    Q_INVOKABLE void randomUseNames() {
//        for ( auto *it : mGoodNameBalModel) {
//            useName(it->get_name());
//            copyProj(6,"@SpreadSheetFF",false,true);
//            sendProjections();
//        }
//    }

    //results
    Q_INVOKABLE void selectP(int row, int command) {
        qDebug() << " meiator selectedP" << row << " commsnd " << command;
        m_pPrevQItemSelectionModel->select(m_pPreviousWeekScheduleModel->index(row),QItemSelectionModel::Select);
    }

    Q_INVOKABLE void toggleP(int row, int command) {
        qDebug() << " meiator toggleP" << row << " commsnd " << command;
        m_pPrevQItemSelectionModel->select(m_pPreviousWeekScheduleModel->index(row),QItemSelectionModel::Toggle);
    }

    Q_INVOKABLE void deselectP(int row, int command) {
        qDebug() << " meiator deselectedP" << row << " commsnd " << command;
        m_pPrevQItemSelectionModel->select(m_pPreviousWeekScheduleModel->index(row),QItemSelectionModel::Deselect);
    }

    Q_INVOKABLE void setNextWeekData(int week) {
        if ( !amLive ) return;

        if ( week <= 0 || week >= 17 )
            return;

        if ( week < m_theWeek)
            return;

        if ( week == m_theNextWeek)
            return;

        m_pNextWeekScheduleModel->clear();
        settheNextWeek(week);
        m_pNextWeekScheduleModel->updateWeeklySchedule(week,
                                  mGateway->dataService->GetWeeklySchedule(week));
    }

    Q_INVOKABLE void setPrevWeekData(int week) {
        setprevSelectedPlayerDisplay("");
        if ( !amLive ) return;

        if ( week <= 0 || week >= 17 )
            return;

        if ( week > m_theWeek)
            return;

        if ( week == m_theWeek && !m_thisWeekPrev )
            return;

        if ( week == m_thePrevWeek && !(week == m_theWeek && m_thisWeekPrev ))
            return;

        if ( week == m_thePrevWeek )
            emit thePrevWeekChanged(m_thePrevWeek);
        else
            setthePrevWeek(week);

        if ( week == m_theWeek) {
           m_pPreviousWeekScheduleModel->clear();
//           for ( auto *it : m_pWeekClosedScheduleModel->)
           m_pPreviousWeekScheduleModel->append(m_pWeekClosedScheduleModel->toList());
        }
        else
            m_pPreviousWeekScheduleModel->updateWeeklySchedule(m_thePrevWeek,
                                      mGateway->dataService->GetWeeklySchedule(m_thePrevWeek));


        const auto &vgr = mGateway->dataService->GetPrevWeekGameResults(m_thePrevWeek);
        mPlayerResultModel.updateRosters(vgr,mGateway->dataService,*m_pPreviousWeekScheduleModel,myFantasyName);
        myPrevGamesSelectionModel.reset();
        m_pResultSelectedModel->setSourceModel(&dummyResultSelectedModel);
        m_pResultSelectedModel->clear();
        m_pResultsViewFilterProxyModel->invalidate();
    }

    Q_INVOKABLE void setPrevWeekResultLeaderSelection(QString in) {
        qDebug() << " setPrevWeekResultLeaderSelection " << in;
        if ( !amLive ) return;

        auto it = mPlayerResultModel.getByUid(in);
        if ( it ) {
            //update_pResultSelectedModel(it->get_awardsModel());

            m_pResultSelectedModel->setSourceModel(it->get_awardsModel());
//            m_pResultSelectedModel->setSortRole("award");//mPlayerProjModel.roleForName("pos"));
//            m_pResultSelectedModel->setDynamicSortFilter(true);
//            m_pResultSelectedModel->setSource();

            QString fordisply("%1 (%2) %3 - %4 ƑɃ");
            QString fd2 = fordisply.arg(it->get_fullname()).arg(it->get_pos()).arg(it->get_teamid())
                    .arg(it->get_fb());
            setprevSelectedPlayerDisplay(fordisply);
        }
    }

    
    bool usingRandomNames = false;

    std::vector<std::string> fnames;
    int fnameindex;

    //fantasy name - manage
//    Q_INVOKABLE void pk2fname(const QString&);
    Q_INVOKABLE void checkname(QString s) {
        qDebug() << " inside checkname" << s;
        emit doNameCheck(s);
    }

    Q_INVOKABLE QString importMnemonic(const QString &importStr);
    Q_INVOKABLE void signPlayer(const QString &name);
    Q_INVOKABLE void useName(const QString &name);

    Q_INVOKABLE QString getSecret();
    Q_INVOKABLE QStringList goodList() { return m_goodList; }
    Q_INVOKABLE QStringList allNamesList() { return m_allNamesList; }

    //portfolio
    Q_INVOKABLE QString getPlayerNamePos(const QString &uid) {
        auto model = mPlayerQuoteSliceModel.getByUid(uid);
        if ( model == nullptr ) {
            qDebug() << " bad data for getPlayerNamePos " << uid;
            return "";
        }
        else
            return model->get_fullname() + " (" + model->get_pos() +")" ;
    }


    //data
    Q_INVOKABLE QString getTeamid(const QString &uid) {
        auto model = mPlayerQuoteSliceModel.getByUid(uid);
        if ( model == nullptr ) {
            qDebug() << " bad data for getTeamid " << uid;
            return "";
        }
        else
            return model->get_teamid();
    }

    //schedule
    Q_INVOKABLE void setScheduleFilter(const QString& filter) {
        setgameFilter(filter);
        //m_pWeeklyScheduleModel->clear();
    }


    Q_INVOKABLE void settheHeight(int h) {
        qDebug() << " new height " << h;
        if ( h > m_height )
            emit NewHeightStop(h);
    }
    //oms
    void subscribeOrderPos(const QString &name);
    void getOrderReq(const QString &name,const QString symbol="");

    //name
    void OnGoodName(const QString &goodname, const fantasybit::FantasyNameBal &fnb);

    void updateCurrentFantasyPlayerProjections();
signals:
    void importSuccess(const QString name, bool passfail);
    void usingFantasyName(const QString &name);
    void nameCheckGet( const QString & name, const QString & status );
    //    void myNameChang (const QString & name, QString status );
    //    void myNameStatusChanged();
    //    void nameStatusChanged (QString, QString);

    void OnClaimName(QString name);

    void OnUseName(QString); //tell agent to use the name
    void ready();
    /*
    void playersNameChanged();
    void playersStatusChanged();
    void leaderBoardchanged();
    void portfolioChanged();
    */

    void doNameCheck(QString);
    void NewProjection(vector<fantasybit::FantasyBitProj>);
    void NewOrder(fantasybit::ExchangeOrder);

    void NewHeightStop(int);

protected slots:
//    void handdleUsingName(const QString &name);
//    void handdleNameStatus(const QString &name,const QString &status );
//    void handdleNameStatuses();

    void nameAvail(QString &s, bool tf) {
        qDebug() << " in side name availa" << s << tf;
       emit nameCheckGet(s, tf ?  "true" : "false");
    }



//    void nameAvaila()
    //quotes
//    void getDepthRep();

    //name
    void getSignedPlayerStatus();

    //trade
//    void doTestTrade();

    // slot to update QML ONLY propertys
    //void handleEngineUpdate(const bool &sta);

    //projections
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
        qDebug() << " mediator selectionChanged " << selected << deselected;
        m_pProjectionsViewFilterProxyModel->invalidate();
    }

    void selectionPrevChanged(const QItemSelection &selected, const QItemSelection &deselected) {
        qDebug() << " mediator selectionPrevChanged " << selected << deselected;
        m_pResultsViewFilterProxyModel->invalidate();
    }

private slots:
//    void OnpPlayerQuoteSliceModelItemChanged (PlayerQuoteSliceModelItem * name);

private:
    std::string lastPk2name;
    //fantasybit::FantasyAgent m_fantasy_agent;
    std::string myFantasyName;

    QString m_playersName;
    QString m_playersStatus;
    std::string m_lastSignedplayer;

    ordsnap_t mMyPosOrders;
    std::unordered_map<std::string, std::string> m_myPubkeyFname;
    std::unordered_map<std::string, uint64_t> m_myPubkeyHash;
    std::unordered_map<uint64_t, std::string> m_myHashFname;

    void doPk2fname(const std::string &pkstr);

    //timers
    QTimer signPlayerStatus;
    QTimer polldepth;
    QTimer tradeTesting;

    //    WeeklyScheduleModel mWeeklyScheduleModel;

    //quotes
    PlayerQuoteSliceModel mPlayerQuoteSliceModel;
    //depth
    DepthMarketModel mDepthMarketModel;

    //fnamebal
    FantasyNameBalModel mFantasyNameBalModel, mGoodNameBalModel;

    //oms
    OpenOrdersModel mOpenOrdersModel;

    //depth todo
    QString testid;
//    WsReq mGetDepthReq;
    bool isbid;
    //    void getOrderReq(uint64_t cname);
    int depthCount;
    int depthBackup;
    int depthInterval;
    TradingPositionsModel mTradingPositionsModel;

    bool amLive = false;

    void getLeaders(int week,bool lastweek, bool all2016 = false) {
#ifdef NO_SQL_LEADERS
        return;
#endif
        QString links("https://158.222.102.83:4545");
        QString route("fantasy/leaders");
        if ( !all2016 )
            route = route.append("?position=all%20positions&week=%1").arg(week);


        qDebug() << " calling route " << route;

        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;

        QUrl url;
        url.setUrl(links);
        RestfullClient rest (url);
        rest.getData(route);
        auto resp = rest.lastReply();

#ifdef TRACE2
        qDebug() << resp;
#endif
        QJsonDocument ret = QJsonDocument::fromJson(resp);
        qDebug() << ret.isNull() << ret.isEmpty() << ret.isArray() << ret.isObject();

        QJsonArray parr = ret.array();
        for (int i=0;i< parr.size();i++  ) {
            QJsonValueRef data = parr[i];

            QJsonObject leaderData = data.toObject();
            auto name = leaderData.value("name").toString();
            auto scoreStr = leaderData.value("score").toString();
            double score = std::stod(scoreStr.toStdString());

            auto *item = mFantasyNameBalModel.getByUid(name);
            if ( item == nullptr ) continue;
            if ( all2016 )
                item->set_leaders2016(score);
            else if ( lastweek )
                item->set_lastweek(score);
            else
                item->set_thisweek(score);
        }
    }

    void updateLiveLeaders(bool getLastWeek = true) {
        mFantasyNameBalModel.updateleaders(mGateway->dataService->GetLeaderBoard());

        if ( m_theWeek-1 > 0 && getLastWeek)
            getLeaders(m_theWeek-1,true,false);
        getLeaders(m_theWeek,false,false);
        getLeaders(0,false,true);
        m_pLeaderBoardSortModel->invalidate();
    }

public slots:
    void NameStatus(fantasybit::MyFantasyName);
    void LiveProj(fantasybit::FantasyBitProj);
    void MyNames(vector<fantasybit::MyFantasyName>);
    void NameBal(fantasybit::FantasyNameBal);
    void PlayerStatusChange(pair<string,fantasybit::PlayerStatus> in);
    void GlobalStateChange(fantasybit::GlobalState);
    void LiveGui(fantasybit::GlobalState);
    void NewWeek(int);
    void GameStart(string);
    void GameOver(string);
    void onControlMessage(QString);
    void NewFantasyName(fantasybit::FantasyNameBal fnb) {
        qDebug() << "NewFantasyName " << fnb.DebugString().data();
        auto it = mFantasyNameBalModel.getByUid(fnb.name().data());
        if ( it == nullptr) {
            auto *item = new FantasyNameBalModelItem(fnb);
            item->set_lastupdate(get_blocknum());
            mFantasyNameBalModel.append(item);
            m_pLeaderBoardSortModel->invalidate();
        }
    }

    void AnyFantasyNameBalance(fantasybit::FantasyNameBal fnb) {
        auto *item = mFantasyNameBalModel.getByUid(fnb.name().data());
        if ( item == nullptr ) return;
        item->update(fnb);
        mFantasyNameBalModel.update(item);
    }

    void Height(int h) {
        qDebug() << " height " << h;
        setheight(h);
    }

    void BlockNum(int n) {
        qDebug() << " BlockNum " << n;
        setblocknum(n);
    }

    void OnFinishedResults() {
        if ( amLive ) {
            updateLiveLeaders(false);

            vector<pair<std::string,WeeklyScheduleModelItem *>> todo;
            for ( const auto item : *m_pWeeklyScheduleModel) {
                std::string id = item->get_gameid().toStdString();
                auto status = mGateway->dataService->GetGameStatus(id);
                if ( status.status() != GameStatus_Status_CLOSED )
                    continue;

                todo.push_back({id,item});
            }

            for ( auto ip : todo ) {
                mPlayerProjModel.ongameStatusChange(ip.first,GameStatus_Status_CLOSED);
                m_pWeekClosedScheduleModel->append(new WeeklyScheduleModelItem(ip.second,GameStatus_Status_CLOSED));
                m_pWeeklyScheduleModel->remove(ip.second);
            }

            if ( todo.size() > 0 ) {
                if ( !m_thisWeekPrev)
                    set_thisWeekPrev(true);

                m_pProjectionsViewFilterProxyModel->invalidate();
            }
        }
    }
    void OnMarketTicker(fantasybit::MarketTicker,int32_t);
//    void OnMarketSnapShot(fantasybit::MarketSnapshot*);
    void OnDepthDelta(fantasybit::DepthFeedDelta*);
    void OnTradeTick(fantasybit::TradeTic*);
    void OnNewPos(fantasybit::FullPosition);
    void OnNewOO(fantasybit::FullOrderDelta);
    void MyPosPriceChange(PlayerQuoteSliceModelItem*);

    /*
    void OnMarketTicker(fantasybit::MarketTicker *mt) {
        if ( mt->symbol() == "" )
            return;
    #ifdef TRACE
        qDebug() << "Mediator OnMarketTicker " << mt->DebugString().data();
    #endif

        if ( !m_pPlayerQuoteSliceModel->Update(mt) ) {
            qDebug() << "mediato OnMarketTicker  bad";
            auto *item = mPlayerProjModel.getByUid(mt->symbol().data());
            if ( item == nullptr )
                qDebug() << "nullptr mediato OnMarketTicker  bad again" << mt->symbol().data();
            else {
                m_pPlayerQuoteSliceModel->append(new PlayerQuoteSliceModelItem(item));
                if ( !m_pPlayerQuoteSliceModel->Update(mt) )
                    qDebug() << "mediato OnMarketTicker  bad again";
                else
                    qDebug() << "mediato OnMarketTicker  good again";
            }
        }
        else
            qDebug() << " mediato OnMarketTicker good";

    }

    void tradeTestingTimeout() {
        if ( testCount++ >= 5) // m_pPlayerQuoteSliceModel->count())
            testCount = 0;
        auto it = m_pPlayerQuoteSliceModel->at(testCount);
//        if (it->get_lastprice() > 5) testCount++;
//        it->setlastprice();
//        it->LastNew(it->get_lastprice() > 1 ? it->get_lastprice()-1 : it->get_lastprice()+1);
    }
*/

/*
    void OnGotMarketSnaps() {
#ifdef TRACE
        qDebug() << " OnGotMarketSnaps " << m_theWeek;
#endif
        if ( m_pPlayerQuoteSliceModel->filledWeekRoster(m_theWeek) )
            return;


#ifdef TRACE
        qDebug() << " OnGotMarketSnaps loop " << mPlayerProjModel.size();
#endif
//        for ( auto it : mPlayerProjModel ) {
//           m_pPlayerQuoteSliceModel->Update(it);
//        }

        for ( auto it : *m_pPlayerQuoteSliceModel ) {
            auto *item = mPlayerProjModel.getByUid(it->get_symbol());
            if ( item == nullptr ) {
                qDebug() << "mediator OnGotMarketSnaps failed to get symbol " << it->get_symbol();
                m_pPlayerQuoteSliceModel->remove(it);
            }
            else
                it->Update(item);
        }

        m_pPlayerQuoteSliceViewFilterProxyModel->invalidate();

    }
*/


private:
    vector<MyFantasyName> myGoodNames;
    void updateWeek();
    int testCount = 0;
    void updateOnChangeFantasyName();
    void updateCurrentFantasyPlayerOrderPositions();
};
}
#endif // MEDIATOR_H




/*

int netqty = p.second.first.netqty;
double avg = 0;
double pnl = 0;
if ( netqty ==0 ) {
    pnl = p.second.first.netprice * 100;
}
else  {
    ViewModel * item = mPlayerListModel.itemByKey(p.first.data());
    int bid = item->propertyValue<PropertyNames::BID>().toInt();
    int ask = item->propertyValue<PropertyNames::ASK>().toInt();
    int price = (netqty > 0) ? bid :  ask;

    if ( bid == 0 && ask == 0 )
        pnl = 0;
    else
        pnl = 100 * ((price * netqty) + p.second.first.netprice);

}

mPlayerListModel.updateItemProperty<PropertyNames::MYPOS>(p.first.data(),netqty);
mPlayerListModel.updateItemProperty<PropertyNames::MYAVG>(p.first.data(),avg);
mPlayerListModel.updateItemProperty<PropertyNames::MYPNL>(p.first.data(),pnl);

if ( p.first == myPlayerid) {
    ui->posQty->setValue(netqty);
    ui->posAvgPrice->setValue(avg);
    ui->posOpenPnl->setValue(pnl);
}

totpnl += pnl;

}

ui->fantasybitPnl->setValue(ui->fantasybitPnl->value()+totpnl);

}*/
