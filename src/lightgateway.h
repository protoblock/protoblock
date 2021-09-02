#ifndef LIGHTGATEWAY_H
#define LIGHTGATEWAY_H

#include "pbgateways.h"
#include "mediator.h"
#include "StateData.pb.h"

namespace pb {
using namespace std;

using namespace fantasybit;

class LightGateway : public QObject, public IPBGateway, public IDataService {
    Q_OBJECT
    Q_INTERFACES(pb::IPBGateway)
    Q_INTERFACES(pb::IDataService)

//    IPBGateway *gatepass;
//    IDataService *datapass;

    QTimer signPlayerStatus;

private:
    QWebSocket m_webSocket;
    std::string lastPk2name;

    fantasybit::FantasyAgent agent;
    std::unordered_map<std::string, std::string> m_myPubkeyFname;

    std::unordered_map<std::string, uint64_t> m_myPubkeyHash;

    std::unordered_map<uint64_t, std::string> m_myHashFname;
    std::string m_lastSignedplayer;
    int noNameCount;
    void globalStateGet() {
        WsReq req;
        req.set_ctype(GETGLOBALSTATE);
        auto txstr = req.SerializeAsString();
        QByteArray qb(txstr.data(),(size_t)txstr.size());
        qDebug() << " globalStateGet sending " << req.DebugString().data();
        m_webSocket.sendBinaryMessage(qb);
    }

    void scheduleGet() {
        WsReq req;
        req.set_ctype(GETSCHEDULE);
        auto txstr = req.SerializeAsString();
        QByteArray qb(txstr.data(),(size_t)txstr.size());
        qDebug() << " scheduleGet sending " << req.DebugString().data();
        m_webSocket.sendBinaryMessage(qb);
    }

    void allNamesGet() {
        WsReq req;
        req.set_ctype(GETALLNAMES);
        auto txstr = req.SerializeAsString();
        QByteArray qb(txstr.data(),(size_t)txstr.size());
        qDebug() << " Get sending " << req.DebugString().data();
        m_webSocket.sendBinaryMessage(qb);
    }

    void gameRostersGet() {
        WsReq req;
        req.set_ctype(GETGAMEROSTER);
        auto txstr = req.SerializeAsString();
        QByteArray qb(txstr.data(),(size_t)txstr.size());
        qDebug() << " Get sending " << req.DebugString().data();
        m_webSocket.sendBinaryMessage(qb);
    }

    void projectionGet(const string &fname, bool getavg) {
        WsReq req;
        req.set_ctype(GETPROJECTIONS);
        GetProjectionReq *gpr = req.MutableExtension(GetProjectionReq::req);
        gpr->set_fname(fname);
        gpr->set_getavg(true);
        auto txstr = req.SerializeAsString();
        QByteArray qb(txstr.data(),(size_t)txstr.size());
        qDebug() << " projectionGet sending " << req.DebugString().data();
        m_webSocket.sendBinaryMessage(qb);
    }

    void getDepth(const QString &pid) {
        qDebug() << "getDepth lg onstart depth" << pid;

        if ( pid == "" ) return;
        WsReq req;
        req.set_ctype(GETDEPTH);
        GetDepthReq *dr = req.MutableExtension(GetDepthReq::req);
        dr->set_pid(pid.toStdString());
        auto txstr = req.SerializeAsString();
        QByteArray qb(txstr.data(),(size_t)txstr.size());
        qDebug() << " getDepthRep sending " << req.DebugString().data();
        m_webSocket.sendBinaryMessage(qb);
    }

    void rowMarketGet() {
        WsReq req;
        req.set_ctype(GETROWMARKET);
        auto txstr = req.SerializeAsString();
        QByteArray qb(txstr.data(),(size_t)txstr.size());
        qDebug() << " rowmarket sending " << req.DebugString().data();
        m_webSocket.sendBinaryMessage(qb);
    }

    void getOrderPos() {
        getOrderReq(myCurrentName.name().data(),"");
    }

    void getOrderPos(const QString &in) {
        getOrderReq(myCurrentName.name().data(),in);
    }

