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
#ifdef PLAYERLOADERFD
#include "../../fantasybit-2015/tradingfootball/playerloader.h"
#endif

using namespace std;

//QML_ENUM_CLASS (nameStatus, none=1, notavil, requested, confirmed )
namespace pb {


class Mediator : public QObject {
    Q_OBJECT

    explicit Mediator(QObject *parent = 0);
    //trading


    //quotes
    PlayerQuoteSliceModel mPlayerQuoteSliceModel;
    PlayerQuoteSliceModel mROWPlayerQuoteSliceModel;

    //depth
    DepthMarketModel mDepthMarketModel;
    DepthMarketModel mROWDepthMarketModel;

    //positions
    TradingPositionsModel mTradingPositionsModel;
    TradingPositionsModel mROWTradingPositionsModel;

    //fnamebal
    FantasyNameBalModel mFantasyNameBalModel, mGoodNameBalModel;
    PlayerSymbolsModel mPlayerSymbolsModel;

    PlayerSymbolsModelItem dummyPlayerSymbolsModelItem;
    PlayerQuoteSliceModelItem dummyPlayerQuoteSliceModelItem;
    OpenOrdersModel dummyOpenOrdersModel;
    FantasyNameBalModelItem dummyFantasyNameBalModelItem;

    PlayerProjModel mPlayerProjModel;
    QItemSelectionModel myGamesSelectionModel;
    PlayerResultModel mPlayerResultModel;
    QItemSelectionModel myPrevGamesSelectionModel;


    QML_READONLY_PTR_PROPERTY(SortFilterProxyModel, pPlayerSymbolsModel)

    QML_READONLY_PTR_PROPERTY(FantasyNameBalModel, pFantasyNameBalModel)

    //Trading
    QML_READONLY_PTR_PROPERTY(PlayerQuoteSliceModelItem, pPlayerQuoteSliceModelItem)
    QML_READONLY_PTR_PROPERTY(PlayerQuoteSliceModelItem, pROWPlayerQuoteSliceModelItem)

    QML_READONLY_PTR_PROPERTY(PlayerQuoteSliceViewFilterProxyModel, pPlayerQuoteSliceViewFilterProxyModel)
    QML_READONLY_PTR_PROPERTY(PlayerQuoteSliceViewFilterProxyModel, pROWPlayerQuoteSliceViewFilterProxyModel)

    QML_READONLY_PTR_PROPERTY(DepthMarketModel, pDepthMarketModel)
    QML_READONLY_PTR_PROPERTY(DepthMarketModel, pROWDepthMarketModel)

    QML_READONLY_PTR_PROPERTY(OpenOrdersModel, pGlobalOpenOrdersModel)
    QML_READONLY_PTR_PROPERTY(OpenOrdersModel, pROWGlobalOpenOrdersModel)

    QML_READONLY_PTR_PROPERTY(TradingPositionsModel, pTradingPositionsModel)
    QML_READONLY_PTR_PROPERTY(TradingPositionsModel, pROWTradingPositionsModel)



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
    QML_READONLY_CSTREF_PROPERTY (qint32, theNextSeason)

    //projections
    QML_READONLY_PTR_PROPERTY(ProjectionsViewFilterProxyModel, pProjectionsViewFilterProxyModel)
    QML_READONLY_PTR_PROPERTY(QStringListModel, pPosFilter)
    QML_READONLY_CSTREF_PROPERTY (QString, gameFilter)

    QML_READONLY_CSTREF_PROPERTY (qint32, theWeek)
    QML_READONLY_CSTREF_PROPERTY (qint32, theSeason)
    QML_READONLY_CSTREF_PROPERTY (QString, liveSync)
    QML_READONLY_CSTREF_PROPERTY (QString, seasonString)
    QML_READONLY_CSTREF_PROPERTY (QString, prevSelectedPlayerDisplay)

    QML_WRITABLE_CSTREF_PROPERTY(bool,useSelected)
    QML_WRITABLE_CSTREF_PROPERTY(bool,thisWeekPrev)



    QML_READONLY_CSTREF_PROPERTY (QString, controlMessage)
    QML_WRITABLE_CSTREF_PROPERTY(bool,busySend)

    pb::IPBGateway *mGateway = nullptr;
    QObject *mOGateway;
    void setupConnection(pb::IPBGateway *ingateway);

