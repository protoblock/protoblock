#include "mediator.h"
#include "StateData.pb.h"
#include <QSettings>
#include "Commissioner.h"
#include "DataPersist.h"
#include <unordered_set>

//using namespace pb;
using namespace fantasybit;
namespace pb {

Mediator::Mediator(QObject *parent) :  QObject(parent),
                    mPlayerQuoteSliceModel{this,"","symbol"},
                    mROWPlayerQuoteSliceModel{this,"","symbol"},
                    mDepthMarketModel{},
                    mROWDepthMarketModel{},
                    mTradingPositionsModel{this,QByteArray(),{"symbol"}},
                    mROWTradingPositionsModel{this,QByteArray(),{"symbol"}},
                    mSwapBuyModel{this},
                    mSwapSellModel{this},
                    mFantasyNameBalModel(this,QByteArray(),{"name"}),
                    mGoodNameBalModel(this,QByteArray(),{"name"}),
                    mPlayerSymbolsModel{this,{"symbol"},{"symbol"}},

                    dummyPlayerSymbolsModelItem("0",""),//@@RB"),
                    dummyPlayerQuoteSliceModelItem(&dummyPlayerSymbolsModelItem,"","",0),
//                    dummyPlayerQuoteSliceModelItem(&dummyPlayerSymbolsModelItem,"","17s",0),

                    dummyFantasyNameBalModelItem(),

                    myGamesSelectionModel{},
                    myPrevGamesSelectionModel{},

                    m_pPlayerSymbolsModel(new SortFilterProxyModel),
                    m_pFantasyNameBalModel(&mFantasyNameBalModel),


                    m_pPlayerQuoteSliceModelItem(&dummyPlayerQuoteSliceModelItem),
                    m_pROWPlayerQuoteSliceModelItem(&dummyPlayerQuoteSliceModelItem),


                    m_pDepthMarketModel(&mDepthMarketModel),
                    m_pROWDepthMarketModel(&mROWDepthMarketModel),

                    m_pGlobalOpenOrdersModel(&dummyOpenOrdersModel),
                    m_pROWGlobalOpenOrdersModel(&dummyOpenOrdersModel),

                    m_pTradingPositionsModel(&mTradingPositionsModel),
                    m_pROWTradingPositionsModel(&mROWTradingPositionsModel),

                    m_pSwapBuyModel(&mSwapBuyModel),
                    m_pSwapSellModel(&mSwapSellModel),

                    m_bitcoinBalance(0),
                    m_pGoodNameBalModel(&mGoodNameBalModel),
                    m_pMyFantasyNameBalance{&dummyFantasyNameBalModelItem},