    void getOrderReq(const QString &name,const QString symbol) {
        if ( name == "" ) return;

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




//    void checkname(const QString &name) {
//        qDebug() << " in checkname " << name;
//        WsReq req;
//        req.set_ctype(CHECKNAME);
//        CheckNameReq cnr;
//        cnr.set_fantasy_name(name.toStdString());
//        req.MutableExtension(CheckNameReq::req)->CopyFrom(cnr);
//        qDebug() << " checkname sending " << req.DebugString().data();
//        auto txstr = req.SerializeAsString();
//        QByteArray qb(txstr.data(),(size_t)txstr.size());
//        m_webSocket.sendBinaryMessage(qb);
//    }

    void signPlayer(const QString &name)  {
        qDebug() << " sign player " << name;
        agent.signPlayer(name.toStdString());
        NameTrans nt{};
        nt.set_public_key(agent.pubKeyStr());
        nt.set_fantasy_name(name.toStdString());

        Transaction trans{};
        trans.set_version(Commissioner::TRANS_VERSION);
        trans.set_type(TransType::NAME);
        trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nt);
        SignedTransaction sn = agent.makeSigned(trans);
//        auto txstr = sn.SerializeAsString();

        auto pk = Commissioner::str2pk(nt.public_key());
        pb::sha256 digest(sn.id());
        if ( !Commissioner::verify(Commissioner::str2sig(sn.sig()),digest,pk) ) {
            qDebug() << " bad signature ";
            return;
        }
        else
            qDebug() << " good signature ";


        qDebug() << " light signPlayer" << name << sn.DebugString().data();
        DoPostTx(sn);

        usingFantasyName(agent.currentClient().data());
        m_myPubkeyFname[agent.pubKeyStr()] =agent.currentClient();
        m_lastSignedplayer = agent.pubKeyStr();
        noNameCount = 0;
        signPlayerStatus.start();
    }

    void doPk2fname(const std::string &pkstr) {
        WsReq req;
        Pk2FnameReq pkreq;
        pkreq.set_pk(pkstr);
        req.set_ctype(PK2FNAME);
        req.MutableExtension(Pk2FnameReq::req)->CopyFrom(pkreq);
        qDebug() << " doPk2fname sending " << req.DebugString().data();

        auto txstr = req.SerializeAsString();
        QByteArray qb(txstr.data(),(size_t)txstr.size());

        m_webSocket.sendBinaryMessage(qb);
    }

//    void OnGoodName(const QString &goodname, const fantasybit::FantasyNameBal &fnb) {
//        MyFantasyName mfn;
//        mfn.set_status(MyNameStatus::confirmed);
//        mfn.set_name(goodname.toStdString());
//        myGoodNames.push_back(mfn);
//        mynamebal[goodname.toStdString()] = fnb;
//        if ( hesLive && amLive )
//            emit NameBal(fnb);
//    }

private:
    void DoPostTx(SignedTransaction &st) {
//#ifdef YES_DOPOSTTX
        auto txstr = st.SerializeAsString();
        RestfullClient rest(QUrl(PAPIURL.data()));
        rest.postRawData("tx","octet-stream",txstr.data(),((size_t)txstr.size()));
//#endif
    }

