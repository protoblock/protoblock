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
    dummyPlayerQuoteSliceModelItem(fantasybit::ROWMarket::default_instance()),
    m_pPlayerQuoteSliceModelItem(&dummyPlayerQuoteSliceModelItem),
    mDepthMarketModel{},
    m_pDepthMarketModel(&mDepthMarketModel),
    m_pGlobalOpenOrdersModel(&dummyOpenOrdersModel),
    mFantasyNameBalModel(this,QByteArray(),{"name"}),
    m_pFantasyNameBalModel(&mFantasyNameBalModel),
    mGoodNameBalModel{this,QByteArray(),{"name"}},
    m_pGoodNameBalModel(&mGoodNameBalModel),
    mTradingPositionsModel{this,QByteArray(),{"symbol"}},
    m_pTradingPositionsModel(&mTradingPositionsModel),
    myGamesSelectionModel{},
    myPrevGamesSelectionModel{},
    m_pQItemSelectionModel(&myGamesSelectionModel),
    m_pPrevQItemSelectionModel(&myPrevGamesSelectionModel),
    m_pLeaderBoardSortModel(new SortFilterProxyModel),
    m_pResultSelectedModel(new SortFilterProxyModel),
    m_blocknum(0),
    m_height(0),
    dummyFantasyNameBalModelItem(),
    m_pMyFantasyNameBalance{&dummyFantasyNameBalModelItem} {

    fnames = {"fname1", "fname2","fname3", "fname4", "fname5"};

    fnameindex = 0;
    //leader models
    m_pLeaderBoardSortModel->setSourceModel(m_pFantasyNameBalModel);
    m_pLeaderBoardSortModel->setSortRole("lastupdate");//mPlayerProjModel.roleForName("pos"));
    m_pLeaderBoardSortModel->setDynamicSortFilter(true);

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
    m_liveSync ="Sync";
    m_seasonString = "";

    //player selection
    m_pProjectionsViewFilterProxyModel =  new ProjectionsViewFilterProxyModel(m_pWeeklyScheduleModel,&myGamesSelectionModel);
    m_pProjectionsViewFilterProxyModel->setSourceModel(&mPlayerProjModel);

    m_pProjectionsViewFilterProxyModel->setSortRole("pos");//mPlayerProjModel.roleForName("pos"));
    m_pProjectionsViewFilterProxyModel->setDynamicSortFilter(false);

    //results
    m_pResultsViewFilterProxyModel =  new ResultsViewFilterProxyModel(m_pPreviousWeekScheduleModel,&myPrevGamesSelectionModel);
    m_pResultsViewFilterProxyModel->setSourceModel(&mPlayerResultModel);

    m_pResultsViewFilterProxyModel->setSortRole("result");//mPlayerProjModel.roleForName("pos"));
    m_pResultsViewFilterProxyModel->setDynamicSortFilter(true);



    //trading
//    m_pPlayerQuoteSliceModel = &m_pPlayerQuoteSliceModel;
    m_pPlayerQuoteSliceViewFilterProxyModel =  new PlayerQuoteSliceViewFilterProxyModel(this);
    m_pPlayerQuoteSliceViewFilterProxyModel->setSourceModel(&mPlayerQuoteSliceModel);
    m_pPlayerQuoteSliceViewFilterProxyModel->setSortRole("blocknum");
    m_pPlayerQuoteSliceViewFilterProxyModel->setDynamicSortFilter(true);


    m_useSelected = true;
    m_busySend = false;
    m_thisWeekPrev = false;


    m_pResultSelectedModel->setSourceModel(&dummyResultSelectedModel);
    m_pResultSelectedModel->setSortRole("award");
    m_pResultSelectedModel->setDynamicSortFilter(true);

//    setcontrolMessage("<html><style type=\"text/css\"></style>Companion Protoblock APP now available on " \
//                      "<a href=\"https://itunes.apple.com/us/app/protoblock-2016/id1133758199?ls=1&mt=8\">iTunes</a>" \
//                      " and <a href=\"https://play.google.com/store/apps/details?id=org.proto.protoblock\">Google Play!</a></html>");


    setcontrolMessage("<html><style type=\"text/css\"></style>Fantasy Ticker Feed now live on Twitter " \
                      "<a href=\"https://twitter.com/prototicker\">@prototicker</a></html>");

    connect(&mPlayerQuoteSliceModel,&PlayerQuoteSliceModel::MyPosPriceChange,
            this, &Mediator::MyPosPriceChange);
//    connect(&tradeTesting, &QTimer::timeout, this, &Mediator::tradeTestingTimeout );
}

