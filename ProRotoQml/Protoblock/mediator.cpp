#include "mediator.h"
#include "StateData.pb.h"
#include <QSettings>
#include "Commissioner.h"
#include "DataPersist.h"
#include <unordered_set>

using namespace fantasybit;
Mediator::Mediator(QObject *parent) :
    QObject(parent),
    m_socketState(Default),
    m_internalSocketState(QAbstractSocket::ListeningState),
    mPlayerQuoteSliceModel{this,"","symbol"},
    m_pPlayerQuoteSliceModel(&mPlayerQuoteSliceModel),
    mDepthMarketModel{},
    m_pDepthMarketModel(&mDepthMarketModel),
    mFantasyNameBalModel{},
    m_pFantasyNameBalModel(&mFantasyNameBalModel),
    mGoodNameBalModel{},
    m_pGoodNameBalModel(&mGoodNameBalModel),
//    mOpenOrdersModel{},
//    m_pGlobalOpenOrdersModel(&mOpenOrdersModel),
//    mTradingPositionsModel(this,{"display"},{"symbol"}),
    m_pTradingPositionsModel{new TradingPositionsModel(this,QByteArray (),{"symbol"})},
    m_currentPidContext("1"),
    myGamesSelectionModel{},
    m_pQItemSelectionModel(&myGamesSelectionModel)
{

    mGetDepthReq.set_ctype(GETDEPTH);
    GetDepthReq dr;
    dr.set_pid("na");
    mGetDepthReq.MutableExtension(GetDepthReq::req)->CopyFrom(dr);

//    qDebug() << m_webSocket.state ();
    connect (this,SIGNAL (error(QString)),this,SLOT (handleError(QString)));
    connect (this,SIGNAL(socketError(QString)), this , SLOT ( handleWebSocketError(QString)) );
    connect(&m_webSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect (&m_webSocket,SIGNAL(aboutToClose()),this,SLOT(handleAboutToClose()));
    connect (&m_webSocket, SIGNAL(disconnected()), this, SLOT(handleClosed()));

    // socket error
    connect (&m_webSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleSocketError(QAbstractSocket::SocketError)));

    // socket statte
    connect (&m_webSocket, SIGNAL (stateChanged(QAbstractSocket::SocketState)),
             this, SLOT(handleSocketState(QAbstractSocket::SocketState)));


    connect(this,SIGNAL(pPlayerQuoteSliceModelItemChanged(PlayerQuoteSliceModelItem *)),
            this,SLOT(OnpPlayerQuoteSliceModelItemChanged(PlayerQuoteSliceModelItem *)));

//    connect(this,SIGNAL(gameFilterChanged(QString &)),m_pQItemSelectionModel,SLOT(gameFilter(QString&)));
    auto mynames = m_fantasy_agent.getMyNames();
//    if ( mynames.size() == 0 ) {
//        auto filepath = lastYearPath();
//        if ( filepath != "" ) {
//            qDebug() <<" found last year!! " << filepath;
//            m_fantasy_agent.readFromSecret(filepath,true);
//            mynames = m_fantasy_agent.getMyNames();
//        }
//    }

    for ( auto &np : m_fantasy_agent.getMyNames()) {
        m_myPubkeyFname[np.second] = "";
//        qDebug() << " Mediator::Mediator name:" << np.first.data() << " pk: " << np.second.data();
    }

    QString wss("ws://%1:%2");
    m_chatServerAddr = wss.arg(PB_WS_CHAT.data()).arg(PB_WS_CHAT_PORT);
    QString lserver = wss.arg(PB_WS_LITE_AGENT.data()).arg(PB_WS_LITE_AGENT_PORT);
    QString txserver = wss.arg(PB_WS_TX.data()).arg(PB_WS_TX_PORT);

#ifndef NODEBUG
    qDebug() << " connecting to lserver" << lserver;
    qDebug() << " connecting to txserver" << txserver;
#endif
    m_webSocket.open(QUrl(lserver));
    m_txsocket.open(QUrl(txserver));

//    init ();
//    connect (this,SIGNAL (error(QString)),this,SLOT (handleError(QString)));
//    connect (this,SIGNAL(socketError(QString)), this , SLOT ( handleWebSocketError(QString)) );
//    connect(&m_webSocket, SIGNAL(connected()), this, SLOT(onConnected()));
//    connect (&m_webSocket,SIGNAL(aboutToClose()),this,SLOT(handleAboutToClose()));
//    connect (&m_webSocket, SIGNAL(disconnected()), this, SLOT(handleClosed()));

//    // socket error
//    connect (&m_webSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleSocketError(QAbstractSocket::SocketError)));

//    // socket statte
//    connect (&m_webSocket, SIGNAL (stateChanged(QAbstractSocket::SocketState)),
//             this, SLOT(handleSocketState(QAbstractSocket::SocketState)));

//    connect(this,SIGNAL (nameStatusChanged(QString,QString))
//            ,this, SLOT (handdleNameStatus(QString,QString)));

//    connect(this,SIGNAL(usingFantasyName(QString)),
//            this,SLOT(handdleUsingName(QString)));



    connect (this ,SIGNAL (engineUpdate(bool)),this,SLOT(handleEngineUpdate(bool)));
    setencyptPath (QString::fromStdString (GET_ROOT_DIR ()));


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


    m_pWeeklyScheduleModel = new WeeklyScheduleModel;
    m_gameFilter = "Scheduled";
    WeeklySchedule ws;
    fantasybit::Reader<ScheduleData> reader5{ GET_ROOT_DIR() + "WeeklySchedule.txt" };
    ScheduleData sd;
    while ( reader5.ReadNext(sd) ) {
        qDebug() << sd.DebugString().data();

        m_pWeeklyScheduleModel->updateWeeklySchedule(sd.week(),sd.weekly(),m_gameFilter);
        ws = sd.weekly();

        break;
    }

    std::map<std::string, std::unordered_set<std::string>> MyTeamRoster;
    std::map<std::string, PlayerStatus> MyPlayerStatus;
    std::map<std::string, PlayerData> MyPlayerData;

//file:///C:/work/build-ProRoto2016-Release-57/Applications/ProtoBlock2016/release/storage/WeeklySchedule.txt
//file:///C:/work/build-ProRoto2016-Release-57/Applications/ProtoBlock2016/release/storage/PlayerData.txt
    Reader<PlayerData> reader3{ GET_ROOT_DIR() + "PlayerData.txt"};
    PlayerData pd;
    while ( reader3.ReadNext(pd) ) {
        pd.ByteSize();
        auto &ps = pd.player_status();
        MyPlayerStatus[pd.playerid()] = ps;
        MyPlayerData[pd.playerid()] = pd;
        if (ps.has_teamid()) {
            //qDebug() << ps.teamid();
            auto itr = MyTeamRoster.find(ps.teamid());
            if ( itr == end(MyTeamRoster))
                MyTeamRoster[ps.teamid()] =  std::unordered_set<std::string>{};

            if ( MyTeamRoster[ps.teamid()].size() > 20) continue;
            MyTeamRoster[ps.teamid()].insert(pd.playerid());
        }

    }

    vector<GameRoster>  vgr;
    for (const auto g : ws.games()) {
        GameRoster gr{};
        gr.info = g;
        gr.status = GameStatus_Status_SCHEDULED;
//        GameStatus gs = g;
//        if ( gs.datetime() != -1)
//        {
//            if ( gs.has_datetime() )
//                gr.info.set_time(gs.datetime());

//            if ( gs.has_status())
//               gr.status = gs.status();
//        }

        {
            auto teamid = g.home();
            auto it = MyTeamRoster.find(teamid);
            if ( it != end(MyTeamRoster))
                qDebug() << " found it" << it->second.size();

            auto &teamroster = MyTeamRoster[teamid];
            qDebug() << teamroster.size();
            for ( auto p : teamroster) {
                auto ps = MyPlayerStatus[p];
                if ( ps.teamid() != teamid  ||
                     false)//ps.status() != PlayerStatus::ACTIVE)
                    continue;

                PlayerData &ppd = MyPlayerData[p];
                PlayerDetail pd;
                pd.base = ppd.player_base();
                pd.team_status = ps.status();
                pd.game_status = PlayerGameStatus::NA;
                gr.homeroster[p] = pd;
            }
        }
        {
            auto teamid = g.away();
            auto it = MyTeamRoster.find(teamid);
            if ( it != end(MyTeamRoster))
                qDebug() << " found it" << it->second.size();

            auto &teamroster = MyTeamRoster[teamid];
            qDebug() << teamroster.size();
            for ( auto p : teamroster) {
                auto ps = MyPlayerStatus[p];
                if ( ps.teamid() != teamid  ||
                     false)//ps.status() != PlayerStatus::ACTIVE)
                    continue;

                PlayerData &ppd = MyPlayerData[p];
                PlayerDetail pd;
                pd.base = ppd.player_base();
                pd.team_status = ps.status();
                pd.game_status = PlayerGameStatus::NA;
                gr.awayroster[p] = pd;
            }
        }


//        gr.homeroster = GetTeamRoster(g.home());
//        gr.awayroster = GetTeamRoster(g.away());
        vgr.push_back(gr);
    }



    myGamesSelectionModel.setModel(m_pWeeklyScheduleModel);
//    m_pQItemSelectionModel = new QItemSelectionModel();
//    m_pQItemSelectionModel->setModel(m_pWeeklyScheduleModel);
    connect(&myGamesSelectionModel,SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this,SLOT(selectionChanged(QItemSelection, QItemSelection)));

//    connect(m_pWeeklyScheduleModel,SLOT())

    m_pProjectionsViewFilterProxyModel =  new ProjectionsViewFilterProxyModel(m_pWeeklyScheduleModel,&myGamesSelectionModel);
    m_pProjectionsViewFilterProxyModel->setSourceModel(&mPlayerProjModel);
//    m_pProjectionsViewFilterProxyModel->setFilterRole(mPlayerProjModel.roleForName("pos"));
    mPlayerProjModel.updateRosters(vgr);
//    m_pProjectionsViewFilterProxyModel->setFilterFixedString("WR");

    m_pProjectionsViewFilterProxyModel->setSortRole("pos");//mPlayerProjModel.roleForName("pos"));
//    m_pProjectionsViewFilterProxyModel->setDynamicSortFilter(true);

    m_pPosFilter = new QStringListModel();
    QStringList list;
    list << "All" << "QB" << "RB" << "WR" << "TE" << "K" << "DEF";
    m_pPosFilter->setStringList(list);
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
    lastPk2name = pk.toStdString();
    doPk2fname(lastPk2name);
}

