#ifndef PBGATEWAYS_H
#define PBGATEWAYS_H

#include <vector>
#include "StaticData.pb.h"
#include "ProtoData.pb.h"
#include <memory>
#include "FantasyName.h"
//#include "playerprojmodel.h"
//#include "playerquoteslicemodel.h"
#include "ExchangeData.h"

namespace pb {

class PlayerQuoteSliceModel;

class ITradingProxy {
public:
    virtual ~ITradingProxy(){}

protected:
    virtual void GotMarketSnaps() = 0;

public: //slots:
    virtual void OnFinishMarketSnapShot(int) = 0;

    virtual PlayerQuoteSliceModel & GetPlayerQuoteSliceModel() = 0;
};

class IDataService {
public:
    virtual std::vector<fantasybit::GameRoster> GetCurrentWeekGameRosters() = 0;

    virtual std::vector<fantasybit::GameResult> GetPrevWeekGameResults(int season,int week)= 0;

    virtual fantasybit::WeeklySchedule GetWeeklySchedule(int season, int week)= 0;

    virtual fantasybit::PlayerBase GetPlayerBase(std::string playerId)= 0;

    virtual fantasybit::PlayerStatus GetPlayerStatus(std::string playerId) = 0;

    virtual std::vector<std::shared_ptr<fantasybit::FantasyName>> GetLeaderBoard()= 0;

    virtual std::unordered_map<std::string,int> GetProjByName(const std::string &nm)= 0;

    virtual std::unordered_map<std::string,int> GetProjById(const std::string &pid)= 0;

    virtual fantasybit::GlobalState GetGlobalState()= 0;

    virtual std::unordered_map<std::string,fantasybit::PlayerDetail> GetTeamRoster(const std::string &teamid)= 0;
    virtual fantasybit::GameStatus GetGameStatus(std::string gid)= 0;


    virtual fantasybit::MyFantasyName importMnemonic(const std::string &in)= 0;
    virtual std::string exportMnemonic(std::string &in)= 0;
    virtual int GetAvgProjection(const std::string &playerid) = 0;


    virtual ordsnap_t
        GetOrdersPositionsByName(const std::string &fname)= 0;

    virtual std::vector<MarketSnapshot>
                    GetCurrentMarketSnaps() = 0;
    virtual std::map<std::string,std::string> GetAllSymbols() = 0;

    virtual PlayerDetail GetPlayerDetail(const std::string &symbol) = 0;

    virtual std::string GetPidfromSymb(const std::string &symb) = 0;

    virtual int GetOpenPnl(const std::string &fname) = 0;

    virtual std::map<std::string, std::string> GetTeamSymbols(const std::list<std::string> &teams) = 0;


};

class IPBGateway {

public:

    virtual ~IPBGateway(){} // do not forget this

protected: //signals: // <- ignored by moc and only serves as documentation aid
            // The code will work exactly the same if signals: is absent.

    virtual void PlayName(string) = 0;
    virtual void NameStatus(fantasybit::MyFantasyName) = 0;
    virtual void LiveProj(fantasybit::FantasyBitProj) = 0;
    virtual void MyNames(std::vector<fantasybit::MyFantasyName>) = 0;
    virtual void NameBal(fantasybit::FantasyNameBal) = 0;
    virtual void PlayerStatusChange(std::pair<std::string,fantasybit::PlayerStatus> in) = 0;
    virtual void GlobalStateChange(fantasybit::GlobalState) = 0;
    virtual void LiveGui(fantasybit::GlobalState) = 0;
    virtual void NewWeek(int) = 0;
    virtual void GameStart(std::string) = 0;
    virtual void GameOver(std::string) = 0;
    virtual void onControlMessage(QString) = 0;

    virtual void NewFantasyName(fantasybit::FantasyNameBal) = 0;
    virtual void AnyFantasyNameBalance(fantasybit::FantasyNameBal) = 0;
    virtual void Height(int) = 0;
    virtual void BlockNum(int) = 0;

public: //slots:
    virtual void OnUseName(QString) = 0;


public:
    IDataService *dataService;
    ITradingProxy *tradingProxy;
};

}


Q_DECLARE_INTERFACE(pb::IPBGateway, "Protoblock.PBGateways/1.0")
Q_DECLARE_INTERFACE(pb::IDataService, "Protoblock.DataService/1.0")
Q_DECLARE_INTERFACE(pb::ITradingProxy, "Protoblock.TradingProxy/1.0")



#endif // PBGATEWAYS_H







/* WebSocket stuff   *


Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
Q_PROPERTY(QString webSocketErrorString  READ webSocketErrorString NOTIFY webSocketErrorStringChanged)

Q_PROPERTY(SocketState socketState READ socketState NOTIFY socketStateChanged)
Q_ENUMS (SocketState)






/*!
 * \brief webSocketErrorString
 * For SOCKET errors only
 *
QString webSocketErrorString()const;

/*!
 * \brief errorString
 * Font NON -Socket releated errors
 *
QString errorString()const;

/*!
 * socket state used alot to check what is going on with the sockets from QML
 *
enum SocketState{
    Unconnected,
    Lookup,
    Connecting,
    Connected,
    Bound,
    Closing,
    Listening,
    Default
};
SocketState socketState()const{
    return m_socketState;
}

    qint64 sendBinaryMessage(const QByteArray &data);
    qint64 sendTextMessage(const QString &message);
    QHostAddress  peerAddress() const;
    QAbstractSocket::SocketState  state()const;
    QWebSocketProtocol::Version  version()const;

    qint64 sendBinaryMessage(const GOOGLE_NAMESPACE::protobuf::Message &data);


    void error(QString);
    void socketError(QString);
    void socketStateChanged();
    void webSocketErrorStringChanged();
    void errorStringChanged();


    void handleError(const QString err);
    void handleWebSocketError(const QString err);


    void handleClosed();
    void onConnected();
    void handleAboutToClose();
    void onTextMessageReceived( QString message);
    void onBinaryMessageRecived(const QByteArray &message);




    void handleSocketError(QAbstractSocket::SocketError err);
//    void handleSocketState(QAbstractSocket::SocketState sta);
    void handleSocketState(QAbstractSocket::SocketState sta);



private:
    QWebSocket m_webSocket, m_txsocket;

    QString m_errorString;
    QString m_webSocketErrorString;
    SocketState m_socketState;
    QAbstractSocket::SocketState m_internalSocketState;





    m_socketState(Default),
    m_internalSocketState(QAbstractSocket::ListeningState),







    mGetDepthReq.set_ctype(GETDEPTH);
    GetDepthReq dr;
    dr.set_pid("na");
    mGetDepthReq.MutableExtension(GetDepthReq::req)->CopyFrom(dr);





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


*/