void Mediator::NameStatus(fantasybit::MyFantasyName myname) {
    qDebug() << myFantasyName.data() << " mediator namestatus " << myname.DebugString().data();

    if ( myname.name() == myFantasyName )
        return;

    if ( nullptr != mGoodNameBalModel.getByUid(myname.name().data()) ) {
//        if ( myFantasyName != "" ) return;
    }
    else {
        auto it = mFantasyNameBalModel.getByUid(myname.name().data());
        if ( it != nullptr) {
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

    update_pMyFantasyNameBalance(mGoodNameBalModel.getByUid(myname.name().data()));
    myFantasyName = myname.name();
    qDebug() << "Mediator  emitting using fantasy name " << myname.name().data();
    myGamesSelectionModel.reset();
    emit usingFantasyName(myname.name().data());
    updateOnChangeFantasyName();
    set_busySend(false);
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

        auto it = mFantasyNameBalModel.getByUid(m.name().data());
        if ( it != nullptr) {
            mGoodNameBalModel.insert(mGoodNameBalModel.size(),it);
            auto it2 = mGoodNameBalModel.getByUid(m.name().data());
        }
    }

    qDebug() << " namename wins " << heighest << hname.data();
    if ( myFantasyName == "" && hname != "" )
        useName(hname.data());

}

void Mediator::NameBal(fantasybit::FantasyNameBal fnb) {
    FantasyNameBalModelItem *item = (FantasyNameBalModelItem *)mGoodNameBalModel.get(fnb.name().data());
    if ( !item || item->get_name() != fnb.name().data()) {
        //mGoodNameBalModel.insert(mGoodNameBalModel.size(),FantasyNameBalModelItem(fnb));
    }
    else {
        item->update(fnb);
        mFantasyNameBalModel.update(item);
    }

}

void Mediator::PlayerStatusChange(pair<string, PlayerStatus> in) {}

void Mediator::GlobalStateChange(GlobalState gs) {
    if ( gs.week() > 0 && gs.week() < 18) {
        if ( amLive && gs.week() > m_theWeek) {
            updateWeek();
        }
        if ( m_thePrevWeek == m_theWeek)
            setthePrevWeek(gs.week());

        settheWeek(gs.week());
        set_busySend(false);
    }
}

void Mediator::LiveGui(GlobalState gs) {

    qDebug() << "Mediator received Livegui ";
    if ( !amLive ) {
        amLive = true;
        m_season = gs.season();
        settheWeek(gs.week());
        setthePrevWeek(gs.week());
        setliveSync("Live");
        setseasonString(gs.state() == GlobalState_State_OFFSEASON ? "Off Season" : "NFL Season");
        updateWeek();
//        tradeTesting.start(5000);
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


        if ( m_theWeek > 0  && m_theWeek < 17) {
            std::map<int,std::vector<std::pair<fantasybit::GameInfo,fantasybit::GameStatus_Status>>> sorted;
            fantasybit::WeeklySchedule weekly = mGateway->dataService->GetWeeklySchedule(m_theWeek);
            for ( auto &gi : weekly.games()) {
                auto status = mGateway->dataService->GetGameStatus(gi.id());
                std::vector<std::pair<fantasybit::GameInfo,fantasybit::GameStatus_Status>> &vec =
                        sorted[gi.time()];
                vec.emplace_back(std::make_pair(gi,status.status()));
            }

            for ( auto tvec : sorted ) {
                qDebug() << " tvec sorted " << tvec.first;
                for ( auto p : tvec.second ) {
                    qDebug() << " p tvec " << p.second << p.first.DebugString().data();
                    if ( p.second == GameStatus_Status_CLOSED )
                        m_pWeekClosedScheduleModel->append(new WeeklyScheduleModelItem(p.first,p.second,this));
                    else
                        m_pWeeklyScheduleModel->append(new WeeklyScheduleModelItem(p.first,p.second,this));
                }
            }
            qDebug() << " done sorted ";


//            if ( status.status() == GameStatus_Status_CLOSED )
//                m_pWeekClosedScheduleModel->append(new WeeklyScheduleModelItem(gi,status.status(),m_pWeekClosedScheduleModel));
//            else
//                m_pWeeklyScheduleModel->append(new WeeklyScheduleModelItem(gi,status.status(),m_pWeeklyScheduleModel));


            const auto &vgr = mGateway->dataService->GetCurrentWeekGameRosters();
            mPlayerProjModel.updateRosters(vgr,mGateway->dataService);

            const auto &vms = mGateway->dataService->GetCurrentMarketSnaps();
            qDebug() << "  vms " << vms.size();
            mPlayerQuoteSliceModel.Update(vms,mPlayerProjModel);
//            OnGotMarketSnaps();

#ifdef NOTDEF
            int i = 0;
            for( auto it : mPlayerProjModel ) {
                if ( i++ > 20 )  break;
                m_pPlayerQuoteSliceModel->append(new PlayerQuoteSliceModelItem(*it));
            }
#endif
            set_thisWeekPrev(m_pWeekClosedScheduleModel->count() > 0);

            if (myFantasyName != "" )
                updateOnChangeFantasyName();

            m_pProjectionsViewFilterProxyModel->invalidate();
        }
        updateLiveLeaders();
    }
}

void Mediator::updateOnChangeFantasyName() {
    updateCurrentFantasyPlayerProjections();
    updateCurrentFantasyPlayerOrderPositions();
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
}

void Mediator::updateCurrentFantasyPlayerOrderPositions() {
    update_pGlobalOpenOrdersModel(&dummyOpenOrdersModel);
    for ( auto tit : mTradingPositionsModel ) {
        auto it = mPlayerQuoteSliceModel.getByUid(tit->get_symbol());
        if ( it == nullptr) continue;
        it->setmyavg(0);
        it->setmyposition(0);
        it->setmypnl(0);
    }


//    ui->posQty->setValue(0);
//    ui->posAvgPrice->setValue(0);
//    ui->posOpenPnl->setValue(0);
//    myPositionsName = myFantasyName;

    auto myorderpositions = mGateway->dataService->GetOrdersPositionsByName(myFantasyName);
    mTradingPositionsModel.updateAllOrders(myFantasyName,myorderpositions);
//    mTradingPositionsModel.setfantasyname(myFantasyName.data());


#ifdef TRACE
    qDebug() << "level2 Trading SetMyPositions" << myFantasyName.data() << myorderpositions.size();
#endif

    int i = 0;
    double totpnl = 0.0;
//    qDebug() << ++i << " here ";
    for ( auto tit : mTradingPositionsModel ) {
//        qDebug() << i << "loop here ";

        auto it = mPlayerQuoteSliceModel.getByUid(tit->get_symbol());
        if ( it == nullptr) continue;
        int netqty = tit->get_netqty();
//        qDebug() << ++i << " here ";

        double avg = 0;
        double pnl = 0;
        if ( netqty == 0 ) {
            pnl = tit->get_netprice() * 100.0;
        }
        else {
            int price = (netqty > 0) ? it->get_bid() : it->get_ask();
            if ( price == 0 )
                price = it->get_lastprice();

//            if ( netqty > 0 ) {
//                if ( (price = it->get_bid()) == 0 ) {
//                    if ( it->get_ask() != 0 && it->get_ask() < it->get_lastprice() )
//                        price = it->get_ask();
//                    else
//                        price = it->get_lastprice();
//                }
//            }
//            else {
//                if ( (price = it->get_ask()) == 0 ) {
//                    if ( it->get_bid() != 0 && it->get_bid() < it->get_lastprice() )
//                        price = it->get_ask();
//                    else
//                        price = it->get_lastprice();
//                }
//            }

            if ( price == 0 )
                pnl = 0.0;
            else
                pnl = 100.0 * ((price * netqty) + tit->get_netprice());
            avg = tit->get_netprice()  / (netqty * -1.0);
//            qDebug() << ++i << " here ";

        }
//        qDebug() << ++i << " here ";


        //mTradingPositionsModel.UpdatePnl(tit,it->get_bid(),it->get_ask());
        it->setmyavg(avg);
        it->setmyposition(netqty);
        qDebug() << tit->get_symbol() << " setmyposition " << netqty;
        it->setmypnl(pnl);
        tit->setopenpnl(pnl);
        totpnl += pnl;
//        qDebug() << ++i << " here ";

    }
    mTradingPositionsModel.settotalopenpnl(totpnl);
//    qDebug() << ++i << " here ";

}

void Mediator::NewWeek(int week) {
    set_busySend(false);
//    set_thisWeekPrev = false;

    if ( m_thePrevWeek == m_theWeek)
        setthePrevWeek(week);

    settheWeek(week);
    if ( amLive ) {
        updateLiveLeaders();
    }
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

    connect( that, SIGNAL(GotMarketSnaps()),
             this, SLOT(OnGotMarketSnaps()));
//    return that;
}

Mediator *Mediator::instance() {
    if (myInstance == NULL) {
        myInstance = new Mediator();
    }
    return myInstance;
}

QString Mediator::importMnemonic(const QString &importStr) {
    qDebug() << " try import";
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

void Mediator::doTrade(QString symbol, bool isbuy, const qint32 price, qint32 size) {

    ExchangeOrder eo;
    eo.set_playerid(symbol.toStdString());
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

void Mediator::doCancel(qint32 id) {
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

    mPlayerQuoteSliceModel.Update(&mt,blocknum);

}

void Mediator::OnTradeTick(fantasybit::TradeTic* tt) {
    if ( tt->symbol() == "" )
        return;
#ifdef TRACE
    qDebug() << "Mediator TradeTic " << tt->DebugString().data();
#endif

    mPlayerQuoteSliceModel.Update(tt);
}

void Mediator::OnDepthDelta(fantasybit::DepthFeedDelta* dfd) {
    if ( dfd->symbol() == "" )
        return;
#ifdef TRACE
    qDebug() << "level2 OnDepthDelta " << dfd->DebugString().data();
#endif

    mPlayerQuoteSliceModel.Update(dfd);
}

void Mediator::OnNewPos(fantasybit::FullPosition fp) {
    qDebug() << "level2 Mediator::OnNewPos " << fp.pos.ToString().data() <<
                fp.playerid.data() << fp.fname.data();

    if ( fp.fname != myFantasyName )
        return;

    auto it = mPlayerQuoteSliceModel.getByUid(fp.playerid.data());
    auto tit = mTradingPositionsModel.getOrCreate(fp.playerid.data());
    if ( tit == nullptr )
        return;

    double holdpnl = tit->get_openpnl();
    tit->Update(fp.pos);

    int netqty = tit->get_netqty();
    double avg = 0;
    double pnl = 0;
    if ( netqty ==0 ) {
        pnl = tit->get_netprice() * 100.0;
    }
    else if ( it != nullptr ) {
        int price = (netqty > 0) ? it->get_bid() : it->get_ask();
        if ( price == 0 )
            price = it->get_lastprice();
        if ( price == 0 )
            pnl = 0;
        else
            pnl = 100.0 * ((price * netqty) + tit->get_netprice());
        avg = tit->get_netprice()  / (netqty * -1.0);
    }

    it->setmyavg(avg);
    it->setmyposition(netqty);
    it->setmypnl(pnl);
    tit->setopenpnl(pnl);

    qDebug() << tit->get_symbol() << " setmyposition " << netqty;

    double newtotal = mTradingPositionsModel.get_totalopenpnl()
                + (pnl - holdpnl);
    mTradingPositionsModel.settotalopenpnl(newtotal);
}

void Mediator::MyPosPriceChange(PlayerQuoteSliceModelItem* it) {
    qDebug() << " MyPosPriceChange " << it->get_symbol();
    auto tit = mTradingPositionsModel.getByUid(it->get_symbol());
    if ( tit == nullptr ) {
        qDebug() << " MyPosPriceChange error tit == nullptr  Mediator::MyPosPriceChange";
        return;
    }

    double holdpnl = tit->get_openpnl();
    int netqty = tit->get_netqty();
    double avg = 0;
    double pnl = 0;
    if ( netqty == 0 ) {
        pnl = tit->get_netprice() * 100.0;
    }
    else if ( it != nullptr ) {
        int price = (netqty > 0) ? it->get_bid() : it->get_ask();
        if ( price == 0 )
            price = it->get_lastprice();
        if ( price == 0 )
            pnl = 0;
        else
            pnl = 100.0 * ((price * netqty) + tit->get_netprice());
        avg = tit->get_netprice()  / (netqty * -1.0);
    }

    it->setmypnl(pnl);
    tit->setopenpnl(pnl);

    qDebug() << tit->get_symbol() << " setmyposition " << netqty;

    double newtotal = mTradingPositionsModel.get_totalopenpnl()
                + (pnl - holdpnl);
    mTradingPositionsModel.settotalopenpnl(newtotal);

}

void Mediator::OnNewOO(fantasybit::FullOrderDelta fo) {
    qDebug() << "level2 Trading::Mediator " << fo.fname << fo.openorder.DebugString().data();

    if ( fo.fname != myFantasyName )
        return;

    mTradingPositionsModel.Update(fo);
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




Mediator *Mediator::myInstance;

}

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