void Mediator::doPk2fname(const std::string &pkstr) {
    WsReq req;
    Pk2FnameReq pkreq;
    pkreq.set_pk(pkstr);
    req.set_ctype(PK2FNAME);
    req.MutableExtension(Pk2FnameReq::req)->CopyFrom(pkreq);
    auto txstr = req.SerializeAsString();
    QByteArray qb(txstr.data(),(size_t)txstr.size());

    m_webSocket.sendBinaryMessage(qb);

}

void Mediator::checkname(const QString &name) {
    qDebug() << " in checkname " << name;
    WsReq req;
    req.set_ctype(CHECKNAME);
    CheckNameReq cnr;
    cnr.set_fantasy_name(name.toStdString());
    req.MutableExtension(CheckNameReq::req)->CopyFrom(cnr);
    auto txstr = req.SerializeAsString();
    QByteArray qb(txstr.data(),(size_t)txstr.size());
    m_webSocket.sendBinaryMessage(qb);

//    qDebug() << " sent in checkname " << name;

}

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


void Mediator::handleError(const QString err) {
    if ( m_errorString == err)
        return;
    m_errorString = err;
    emit errorStringChanged();

}

void Mediator::handleWebSocketError(const QString err)
{
    if (m_webSocketErrorString == err)
        return;
    m_webSocketErrorString = err;
    webSocketErrorStringChanged();

//    m_pTradingPositionsModel->getByUid("")->get_pOpenOrdersModel()
}

