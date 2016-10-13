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
    m_pPlayerQuoteSliceModel(&mPlayerQuoteSliceModel),
    mDepthMarketModel{},
    m_pDepthMarketModel(&mDepthMarketModel),
    mFantasyNameBalModel(this,QByteArray(),{"name"}),
    m_pFantasyNameBalModel(&mFantasyNameBalModel),
    mGoodNameBalModel{this,QByteArray(),{"name"}},
    m_pGoodNameBalModel(&mGoodNameBalModel),
//    mOpenOrdersModel{},
//    m_pGlobalOpenOrdersModel(&mOpenOrdersModel),
//    mTradingPositionsModel(this,{"display"},{"symbol"}),
    m_pTradingPositionsModel{new TradingPositionsModel(this,QByteArray (),{"symbol"})},
//    m_currentPidContext("1"),
    myGamesSelectionModel{},
    m_pQItemSelectionModel(&myGamesSelectionModel),
    m_pLeaderBoardSortModel(new SortFilterProxyModel),
    m_blocknum(0),
    m_height(0) {

    fnames = {"fname1", "fname2","fname3", "fname4", "fname5"};

    fnameindex = 0;
    //leader models
    m_pLeaderBoardSortModel->setSourceModel(m_pFantasyNameBalModel);
    m_pLeaderBoardSortModel->setSortRole("updatetime");//mPlayerProjModel.roleForName("pos"));
    m_pLeaderBoardSortModel->setDynamicSortFilter(true);

    //schedule models
    m_pWeeklyScheduleModel = new WeeklyScheduleModel;
    myGamesSelectionModel.setModel(m_pWeeklyScheduleModel);
    m_gameFilter = "Scheduled";


    m_theWeek = 0;
    m_liveSync ="Sync";
    m_seasonString = "";
    //player selection
    m_pProjectionsViewFilterProxyModel =  new ProjectionsViewFilterProxyModel(m_pWeeklyScheduleModel,&myGamesSelectionModel);
    m_pProjectionsViewFilterProxyModel->setSourceModel(&mPlayerProjModel);

    m_pProjectionsViewFilterProxyModel->setSortRole("pos");//mPlayerProjModel.roleForName("pos"));
    m_pProjectionsViewFilterProxyModel->setDynamicSortFilter(false);
    m_useSelected = false;

//setProperty
//    connect(this,SIGNAL(pPlayerQuoteSliceModelItemChanged(PlayerQuoteSliceModelItem *)),
//            this,SLOT(OnpPlayerQuoteSliceModelItemChanged(PlayerQuoteSliceModelItem *)));


//    connect(this,SIGNAL(gameFilterChanged(QString &)),m_pQItemSelectionModel,SLOT(gameFilter(QString&)));
//    auto mynames = m_fantasy_agent.getMyNames();
//    if ( mynames.size() == 0 ) {
//        auto filepath = lastYearPath();
//        if ( filepath != "" ) {
//            qDebug() <<" found last year!! " << filepath;
//            m_fantasy_agent.readFromSecret(filepath,true);
//            mynames = m_fantasy_agent.getMyNames();
//        }
//    }

    //todo fnames
//    for ( auto &np : m_fantasy_agent.getMyNames()) {
//        m_myPubkeyFname[np.second] = "";
//        // qDebug() << " Mediator::Mediator name:" << np.first.data() << " pk: " << np.second.data();
//    }

/*
    signPlayerStatus.setInterval(2000);
    connect(&signPlayerStatus, SIGNAL(timeout()),
            this, SLOT(getSignedPlayerStatus()));

    tradeTesting.setInterval(5000);
    connect(&tradeTesting, SIGNAL(timeout()),
            this, SLOT(doTestTrade()));

    polldepth.setInterval(1000);
    connect(&polldepth, SIGNAL(timeout()),
            this, SLOT(getDepthRep()));

    depthCount = 0;
    depthBackup = 0;
    depthInterval = 1000;

*/

//    WeeklySchedule ws;
//    fantasybit::Reader<ScheduleData> reader5{ GET_ROOT_DIR() + "WeeklySchedule.txt" };
//    ScheduleData sd;
//    while ( reader5.ReadNext(sd) ) {
//        qDebug() << sd.DebugString().data();

//        m_pWeeklyScheduleModel->updateWeeklySchedule(sd.week(),sd.weekly(),m_gameFilter);
//        ws = sd.weekly();

//        break;
//    }
//    connect(m_pWeeklyScheduleModel,SLOT())

//    m_pProjectionsViewFilterProxyModel->setFilterRole(mPlayerProjModel.roleForName("pos"));

//    m_pProjectionsViewFilterProxyModel->setFilterFixedString("WR");


//    m_pPosFilter = new QStringListModel();
//    QStringList list;
//    list << "All" << "QB" << "RB" << "WR" << "TE" << "K" << "DEF";
//    m_pPosFilter->setStringList(list);
//    m_pProjectionsViewFilterProxyModel->sort(0);


}