    void DoPostTr(SignedTransaction &st) {
        auto txstr = st.SerializeAsString();
        RestfullClient rest(QUrl(PAPIURL.data()));
        //rest.postRawData("tx","octet-stream",txstr.data(),((size_t)txstr.size()));
        rest.postRawData("trade","octet-stream",txstr.data(),((size_t)txstr.size()));
    }


signals:
    void usingFantasyName(const QString &name);
    void error(QString);
    void importSuccess(const QString name, bool passfail);


protected slots:
    void onConnected() {
        errCount = 0;
        QHostAddress hInfo = m_webSocket.peerAddress ();
        qDebug() << "connected to " <<  hInfo.toString () << " on Port " << m_webSocket.peerPort ();
        connect(&m_webSocket, SIGNAL(binaryMessageReceived(QByteArray)),
                this, SLOT ( onBinaryMessageRecived(QByteArray) ));

//        std::string sent = "";
//        if ( agent.HaveClient() ) {
//            sent = agent.pubKeyStr();
//            qDebug() << "onConnected have name " << sent;
//            if ( m_myPubkeyFname[sent] == "" ) {
//                doPk2fname(sent);
//            }
//        }
//        for ( auto &np : m_myPubkeyFname) {
//            if ( np.first == sent )
//                continue;

//            if ( np.second != "")
//                continue;

//            doPk2fname(np.first);
//        }

        bool doProj = false;
        if ( gotprojections || myCurrentName.name() != "" )
            doProj = true;

        bool doPos = false;
        if ( gotorders || myCurrentName.name() != "" )
            doPos = true;


        globalStateGet();
        scheduleGet();
        rowMarketGet();
        allNamesGet();
        gameRostersGet();

        if ( doProj )
            projectionGet(myCurrentName.name().data(),true);

        if ( doPos )
            getOrderPos(myCurrentName.name().data());
    }
    void onBinaryMessageRecived(const QByteArray &message) {
        qDebug() << "LightGateway::onBinaryMessageRecived size" << message.size();

        fantasybit::WSReply rep;
        if ( !rep.ParseFromArray(message.data(),message.size()) ) {
            qDebug() << " !no parse";
            qDebug() << "Mediator::onBinaryMessageRecived error";
            return;
        }
        else
            qDebug() << " yes parse " << rep.ctype();

        #ifdef TRACE2
           // qDebug() << "LightGateway::onBinaryMessageRecived " << rep.ShortDebugString().data();
        #endif

        switch ( rep.ctype()) {
            case GETGAMESTART:
                qDebug() << "emit game start";
                emit GameStart(rep.data());
                break;
            case PK2FNAME: {
                const Pk2FnameRep &pk2 = rep.GetExtension(Pk2FnameRep::rep);
                auto name= pk2.fname();

                if ( m_lastSignedplayer == pk2.req().pk()) {
                    if ( name == "" ) {
                        noNameCount++;
                        if ( noNameCount > 50) {
                            signPlayerStatus.stop();
                            noNameCount = 0;
                        }
                        break;
                    }
                    else {
                        signPlayerStatus.stop();
                        noNameCount = 0;
                        if ( m_myPubkeyFname.at(m_lastSignedplayer) ==  name) {
                            auto fnp = Commissioner::AddName(name,m_lastSignedplayer);
                            if ( fnp != nullptr ) {
                                fnp->setBlockNump(pk2.fnb().block(),pk2.fnb().count());
                                emit NewFantasyName(pk2.fnb());
                                UseName(name.data());
                            }
                        }

                        m_lastSignedplayer = "";
                    }
                }
                break;
            }

            case GETALLNAMES: {
                const GetAllNamesRep &np = rep.GetExtension(GetAllNamesRep::rep);
                for (int i = np.names_size()-1; i >= 0; i--) {
                    auto &fn = np.fnb(i);
                    //qDebug() << fn.DebugString().data();
                    std::shared_ptr<FantasyName> fnp = Commissioner::getName(fn.name());
                    if ( !fnp )
                        fnp = Commissioner::AddName(fn.name(),fn.public_key());

                    if ( fnp != nullptr ) {
                        fnp->initBalance(fn.bits());
                        fnp->initStakePNL(fn.stake()-fn.bits());
                        fnp->setBlockNump (fn.block(),fn.count());
//                        fnp->bal16 = fn.stake16();
//                        fnp->balthis = fn.thisweek();
//                        fnp->ballast = fn.lastweek();
                    }
                }

                qDebug() << "GETALLNAMES" << hesLive << amLive;
                if (  hesLive && amLive) {
                    emit OnLeaders();
                    if ( !gotallnames )
                        GetMyNames();
                }

                gotallnames = true;

                break;
            }
            case GETGLOBALSTATE: {
                const GetGlobalStateRep &np = rep.GetExtension(GetGlobalStateRep::rep);
                qDebug() << "LightGateway::GETGLOBALSTATE " << np.DebugString().data();

                if ( !amLive )
                    OnLiveGui(np.globalstate());
                else {
                    if ( hesLive )
                        emit GlobalStateChange(np.globalstate());

                    //amlive so already got gs?
                    if ( m_gs.week() != np.globalstate().week()) {
                        scheduleGet();
                        allNamesGet();
                    }

                    m_gs = np.globalstate();
                }


                break;
            }
            case GETSCHEDULE: {
                const GetScheduleRep &np = rep.GetExtension(GetScheduleRep::rep);
                if ( !gotschedule || !amLive || m_gs.week() == np.scheduledata().week()) {
                    mScheduleData = np.scheduledata();
                    gotschedule = true;
                    if ( amLive && hesLive)
                        emit OnSchedule(mScheduleData.week());
                }
                break;
            }
            case GETGAMEROSTER: {
                if ( myfantasynames.size() == 0 )
                     projectionGet("",true);

                mGetCurrRostersRep = rep.ReleaseExtension(fantasybit::GetCurrRostersRep::rep);
                if ( !gotroster || !amLive || m_gs.week() == mGetCurrRostersRep->week()) {

                    if ( amLive && hesLive) {
                        emit OnCurrentRosters();
                        if ( gotprojections )
                            emit OnCurrentProjections(mGetProjectionRep->projs().name().data());
                    }

                    gotroster = true;
                }
                break;
            }

            case GETPROJECTIONS: {
                mGetProjectionRep = rep.ReleaseExtension(GetProjectionRep::rep);
                gotprojections = true;

                if ( amLive && hesLive ) {
                    emit OnCurrentProjections(mGetProjectionRep->projs().name().data());
                }

                break;
            }

            case GETORDERS:  {
                mGetOrdersRep = rep.ReleaseExtension(GetOrdersRep::rep);
                gotorders = true;
                if ( amLive && hesLive ) {
                    emit OnCurrentOrder(mGetOrdersRep);
                }

                break;
            }

            case GETROWMARKET:  {
                mGetRowMarketRep = rep.ReleaseExtension(GetROWMarketRep::rep);
                gotrowmarket = true;
                if ( amLive && hesLive ) {
                    emit OnRowMarket(mGetRowMarketRep);
                }

                break;
            }

            case GETDEPTH:  {
                mGetDepthRep = rep.ReleaseExtension(GetDepthRep::rep);
                gotdepth = true;
                if ( amLive && hesLive ) {
                    emit OnGotDepth(mGetDepthRep);
                }

                break;
            }

            default:
                    break;
        }
    }