QString Mediator::webSocketErrorString() const
{
    return m_webSocketErrorString;
}

QString Mediator::errorString() const
{
    return m_errorString;
}


Mediator *Mediator::instance() {
    if (myInstance == NULL) {
        myInstance = new Mediator();
    }
    return myInstance;
}

///*!
// * \brief Mediator::playersStatus
// * \return
//    returns the current fantasy players status that is in focus
//*/
//QString Mediator::playersStatus() const
//{
//    return m_playersStatus;
//}

///*!
// * \brief Mediator::setPlayersStatus
// * \param playersStatus
//    set the curreent fantasy players status
//*/
//void Mediator::setPlayersStatus(const QString &playersStatus)
//{
//    if(m_playersStatus == playersStatus)
//        return;
//    m_playersStatus = playersStatus;
//    emit playersStatusChanged();
//}

///*!
// * \brief Mediator::playersName
// * \return
//    return the current fantasys players name that is in focus
//*/
//QString Mediator::playersName()
//{
//    return m_fantasy_agent.currentClient().data();
//}

///*!
// * \brief Mediator::setPlayersName
// * \param playersName
//    Set the Current players name that is in focus
//*/
//void Mediator::setPlayersName(const QString &playersName)
//{
//    if( m_playersName == playersName)
//        return;
//    m_playersName = playersName;
//    emit playersNameChanged();
//}

//Mediator::MyNameStatus Mediator::myNameStatus() const
//{
//    return m_myNameStatus;
//}

//FIXME when the status of a current name is changed update this
//void Mediator::setMyNameStatus(const Mediator::MyNameStatus &myNameStatus)
//{
//    if (m_myNameStatus == myNameStatus){
//        return;
//    }else {
//        switch (myNameStatus) {
//        case None :
//            m_myNameStatus = None;
//            break;
//        case NotAvil:
//            m_myNameStatus = NotAvil;
//            break;
//        case Requested:
//            m_myNameStatus = Requested;
//            break;
//        case TransactionSent:
//            m_myNameStatus = TransactionSent;
//            break;
//        case Confirmed:
//            m_myNameStatus = Confirmed;
//            break;
//        }
//        emit myNameStatusChanged();
//    }
//}

qint64 Mediator::sendBinaryMessage(const QByteArray &data) {
    return m_webSocket.sendBinaryMessage(data);
}

qint64 Mediator::sendBinaryMessage(const GOOGLE_NAMESPACE::protobuf::Message &data) {
    auto txstr = data.SerializeAsString();

    QByteArray qb(txstr.data(),(size_t)txstr.size());

    return sendBinaryMessage(qb);
}