    //previous week stuff
    QML_READONLY_CSTREF_PROPERTY (qint32, thePrevWeek)
    QML_READONLY_CSTREF_PROPERTY (qint32, thePrevSeason)


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

    QML_READONLY_CSTREF_PROPERTY (qint32, height)
    QML_READONLY_CSTREF_PROPERTY (qint32, blocknum)


//    QML_READONLY_PTR_PROPERTY(SortFilterProxyModel, pPlayerProjModel)

    static Mediator *myInstance;
public:
    static Mediator *instance();

#ifdef PLAYERLOADERFD
    Q_INVOKABLE void copyFDProj() {
        ProjectionsLoaderFD pl;
        CopyProjectionsFrmoFantasyData(pl.loadProj(m_theWeek));
    }
#endif

    void CopyTheseProjections(const std::vector<fantasybit::PlayerPoints> &these) {
        for ( auto t : these) {
            auto *item = mPlayerProjModel.getByUid(t.playerid().data());
            if ( !item ) continue;
            if ( !item->get_isopen() ) continue;
            if ( t.points() <= 0 ) continue;
            item->set_projection(t.points());
        }
        m_pProjectionsViewFilterProxyModel->invalidate();
    }

    void CopyProjectionsFrmoFantasyData(const std::vector<fantasybit::PlayerPoints> &these) {
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


//    Q_INVOKABLE PlayerQuoteSliceViewFilterProxyModel * getProxyQuoteModel(bool isweekly = true) {
//        return isweekly ? m_pPlayerQuoteSliceViewFilterProxyModel : m_pROWPlayerQuoteSliceViewFilterProxyModel;
//    }

    inline PlayerQuoteSliceModel & getQuoteModel(bool isweekly = true) {
        return isweekly ? mPlayerQuoteSliceModel : mROWPlayerQuoteSliceModel;
    }

    inline PlayerQuoteSliceModel & getQuoteModel(const QString &symbol) {
        return getQuoteModel(fantasybit::isWeekly(symbol));
    }

    inline PlayerQuoteSliceModel & getQuoteModel(const std::string &symbol) {
        return getQuoteModel(fantasybit::isWeekly(symbol));
    }

    Q_INVOKABLE bool isGlobalWeekly(const QString &symbol) {
        return fantasybit::isWeekly(symbol);
    }

    Q_INVOKABLE void startDepth(const QString& symbol) {
        qDebug() << " startDepth " << symbol;
        bool isweekly = isWeekly(symbol);
        if ( symbol == (isweekly ? m_pPlayerQuoteSliceModelItem->get_symbol() :
                         m_pROWPlayerQuoteSliceModelItem->get_symbol()) )
            return;

        auto *it = getQuoteModel(isweekly).getByUid(symbol);
        if ( it != nullptr ) {
            isweekly ? update_pPlayerQuoteSliceModelItem(it)
                     : update_pROWPlayerQuoteSliceModelItem(it);

            isweekly ? update_pDepthMarketModel(it->get_pDepthMarketModel())
                     : update_pROWDepthMarketModel(it->get_pDepthMarketModel());
        }
        else {
            isweekly ? update_pDepthMarketModel(&mDepthMarketModel)
                     : update_pROWDepthMarketModel(&mROWDepthMarketModel);
        }

        auto tit = isweekly ? mTradingPositionsModel.getOrCreate(symbol) :
                             mROWTradingPositionsModel.getOrCreate(symbol);

        if ( tit != nullptr ) {
            isweekly ? update_pGlobalOpenOrdersModel(tit->get_pOpenOrdersModel())
                     : update_pROWGlobalOpenOrdersModel(tit->get_pOpenOrdersModel());
        }
        else {
//            qDebug() << " startDepth !good getOrCreate" << symbol;
            isweekly ? update_pGlobalOpenOrdersModel(&dummyOpenOrdersModel)
                     :  update_pROWGlobalOpenOrdersModel(&dummyOpenOrdersModel);
        }
    }

    Q_INVOKABLE void doCancel(qint32 id);
    Q_INVOKABLE void doTrade(QString playerid, QString symbol, bool isbuy, const qint32 price, qint32 size);

//    Q_INVOKABLE QString getOrderModelSymbol() {
//        return m_pGlobalOpenOrdersModel->get_pidsymbol();
//    }

    //projections
    Q_INVOKABLE void select(int row, int command) {
//        qDebug() << " meiator selected" << row << " commsnd " << command;
        m_pQItemSelectionModel->select(m_pWeeklyScheduleModel->index(row),QItemSelectionModel::Select);
    }

    Q_INVOKABLE void toggle(int row, int command) {
//        qDebug() << " meiator selected" << row << " commsnd " << command;
        m_pQItemSelectionModel->select(m_pWeeklyScheduleModel->index(row),QItemSelectionModel::Toggle);
    }

    Q_INVOKABLE void deselect(int row, int command) {
//        qDebug() << " meiator deselected" << row << " commsnd " << command;
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
//        m_pProjectionsViewFilterProxyModel->invalidate();
//        m_pProjectionsViewFilterProxyModel->endResetModel();


    }

    Q_INVOKABLE void copyProj(int column, QString value, bool clone, bool random = false) {
//        qDebug() << "CopyProj " << column << value << clone;

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
                    if ( clone || it->get_projection () == 0) {
                        if  ( it->get_projection() != it->get_avg()) {
                            int projection = it->get_avg();
                            if ( random ) {
                                 int num = qrand() % ((200 + 1) - 1) + 1;
                                 double percent = num / 100.0;
                                 projection = (double)projection * percent;
                            }
                            it->set_projection(projection);
                        }
                    }

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
                     srand((unsigned)time(NULL));
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

    Q_INVOKABLE void randomUseNames() {
        for ( auto *it : mGoodNameBalModel) {
            useName(it->get_name());
            QThread::currentThread()->msleep(1000);
            copyProj(6,"JayBNY",false,true);
            sendProjections();
            QThread::currentThread()->msleep(1000);
        }
    }

    //results
    Q_INVOKABLE void selectP(int row, int command) {
//        qDebug() << " meiator selectedP" << row << " commsnd " << command;
        m_pPrevQItemSelectionModel->select(m_pPreviousWeekScheduleModel->index(row),QItemSelectionModel::Select);
    }

    Q_INVOKABLE void toggleP(int row, int command) {
//        qDebug() << " meiator toggleP" << row << " commsnd " << command;
        m_pPrevQItemSelectionModel->select(m_pPreviousWeekScheduleModel->index(row),QItemSelectionModel::Toggle);
    }

    Q_INVOKABLE void deselectP(int row, int command) {
//        qDebug() << " meiator deselectedP" << row << " commsnd " << command;
        m_pPrevQItemSelectionModel->select(m_pPreviousWeekScheduleModel->index(row),QItemSelectionModel::Deselect);
    }

    Q_INVOKABLE void setNextWeekData(int week) {
        if ( !amLive ) return;


        int season = m_theNextSeason;

        if ( week <= 0 ) {
            season = m_theNextSeason-1;

            if ( season < m_theSeason )
                return;

            week = 16;
        }
        else if (week >= 17) {
            season = m_theNextSeason + 1;

            if ( season > m_theSeason )
                return;

            week = 1;
        }

        if ( season != m_theSeason)
            return;

        if ( week < m_theWeek)
            return;

        if ( week == m_theNextWeek)
            return;

        m_pNextWeekScheduleModel->clear();
        settheNextWeek(week);
        settheNextSeason(season);
        m_pNextWeekScheduleModel->updateWeeklySchedule(season,week,
                                  mGateway->dataService->GetWeeklySchedule(season,week));
    }

    Q_INVOKABLE void setPrevWeekData(int week, int season) {
//        qDebug() << "setPrevWeekData" << week << m_thePrevSeason << m_theSeason << m_thePrevWeek << m_theWeek << m_thisWeekPrev;
        setprevSelectedPlayerDisplay("");
        if ( !amLive ) return;

//        int season = m_thePrevSeason;
        if ( season != m_thePrevSeason )
            setthePrevSeason(season);
//        m_thePrevSeason = season;

        if ( week <= 0 ) {
            season = m_thePrevSeason-1;

            if ( season < 2014 )
                return;

            week = 16;
        }
        else if (week >= 17) {
            season = m_thePrevSeason + 1;

            if ( season > m_theSeason)
                return;

            week = 1;
        }

        if ( week > m_theWeek && season == m_theSeason )
            return;

        if ( week == m_theWeek && season == m_theSeason && !m_thisWeekPrev )
            return;

        if ( week == m_thePrevWeek && season == m_theSeason && !(week == m_theWeek && m_thisWeekPrev ))
            return;

        if ( week == m_thePrevWeek )
            emit thePrevWeekChanged(m_thePrevWeek);
        else {
            setthePrevWeek(week);
            if ( season != m_thePrevSeason )
                setthePrevSeason(season);
        }

        if ( week == m_theWeek && season == m_theSeason) {
           m_pPreviousWeekScheduleModel->clear();
//           for ( auto *it : m_pWeekClosedScheduleModel->)
           m_pPreviousWeekScheduleModel->append(m_pWeekClosedScheduleModel->toList());
        }
        else
            m_pPreviousWeekScheduleModel->updateWeeklySchedule(m_thePrevSeason,m_thePrevWeek,
                                      mGateway->dataService->GetWeeklySchedule(m_thePrevSeason,m_thePrevWeek));


        const auto &vgr = mGateway->dataService->GetPrevWeekGameResults(m_thePrevSeason,m_thePrevWeek);
        mPlayerResultModel.updateRosters(vgr,mGateway->dataService,*m_pPreviousWeekScheduleModel,myFantasyName);
        myPrevGamesSelectionModel.reset();
        m_pResultSelectedModel->setSourceModel(&dummyResultSelectedModel);
        m_pResultSelectedModel->clear();
        m_pResultsViewFilterProxyModel->invalidate();
    }

    Q_INVOKABLE void setPrevWeekResultLeaderSelection(QString in) {
//        qDebug() << " setPrevWeekResultLeaderSelection " << in;
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


//    bool usingRandomNames = false;

    std::vector<std::string> fnames;
    int fnameindex;

    //fantasy name - manage
//    Q_INVOKABLE void pk2fname(const QString&);
    Q_INVOKABLE void checkname(QString s) {
//        qDebug() << " inside checkname" << s;
        emit doNameCheck(s);
    }

    Q_INVOKABLE QString importMnemonic(const QString &importStr);
    Q_INVOKABLE void signPlayer(const QString &name);
    Q_INVOKABLE void useName(const QString &name);

    Q_INVOKABLE QString getSecret();
    Q_INVOKABLE QStringList goodList() { return m_goodList; }
    Q_INVOKABLE QStringList allNamesList() { return m_allNamesList; }

    //trading
    Q_INVOKABLE QString checkFullName(const QString &fullname, const QString &syb) {
        if ( fullname != "" )
            return fullname;

        return mPlayerSymbolsModel.Update(syb, mGateway->dataService->GetPlayerDetail(syb.toStdString()));
    }


//    Q_INVOKABLE QString getPlayerNamePos(const QString &uid) {

//        auto model = mPlayaddtrerQuoteSliceModel.getByUid(uid);
//        if ( model == nullptr ) {
//            qDebug() << " bad data for getPlayerNamePos " << uid;
//            return "";
//        }
//        else
//            return model->get_fullname() + " (" + model->get_pos() +")" ;
//    }

    Q_INVOKABLE void addTradingSymbol(const QString &symbol,bool isweekly);

    void addQuoteSymbol(const std::string &symbol) {
        QString stripped = fantasybit::stripSymbol(symbol).data();
        auto it = mPlayerSymbolsModel.getByUid(stripped);
        if ( it == nullptr ) {
            qDebug() << "error addQuoteSymbol" << symbol.data();
        }
        else if ( checkFullName(it->get_fullname(),stripped) == "")
            qDebug() << "error addQuoteSymbol checkFullName" << symbol.data() << stripped;
        else {
//            bool isweekly = fantasybit::isWeekly(symbol);
            getQuoteModel(symbol).UpdateSymbol(it,m_blocknum,symbol);
        }
    }

    //data
//    Q_INVOKABLE QString getTeamid(const QString &uid) {
//        auto model = mPlayerQuoteSliceModel.getByUid(uid);
//        if ( model == nullptr ) {
//            qDebug() << " bad data for getTeamid " << uid;
//            return "";
//        }
//        else
//            return model->get_teamid();
//    }

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
//    void subscribeOrderPos(const QString &name);
//    void getOrderReq(const QString &name,const QString symbol="");

    //name
//    void OnGoodName(const QString &goodname, const fantasybit::FantasyNameBal &fnb);

    void updateCurrentFantasyPlayerProjections();
signals:
    void importSuccess(const QString name, bool passfail);
    void usingFantasyName(const QString &name);
    void nameCheckGet( const QString & name, const QString & status );
    void OnClaimName(QString name);

    void OnUseName(QString); //tell agent to use the name
    void ready();
//    void updateWeekData();
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

    void haveWeeklySymbol();
    void haveRowSymbol();

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
//        qDebug() << " mediator selectionChanged " << selected << deselected;
        m_pProjectionsViewFilterProxyModel->invalidate();
    }

    void selectionPrevChanged(const QItemSelection &selected, const QItemSelection &deselected) {
//        qDebug() << " mediator selectionPrevChanged " << selected << deselected;
        m_pResultsViewFilterProxyModel->invalidate();
    }

private slots:
//    void OnpPlayerQuoteSliceModelItemChanged (PlayerQuoteSliceModelItem * name);

private:
//    std::string lastPk2name;
    //fantasybit::FantasyAgent m_fantasy_agent;
    std::string myFantasyName;

//    QString m_playersName;
//    QString m_playersStatus;
//    std::string m_lastSignedplayer;

//    ordsnap_t mMyPosOrders;
//    std::unordered_map<std::string, std::string> m_myPubkeyFname;
//    std::unordered_map<std::string, uint64_t> m_myPubkeyHash;
//    std::unordered_map<uint64_t, std::string> m_myHashFname;

//    void doPk2fname(const std::string &pkstr);

    //timers
//    QTimer signPlayerStatus;
//    QTimer polldepth;
//    QTimer tradeTesting;

    //    WeeklyScheduleModel mWeeklyScheduleModel;


    //oms
//    OpenOrdersModel mOpenOrdersModel;

    //depth todo
//    QString testid;
//    WsReq mGetDepthReq;
//    bool isbid;
    //    void getOrderReq(uint64_t cname);
//    int depthCount;
//    int depthBackup;
//    int depthInterval;


    bool amLive = false;

    void getLeaders(int week,bool lastweek, bool all20XX = false) {
#ifdef NO_SQL_LEADERS
        return;
#endif
        QString links("https://app.trading.football:4545");
        QString route("fantasy/leaders");
        if ( !all20XX )
            route = route.append("?position=all%20positions&week=%1").arg(week);


//        qDebug() << " calling route " << route;

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
            if ( all20XX )
                item->set_leaders20XX(score);
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

    void UpdateSeasonWeek(int season, int week) {
        mSeasonSuffix = to_string(season-2000);
        mWeeklySuffix = mSeasonSuffix;
        mSeasonSuffix += "s";
        mWeeklySuffix += "w";
        mWeeklySuffix += (week <= 9) ? "0" : "";
        mWeeklySuffix += to_string(week);

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
    void NewSeason(int);
    void GameStart(string);
    void GameOver(string);
    void onControlMessage(QString);
    void NewFantasyName(fantasybit::FantasyNameBal fnb) {
//        qDebug() << "NewFantasyName " << fnb.DebugString().data();
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
        //qDebug() << " BlockNum " << n;
        setblocknum(n);
    }

    void OnFinishedResults() {
        if ( amLive ) {
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
                m_pWeekClosedScheduleModel->append(new WeeklyScheduleModelItem(ip.second,GameStatus_Status_CLOSED,this));
                m_pWeeklyScheduleModel->remove(ip.second);
            }

            if ( todo.size() > 0 ) {
                if ( !m_thisWeekPrev)
                    set_thisWeekPrev(true);

                m_pProjectionsViewFilterProxyModel->invalidate();
            }

            updateLiveLeaders(false);
        }
    }
    void OnMarketTicker(fantasybit::MarketTicker,int32_t);
    void OnDepthDelta(fantasybit::DepthFeedDelta*);
    void OnTradeTick(fantasybit::TradeTic*);
    void OnNewPos(fantasybit::FullPosition);
    void OnNewOO(fantasybit::FullOrderDelta);
    void MyPosPriceChange(PlayerQuoteSliceModelItem*);

private:
//    vector<MyFantasyName> myGoodNames;
    void updateWeek();

    void updateOnChangeFantasyName();
    void updateCurrentFantasyPlayerOrderPositions();
    std::string mSeasonSuffix, mWeeklySuffix;
    double calcTotalPnl(TradingPositionsModel &tpm, PlayerQuoteSliceModel &pqsm);
    OpenOrdersModel *updateGlobalOrder(TradingPositionsModel &tpm, PlayerQuoteSliceModelItem *pqsmi);
};
}
#endif // MEDIATOR_H

    /*
     *     int testCount = 0;
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