                    m_pLeaderBoardSortModel(new SortFilterProxyModel),
                    m_pQItemSelectionModel(&myGamesSelectionModel),
                    m_pPrevQItemSelectionModel(&myPrevGamesSelectionModel),
                    m_pResultSelectedModel(new SortFilterProxyModel),
                    m_height(0),
                    m_theSeason(2018),
                    m_blocknum(0) ,
                    myFantasyName("") ,
                    m_pAccountsModel(new SortFilterProxyModel)  {

    fnames = {"fname1", "fname2","fname3", "fname4", "fname5"};

//    PlayerQuoteSliceModelItem dumm(&dummyPlayerSymbolsModelItem,"","17s");
//    auto f = dumm.get_firstname();
//    qDebug() << f << " ||";

    fnameindex = 0;
    //leader models
    m_pLeaderBoardSortModel->setSourceModel(m_pFantasyNameBalModel);
    m_pLeaderBoardSortModel->setSortRole("lastupdate");//mPlayerProjModel.roleForName("pos"));
//    m_pLeaderBoardSortModel->setDynamicSortFilter(true);
    m_pLeaderBoardSortModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_pLeaderBoardSortModel->setFilterRole({"name"});
    m_pLeaderBoardSortModel->setFilterSyntax(SortFilterProxyModel::FilterSyntax::FixedString);

    //wallet models
    m_pAccountsModel->setSourceModel(m_pFantasyNameBalModel);
    m_pAccountsModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_pAccountsModel->setFilterRole({"name"});
    m_pAccountsModel->setFilterSyntax(SortFilterProxyModel::FilterSyntax::FixedString);
    m_pAccountsModel->setSortRole("name");
//    m_pAccountsModel->setDynamicSortFilter(true);





    m_pPlayerSymbolsModel->setSourceModel(&mPlayerSymbolsModel);
    m_pPlayerSymbolsModel->setSortRole({"fullname"});
    m_pPlayerSymbolsModel->setDynamicSortFilter(true);
    m_pPlayerSymbolsModel->setFilterRole({"symbol"});

    //schedule models
    m_pWeeklyScheduleModel = new WeeklyScheduleModel(this);
    m_pWeekClosedScheduleModel = new WeeklyScheduleModel;

    myGamesSelectionModel.setModel(m_pWeeklyScheduleModel);
    m_gameFilter = "Scheduled";

    m_pPreviousWeekScheduleModel = new WeeklyScheduleModel;
    myPrevGamesSelectionModel.setModel(m_pPreviousWeekScheduleModel);

    m_pNextWeekScheduleModel = new WeeklyScheduleModel;

    m_theWeek = 0;
    m_thePrevWeek = 0;
    m_theNextWeek = 0;
    m_liveSync = "Sync";
    m_seasonString = "";
    m_theNextSeason = 0;
    m_thePrevSeason = 0;
    m_theSeason = 2014;
    mSeasonSuffix = "14s";
    mWeeklySuffix = "00w";
    m_allowAction = false;

    //player selection
    m_pProjectionsViewFilterProxyModel =  new ProjectionsViewFilterProxyModel(m_pWeeklyScheduleModel,&myGamesSelectionModel);
    m_pProjectionsViewFilterProxyModel->setSourceModel(&mPlayerProjModel);

    m_pProjectionsViewFilterProxyModel->setSortRole("knownProjection");//mPlayerProjModel.roleForName("pos"));
    m_pProjectionsViewFilterProxyModel->setDynamicSortFilter(false);

    //results
    m_pResultsViewFilterProxyModel =  new ResultsViewFilterProxyModel(m_pPreviousWeekScheduleModel,&myPrevGamesSelectionModel);
    m_pResultsViewFilterProxyModel->setSourceModel(&mPlayerResultModel);

    m_pResultsViewFilterProxyModel->setSortRole("result");//mPlayerProjModel.roleForName("pos"));
    m_pResultsViewFilterProxyModel->setDynamicSortFilter(true);


    m_pPlayerQuoteSliceViewFilterProxyModel =  new PlayerQuoteSliceViewFilterProxyModel(this);
    m_pPlayerQuoteSliceViewFilterProxyModel->setSourceModel(&mPlayerQuoteSliceModel);
    m_pPlayerQuoteSliceViewFilterProxyModel->setSortRole("symbol");
    m_pPlayerQuoteSliceViewFilterProxyModel->setDynamicSortFilter(true);

    //trading
//    m_pPlayerQuoteSliceModel = &m_pPlayerQuoteSliceModel;
    m_pROWPlayerQuoteSliceViewFilterProxyModel =  new PlayerQuoteSliceViewFilterProxyModel(this);
    m_pROWPlayerQuoteSliceViewFilterProxyModel->setSourceModel(&mROWPlayerQuoteSliceModel);
    m_pROWPlayerQuoteSliceViewFilterProxyModel->setSortRole("symbol");
    m_pROWPlayerQuoteSliceViewFilterProxyModel->setDynamicSortFilter(true);



    m_useSelected = true;
    m_busySend = true;
    m_thisWeekPrev = false;


    m_pResultSelectedModel->setSourceModel(&dummyResultSelectedModel);
    m_pResultSelectedModel->setSortRole("award");
    m_pResultSelectedModel->setDynamicSortFilter(true);


    setcontrolMessage("<html><style type=\"text/css\"></style>Fantasy Ticker Feed now live on Twitter " \
                      "<a href=\"https://twitter.com/prototicker\">@prototicker</a></html>");

    connect(&mPlayerQuoteSliceModel,&PlayerQuoteSliceModel::MyPosPriceChange,
            this, &Mediator::MyPosPriceChange);

    connect(&mROWPlayerQuoteSliceModel,&PlayerQuoteSliceModel::MyPosPriceChange,
            this, &Mediator::MyPosPriceChange);

    connect(this, &Mediator::blocknum_string_numChanged, this, &Mediator::doBlockString);
}

void Mediator::NameStatus(fantasybit::MyFantasyName myname) {
    qDebug() << myFantasyName.data() << " mediator namestatus " << myname.DebugString().data();

    if ( myname.name() == myFantasyName &&  myname.name() != otherFantasyName)
        return;

    if ( nullptr != mGoodNameBalModel.getByUid(myname.name().data()) ) {
//        if ( myFantasyName != "" ) return;
        qDebug() << " already have name in GoodName";
    }
    else {
        auto it = mFantasyNameBalModel.getByUid(myname.name().data());
        if ( it != nullptr) {
            qDebug() << " dont have name in GoodName, have name in mFantasyNameBalMode adding good";

            auto it2 = new FantasyNameBalModelItem(*it);
            mGoodNameBalModel.append(it2);
        }
        else {
            qDebug() <<  " mediator namestatus not in  mFantasyNameBalModel";
            if ( myname.status() < MyNameStatus::requested) {
                qDebug() << " not comfirmed ";
                return;
            }
            else {
                QString nn = myname.name().data();
                mGoodNameBalModel.append(new FantasyNameBalModelItem(nn));
            }
        }
    }

    otherFantasyName = "";
    myFantasyName = myname.name();
    auto mgit = mGoodNameBalModel.getByUid(myname.name().data());
    mgit->updatePnl(mGateway->dataService->GetOpenPnl(myFantasyName));
    update_pMyFantasyNameBalance(mgit);
    qDebug() << "Mediator  emitting using fantasy name " << myname.name().data();
    myGamesSelectionModel.reset();
    emit usingFantasyName(myname.name().data(),mgit->get_btcaddr());
    updateOnChangeFantasyName();
    set_busySend(false);
    checkAllowAction();
}

void Mediator::OnPlayName(string pname) {
    qDebug() << myFantasyName.data() << " mediator PlayName " << pname.data();

    if ( pname == otherFantasyName )
        return;

    if ( otherFantasyName == pname )
        return;

    otherFantasyName = pname;
    myFantasyName = otherFantasyName;
    auto mgit = m_pFantasyNameBalModel->getByUid(pname.data());
    mgit->updatePnl(mGateway->dataService->GetOpenPnl(myFantasyName));
    update_pMyFantasyNameBalance(mgit);
    qDebug() << "Mediator PlayName emitting using fantasy name " << pname.data();
    myGamesSelectionModel.reset();
    emit usingFantasyName(pname.data(),"");
    updateOnChangeFantasyName();
    checkAllowAction();
}

void Mediator::LiveProj(FantasyBitProj proj) {
    auto *item = mPlayerProjModel.getByUid(proj.playerid().data());
    if ( item ) {

        item->set_avg(mGateway->dataService->GetAvgProjection(proj.playerid()));

        if ( proj.name() == myFantasyName ) {
            set_busySend(false);
            if ( proj.proj() > 0 ) {
                item->set_knownProjection(proj.proj());
                item->set_projection(proj.proj());
            }
        }
    }

    auto *item2 = mFantasyNameBalModel.getByUid(proj.name().data());
    if ( !item2 ) return;
    item2->set_lastupdate(proj.block());
    item2->set_numberproj(proj.count());
}

void Mediator::MyNames(vector<MyFantasyName> mfn) {

    qDebug() << " mediator myname " << mfn.size();
    int heighest = 0;
    string hname  = "";
    for ( auto m : mfn ) {
        if ( m.status() >= heighest) {
            heighest = m.status();
            hname = m.name();
        }
        if ( m.name() == myFantasyName )
            continue;

        if ( nullptr != mGoodNameBalModel.getByUid(m.name().data()) ) continue;

        qDebug() <<  m.name ().data () << " not in mGoodNameBalModel";
        auto it = mFantasyNameBalModel.getByUid(m.name().data());
        if ( it != nullptr) {
            qDebug() <<  m.name ().data () << " is in mFantasyNameBalModel";
            mGoodNameBalModel.append(new FantasyNameBalModelItem(*it));
            auto it2 = mGoodNameBalModel.getByUid(m.name().data());
            if ( it2 == nullptr)
                qDebug() << " nullptr? should have in good name wtf";
            else
                it2->updatePnl(mGateway->dataService->GetOpenPnl(m.name().data()));
        }
        else
            qDebug() <<  m.name ().data () << " not in mFantasyNameBalModel";

    }

    qDebug() << " namename wins " << heighest << hname.data();
    if ( myFantasyName == "" && hname != "" && heighest >= 20)
        useName(hname.data());
    else
        emit noName();

}

void Mediator::NameBal(fantasybit::FantasyNameBal fnb) {
    qDebug() << "  NameBal " << fnb.DebugString ().data ();
    FantasyNameBalModelItem *item =  mGoodNameBalModel.getByUid (fnb.name().data());
    if ( !item || item->get_name() != fnb.name().data()) {
        //mGoodNameBalModel.insert(mGoodNameBalModel.size(),FantasyNameBalModelItem(fnb));
    }
    else {
        item->update(fnb);
        mFantasyNameBalModel.update(item);
    }

}

void Mediator::PlayerStatusChange(pair<string, PlayerStatus> in) {
    auto *item = mPlayerProjModel.getByUid(in.first.data());

    PlayerSymbolsModelItem *it = nullptr;
    if ( item ) {
        item->setstatus(in.second.status());
        it = item->get_pPlayerSymbolsModelItem();
    }

    if ( it == nullptr )
        it = mPlayerSymbolsModel.getByUid(in.second.symbol().data());

    if ( it == nullptr) {
        qDebug() << "PlayerStatusChange error " << in.second.DebugString().data();
        return;
    }

    checkFullName(it->get_fullname(),it->get_symbol());
    it->setteamid(in.second.teamid().data());
}

void Mediator::GlobalStateChange(GlobalState gs) {
    qDebug() << "Mediator GlobalStateChange " << gs.DebugString().data();

//    if ( gs.week() > 0 && gs.week() < 18)
    {
        UpdateSeasonWeek(gs.season(),gs.week());

        if ( m_thePrevWeek == m_theWeek)
            setthePrevWeek(gs.week());

        if ( amLive && gs.week() > m_theWeek) {
            settheWeek(gs.week());
            updateWeek();
        }
        else if ( !(gs.season() == 2014 && gs.week() <= 1) )
            updateWeek();

        set_busySend(false);
    }
}

void Mediator::LiveGui(GlobalState gs) {

    qDebug() << "Mediator received Livegui " << gs.DebugString().data();
    if ( !amLive ) {
        amLive = true;
        settheWeek(gs.week());
        settheSeason(gs.season());

        UpdateSeasonWeek(gs.season(),gs.week());

        setliveSync("Live");
        if ( gs.state() == GlobalState_State_OFFSEASON ) {
            setseasonString("Off Season");
//            setthePrevWeek(16);
            setthePrevSeason(m_theSeason);
            setthePrevWeek(gs.week());
            settheNextSeason(m_theSeason);
        }
        else {
            setseasonString("NFL Season");
            setthePrevWeek(gs.week());
            setthePrevSeason(m_theSeason);
            settheNextSeason(m_theSeason);
        }

        updateWeek();
//        tradeTesting.start(5000);

        setcontrolMessage("<html><style type=\"text/css\"></style>Follow us on Twitter. " \
                          "<a href=\"https://twitter.com/protoblock\">@protoblock</a></html>");

        checkAllowAction();
    }

//    FantasyNameBal fnb;
//    fnb.set_name("testname");
//    NewFantasyName(fnb);
}

void Mediator::updateWeek() {
    if ( mGateway == nullptr ) {
        qDebug() << " mGateway null ";
    }
    else if ( mGateway->dataService == nullptr)
        qDebug() << " mGateway->dataService null ";
    else {
        set_busySend(false);

        {
#ifndef SUPERBOWL52LIVE
            auto gss = mGateway->dataService->GetAllSymbols();
#else
            auto gss = mGateway->dataService->GetTeamSymbols({"NE","PHI"});
#endif
            for ( auto gs : gss ) {
                if ( mPlayerSymbolsModel.getByUid(gs.first.data()) == nullptr)
                    mPlayerSymbolsModel.append(new PlayerSymbolsModelItem(gs.second.data(),gs.first.data()));
            }
        }

        if ( m_theWeek > 0  && m_theWeek < 17) {
            m_pWeeklyScheduleModel->clear ();
            m_pWeekClosedScheduleModel->clear ();

            //DO Schedule
            std::map<int,std::vector<std::pair<fantasybit::GameInfo,fantasybit::GameStatus>>> sorted;
            fantasybit::WeeklySchedule weekly = mGateway->dataService->GetWeeklySchedule(m_theSeason,m_theWeek);
            for ( auto &gi : weekly.games()) {
                auto status = mGateway->dataService->GetGameStatus(gi.id());
                std::vector<std::pair<fantasybit::GameInfo,fantasybit::GameStatus>> &vec =
                        sorted[gi.time()];
                vec.emplace_back(std::make_pair(gi,status));
            }

            vector<WeeklyScheduleModelItem *> holdingame;
            for ( auto tvec : sorted ) {
//                qDebug() << " tvec sorted " << tvec.first;
                for ( auto p : tvec.second ) {
//                    qDebug() << " p tvec " << p.second << p.first.DebugString().data();
                    if ( p.second.status() == GameStatus_Status_CLOSED )
                        m_pWeekClosedScheduleModel->append(new WeeklyScheduleModelItem(p.first,p.second,this));
                    else if ( p.second.status() == GameStatus_Status_INGAME )
                        holdingame.push_back(new WeeklyScheduleModelItem(p.first,p.second,this));
                    else
                        m_pWeeklyScheduleModel->append(new WeeklyScheduleModelItem(p.first,p.second,this));
                }
            }

            //ingame goes at end
            for ( auto h : holdingame)
                m_pWeeklyScheduleModel->append(h);

            set_thisWeekPrev(m_pWeekClosedScheduleModel->count() > 0);
            //END Schedule



            //playerprojmodels
            const auto &vgr = mGateway->dataService->GetCurrentWeekGameRosters();
            mPlayerSymbolsModel.Update(vgr);
            mPlayerProjModel.updateRosters(vgr,mGateway->dataService,mPlayerSymbolsModel);

//            m_pPlayerProjModel->setSortRole ()

            /*
            const auto &vms = mGateway->dataService->GetCurrentMarketSnaps();
            qDebug() << "  vms " << vms.size();
            string suffix = "17w";
            suffix += (m_theWeek < 9) ? "0" : "";
            suffix += to_string(m_theWeek);
            mPlayerQuoteSliceModel.Update(vms,mPlayerProjModel, suffix );
//            OnGotMarketSnaps();

            */

        }
        else { //off season
            m_pWeeklyScheduleModel->clear();
            mPlayerProjModel.clear();
            set_thisWeekPrev(false);
        }

//        std::string sss = "BMWR17w01";
//        if ( !getQuoteModel().Update(ms)) {
//            addQuoteSymbol(ms.symbol());
//            if ( !getQuoteModel(ms.symbol()).Update(ms))
//                qDebug() << "error updateweek" << ms.DebugString().data();
//        }

        //clear weekly
        getQuoteModel (true).clear ();
        //get snapshot for all symbols
        const auto &vms = mGateway->dataService->GetCurrentMarketSnaps();
        qDebug() << "  vms " << vms.size();
        for ( auto ms : vms ) {
//            if ( ms.symbol () == "BMWR17s")
//                ms.set_symbol ( "BMWR17w01");
            if ( !getQuoteModel(ms.symbol()).Update(ms)) {
                addQuoteSymbol(ms.symbol());
                if ( !getQuoteModel(ms.symbol()).Update(ms))
                    qDebug() << "error updateweek" << ms.DebugString().data();
            }

            if ( amLive ) {
                if ( getQuoteModel(ms.symbol()).count () == 1) {
                    if ( isWeekly(ms.symbol ())) {
                        qDebug() << " emit haveWeeklySymbol";
                        emit haveWeeklySymbol ();
                    }
                    else emit haveRowSymbol ();
                }
            }
        }



        if (myFantasyName != "" ) {
            updateOnChangeFantasyName();
            m_pMyFantasyNameBalance->updatePnl(mGateway->dataService->
                                             GetOpenPnl(m_pMyFantasyNameBalance->get_name().toStdString()));
        }

        updateLiveLeaders();
//        emit updateWeekData ();
//        m_pProjectionsViewFilterProxyModel->setDynamicSortFilter(false);

    }
}

#ifdef NOTDEF
            int i = 0;
            for( auto it : mPlayerProjModel ) {
                if ( i++ > 20 )  break;
                m_pPlayerQuoteSliceModel->append(new PlayerQuoteSliceModelItem(*it));
            }
#endif

void Mediator::OnSwapData(fantasybit::SwapOrder so) {
    if ( so.isask() ) {
        m_pSwapSellModel->add(so);
    }
    else {
        if ( so.ref() != "" )
            doSwapSent(so);
        else
            m_pSwapBuyModel->add(so);
    }
}


void Mediator::updateOnChangeFantasyName() {
    updateCurrentFantasyPlayerProjections();
    updateCurrentFantasyPlayerOrderPositions();
    setbitcoinBalance(BitcoinUtils::getBTCbalance(m_pMyFantasyNameBalance->get_btcaddr()));

    m_pSwapSellModel->clear();
    m_pSwapBuyModel->clear();
    const auto &vso = mGateway->dataService->GetCurrentSwapSnaps();
    for  (auto so : vso ) {
        if ( so.isask() )
            m_pSwapSellModel->add(so);
        else if ( so.ref() != "" )
            doSwapSent(so);
        else
            m_pSwapBuyModel->add(so);
    }


}

void Mediator::updateCurrentFantasyPlayerProjections(){
    //update to recent projection projection and mark them a sent
    auto  recentProjections = mGateway->dataService->GetProjByName(myFantasyName);
    qDebug() << "updateCurrentFantasyPlayerProjections " << recentProjections.size();

    for ( auto it : mPlayerProjModel)  {
        if ( !it ) continue;
        it->set_knownProjection(0);
        it->set_projection(0);
    }

    for ( auto it = recentProjections.begin(); it != recentProjections.end(); ++it ){
        auto *item = mPlayerProjModel.getByUid(it->first.data());
        if ( !item ) continue;
        item->set_knownProjection(it->second);
        item->set_projection(it->second);
    }

    m_pProjectionsViewFilterProxyModel->setSortRole ("knownProjection");
    m_pProjectionsViewFilterProxyModel->sortAgain ("knownProjection",Qt::SortOrder::DescendingOrder);

}

void Mediator::updateCurrentFantasyPlayerOrderPositions() {


    for ( auto tit : mTradingPositionsModel ) {
        auto it = mPlayerQuoteSliceModel.getByUid(tit->get_symbol());
        if ( it == nullptr) continue;
        it->setmyavg(0);
        it->setmyposition(0);
        it->setmypnl(0);
    }

    for ( auto tit : mROWTradingPositionsModel ) {
        auto it = mROWPlayerQuoteSliceModel.getByUid(tit->get_symbol());
        if ( it == nullptr) continue;
        it->setmyavg(0);
        it->setmyposition(0);
        it->setmypnl(0);
    }


    auto myorderpositions = mGateway->dataService->GetOrdersPositionsByName(myFantasyName);
    mTradingPositionsModel.clearForUpdateFname(myFantasyName);
    mROWTradingPositionsModel.clearForUpdateFname(myFantasyName);
    TradingPositionsModelItem *t = nullptr;
    for ( auto p : myorderpositions ) {
      //  qDebug() << "level2 Trading SetMyPositions" << p.first << p.second;
        auto &mypair = p.second;
        auto &myorders = mypair.second;

        auto t = isWeekly(p.first) ? &mTradingPositionsModel : &mROWTradingPositionsModel;

        auto it = new TradingPositionsModelItem(p,t);
        t->append(it);
        t->settotalopenpnl(it->get_openpnl());
    }

//    mTradingPositionsModel.updateAllOrders(myFantasyName,myorderpositions);
//    mTradingPositionsModel.setfantasyname(myFantasyName.data());


#ifdef TRACE
    qDebug() << "level2 Trading SetMyPositions" << myFantasyName.data() << myorderpositions.size();
#endif

   mTradingPositionsModel.settotalopenpnl(calcTotalPnl(mTradingPositionsModel,mPlayerQuoteSliceModel));
   mROWTradingPositionsModel.settotalopenpnl(calcTotalPnl(mROWTradingPositionsModel,mROWPlayerQuoteSliceModel));

   update_pGlobalOpenOrdersModel(updateGlobalOrder(mTradingPositionsModel,m_pPlayerQuoteSliceModelItem));
   update_pROWGlobalOpenOrdersModel(updateGlobalOrder(mROWTradingPositionsModel,m_pROWPlayerQuoteSliceModelItem));


}

OpenOrdersModel *Mediator::updateGlobalOrder(TradingPositionsModel &tpm, PlayerQuoteSliceModelItem *pqsmi) {
    if ( pqsmi ) {
        auto tit = tpm.getByUid(pqsmi->get_symbol());
        if ( tit != nullptr ){
            return tit->get_pOpenOrdersModel();
        }
    }

    return &dummyOpenOrdersModel;
}

double Mediator::calcTotalPnl(TradingPositionsModel &tpm, PlayerQuoteSliceModel &pqsm) {
    int i = 0;
    double totpnl = 0.0;
    for ( auto tit : tpm ) {
        auto it = pqsm.getByUid(tit->get_symbol());
        if ( it == nullptr) continue;
        int netqty = tit->get_netqty();
//        qDebug() << ++i << " here ";

        double avg = 0;
        double pnl = 0;
        if ( netqty == 0 ) {
            pnl = tit->get_netprice() * tit->get_multiplier();
        }
        else {
            int price = (netqty > 0) ? it->get_bid() :
                                       it->get_ask();
            if ( price == 0 )
                price = (netqty > 0) ? 0 : ( fantasybit::isWeekly(tit->get_symbol()) ? 41 : 401) ;
//                price = it->get_lastprice();


//            if ( price == 0 )
//                pnl = 0.0;
//            else
            pnl = tit->get_multiplier() * ((price * netqty) + tit->get_netprice());
            avg = tit->get_netprice()  / (netqty * -1.0);
        }

        it->setmyavg(avg);
        it->setmyposition(netqty);
        qDebug() << tit->get_symbol() << " setmyposition " << netqty << "tit->get_netprice()" << tit->get_netprice() << "tit->get_multiplier()" << tit->get_multiplier();
        it->setmypnl(pnl);
        tit->setopenpnl(pnl);
        totpnl += pnl;
    }

    return totpnl;
}

void Mediator::NewWeek(int week) {

    if ( m_theSeason == 2014 ) {
        if ( mGateway->dataService->GetGlobalState().season() > 2014 )
            settheSeason(mGateway->dataService->GetGlobalState().season());
    }

    UpdateSeasonWeek(m_theSeason,week);

    set_busySend(false);
//    set_thisWeekPrev = false;

    if ( m_thePrevWeek == m_theWeek)
        setthePrevWeek(week);

    settheWeek(week);
    if ( amLive ) {
        updateLiveLeaders();
    }
    else {
        m_pWeeklyScheduleModel->clear();
        //ToDo: maybe only if live
        if ( !(m_theSeason == 2014 && week <= 1) )
            updateWeek();
    }
}

void Mediator::NewSeason(int season) {
    settheSeason(season);
    UpdateSeasonWeek(season,1);
}

void Mediator::GameStart(string gameid) {
    m_pWeeklyScheduleModel->UpdateStatus(gameid,GameStatus_Status_INGAME);

    for ( auto it : mPlayerProjModel) {
        if ( it->get_gameid() != gameid.data() )
            continue;

        int projection = it->get_projection();
        int knownprojection = it->get_knownProjection();
        if ( knownprojection == projection)
            it->set_projection(knownprojection);
        it->setisopen(false);
    }
    m_pProjectionsViewFilterProxyModel->invalidate();
}

void Mediator::GameOver(string) {}

void Mediator::onControlMessage(QString msg) {
    setcontrolMessage(msg);
}

void Mediator::setupConnection(pb::IPBGateway *ingateway) {

    QObject* that = dynamic_cast<QObject*>(ingateway);
    mOGateway = that;


    connect(that, SIGNAL(PlayName(string)),
            this, SLOT(OnPlayName(string)));

    connect(that, SIGNAL(NameStatus(fantasybit::MyFantasyName)),
            this, SLOT(NameStatus(fantasybit::MyFantasyName)));

    connect( that, SIGNAL(LiveProj(fantasybit::FantasyBitProj)),
            this, SLOT(LiveProj(fantasybit::FantasyBitProj )));

    connect( that, SIGNAL(MyNames(vector<fantasybit::MyFantasyName>)),
            this, SLOT(MyNames(vector<fantasybit::MyFantasyName> )));
    connect( that, SIGNAL(NameBal(fantasybit::FantasyNameBal)),
            this, SLOT(NameBal(fantasybit::FantasyNameBal )));
    connect( that, SIGNAL( PlayerStatusChange(pair<string,fantasybit::PlayerStatus>) ),
            this, SLOT(PlayerStatusChange(pair<string,fantasybit::PlayerStatus> )));


    connect( that, SIGNAL(  GlobalStateChange(fantasybit::GlobalState)  ),
            this,      SLOT(    GlobalStateChange(fantasybit::GlobalState)        ));


    connect( that, SIGNAL   (  NewWeek(int)    ),
            this,      SLOT     (  NewWeek(int)     ));
    connect( that, SIGNAL   (  GameStart(string)    ),
            this,      SLOT     (  GameStart(string)     ));
    connect( that, SIGNAL   (  GameOver(string)    ),
            this,      SLOT     (  GameOver(string)     ));
    connect( that, SIGNAL   (  onControlMessage(QString)    ),
            this,      SLOT     (  onControlMessage(QString)     ));

    QObject::connect(this,SIGNAL(OnClaimName(QString)),that,SLOT(OnClaimName(QString)));

    connect( this, SIGNAL(OnUseName(QString)),
             that, SLOT(UseName(QString)));

    connect( this, SIGNAL(doNameCheck(QString)),
             that, SLOT(nameCheck(QString)));

    connect( that, SIGNAL(nameAvail(QString &,bool)),
             this, SLOT(nameAvail(QString &,bool)));

    connect( that, SIGNAL(NewFantasyName(fantasybit::FantasyNameBal)),
             this, SLOT(NewFantasyName(fantasybit::FantasyNameBal)));

    connect( that, SIGNAL(AnyFantasyNameBalance(fantasybit::FantasyNameBal)),
             this, SLOT(AnyFantasyNameBalance(fantasybit::FantasyNameBal)));

    connect( that, SIGNAL(Height(int)),
             this, SLOT(Height(int)));

    connect( that, SIGNAL(BlockNum(int)),
             this, SLOT(BlockNum(int)));


    connect( that, SIGNAL(FinishedResults()),
             this, SLOT(OnFinishedResults()));

//    connect( that, SIGNAL(GotMarketSnaps()),
//             this, SLOT(OnGotMarketSnaps()));
//    return that;
}

Mediator *Mediator::instance() {
    if (myInstance == NULL) {
        myInstance = new Mediator();
    }
    return myInstance;
}

QString Mediator::importMnemonic(const QString &importStr) {
    auto ret = mGateway->dataService->importMnemonic(importStr.toStdString());

    qDebug() << " try import";
    if ( ret.status() == MyNameStatus::confirmed) {
        emit importSuccess(ret.name().data(),true);
//        useName(ret.name().data());
        useName(ret.name().data());
        return ret.name().data();
    }
    else {
        emit importSuccess(ret.name().data(),false);
        return "";
    }
//    auto pk = m_fantasy_agent.startImportMnemonic(importStr.toStdString());
//    if ( pk == "" )
//        return "";

//    auto iter = m_myPubkeyFname.find(pk);
//    if ( iter != end(m_myPubkeyFname)) {
//        if ( iter->second == "" )
//            pk2fname(pk.data());
//        else
//            usingFantasyName(iter->second.data());

//        return pk.data();
//    }

//    m_myPubkeyFname[pk] = "";
//    pk2fname(pk.data());
//    return pk.data();
    return ret.name().data();
}

void Mediator::signPlayer(const QString &name)  {
    qDebug() << " emit claim name " << name;
    emit OnClaimName(name);
//    emit OnClaimName(name);

//    m_fantasy_agent.signPlayer(name.toStdString());
//    NameTrans nt{};
//    nt.set_public_key(m_fantasy_agent.pubKeyStr());
//    nt.set_fantasy_name(name.toStdString());


//    Transaction trans{};
//    trans.set_version(Commissioner::TRANS_VERSION);
//    trans.set_type(TransType::NAME);
//    trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nt);
//    SignedTransaction sn = m_fantasy_agent.makeSigned(trans);
//    auto txstr = sn.SerializeAsString();

//    auto pk = Commissioner::str2pk(nt.public_key());
//    pb::sha256 digest(sn.id());
//    if ( !Commissioner::verify(Commissioner::str2sig(sn.sig()),digest,pk) )
//        qDebug() << " bad signature ";
//    else
//        qDebug() << " good signature ";

//    QByteArray qb(txstr.data(),(size_t)txstr.size());

//    qDebug() << " mediator signPlayer" << name << sn.DebugString().data();
//    m_txsocket.sendBinaryMessage(qb);

//    usingFantasyName(m_fantasy_agent.currentClient().data());
//    m_myPubkeyFname[m_fantasy_agent.pubKeyStr()] = "";
//    m_lastSignedplayer = m_fantasy_agent.pubKeyStr();
//    signPlayerStatus.start();
}

void Mediator::getSignedPlayerStatus() {
    //doPk2fname(m_lastSignedplayer);
}

void Mediator::useName(const QString &name) {
    qDebug() << " Mediator::useName " << name;

    emit OnUseName(name);
}

QString Mediator::init() {
    connect( mOGateway, SIGNAL   ( LiveGui(fantasybit::GlobalState)     ),
            this,      SLOT     (  LiveGui(fantasybit::GlobalState)     ));

    ready();

    connect(&myGamesSelectionModel,SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this,SLOT(selectionChanged(QItemSelection, QItemSelection)));

    connect(&myPrevGamesSelectionModel,SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this,SLOT(selectionPrevChanged(QItemSelection, QItemSelection)));

    return myFantasyName.data(); //todo check if missed live event
}

QString Mediator::getSecret() {
    auto sec = mGateway->dataService->exportMnemonic(myFantasyName);
    return sec.data();
}

void Mediator::doTrade(QString playerid, QString symbol, bool isbuy, const qint32 price, qint32 size) {
    if ( otherFantasyName != "" )
        return;

    ExchangeOrder eo;
    eo.set_playerid(playerid.toStdString());
    eo.set_symbol(symbol.toStdString());
    eo.set_type(ExchangeOrder::NEW);

    OrderCore core;
    core.set_buyside(isbuy);
    core.set_size(size);
    core.set_price(price);

#ifdef TRACE
    qDebug() << symbol << "doTrade NewOrder " << core.DebugString().data();
#endif
    eo.mutable_core()->CopyFrom(core);
    emit NewOrder(eo);
}

void Mediator::doTransfer(const qint32 amount, QString toname) {
    if ( otherFantasyName != "" )
        return;

    qDebug() << " do transfer" << amount << "from: " << myFantasyName.data() << "to: " << toname;
    TransferTrans tt;
    tt.set_from(myFantasyName);
    tt.set_to(toname.toStdString());
    tt.set_amount(amount);

    emit NewTransfer(tt);
}

void Mediator::doSwap(quint64 qty, quint64 rate, bool isask, QString with, quint64 min) {

    qDebug() << " doSwap " << qty << "rate: " << rate << isask << with << min;

    if ( isask && with == "" ) {
        SwapAsk ask;
        ask.set_rate(fantasybit::satRateSwap(rate));
        ask.set_satoshi_min(fantasybit::minSatQtySwap(min));

        qint64 mn = m_pMyFantasyNameBalance->get_net();
        if ( mn <= 0 ) {
            qDebug() << " zero balance ";
            return;
        }

        if ( qty == 0 )
            qty = static_cast<quint64>(mn);

        qDebug() << " doSwap1 " << ask.DebugString().data();

        if ( qty < fantasybit::minFBSwapQty(ask.rate(),ask.satoshi_min()) ) {
            qDebug() << " not enough FB for Swap " << qty;
            return;
        }

        ask.set_fb_qty(qty);
        ask.set_satoshi_max( fantasybit::maxSatQtySwap(qty,rate) );

        if ( ask.satoshi_max() < ask.satoshi_min() ) {
            qDebug() << " max < min " << ask.DebugString().data();
            return;
        }

        emit NewSwapAsk(ask);
    }
    else if ( isask && with != "" ) {

        const SwapBuyer &sb = mGateway->dataService->GetSwapBid(with);
        qDebug() << sb.bid.DebugString().data();

        if ( sb.is_pending ) {
            qDebug() << "Swap buy order already filled?";
            return;
        }

        if ( with.toStdString() != sb.fname ) {
            qDebug() << "Fatal With != fname" << with << sb.fname.data();
            return;
        }

        //sanity qty checks
        qint64 mn = m_pMyFantasyNameBalance->get_net();
        if ( mn <= 0 ) {
            qDebug() << " zero balance ";
            return;
        }

        if ( qty == 0 )
            qty = static_cast<quint64>(mn);

        auto minfb = fantasybit::FBSwapQty(sb.bid.rate(),sb.bid.satoshi_min());
        auto maxfb = fantasybit::FBSwapQty(sb.bid.rate(),sb.bid.satoshi_max());

        int addfreebee = 0;
        if ( sb.bid.rate() * maxfb < sb.bid.satoshi_max())
            addfreebee = sb.bid.satoshi_max() - ( sb.bid.rate() * maxfb );

        if ( minfb < fantasybit::minFBSwapQty(sb.bid.rate(),sb.bid.satoshi_min()) )  {
            qDebug() << " bad data " ;
            return;
        }

        if ( qty > maxfb )
            qty = std::min(static_cast<quint64>(mn),maxfb);

        if ( qty < minfb ) {
            qDebug() << " not enough FB for Swap " << qty;
            return;
        }

        SwapFill sf;
        sf.mutable_swapbid()->CopyFrom(sb.bid);
        sf.set_satoshi_fee(fantasybit::MIN_SAT_BYTE_TX_FEE);
        sf.set_fb_qty(qty);
        sf.set_counterparty(sb.fname);

        int minconfirms = 1;
        auto *item = mFantasyNameBalModel.getByUid(with);
        if ( item == nullptr ) {
            qDebug() << " cant find fname" << with;
            return;
        };

        string btcaddr = item->get_btcaddr().toStdString();
        for ( const auto &utxo : sb.bid.utxos().utxo() ) {
            if ( minconfirms < BitcoinUtils::checkUtxo(utxo,btcaddr) ) {
                qDebug() << minconfirms << " cant find UTXO (confirms) " << utxo.DebugString().data();
                return;
            }
        }

        if ( sb.bid.utxos().utxo().size() != 1) {
            qDebug() << " not yet";
            return;
        }

        auto myinput = sb.bid.utxos().utxo().Get(0);

        string input, inputscript;
        BitcoinUtils::createInputsFromUTXO(myinput,input,inputscript);

        QString tx_template =
                BitcoinUtils::createTX(myinput,input,inputscript,
                               m_pMyFantasyNameBalance->get_btcaddr().toStdString(),
                               fantasybit::SATSwapQty(sf.swapbid().rate(),sf.fb_qty()) + addfreebee,
                               sf.satoshi_fee(),
                               btcaddr).data();

        auto tosigntraw = tx_template.arg(inputscript.data()).toStdString();
        tosigntraw.append(SIGHASH_ALL_4);
        auto dblhash = pb::hashit(pb::hashfromhex(tosigntraw));
        sf.set_hash_to_sign(dblhash.str());

        qDebug() << sf.DebugString().data();
        emit NewSwapFill(sf);
    }
    else if ( !isask && with == "" ) {
        SwapBid bid;

        bid.set_rate(fantasybit::satRateSwap(rate));
        bid.set_satoshi_min(fantasybit::minSatQtySwapFromRate(min,bid.rate()));

        quint64 bb = m_bitcoinBalance;
        if ( bb == 0 ) {
            qDebug() << " zero bitcoin balance ";
            return;
        }

        if ( qty == 0 || qty > bb)
            qty = bb;

        if ( qty < bid.satoshi_min() ) {
            qDebug() << " not enough BTC for Swap " << qty;
            return;
        }

        bid.set_satoshi_max( fantasybit::maxSatQtySwapFromRate(qty,rate) );

        if ( bid.satoshi_max() < bid.satoshi_min() ) {
            qDebug() << " max < min " << bid.DebugString().data();
            return;
        }

        if ( !BitcoinUtils::getUtxos(*bid.mutable_utxos(),
                                     m_pMyFantasyNameBalance->get_btcaddr().toStdString(),
                                     bid.satoshi_max(),bid.satoshi_min())) {
            qDebug() << " not enough utxos";
            return;
        }
        else
            qDebug() << bid.DebugString().data();


        emit NewSwapBid(bid);
    }
}



void Mediator::doSwapSent(const fantasybit::SwapOrder  &so) {
    qDebug() << "twitch11 doSwapSent  " << so.DebugString().data();

    //if I am alice
    if (so.fname() != myFantasyName )
        return;

    //YES I CAN BUY FANTASYBIT!!
    const SwapFill &sf = mGateway->dataService->GetSwapFill(so.fname().data(),
                                                            so.ref().data());

    //verify the hash to sign
    bool dosanity = true;
    if ( dosanity ) {
        //sanity qty checks
        auto *item = mFantasyNameBalModel.getByUid(so.ref().data());
        if ( item == nullptr ) {
            qDebug() << " cant find fname" << so.ref().data();
            return;
        };

        qint64 mn = item->get_net();
        if ( mn <= 0 ) {
            qDebug() << " zero balance ";
            return;
        }

        quint64 qty = sf.fb_qty();
        if ( qty == 0 )
            qty = static_cast<quint64>(mn);

        auto minfb = fantasybit::FBSwapQty(sf.swapbid().rate(),sf.swapbid().satoshi_min());
        auto maxfb = fantasybit::FBSwapQty(sf.swapbid().rate(),sf.swapbid().satoshi_max());

        int addfreebee = 0;
        if ( sf.swapbid().rate() * maxfb < sf.swapbid().satoshi_max())
            addfreebee = sf.swapbid().satoshi_max() - ( sf.swapbid().rate() * maxfb );

        if ( minfb < fantasybit::minFBSwapQty(sf.swapbid().rate(),sf.swapbid().satoshi_min()) )  {
            qDebug() << " bad data " ;
            return;
        }

        if ( qty > maxfb )
            qty = std::min(static_cast<quint64>(mn),maxfb);

        if ( qty < minfb ) {
            qDebug() << " not enough FB for Swap " << qty;
            return;
        }

        if ( sf.satoshi_fee() < fantasybit::MIN_SAT_BYTE_TX_FEE ||
             sf.fb_qty() != qty ) {
            qDebug() << " bad swapfil data ";
            return;
        }

        int minconfirms = 0;
        string btcaddr = m_pMyFantasyNameBalance->get_btcaddr().toStdString();
        for ( const auto &utxo : sf.swapbid().utxos().utxo() ) {
            if ( minconfirms < BitcoinUtils::checkUtxo(utxo,btcaddr) ) {
                qDebug() << minconfirms << " cant find UTXO (confirms) " << utxo.DebugString().data();
                return;
            }
        }

        if ( sf.swapbid().utxos().utxo().size() != 1) {
            qDebug() << " not yet";
            return;
        }

        auto myinput = sf.swapbid().utxos().utxo().Get(0);
        string input, inputscript;
        BitcoinUtils::createInputsFromUTXO(myinput,input,inputscript);

        QString tx_template =
                BitcoinUtils::createTX(myinput,input,inputscript,
                               item->get_btcaddr().toStdString(),
                               fantasybit::SATSwapQty(sf.swapbid().rate(),sf.fb_qty()) + addfreebee,
                               sf.satoshi_fee(),
                               btcaddr).data();


        auto tosigntraw = tx_template.arg(inputscript.data()).toStdString();
        tosigntraw.append(SIGHASH_ALL_4);
        auto dblhash = pb::hashit(pb::hashfromhex(tosigntraw));
        auto hash_to_sign = dblhash.str();
        if ( hash_to_sign != sf.hash_to_sign() ) {
            qDebug() << "BAD HASH TO SIGN!!" << hash_to_sign.data() << sf.hash_to_sign();
            return;
        }
        else
            qDebug() << "Ok - Good hash_to_sign";
    }
    // create SwapSent TX
    SwapSent ss;
    ss.mutable_swapfill()->CopyFrom(sf);

    // send SwapSent TX tp be signed
    emit NewSwapSent(ss);

    // ?? send bitcoin tx to bitcon blockchain
    // BitcoinApi::sendrawTx(mTx);

    qDebug() << "doSwapSent2  " << sf.DebugString().data();

    //https://bitcoin.stackexchange.com/questions/3374/how-to-redeem-a-basic-tx
}


//void Mediator::doSwap() {

//    SwapAsk ask;

//}

void Mediator::doCancel(qint32 id) {
    if ( otherFantasyName != "" )
        return;

    ExchangeOrder eo;
    eo.set_type(ExchangeOrder::CANCEL);

    eo.set_cancel_oref(id);

#ifdef TRACE
    qDebug() << id << "doCancel";
#endif
    emit NewOrder(eo);
}

void Mediator::OnMarketTicker(fantasybit::MarketTicker mt, int32_t blocknum) {
    if ( mt.symbol() == "" )
        return;
#ifdef TRACE
    qDebug() << "Mediator OnMarketTicker " << mt.DebugString().data();
#endif

    if ( !getQuoteModel(mt.symbol()).Update(&mt,blocknum) ) {
        addQuoteSymbol(mt.symbol());

        if ( !getQuoteModel(mt.symbol()).Update(&mt,blocknum) )
            qDebug() << "error OnMarketTicker" << mt.DebugString().data();
    }

}

void Mediator::OnTradeTick(fantasybit::TradeTic* tt) {
    if ( tt->symbol() == "" )
        return;
#ifdef TRACE
    qDebug() << "Mediator TradeTic " << tt->DebugString().data();
#endif

    if ( !getQuoteModel(tt->symbol()).Update(tt) ) {
        addQuoteSymbol(tt->symbol());

        if ( !getQuoteModel(tt->symbol()).Update(tt))
            qDebug() << "error OnMarketTicker" << tt->DebugString().data();
    }
}

void Mediator::OnDepthDelta(fantasybit::DepthFeedDelta* dfd) {
    if ( dfd->symbol() == "" )
        return;
#ifdef TRACE
    qDebug() << "level2 OnDepthDelta " << dfd->DebugString().data();
#endif

     if ( !getQuoteModel(dfd->symbol()).Update(dfd)) {
         addQuoteSymbol(dfd->symbol());

         if ( !getQuoteModel(dfd->symbol()).Update(dfd) )
             qDebug() << "error OnMarketTicker" << dfd->DebugString().data();
     }
}

void Mediator::OnNewPos(fantasybit::FullPosition fp) {
    qDebug() << "level2 Mediator::OnNewPos " << fp.pos.ToString().data() <<
                fp.symbol.data() << fp.fname.data();

    if ( fp.fname != myFantasyName )
        return;

    bool isweekly = isWeekly(fp.symbol);
    auto it = getQuoteModel(isweekly).getByUid(fp.symbol.data());
    auto tit = isweekly ? mTradingPositionsModel.getOrCreate(fp.symbol.data()) :
                       mROWTradingPositionsModel.getOrCreate(fp.symbol.data());

    if ( tit == nullptr )
        return;

    double holdpnl = tit->get_openpnl();
    tit->Update(fp.pos);

    int netqty = tit->get_netqty();
    double avg = 0;
    double pnl = 0;
    if ( netqty ==0 ) {
        pnl = tit->get_netprice() * tit->get_multiplier();
    }
    else if ( it != nullptr ) {
        int price = (netqty > 0) ? it->get_bid() : it->get_ask();
        if ( price == 0 )
            price = (netqty > 0) ? 0 : ( isweekly ? 41 : 401) ;

//        if ( price == 0 )
//            price = it->get_lastprice();
//        if ( price == 0 )
//            pnl = 0;
//        else
        pnl = tit->get_multiplier() * ((price * netqty) + tit->get_netprice());
        avg = tit->get_netprice()  / (netqty * -1.0);
    }

    it->setmyavg(avg);
    it->setmyposition(netqty);
    it->setmypnl(pnl);
    tit->setopenpnl(pnl);

    qDebug() << tit->get_symbol() << " setmyposition " << netqty;

    auto &tpm = isweekly ? mTradingPositionsModel : mROWTradingPositionsModel;
    double newtotal = tpm.get_totalopenpnl() + (pnl - holdpnl);
    tpm.settotalopenpnl(newtotal);
    m_pMyFantasyNameBalance->updatePnl(mGateway->dataService->GetOpenPnl(myFantasyName));

}

void Mediator::MyPosPriceChange(PlayerQuoteSliceModelItem* it) {
    qDebug() << " MyPosPriceChange " << it->get_symbol();

    bool isweekly = fantasybit::isWeekly(it->get_symbol());
    auto tit = isweekly ? mTradingPositionsModel.getByUid(it->get_symbol()) :
                       mROWTradingPositionsModel.getByUid(it->get_symbol());

    if ( tit == nullptr ) {
        qDebug() << " MyPosPriceChange error tit == nullptr  Mediator::MyPosPriceChange";
        return;
    }

    double holdpnl = tit->get_openpnl();
    int netqty = tit->get_netqty();
    double avg = 0;
    double pnl = 0;
    if ( netqty == 0 ) {
        pnl = tit->get_netprice() * tit->get_multiplier();
    }
    else if ( it != nullptr ) {
        int price = (netqty > 0) ? it->get_bid() : it->get_ask();
        if ( price == 0 )
            price = (netqty > 0) ? 0 : ( isweekly ? 41 : 401) ;

//        if ( price == 0 )
//            price = it->get_lastprice();
//        if ( price == 0 )
//            pnl = 0;
//        else
        pnl = tit->get_multiplier() * ((price * netqty) + tit->get_netprice());
        avg = tit->get_netprice()  / (netqty * -1.0);
    }

    it->setmypnl(pnl);
    tit->setopenpnl(pnl);

    qDebug() << tit->get_symbol() << " setmyposition " << netqty;

    auto &tpm = isweekly ? mTradingPositionsModel : mROWTradingPositionsModel;

    double newtotal = tpm.get_totalopenpnl()
                + (pnl - holdpnl);

    tpm.settotalopenpnl(newtotal);
    m_pMyFantasyNameBalance->updatePnl(mGateway->dataService->
                                     GetOpenPnl(m_pMyFantasyNameBalance->get_name().toStdString()));
}

void Mediator::OnNewOO(fantasybit::FullOrderDelta fo) {
    qDebug() << "level2 Trading::Mediator " << fo.fname << fo.symbol.data () << fo.openorder.DebugString().data();

    if ( fo.fname != myFantasyName )
        return;

    (isWeekly(fo.symbol) ? mTradingPositionsModel : mROWTradingPositionsModel).Update(fo);
}


Mediator *Mediator::myInstance;

void Mediator::addTradingSymbol(const QString &symbol, bool isweekly) {
    qDebug() << "mediator addTradingSymbol " << symbol <<  isweekly;
    auto it = mPlayerSymbolsModel.getByUid(symbol);
    if ( it == nullptr ) {
        qDebug() << "error addTradingSymbol" << symbol;
    }
    else {
#ifdef SUPERBOWL52LIVE
        mPlayerQuoteSliceModel.UpdateSymbols(it,m_blocknum,"17w21");
#else
        getQuoteModel(isweekly).UpdateSymbols(it,m_blocknum,
                                              isweekly ? mWeeklySuffix : mSeasonSuffix);
#endif
    }
}

}