void Mediator::handleClosed() {
    qDebug() << "Close Reason " << m_webSocket.closeReason ();
    return;
}

void Mediator::onConnected() {
    QHostAddress hInfo = m_webSocket.peerAddress ();
    qDebug() << "connected to " <<  hInfo.toString () << " on Port " << m_webSocket.peerPort ();
    connect(&m_webSocket, SIGNAL(binaryMessageReceived(QByteArray)),
            this, SLOT ( onBinaryMessageRecived(QByteArray) ));

//    WsReq reqstat;
//    reqstat.set_ctype(GETSTATUS);
//    auto mynamepk = m_fantasy_agent.getMyNames();
//    for ( auto &np : mynamepk) {
//        NameStatusReq nsq;
//        nsq.set_name(np.first);
//        nsq.set_pk(np.second);
//        reqstat.MutableExtension(NameStatusReq::req)->CopyFrom(nsq);
//        sendBinaryMessage(reqstat);
//    }

    std::string sent = "";
    if ( m_fantasy_agent.HaveClient() ) {
        sent = m_fantasy_agent.pubKeyStr();
        if ( m_myPubkeyFname[sent] == "" ) {
//            subscribeOrderPos(m_fantasy_agent.currentClient().data());
            doPk2fname(sent);
        }
    }
    for ( auto &np : m_myPubkeyFname) {
        if ( np.first == sent )
            continue;

        if ( np.second != "")
            continue;

        doPk2fname(np.first);
    }

    allNamesGet();
    rowMarketGet();

}

void Mediator::handleAboutToClose()
{
    // Read the error forr the connection and then //
    // start timmer to reconnect to server//
}


void Mediator::onTextMessageReceived(QString message) {
        qDebug() << "Message received:" << message;
}


