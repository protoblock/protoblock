#include "server.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
//#include "tempapi.h"
#include "StateData.pb.h"
//#include "ldbhashreadertool.h"
//#include "pbjson.hpp"
//#include "src/protoblockapi.h"

#include "Commissioner.h"

QT_USE_NAMESPACE

Server::Server(quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("WS Server"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_clients(),
    m_debug(debug)
{

    mNameData.init();
    mNFLStateData.init();
    mport = port;


//    LdbHashReaderTool *ldb = LdbHashReaderTool::instance();
//    auto ret =
//            ldb->getJsonDrill("5fa4d88d787667587fee6afd86a24743b2cb914231ea9e75a144e954b85e0346","WeekGameStatusMeta");
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {

        QHostAddress hInf = m_pWebSocketServer->serverAddress();

        qDebug() << "WS server " << hInf.toString() << " listening on port" << port << m_pWebSocketServer->serverName() << " ";
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Server::onNewConnection);

        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &Server::closed);
    }


    connect (this,SIGNAL(error(QString)),this,SLOT(handleError(QString)));

//                fantasybit::TempApi tempapi;

    AllNamesRepPtr = &AllNamesRep;
}

Server::~Server()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}


Server::Incoming Server::setEnum(const QString &string)
{

    //FIXME check for argument





    if ( string  == "BlockMeta" ) { return GetBlockHeader; }
    if ( string == "LeaderBoardTree"){return GetLeaderBoardTree;}
    if( string == "Ticker" ) {return GetTicker;}
    if ( string == "Stats"){return GetStats;}
    if(string == "PlayerInfo"){return GetPlayerInfo;}
    if(string == "AwardsItem"){return GetAwardsItem;}
    if(string == "MerkleTree"){return GetMerkleTree;}
    if(string == "GetDrillMerkle"){return GetDrillMerkle;}
    if ( string == "LeaderBoardItem"){ return GetLeaderBoardItem;  }
    else{
        return UnKnown;
        error ("Unknow command to the Ws Server");
    }
}

QStringList Server::createCommandArgument(const QString &cmd)
{
    QStringList argument = cmd.split (",");
//    if(argument.length () > 0 && argument.at (1) == ""){
//        QString er = QString("You must pass in a argument to the function %1").arg (argument.at (0));
//        error (er);
//        return QStringList();
//    }
//    else
//    {
    return argument;
//    }
}


void Server::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Server::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Server::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Server::socketDisconnected);
//    connect (pSocket, &QWebSocket::aboutToClose (),this SLOT());
    m_clients << pSocket;
}


void Server::processTextMessage(QString message)
{
//    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    qDebug() << " processTextMessage ";
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());


    QString ret("false");
    if ( fantasybit::Commissioner::isAliasAvailable(message.toStdString()) )
        ret = "true";

    pClient->sendTextMessage(ret);

    return;

}


void Server::processBinaryMessage(QByteArray message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if ( mport == PB_WS_LITE_AGENT_PORT) {
        fantasybit::WsReq req;
        req.ParseFromString(message.toStdString());
        fantasybit::WSReply rep;

        qDebug() << " processBinaryMessage " << req.DebugString().data();
        switch ( req.ctype() ) {
        case PK2FNAME:
        {
            Pk2FnameRep pkr;
            pkr.mutable_req()->CopyFrom(req.GetExtension(Pk2FnameReq::req));
            rep.set_ctype(PK2FNAME);
            auto fname = Commissioner::getName(Commissioner::str2pk(pkr.req().pk()));
            if ( fname == nullptr)
                pkr.set_fname("");
            else
                pkr.set_fname(fname->alias());


            rep.MutableExtension(Pk2FnameRep::rep)->CopyFrom(pkr);
            break;
        }
        case CHECKNAME:
        {
            rep.set_ctype(CHECKNAME);
            CheckNameRep cr;
            cr.mutable_req()->CopyFrom(req.GetExtension(CheckNameReq::req));
            if ( fantasybit::Commissioner::isAliasAvailable(cr.req().fantasy_name()))
                cr.set_isavail("true");
            else
                cr.set_isavail("false");

            rep.MutableExtension(CheckNameRep::rep)->CopyFrom(cr);
            break;
        }
        case GETALLNAMES: {
            rep.set_ctype(GETALLNAMES);
            rep.MutableExtension(GetAllNamesRep::rep)->CopyFrom(*AllNamesRepPtr);
            break;
        }
//        case NameStatusReq:
//        {
//            rep.set_ctype(GETSTATUS);
//        }
        default:
            return;
        }

//        rep.mutable_req()->CopyFrom(req);

        auto repstr = rep.SerializeAsString();
        QByteArray qb(repstr.data(),(size_t)repstr.size());
        pClient->sendBinaryMessage(qb);
        if ( rep.ctype() == GETALLNAMES)
            qDebug() << rep.ctype() <<" size " << AllNamesRepPtr->names_size();
        else
            qDebug() << rep.DebugString().data();
        return;
    }
    else {
        fantasybit::SignedTransaction st;
        st.ParseFromString(message.toStdString());


        const Transaction &t = st.trans();
        pb::sha256 digest = pb::sha256(t.SerializeAsString());
        secp256k1_ecdsa_signature sig = Commissioner::str2sig(st.sig());

        if (t.type() == TransType::NAME) {
            auto nt = t.GetExtension(fantasybit::NameTrans::name_trans);

            if (!verify_name(st, nt, sig, digest)) {
                qInfo() << " !verify name";
                return;
            }

            mNameData.AddNewName(nt.fantasy_name(), nt.public_key() );
            qInfo() <<  "verified " << FantasyName::name_hash(nt.fantasy_name()) << " adding name";
            AllNamesRep.add_names(nt.fantasy_name());
            AllNamesRep2.add_names(nt.fantasy_name());
            if ( AllNamesRepPtr->names_size() >= 1000) {
                AllNamesRepPtr->clear_names();
                AllNamesRepPtr = (AllNamesRep.names_size() > AllNamesRep2.names_size()) ? &AllNamesRep : &AllNamesRep2;
            }
//            fantasybit::WSReply rep;
//            rep.set_ctype(PK2FNAME);

//            Pk2FnameReq req;
//            req.set_pk(nt.public_key());

//            Pk2FnameRep pkr;
//            pkr.set_fname(nt.fantasy_name());
//            pkr.mutable_req()->CopyFrom(req);

//            rep.MutableExtension(Pk2FnameRep::rep)->CopyFrom(pkr);
//            auto repstr = rep.SerializeAsString();
//            QByteArray qb(repstr.data(),(size_t)repstr.size());
//            pClient->sendBinaryMessage(qb);
//            qDebug() << rep.DebugString().data();
            return;
        }
    }
}