//    if ( !m_fantasy_agent.HaveClient() ) {
//        qDebug() << "error no CLient";

//        return;
//    }
//    ExchangeOrder eo;
//    eo.set_type(ExchangeOrder::CANCEL);
//    eo.set_cancel_oref(id);

//#ifdef TRACE
//    qDebug() << "cancelTrade id " << id;
//#endif

//    Transaction trans{};
//    trans.set_version(Commissioner::TRANS_VERSION);
              //    trans.set_type(TransType::EXCHANGE);
              //    trans.MutableExtension(ExchangeOrder::exchange_order)->CopyFrom(eo);

              //    SignedTransaction sn = m_fantasy_agent.makeSigned(trans);

              //    auto pk = m_fantasy_agent.pubKey();
              //    pb::sha256 digest(sn.id());
              //    if ( !Commissioner::verify(Commissioner::str2sig(sn.sig()),digest,pk) )
              //        qDebug() << " bad signature ";
              //    else
              //        qDebug() << " good signature ";

              //    auto txstr = sn.SerializeAsString();

              //    QByteArray qb(txstr.data(),(size_t)txstr.size());

              //    qDebug() << " mediator doTestTrade";
              //    m_txsocket.sendBinaryMessage(qb);

              //    depthCount = 0;
              //    depthBackup-=10;
              //    if ( depthBackup < 0 ) {
              //        depthBackup = 0;
              //        depthInterval = 1000;
              //    }
              //    else
              //        depthInterval = 1000 * (depthBackup / 5);

              //    if ( depthInterval < 1600 ) {
              //       getDepthRep();
              //       depthInterval = 1000;
              //    }
              //    polldepth.start(depthInterval);
              ////    qDebug() << " doTrade depthInterval " << depthInterval << " depthBackup " << depthBackup << " depthCount " << depthCount;
              ////    getOrderReq(FantasyName::name_hash(m_fantasy_agent.currentClient()));
              ////    getOrderPos();


//#include <QStandardPaths>
//std::string Mediator::lastYearPath() {
//    std::string ret = "";
//#ifdef Q_OS_WIN32
//   QSettings settings(QStringLiteral("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{79913283-A35B-4274-927C-1B52D286D939}_is1"), QSettings::NativeFormat);
//    QSettings settings(QStringLiteral("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), QSettings::NativeFormat);

//    qDebug() << "settings " << settings.status();
//    qDebug() << "child keys = " <<  settings.childKeys();
//    for ( auto k : settings.allKeys() )
//        qDebug() << "HKLM " << k;
//    QString path = settings.value( "InstallLocation" ).toString();
//    ret = path.toStdString();
//#ifdef Q_OS_MAC
//    QString path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/tradingfootball/");
//    ret = path.toStdString();
//#endif

//    "HKCU\\SOFTWARE\\Classes\\VirtualStore\\MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{79913283-A35B-4274-927C-1B52D286D939}_is1"

//    return ret;
//}

//HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{79913283-A35B-4274-927C-1B52D286D939}_is1