void Mediator::onBinaryMessageRecived(const QByteArray &message) {
    fantasybit::WSReply rep;
    if ( !rep.ParseFromString(message.toStdString()) ) {
        qDebug() << "Mediator::onBinaryMessageRecived error";
        return;
    }


//    if ( rep.ctype() != GETALLNAMES)
//        qDebug() << "Mediator::onBinaryMessageRecived " << rep.DebugString().data();
//    else
//        qDebug() << "Mediator::onBinaryMessageRecived GETALLNAMES";
#ifdef TRACE2
    qDebug() << "Mediator::onBinaryMessageRecived " << rep.DebugString().data();
#endif

    switch ( rep.ctype()) {
        case PK2FNAME:
        {
            const Pk2FnameRep &pk2 = rep.GetExtension(Pk2FnameRep::rep);
            auto name= pk2.fname();
            if ( name == "" ) {
                if ( pk2.req().pk() == lastPk2name) {
                    error(QString("import failed. please input valid secret"));
//                    qDebug() << "Mediator::onBinaryMessageRecived import failed. please input valid secret";
                    QString err = "import failed. no name for: ";
                    err.append(lastPk2name.data());
                    emit importSuccess(err,false);
                    lastPk2name = "";
                }
                return;
            }


            bool was_pending = m_fantasy_agent.finishImportMnemonic(pk2.req().pk(), name);

            //FIXME lets make this as a real map to pass to a string
//            m_nameStatuses[name.data()] = QString("confirmed");//            auto it = m_myPubkeyHash.find(pk2.req().pk());
            //            if ( it != end(m_myPubkeyHash))
            //                chash = it->second;

            //            auto it2 = m_myPubkeyFname.find(pk2.req().pk());
            //            if ( it2 != end(m_myPubkeyFname))
            //                currname = it2->second;


            std::string currname = m_myPubkeyFname[pk2.req().pk()];
            uint64_t chash =  m_myPubkeyHash[pk2.req().pk()];
            if ( chash != 0 && currname != "" )
                m_myHashFname[chash] = currname;
            else
            if ( currname == "" && chash == 0);
            else if ( chash != 0 )
                    qDebug() << " bad chash - fname - should not be here!";            
            else { //currname != ""
                chash = FantasyName::name_hash(currname);
                auto it3 = m_myHashFname.find(chash);
                if ( it3 == end(m_myHashFname)) {
                    m_myHashFname[chash] = currname;
                    m_myPubkeyHash[pk2.req().pk()] = chash;
                }
            }


//            std::string currname = m_myPubkeyFname[pk2.req().pk()] ;
            if ( currname == "") {
                if ( m_lastSignedplayer == pk2.req().pk()) {
                    signPlayerStatus.stop();
                    m_lastSignedplayer = "";
                    allNamesGet();
                }

                m_myPubkeyFname[pk2.req().pk()] = name;
                chash = FantasyName::name_hash(name);
                m_myHashFname[chash] = name;
                m_myPubkeyHash[pk2.req().pk()] = chash;

                QString goodname = name.data();
                OnGoodName(goodname,pk2.fnb());
//                getOrderPos();
//                qDebug() << " new good name! " << goodname;
            }
                //            nameStatusChanged( name.data() , "confirmed" );

            if ( !m_fantasy_agent.HaveClient() ||
                 lastPk2name == pk2.req().pk()) {
                if ( !m_fantasy_agent.UseName(name) ) {
                    qDebug() << "error using name " << name.data () ;
//                    error(QString("error using name").append(name.data()));
                }
                else {
//                    usingFantasyName(m_fantasy_agent.currentClient().data());
                  subscribeOrderPos(currname.data());
                }
            }

            if ( was_pending ) {
                importSuccess(m_fantasy_agent.currentClient().data(), true);
                usingFantasyName(m_fantasy_agent.currentClient().data());
//                subscribeOrderPos(currname.data());
//                getOrderPos();
            }

            break;
        }
        case CHECKNAME: {
            const CheckNameRep &cn = rep.GetExtension(CheckNameRep::rep);
//            qDebug() << " emit " << cn.req().fantasy_name().data() << cn.isavail();
            nameCheckGet(cn.req().fantasy_name().data(),cn.isavail().data());
//            update_checkname(rep.data().data());
            break;
        }
        case GETSTATUS:
        {
//            const NameStatusRep &np = rep.GetExtension(NameStatusRep::rep);
//            QString statusstr = fantasybit::MyNameStatus_Name(np.status()).data();
//            auto name = np.req().name();
//            m_nameStatuses[name.data()] = statusstr;
//            qDebug() << " emit " << name.data() << statusstr;
//            nameStatusChanged( name.data() , statusstr );
            break;
        }
        case GETALLNAMES: {
            m_allNamesList.clear();
//            m_allNames2.clear();
            const GetAllNamesRep &np = rep.GetExtension(GetAllNamesRep::rep);
            for (int i = np.names_size()-1; i >= 0; i--) {
               m_allNamesList.append(np.names(i).data());
//               m_allNames2.append(np.names(i).data());
            }
            leaderBoardchanged();

            m_pFantasyNameBalModel->clear();
            for ( const auto &fnbi : np.fnb()) {
                m_pFantasyNameBalModel->prepend(new FantasyNameBalModelItem(fnbi));
            }
#ifdef TRACE
            //qDebug() << "GETALLNAMES" <<  np.DebugString().data();
#endif

//            setallNames2(m_allNamesList)
            break;
        }
        case GETROWMARKET: {
//            qDebug() << rep.DebugString().data();
            m_pPlayerQuoteSliceModel->clear();
    //            m_allNames2.clear();
            const GetROWMarketRep &np = rep.GetExtension(GetROWMarketRep::rep);
            bool first = true;
            for( const auto &rowm : np.rowmarket()) {

                auto mynew = new PlayerQuoteSliceModelItem(rowm);
                if ( first ) {
                    m_pPlayerQuoteSliceModelItem = mynew;
                    first = false;
                }

                m_pPlayerQuoteSliceModel->append(mynew);
//                WsReq req;
//                GetDepthReq gdr;
//                gdr.set_pid(rowm.pid());
//                req.set_ctype(GETDEPTH);
//                req.MutableExtension(GetDepthReq::req)->CopyFrom(gdr);
//                auto txstr = req.SerializeAsString();
//                QByteArray qb(txstr.data(),(size_t)txstr.size());
//                qDebug() << " rowmarket sending " << req.DebugString().data();
//                m_webSocket.sendBinaryMessage(qb);
            }
        }
        break;
        case GETDEPTH: {
            depthBackup--;
            m_pDepthMarketModel->updateFullDepth(rep.GetExtension(GetDepthRep::rep));
#ifdef TRACE3
            qDebug() << " got depth " << depthBackup;//rep.DebugString().data();
#endif

            break;
        }
        case GETORDERS: {
            qDebug() << rep.DebugString().data();
            auto fname = rep.GetExtension(GetOrdersRep::rep).oorders().fname();
            TradingPositionsModel *tmodel;

            bool dosignal;
            if ( fname == m_fantasy_agent.currentClient()) {
                tmodel = modelMap[fname] = m_pTradingPositionsModel;

                if ( m_pTradingPositionsModel->get_fantasyname().toStdString() != fname )
                    dosignal = true;

            }
            else {
                auto it = modelMap.find(fname);

                if ( it ==  end(modelMap))
                    tmodel = new TradingPositionsModel{new TradingPositionsModel(this,QByteArray (),{"symbol"})};
                else
                    tmodel = it->second;
            }

//            tmodel->clear();
            tmodel->updateAllOrders(rep.GetExtension(GetOrdersRep::rep).oorders());

            break;
        }
        default:
            break;
    }
    //    emit gotPk2fname(name);
#ifdef TRACE
        //qDebug() << "GETALLNAMES" <<  np.DebugString().data();
#endif

//            setallNames2(m_allNamesList)

}