//QString Mediator::nameS                           tatusGet(const QString &name) {
//    QVariantMap::const_iterator iter = m_nameStatuses.find(name);
//    if ( !iter->isValid() )
//        return "none";
//    else
//        qDebug() << "Suggested Name:  "  << iter.value ().toString ();
//        return iter.value().toString();
//}

void Mediator::pk2fname(const QString &pk) {
//    lastPk2name = pk.toStdString();
//    doPk2fname(lastPk2name);
}

void Mediator::doPk2fname(const std::string &pkstr) {
//    WsReq req;
//    Pk2FnameReq pkreq;
//    pkreq.set_pk(pkstr);
//    req.set_ctype(PK2FNAME);
//    req.MutableExtension(Pk2FnameReq::req)->CopyFrom(pkreq);
//    auto txstr = req.SerializeAsString();
//    QByteArray qb(txstr.data(),(size_t)txstr.size());

//    m_webSocket.sendBinaryMessage(qb);

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
        if ( it != nullptr)
            mGoodNameBalModel.append(new FantasyNameBalModelItem(*it));
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

    myFantasyName = myname.name();
    qDebug() << "Mediator  emitting using fanetay name " << myname.name().data();
    emit usingFantasyName(myname.name().data());
    updateCurrentFantasyPlayerProjections();
}