    void getSignedPlayerStatus() {
        qDebug() << " getSignedPlayerStatus ";
        doPk2fname(m_lastSignedplayer);
    }
    void handleAboutToClose() {
        qDebug() << "handleAboutToClose" << errCount;
    }

    void handleClosed() {
        qDebug() << "handleClosed" << errCount;
        if ( errCount < 100 )
            m_webSocket.open(theServer);
    }

//    void handleWebSocketError(QString err) {
//        qDebug() << "handleWebSocketError" << err;
//        errCount++;
//    }

    void handleSocketError(QAbstractSocket::SocketError wse) {
        qDebug() << "handleSocketError" << wse;
        errCount++;
    }

    void handleSocketState(QAbstractSocket::SocketState ss) {
        qDebug() << "handleSocketState" << ss;
    }

//    void handleError(QString serr) {
//        qDebug() << "handleSocketState" << serr;
//    }

//    void socketError(QString se) {
//        qDebug() << "socketError" << serr;
//    }

private: //dataservice

    ScheduleData mScheduleData;
    GetCurrRostersRep *mGetCurrRostersRep;
    GetProjectionRep *mGetProjectionRep;
    GetOrdersRep *mGetOrdersRep;
    GetROWMarketRep *mGetRowMarketRep;
    GetDepthRep *mGetDepthRep;

//    std::map<int32_t,GetProjectionRep *> myGetProjectionRep;
//    std::unordered_map<std::string,GetProjectionRep *> mGetProjMap;
//    ProjByName * lastAvg;
    bool gotschedule = false;
    bool gotallnames = false;
    //bool gotglobalstate = false;
    bool gotroster = false;
    //bool gotaverage = false;
    bool gotprojections = false;
    bool gotorders = false;
    bool gotrowmarket = false;
    bool gotdepth = false;