void Mediator::OnGoodName(const QString &goodname, const fantasybit::FantasyNameBal &fnb) {
    m_goodList.append(goodname);
    mGoodNameBalModel.append(new FantasyNameBalModelItem(fnb));
    subscribeOrderPos(goodname);
}


/*!
 * \brief Mediator::handleEngineUpdate
    THIS SLOT IS QML ONLY !
    This means that somewhere in this code someone has updateed the engine status
    engineUpdate(bool )  fires this slot.
    This slot is used to set A QML property (read only)
    the QML property is called engineStatus
    It returns a bool that means the engine is in good shape.
*/
void Mediator::handleEngineUpdate(const bool &sta)
{
    setengineStatus (sta);
}



/*!
 * \brief GetUserInfo::importMnemonic
 * \param importStr
 * \return
 * This will return the mn for import
 */
// FIXME error checking
QString Mediator::importMnemonic(const QString &importStr) {
    auto pk = m_fantasy_agent.startImportMnemonic(importStr.toStdString());
    if ( pk == "" )
        return "";

    auto iter = m_myPubkeyFname.find(pk);
    if ( iter != end(m_myPubkeyFname)) {
        if ( iter->second == "" )
            pk2fname(pk.data());
        else
            usingFantasyName(iter->second.data());

        return pk.data();
    }

    m_myPubkeyFname[pk] = "";
    pk2fname(pk.data());
    return pk.data();
}

// FIXME ? set this to be a bool and not a void so that
// we know if it was good or not ?
void Mediator::signPlayer(const QString &name)  {
    m_fantasy_agent.signPlayer(name.toStdString());
    NameTrans nt{};
    nt.set_public_key(m_fantasy_agent.pubKeyStr());
    nt.set_fantasy_name(name.toStdString());


    Transaction trans{};
    trans.set_version(Commissioner::TRANS_VERSION);
    trans.set_type(TransType::NAME);
    trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nt);
    SignedTransaction sn = m_fantasy_agent.makeSigned(trans);
    auto txstr = sn.SerializeAsString();

    auto pk = Commissioner::str2pk(nt.public_key());
    pb::sha256 digest(sn.id());
    if ( !Commissioner::verify(Commissioner::str2sig(sn.sig()),digest,pk) )
        qDebug() << " bad signature ";
    else
        qDebug() << " good signature ";

    QByteArray qb(txstr.data(),(size_t)txstr.size());

    qDebug() << " mediator signPlayer" << name << sn.DebugString().data();
    m_txsocket.sendBinaryMessage(qb);

//    m_nameStatuses[name] = QString("requested");
//    nameStatusChanged(name,"requested");
    usingFantasyName(m_fantasy_agent.currentClient().data());
    m_myPubkeyFname[m_fantasy_agent.pubKeyStr()] = "";
    m_lastSignedplayer = m_fantasy_agent.pubKeyStr();
    signPlayerStatus.start();
}


void Mediator::getSignedPlayerStatus() {
    doPk2fname(m_lastSignedplayer);
}


void Mediator::doTrade(QString symbol, bool isbuy, const qint32 price, qint32 size) {

    if ( !m_fantasy_agent.HaveClient() ) {
        qDebug() << "error no CLient" << "  " << symbol;

        return;


    }
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
    //emit SendOrder(eo);

    Transaction trans{};
    trans.set_version(Commissioner::TRANS_VERSION);
    trans.set_type(TransType::EXCHANGE);
    trans.MutableExtension(ExchangeOrder::exchange_order)->CopyFrom(eo);

    SignedTransaction sn = m_fantasy_agent.makeSigned(trans);

    auto pk = m_fantasy_agent.pubKey();
    pb::sha256 digest(sn.id());
    if ( !Commissioner::verify(Commissioner::str2sig(sn.sig()),digest,pk) )
        qDebug() << " bad signature ";
    else
        qDebug() << " good signature ";



    auto txstr = sn.SerializeAsString();

    QByteArray qb(txstr.data(),(size_t)txstr.size());

    qDebug() << " mediator doTestTrade";
    m_txsocket.sendBinaryMessage(qb);

    depthCount = 0;
    depthBackup-=10;
    if ( depthBackup <= 0 ) {
        depthBackup = 0;
        depthInterval = 1000;
    }
    else
        depthInterval = 1000 * (depthBackup / 5);

    if ( depthInterval < 1600 ) {
//       getDepthRep();
       depthInterval = 1000;
    }
    polldepth.start(depthInterval);
//    qDebug() << " doTrade depthInterval " << depthInterval << " depthBackup " << depthBackup << " depthCount " << depthCount;
//    getOrderReq(FantasyName::name_hash(m_fantasy_agent.currentClient()));

//    getOrderPos();
}