std::shared_ptr<FantasyName> Server::getFNverifySt(const SignedTransaction &st) {

    std::shared_ptr<FantasyName> ret;
    if (st.trans().version() != Commissioner::TRANS_VERSION) {
        qCritical() << " !verifySignedTransaction wrong trans version! ";
        return ret;
    }

    pb::sha256 digest = pb::sha256(st.trans().SerializeAsString());
    if (digest.str() != st.id()) {
        qCritical() << "digest.str() != st.id() ";
        return ret;
    }

    if ( st.fantasy_name() == "") {
        qCritical() << " Blank FantasyName";
        return ret;
    }

    ret = Commissioner::getName(st.fantasy_name());
    if ( !ret )
        qCritical() << " cant find FantasyName" << st.fantasy_name();
    else {
        secp256k1_ecdsa_signature sig = Commissioner::str2sig(st.sig());
        auto pk =  ret->pubkey ();
        if ( !Commissioner::verify(sig,digest, pk) ) {
            ret.reset();
            qCritical() << "verify error" << st.fantasy_name() << "getFNverifySt";
        }
    }
    return ret;
}

bool Server::verify_name(const fantasybit::SignedTransaction &st, const NameTrans &nt,
    const secp256k1_ecdsa_signature& sig, const pb::sha256 &digest) {

    if ( !Commissioner::isAliasAvailable(nt.fantasy_name()) )
    {
        qCritical() << std::string("Processor::process failure: FantasyName(").
                        append(nt.fantasy_name() + ")  hash already used ");
        return false;
    }


    auto pk = Commissioner::str2pk(nt.public_key());
    auto name = Commissioner::getName(pk);
    if ( name != nullptr ) {
        qCritical() << std::string("verfiy_name failure: FantasyName(").
                        append(nt.fantasy_name() + ")  pubkey already n use") +
                         name->ToString();
                        //.append(st.DebugString());

        return false;
    }

    auto proof = nt.proof();
    switch (proof.type())
    {
        case NameProof_Type_TWEET:
        {
            auto tp = proof.GetExtension(TweetProof::tweet_proof);
            //TODO verify tweet
            return true;
        }
        break;

        case NameProof_Type_ORACLE:
        {
            return true;//TODO
            //verify oracle
            if (!Commissioner::verifyOracle(sig, digest))
#ifdef NO_ORACLE_CHECK_TESTING
            if (!Commissioner::verifyByName(sig, digest, st.fantasy_name()))
#endif
            {
                qCritical() << std::string("Processor::process name proof oracle failed")
                                 .append(st.DebugString());

                return false;
            }
            else
                return true;
        }
        break;

        default:
            return true;
            break;
    }
}



void Server::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "socketDisconnected:" << pClient  << " Reason: " << pClient->closeReason ();
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void Server::handleError(const QString err)
{
    qDebug() << "ProRoto Error " << err ;
}

fantasybit::GetAllNamesRep Server::AllNamesRep{};