    std::unordered_map<std::string,int> avgProj;
    std::unordered_map<std::string,
            std::unordered_map<std::string,int>> FantasyNameProjections;


public:
    LightGateway() {
        dataService = this;
        connect(&m_webSocket, SIGNAL(connected()), this, SLOT(onConnected()));
        connect (&m_webSocket,SIGNAL(aboutToClose()),this,SLOT(handleAboutToClose()));
        connect (&m_webSocket, SIGNAL(disconnected()), this, SLOT(handleClosed()));

        // socket error
        connect (&m_webSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,
                 SLOT(handleSocketError(QAbstractSocket::SocketError)));

        // socket statte
        connect (&m_webSocket, SIGNAL (stateChanged(QAbstractSocket::SocketState)),
                 this, SLOT(handleSocketState(QAbstractSocket::SocketState)));

        QString wss("ws://%1:%2");
        QString lserver = wss.arg(PB_WS_LITE_AGENT.data()).arg(PB_WS_LITE_AGENT_PORT);

    #ifndef NODEBUG
        qDebug() << " connecting to lserver" << lserver;
//        qDebug() << " connecting to txserver" << txserver;
    #endif

        theServer = QUrl(lserver);
        m_webSocket.open(theServer);
//        m_txsocket.open(QUrl(txserver));

        signPlayerStatus.setInterval(2000);

        connect(&signPlayerStatus, SIGNAL(timeout()),
                this, SLOT(getSignedPlayerStatus()),Qt::QueuedConnection);


        connect(Mediator::instance(),&Mediator::ready,this, &LightGateway::ClientReady,Qt::QueuedConnection);


    }

    QUrl theServer;
    int errCount = 0;
signals:
    void NameStatus(fantasybit::MyFantasyName);
    void LiveProj(fantasybit::FantasyBitProj);
    void MyNames(vector<fantasybit::MyFantasyName>);
    void NameBal(fantasybit::FantasyNameBal);
    void PlayerStatusChange(pair<string,fantasybit::PlayerStatus>);
    void GlobalStateChange(fantasybit::GlobalState);
    void LiveGui(fantasybit::GlobalState);
    void NewWeek(int);
    void GameStart(string);
    void GameOver(string);
    void onControlMessage(QString);
    void OnUseName(QString);
    void doOnClaimName(QString);

    void nameAvail(QString, bool);

    void NewFantasyName(fantasybit::FantasyNameBal);
    void AnyFantasyNameBalance(fantasybit::FantasyNameBal);
    void Height(int);
    void BlockNum(int);

private:
    std::map<std::string,fantasybit::MyFantasyName> myfantasynames{};
    void GetMyNames() {
        qDebug() << " light GetMyNames ";
        vector<MyFantasyName> my;
        my = agent.getMyNamesStatus();
        for(auto p : my) {
            myfantasynames[p.name()] = p;
        }

        qDebug() << " MyNames(my)";
        emit MyNames(my);
    }

public slots:
    void OnLiveGui(fantasybit::GlobalState gs) {

        qDebug() << "lightGateway received Livegui ";
        amLive = true;
        m_gs = gs;
        if ( hesLive ) {
            doBothLive();
        }
    }

    void OnClaimName(QString s) {
        signPlayer(s);
    }

    void nameCheck(QString s) {
        qDebug() << " in name check " << s;
        emit nameAvail(s,Commissioner::isAliasAvailable(s.toStdString()));
    }

    void OnGetMyNames() {}

    void UseName(QString name) {
        qDebug() << " OnUseName " << name;
        myCurrentName.set_name(name.toStdString());
        myCurrentName.set_status(MyNameStatus::requested);

//        myfantasynames = agent.getMyNamesStatus();

        auto it = myfantasynames.find(name.toStdString());
        if ( it != end(myfantasynames)) {
            if ( agent.UseName(myCurrentName.name()) ) {
                myCurrentName = it->second;
            }
        }

//        if ( myCurrentName.status() >= MyNameStatus::requested)
//            DoSubscribe(myCurrentName.name(),true);
        projectionGet(myCurrentName.name().data(),true);
        getOrderPos();

        qDebug() << "light NameStatus(myCurrentName)" << myCurrentName.DebugString().data();
        emit NameStatus(myCurrentName);
    }