void Mediator::doCancel(qint32 id) {

    if ( !m_fantasy_agent.HaveClient() ) {
        qDebug() << "error no CLient";

        return;
    }
    ExchangeOrder eo;
    eo.set_type(ExchangeOrder::CANCEL);
    eo.set_cancel_oref(id);

#ifdef TRACE
    qDebug() << "cancelTrade id " << id;
#endif

    Transaction trans{};
    trans.set_version(Commissioner::TRANS_VERSION);
    trans.set_type(TransType::EXCHANGE);
    trans.MutableExtension(ExchangeOrder::exchange_order)->CopyFrom(eo);

    SignedTransaction sn = m_fantasy_agent.makeSigned(trans);

    auto pk = m_fantasy_agent.pubKey();
    pb::sha256 digest(sn.id());
    if ( !Commissioner::verify(Commissioner::str2sig(sn.sig()),digest,pk) )
        qDebug() << " bad signature ";
    else
        qDebug() << " good signature ";

    auto txstr = sn.SerializeAsString();

    QByteArray qb(txstr.data(),(size_t)txstr.size());

    qDebug() << " mediator doTestTrade";
    m_txsocket.sendBinaryMessage(qb);

    depthCount = 0;
    depthBackup-=10;
    if ( depthBackup < 0 ) {
        depthBackup = 0;
        depthInterval = 1000;
    }
    else
        depthInterval = 1000 * (depthBackup / 5);

    if ( depthInterval < 1600 ) {
       getDepthRep();
       depthInterval = 1000;
    }
    polldepth.start(depthInterval);
//    qDebug() << " doTrade depthInterval " << depthInterval << " depthBackup " << depthBackup << " depthCount " << depthCount;
//    getOrderReq(FantasyName::name_hash(m_fantasy_agent.currentClient()));
//    getOrderPos();
}

void Mediator::doTestTrade() {
//    return;
    int number = 200;
    int randomValue = qrand() % number;

    ExchangeOrder eo;
    eo.set_playerid(testid.toStdString());
    eo.set_type(ExchangeOrder::NEW);

    OrderCore core;
    core.set_buyside(isbid = !isbid);

    number = 20;
    int sz = qrand() % number;
    if ( sz <= 1 ) sz = 1;
    core.set_size(sz);
    core.set_price(randomValue);

#ifdef TRACE
    qDebug() << "level2 NewOrder " << core.DebugString();
#endif

    eo.mutable_core()->CopyFrom(core);
    //emit SendOrder(eo);

    Transaction trans{};
    trans.set_version(Commissioner::TRANS_VERSION);
    trans.set_type(TransType::EXCHANGE);
    trans.MutableExtension(ExchangeOrder::exchange_order)->CopyFrom(eo);

    SignedTransaction sn = m_fantasy_agent.makeSigned(trans);

    auto pk = m_fantasy_agent.pubKey();
    pb::sha256 digest(sn.id());
    if ( !Commissioner::verify(Commissioner::str2sig(sn.sig()),digest,pk) )
        qDebug() << " bad signature ";
    else
        qDebug() << " good signature ";



    auto txstr = sn.SerializeAsString();

    QByteArray qb(txstr.data(),(size_t)txstr.size());

    qDebug() << " mediator doTestTrade";
    m_txsocket.sendBinaryMessage(qb);
}


void Mediator::useName(const QString &name) {
    qDebug() << " Mediator::useName " << name;
    if ( m_fantasy_agent.UseName(name.toStdString()) ) {
//        qDebug() << " Mediator::useName  usingFantasyName" << name;
//        subscribeOrderPos(name);
        usingFantasyName(name);
//        getOrderPos();
    }

#ifdef CRAZY__no_TESTINGit
    if ( name == "MikeClayNFL" ) {
        getOrderReq(name);
        if ( testid == "1806")
            testid = "1";
        else
            testid = "1806";

        tradeTesting.start();
        startDepth(testid);
    }
    else {
        tradeTesting.stop();
        stopDepth(testid);
    }
#endif

}

/*!
 * \brief Mediator::init
    Try to find the best fantasy name that there is to use
    ? There might be a reace on on this ?
 */
QString Mediator::init() {

    engineUpdate(true);

    std::string dname = m_fantasy_agent.defaultName();
    if ( dname == "") {
        return "";
        // FIXME this should be a error signal
        // Also we should update the engine status to false
        // as we could not mke it the end of fantasyadgent ?
    }

//    qDebug() << " Mediator::init() " << dname.data();
    QString defaultName = QString::fromStdString (m_fantasy_agent.currentClient().data());
    usingFantasyName( defaultName, true ) ;



    return defaultName;
//    m_nameStatuses[defaultName] = QString("requested");
//    nameStatusChanged( defaultName , "requested" );

//    // HERE I am setting the engine as true because it is up and we made ith through all the stuff that was needed
//    engineUpdate(true);
//    usingFantasyName( defaultName, true ) ;
//    return defaultName;
}