void Mediator::LiveProj(FantasyBitProj proj) {
    auto *item = mPlayerProjModel.getByUid(proj.playerid().data());
    if ( item ) {

        item->set_avg(mGateway->dataService->GetAvgProjection(proj.playerid()));

        if ( proj.name() == myFantasyName ) {
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
        if ( it != nullptr)
            mGoodNameBalModel.insert(mGoodNameBalModel.size(),it);
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

void Mediator::PlayerStatusChange(pair<string, PlayerStatus> in) {


}

void Mediator::GlobalStateChange(GlobalState gs)
{
    if ( gs.week() > 0 && gs.week() < 18) {
        if ( amLive && gs.week() > m_theWeek) {
            updateWeek();
        }
        settheWeek(gs.week());
    }
}

void Mediator::LiveGui(GlobalState gs) {

    qDebug() << "Mediator received Livegui ";
    if ( !amLive ) {
        amLive = true;
        m_season = gs.season();
        settheWeek(gs.week());
        setliveSync("Live");
        setseasonString(gs.state() == GlobalState_State_OFFSEASON ? "Off Season" : "NFL Season");
        updateWeek();
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
        updateLiveLeaders();

        if ( m_theWeek > 0  && m_theWeek < 17) {
            //setCurrentWeekData();

            m_pWeeklyScheduleModel->updateWeeklySchedule(m_theWeek,
                          mGateway->dataService->GetWeeklySchedule(m_theWeek));
            const auto &vgr = mGateway->dataService->GetCurrentWeekGameRosters();
            mPlayerProjModel.updateRosters(vgr,mGateway->dataService);

            for ( auto gr : vgr ) {
                m_pWeeklyScheduleModel->UpdateStatus(gr.info.id(),gr.status);
            }
            if (myFantasyName != "" )
                updateCurrentFantasyPlayerProjections();

        }
    }
}

void Mediator::updateCurrentFantasyPlayerProjections(){
    //update to recent projection projection and mark them a sent
    auto  recentProjections = mGateway->dataService->GetProjByName(myFantasyName);
    qDebug() << "updateCurrentFantasyPlayerProjections " << recentProjections.size();

    for ( auto it : mPlayerProjModel)  {
        if ( !it ) continue;
        it->set_knownProjection (0);
        it->set_projection(0);
    }

    for ( auto it = recentProjections.begin(); it != recentProjections.end(); ++it ){
        auto *item = mPlayerProjModel.getByUid(it->first.data());
        if ( !item ) continue;
        item->set_knownProjection(it->second);
        item->set_projection(it->second);
    }

}

void Mediator::NewWeek(int week) {
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

void Mediator::GameOver(string) {

}

void Mediator::onControlMessage(QString) {

}



/*****************************************/


//void Mediator::checkname(const QString &name) {
//    qDebug() << " in checkname " << name;
//    WsReq req;
//    req.set_ctype(CHECKNAME);
//    CheckNameReq cnr;
//    cnr.set_fantasy_name(name.toStdString());
//    req.MutableExtension(CheckNameReq::req)->CopyFrom(cnr);
//    auto txstr = req.SerializeAsString();
//    QByteArray qb(txstr.data(),(size_t)txstr.size());
//    m_webSocket.sendBinaryMessage(qb);
//}
/*
void Mediator::subscribeOrderPos(const QString &name) {
    auto stdname = name.toStdString();
    uint64_t cname = FantasyName::name_hash(stdname);//m_myPubkeyHash[m_myHashFname[stdname]];
    if ( cname != 0 )
        m_myHashFname.insert({cname,stdname});

    WsReq req;
    req.set_ctype(SUBSCRIBEFNAME);
    SubscribeReq sr;
    sr.set_name(name.toStdString());
    sr.set_fchash(cname);
    req.SetAllocatedExtension(SubscribeReq::req,&sr);
    auto txstr = req.SerializeAsString();
    qDebug() << " subscribeOrderPos sending " << req.DebugString().data();
    req.ReleaseExtension(SubscribeReq::req);
    QByteArray qb(txstr.data(),(size_t)txstr.size());
    m_webSocket.sendBinaryMessage(qb);
}

void Mediator::getOrderReq(const QString &name,const QString symbol) {
    WsReq req;
    req.set_ctype(GETORDERS);
    GetOrdersReq sr;
    sr.set_fname(name.toStdString());
    if ( symbol != "" )
        sr.set_symbol(symbol.toStdString());
    req.SetAllocatedExtension(GetOrdersReq::req,&sr);
    auto txstr = req.SerializeAsString();
    qDebug() << " getOrderReq sending " << req.DebugString().data();
    req.ReleaseExtension(GetOrdersReq::req);
    QByteArray qb(txstr.data(),(size_t)txstr.size());
    m_webSocket.sendBinaryMessage(qb);
}

*/


//void Mediator::getOrderReq(uint64_t hname) {
//    WsReq req;
//    req.set_ctype(GETORDERS);
//    GetOrdersReq sr;
//    sr.set_fchash(hname);
//    auto it = m_myHashFname.find(hname);
//    if( it == end(m_myHashFname)) {
//        qDebug() << " have hash ehats ny mname " << hname << " criticl ";
//        return;
//    }

//    sr.set_fname(it->second);
//    req.SetAllocatedExtension(GetOrdersReq::req,&sr);
//    auto txstr = req.SerializeAsString();
//    qDebug() << " subscribeOrderPos sending " << req.DebugString().data();
//    req.ReleaseExtension(GetOrdersReq::req);
//    QByteArray qb(txstr.data(),(size_t)txstr.size());
//    m_webSocket.sendBinaryMessage(qb);
//}

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


//    return that;
}

Mediator *Mediator::instance() {
    if (myInstance == NULL) {
        myInstance = new Mediator();
    }
    return myInstance;
}


//void Mediator::OnGoodName(const QString &goodname, const fantasybit::FantasyNameBal &fnb) {
//    m_goodList.append(goodname);
//    mGoodNameBalModel.append(new FantasyNameBalModelItem(fnb));
//    subscribeOrderPos(goodname);
//}



/*!
 * \brief GetUserInfo::importMnemonic
 * \param importStr
 * \return
 * This will return the mn for import
 */
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

void Mediator::doTrade(QString symbol, bool isbuy, const qint32 price, qint32 size) {

//    if ( !m_fantasy_agent.HaveClient() ) {
//        qDebug() << "error no CLient" << "  " << symbol;
//        return;
//    }
//    ExchangeOrder eo;
//    eo.set_playerid(symbol.toStdString());
//    eo.set_type(ExchangeOrder::NEW);

//    OrderCore core;
//    core.set_buyside(isbuy);
//    core.set_size(size);
//    core.set_price(price);

//#ifdef TRACE
//    qDebug() << symbol << "doTrade NewOrder " << core.DebugString().data();
//#endif
//    eo.mutable_core()->CopyFrom(core);
//    //emit SendOrder(eo);

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
//    if ( depthBackup <= 0 ) {
//        depthBackup = 0;
//        depthInterval = 1000;
//    }
//    else
//        depthInterval = 1000 * (depthBackup / 5);

//    if ( depthInterval < 1600 ) {
////       getDepthRep();
//       depthInterval = 1000;
//    }
//    polldepth.start(depthInterval);
////    qDebug() << " doTrade depthInterval " << depthInterval << " depthBackup " << depthBackup << " depthCount " << depthCount;
////    getOrderReq(FantasyName::name_hash(m_fantasy_agent.currentClient()));

////    getOrderPos();
}

void Mediator::doCancel(qint32 id) {

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
}

//void Mediator::doTestTrade() {
////    return;
//    int number = 200;
//    int randomValue = qrand() % number;

//    ExchangeOrder eo;
//    eo.set_playerid(testid.toStdString());
//    eo.set_type(ExchangeOrder::NEW);

//    OrderCore core;
//    core.set_buyside(isbid = !isbid);

//    number = 20;
//    int sz = qrand() % number;
//    if ( sz <= 1 ) sz = 1;
//    core.set_size(sz);
//    core.set_price(randomValue);

//#ifdef TRACE
//    qDebug() << "level2 NewOrder " << core.DebugString();
//#endif

//    eo.mutable_core()->CopyFrom(core);
//    //emit SendOrder(eo);

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
//}

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

    return myFantasyName.data(); //todo check if missed live event
}

QString Mediator::getSecret() {
    auto sec = mGateway->dataService->exportMnemonic(myFantasyName);
    qDebug() << " returned secert" << sec.data();
    return sec.data();
}

//void Mediator::handdleUsingName(const QString &name)
//{
//#ifdef TRACE
//    qDebug() << " handdleUsingName " << name;
//#endif
//    setPlayersName (name);
//}

//void Mediator::handdleNameStatus(const QString &name, const QString &status)
//{
//    setPlayersName (name);
//    setPlayersStatus (status);
//}


void Mediator::OnpPlayerQuoteSliceModelItemChanged(PlayerQuoteSliceModelItem *name)
{
    qDebug() << name;
}

void Mediator::allNamesGet() {
//    WsReq req;
//    req.set_ctype(GETALLNAMES);
//    auto txstr = req.SerializeAsString();
//    QByteArray qb(txstr.data(),(size_t)txstr.size());
//    qDebug() << " allNamesGet sending " << req.DebugString().data();
//    m_webSocket.sendBinaryMessage(qb);
}

void Mediator::rowMarketGet() {
//    WsReq req;
//    req.set_ctype(GETROWMARKET);
//    auto txstr = req.SerializeAsString();
//    QByteArray qb(txstr.data(),(size_t)txstr.size());
//    qDebug() << " rowmarket sending " << req.DebugString().data();
//    m_webSocket.sendBinaryMessage(qb);
}

void Mediator::getOrderPos() {
    getOrderPos("");
}

void Mediator::getOrderPos(const QString &in) {
//    if ( m_fantasy_agent.HaveClient() )
//        getOrderReq(m_fantasy_agent.currentClient().data(),in);
}

void Mediator::getDepthRep() {
//    if ( m_currentPidContext != "" ) {
//        auto txstr = mGetDepthReq.SerializeAsString();
//        QByteArray qb(txstr.data(),(size_t)txstr.size());
//    //    qDebug() << " getDepthRep sending " << mGetDepthReq.DebugString().data();
//        m_webSocket.sendBinaryMessage(qb);

//    }

//    if ( depthBackup > 5 || depthCount > 20 ) {
//        depthInterval *= 2;
//        depthCount = 0;
//        depthBackup -= 5;
//        if ( depthBackup < 0 ) depthBackup = 0;
//        if ( polldepth.isActive() ) {
//            polldepth.start(depthInterval);
////            qDebug() << "getDepthRep depthInterval " << depthInterval << " depthBackup " << depthBackup;
//        }
//    }
//    else if ( depthInterval > 1000 && depthCount <= 0 ){
//        //reset
////        qDebug() << "getDepthRep depthInterval " << depthInterval << " depthBackup " << depthBackup;
//        depthInterval = 100 * depthBackup * 2;
//        if ( depthInterval < 1000 ) depthInterval = 1000;

//        if ( polldepth.isActive() ) {
//            polldepth.stop();
//            polldepth.start(depthInterval);
////            qDebug() << "getDepthRep depthInterval " << depthInterval << " depthBackup " << depthBackup;
//        }
//    }

////    qDebug() << "getDepthRep depthInterval " << depthInterval << " depthBackup " << depthBackup;

//    depthBackup++;
//    depthCount++;

}


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