    void ClientReady() {
        if ( amLive )
            doBothLive();
        hesLive = true;
    }

    void doBothLive() {
        emit LiveGui(m_gs);

        if ( gotschedule )
            emit OnSchedule(mScheduleData.week());
        if ( gotallnames ) {
            emit OnLeaders();
            GetMyNames();
        }
        if ( gotroster ) {
            emit OnCurrentRosters();
        }
        if ( gotprojections)
            emit OnCurrentProjections(mGetProjectionRep->projs().name().data());


        connect(Mediator::instance(),&Mediator::NewProjection,
                this,&LightGateway::OnProjTX,Qt::QueuedConnection);

        connect(Mediator::instance(),&Mediator::NewOrder,
                this, &LightGateway::OnNewOrder,Qt::QueuedConnection);

        connect(Mediator::instance(),&Mediator::OnStartDepth,
                this, &LightGateway::OnStartDepth,Qt::QueuedConnection);

        Mediator *med = Mediator::instance();

        connect(this,&LightGateway::OnCurrentOrder,
                med, &Mediator::OnCurrentOrder,Qt::QueuedConnection);

        connect(this,&LightGateway::OnRowMarket,
                med, &Mediator::OnRowMarket,Qt::QueuedConnection);

        connect(this,&LightGateway::OnGotDepth,
                med, &Mediator::OnGotDepth,Qt::QueuedConnection);

        if ( gotorders )
            emit OnCurrentOrder(mGetOrdersRep);

        if ( gotrowmarket )
            emit OnRowMarket(mGetRowMarketRep);

        if ( gotdepth )
            emit OnGotDepth(mGetDepthRep);

    }

    void OnStartDepth(QString symbol) {
        qDebug() << " lg onstart depth" << symbol;
        if ( symbol == "") return;

        getDepth(symbol);
        getOrderPos(symbol);
    }

    void OnProjTX(vector<fantasybit::FantasyBitProj> vinp) {
        if ( !amLive ) return;

        if ( !agent.HaveClient() ) return;

        Transaction trans{};

        if ( vinp.size() == 0) return;
        if ( vinp.size() == 1) {
            FantasyBitProj &inp = vinp[0];
            ProjectionTrans pj{};
            pj.set_playerid(inp.playerid());
            pj.set_points(inp.proj());

            auto &gs = m_gs;
            pj.set_week(gs.week());
            pj.set_season(gs.season());

            trans.set_version(Commissioner::TRANS_VERSION);
            trans.set_type(TransType::PROJECTION);
            trans.MutableExtension(ProjectionTrans::proj_trans)->CopyFrom(pj);
        }
        else {
            ProjectionTransBlock pj{};

            auto &gs = m_gs;
            pj.set_week(gs.week());
            pj.set_season(gs.season());

            for (auto fbj : vinp ) {
                PlayerPoints pp;
                pp.set_playerid(fbj.playerid());
                pp.set_points(fbj.proj());
                pj.add_player_points()->CopyFrom(pp);
            }

            trans.set_version(Commissioner::TRANS_VERSION);
            trans.set_type(TransType::PROJECTION_BLOCK);
            trans.MutableExtension(ProjectionTransBlock::proj_trans_block)->CopyFrom(pj);
        }

        SignedTransaction sn = agent.makeSigned(trans);
        agent.onSignedTransaction(sn);
        DoPostTx(sn);
//        DoSubscribe(myCurrentName.name(),true);
//        count = bcount = 0;
//        timer->start(intervalstart);
    }