QString Mediator::getSecret() {
    if ( !m_fantasy_agent.HaveClient() )
        return "error- no name";
    else
        return m_fantasy_agent.getMnemonic(m_fantasy_agent.currentClient()).data();
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


// THIS SHOULD be a error signal that alerts others that something is going on.
void Mediator::handleSocketError(QAbstractSocket::SocketError err)
{
    qDebug()<< "Socket Error " << err << m_webSocket.errorString () ;
    socketError ( m_webSocket.errorString () );
}

void Mediator::handleSocketState(QAbstractSocket::SocketState sta)
{
    if(m_internalSocketState == sta){
        return;
    }
    else {
        qDebug() << "Socket State Has Changed " << sta;
        switch(sta){
        case QAbstractSocket::UnconnectedState :
            m_socketState = Unconnected;
            m_internalSocketState = QAbstractSocket::UnconnectedState;
            break;
        case QAbstractSocket::HostLookupState :
            m_socketState = Lookup;
            m_internalSocketState = QAbstractSocket::HostLookupState;
            break;
        case QAbstractSocket::ConnectingState :
            m_socketState = Connecting;
            m_internalSocketState = QAbstractSocket::ConnectingState;
            break;
        case QAbstractSocket::ConnectedState :
            m_socketState = Connected;
            m_internalSocketState = QAbstractSocket::ConnectedState;
            break;
        case QAbstractSocket::BoundState :
            m_socketState = Bound;
            m_internalSocketState = QAbstractSocket::BoundState ;
            break;
        case QAbstractSocket::ClosingState :
            m_socketState = Closing;
            m_internalSocketState = QAbstractSocket::ClosingState;
            break;
        case QAbstractSocket::ListeningState :
            m_socketState = Listening;
            m_internalSocketState = QAbstractSocket::ListeningState;
            break;
        default :
            m_socketState = Default;
            break;
        }
        emit socketStateChanged ();
    }
}

void Mediator::OnpPlayerQuoteSliceModelItemChanged(PlayerQuoteSliceModelItem *name)
{
    qDebug() << name;
}

//void handleSocketState(QAbstractSocket::SocketState sta)
//{

//}

//#include <QStandardPaths>
//std::string Mediator::lastYearPath() {
//    std::string ret = "";
//#ifdef Q_OS_WIN32
////    QSettings settings(QStringLiteral("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{79913283-A35B-4274-927C-1B52D286D939}_is1"), QSettings::NativeFormat);
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

////    "HKCU\\SOFTWARE\\Classes\\VirtualStore\\MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{79913283-A35B-4274-927C-1B52D286D939}_is1"

//    return ret;
//}

////HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{79913283-A35B-4274-927C-1B52D286D939}_is1


void Mediator::allNamesGet() {
    WsReq req;
    req.set_ctype(GETALLNAMES);
    auto txstr = req.SerializeAsString();
    QByteArray qb(txstr.data(),(size_t)txstr.size());
    qDebug() << " allNamesGet sending " << req.DebugString().data();
    m_webSocket.sendBinaryMessage(qb);
}

void Mediator::rowMarketGet() {
    WsReq req;
    req.set_ctype(GETROWMARKET);
    auto txstr = req.SerializeAsString();
    QByteArray qb(txstr.data(),(size_t)txstr.size());
    qDebug() << " rowmarket sending " << req.DebugString().data();
    m_webSocket.sendBinaryMessage(qb);
}

void Mediator::getOrderPos() {
    getOrderPos("");
}

void Mediator::getOrderPos(const QString &in) {
    if ( m_fantasy_agent.HaveClient() )
        getOrderReq(m_fantasy_agent.currentClient().data(),in);
}


void Mediator::getDepthRep() {
    if ( m_currentPidContext != "" ) {
        auto txstr = mGetDepthReq.SerializeAsString();
        QByteArray qb(txstr.data(),(size_t)txstr.size());
    //    qDebug() << " getDepthRep sending " << mGetDepthReq.DebugString().data();
        m_webSocket.sendBinaryMessage(qb);

    }

    if ( depthBackup > 5 || depthCount > 20 ) {
        depthInterval *= 2;
        depthCount = 0;
        depthBackup -= 5;
        if ( depthBackup < 0 ) depthBackup = 0;
        if ( polldepth.isActive() ) {
            polldepth.start(depthInterval);
//            qDebug() << "getDepthRep depthInterval " << depthInterval << " depthBackup " << depthBackup;
        }
    }
    else if ( depthInterval > 1000 && depthCount <= 0 ){
        //reset
//        qDebug() << "getDepthRep depthInterval " << depthInterval << " depthBackup " << depthBackup;
        depthInterval = 100 * depthBackup * 2;
        if ( depthInterval < 1000 ) depthInterval = 1000;

        if ( polldepth.isActive() ) {
            polldepth.stop();
            polldepth.start(depthInterval);
//            qDebug() << "getDepthRep depthInterval " << depthInterval << " depthBackup " << depthBackup;
        }
    }

//    qDebug() << "getDepthRep depthInterval " << depthInterval << " depthBackup " << depthBackup;

    depthBackup++;
    depthCount++;

}



Mediator *Mediator::myInstance;