    void OnNewOrder(fantasybit::ExchangeOrder eo) {
        if ( !amLive ) return;

        if ( !agent.HaveClient() ) return;

        Transaction trans{};

        trans.set_version(Commissioner::TRANS_VERSION);
        trans.set_type(TransType::EXCHANGE);
        trans.MutableExtension(ExchangeOrder::exchange_order)->CopyFrom(eo);
        SignedTransaction sn = agent.makeSigned(trans);
        agent.onSignedTransaction(sn);
        DoPostTr(sn);
    }

private:
    bool amLive = false, hesLive = false;
    fantasybit::GlobalState m_gs;
    fantasybit::MyFantasyName myCurrentName{};

//    myCurrentName{};//m_mynames, myGoodNames;
//    std::map<std::string, FantasyNameBal> myfantasynames{};//mynamebal;
//    vector<FantasyNameBal> holdfresh;
//    MyFantasyName myName;

signals:
    void OnSchedule(int);
    void OnLeaders();
    void OnCurrentRosters();
    void OnCurrentProjections(const QString &);
    void OnCurrentOrder(GetOrdersRep *rep);
    void OnRowMarket(GetROWMarketRep *rep);
    void OnGotDepth(GetDepthRep *);

public:
    std::vector<fantasybit::GameRoster> GetCurrentWeekGameRosters() {
        std::vector<fantasybit::GameRoster> ret;

        for (auto &gdr : mGetCurrRostersRep->gamerosters()) {
           ret.push_back(GameRoster());
           GameRoster &gr = ret.back();
           gr.info.set_id(gdr.game_data().gameid());
           gr.info.set_home(gdr.homeroster().teamid());
           gr.info.set_away(gdr.awayroster().teamid());
           gr.status = gdr.game_data().status().status();

           for ( auto &pd : gdr.homeroster().players() ) {
               gr.homeroster.insert({pd.playerid(),
                                    {pd.player_base(),
                                     pd.player_status().status(),
                                     pd.player_game_status()}
                                    });
           }
           for ( auto &pd : gdr.awayroster().players() ) {
               gr.awayroster.insert({pd.playerid(),
                                    {pd.player_base(),
                                     pd.player_status().status(),
                                     pd.player_game_status()}
                                    });
           }
        }
        return ret;
    }

    std::vector<fantasybit::GameResult> GetPrevWeekGameResults(int week) {
        std::vector<fantasybit::GameResult> ret;
        return ret;
    }

    fantasybit::WeeklySchedule GetWeeklySchedule(int week) {
        if ( mScheduleData.week() != week)
            mEmitSchedule = week;
        return mScheduleData.weekly();
    }

    int mEmitSchedule = 0;
    fantasybit::PlayerBase GetPlayerBase(std::string playerId) {
        qWarning() << " lite GetPlayerBase not implimented";
        return PlayerBase();
    }

    std::vector<std::shared_ptr<fantasybit::FantasyName>> GetLeaderBoard() {
        return Commissioner::GetFantasyNames();
    }

    std::unordered_map<std::string,int> GetProjByName(const std::string &nm) {
        qWarning() << " lite GetProjByName not implimented";

        return std::unordered_map<std::string,int>();
    }

    std::unordered_map<std::string,int> GetProjById(const std::string &pid) {
        qWarning() << " lite GetProjById not implimented";
        return std::unordered_map<std::string,int>();
    }

    fantasybit::GlobalState GetGlobalState() {
        return m_gs;
    }

    std::unordered_map<std::string,fantasybit::PlayerDetail> GetTeamRoster(const std::string &teamid) {
        qWarning() << " lite GetTeamRoster not implimented";
        return std::unordered_map<std::string,fantasybit::PlayerDetail>();
    }

    fantasybit::GameStatus GetGameStatus(std::string gid) {
        qWarning() << " lite GetGameStatus not implimented";
        return GameStatus();//GameStatus_Status_SCHEDULED;
    }


    fantasybit::MyFantasyName importMnemonic(std::string &in) {
        return agent.UseMnemonic(in);
    }

    std::string exportMnemonic(std::string &in) {
        return agent.getMnemonic(in);
    }

    int GetAvgProjection(const std::string &playerid) {
        qWarning() << " lite GetAvgProjection not implimented";
        return 0;
    }

    fantasybit::GetProjectionRep * GetLastProjAvg() {
//        fantasybit::ProjByName *ret = nullptr;
//        auto it = myProjByName.find(name);
//       if ( it != end(myProjByName)) {
//            ret = it->second;
//            myProjByName.erase(it);
//        }
        return mGetProjectionRep;
    }

};

}
#endif // FULLGATEWAY_H
