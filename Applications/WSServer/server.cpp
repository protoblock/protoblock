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
#include "txpool.h"

QT_USE_NAMESPACE

TxServer::TxServer(quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("WS TxServer"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_clients(),
    m_debug(debug)
{

    mNameData.init();
    TxPool::init();
#ifdef PROD_SEASON_TRADING
    mNFLStateData.init();
#endif
    mport = port;

    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {

        QHostAddress hInf = m_pWebSocketServer->serverAddress();

        qDebug() << "WS TxServer " << hInf.toString() << " listening on port" << port << m_pWebSocketServer->serverName() << " ";
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &TxServer::onNewConnection);

        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &TxServer::closed);
    }


    connect (this,SIGNAL(error(QString)),this,SLOT(handleError(QString)));
    AllNamesRepPtr = &Server::AllNamesRep;
    if ( AllNamesRepPtr->names_size() > MaxNames)
        AllNamesRepPtr->mutable_names()->DeleteSubrange(0,AllNamesRepPtr->names_size() - MaxNames);
}

TxServer::~TxServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}


LiteServer::LiteServer(quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("WS LiteServer"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_clients(),
    m_debug(debug)
{

    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {

        QHostAddress hInf = m_pWebSocketServer->serverAddress();

        qDebug() << "WS LiteServer " << hInf.toString() << " listening on port" << port << m_pWebSocketServer->serverName() << " ";
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &LiteServer::onNewConnection);

        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &LiteServer::closed);
    }


    connect (this,SIGNAL(error(QString)),this,SLOT(handleError(QString)));

}

LiteServer::~LiteServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}


void LiteServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &LiteServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &LiteServer::socketDisconnected);
    m_clients << pSocket;
}

void TxServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &TxServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &TxServer::socketDisconnected);
    m_clients << pSocket;
}


//enum CType {
//    CHECKNAME = 1;
//    NEWTX = 2;
//    PK2FNAME = 3;
//    GETSTATUS = 4;
//    GETALLNAMES = 5;
//    GETROWMARKET = 6;
//    GETDEPTH = 7;
//}


void LiteServer::processBinaryMessage(const QByteArray &message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

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
            rep.SerializeToString(&mRepstr);
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
            rep.SerializeToString(&mRepstr);
            break;
        }
        case GETALLNAMES: {
            rep.set_ctype(GETALLNAMES);
    //            rep.MutableExtension(GetAllNamesRep::rep)->CopyFrom(*AllNamesRepPtr);
            rep.SetAllocatedExtension(GetAllNamesRep::rep,&Server::AllNamesRep);
            rep.SerializeToString(&mRepstr);
            rep.ReleaseExtension(GetAllNamesRep::rep);
            break;
        }

#ifdef PROD_SEASON_TRADING
        case GETDEPTH:
    //            GetDepthRep rrr;
    //            rrr.add_depthitems(
            rep.SerializeToString(&mRepstr);
            break;
        case GETROWMARKET:
            rep.set_ctype(GETROWMARKET);
    //            rep.SetAllocatedExtension( MutableExtension(GetAllNamesRep::rep)->CopyFrom(*AllNamesRepPtr);
            rep.SerializeToString(&mRepstr);
            break;
#endif
        default:
            return;
    }

//        rep.mutable_req()->CopyFrom(req);

    QByteArray qb(mRepstr.data(),(size_t)mRepstr.size());
    pClient->sendBinaryMessage(qb);
    if ( rep.ctype() == GETALLNAMES)
        qDebug() << rep.ctype() <<" size " << Server::AllNamesRep.names_size();
    else
        qDebug() << rep.DebugString().data();
    return;
}

void TxServer::processBinaryMessage(const QByteArray &message) {
    fantasybit::SignedTransaction st;
    std::string ststr = message.toStdString();
    st.ParseFromString(ststr);

    TxPool::addTxPool(st.id(),ststr);

    const Transaction &t = st.trans();
    pb::sha256 digest = pb::hashit(t.SerializeAsString());
    secp256k1_ecdsa_signature sig = Commissioner::str2sig(st.sig());

//        enum TransType {
//          NAME = 0,
//          PROJECTION = 1,
//          RESULT = 2,
//          DATA = 3,
//          PROJECTION_BLOCK = 4,
//          MASTER_NAME = 5,
//          TIME = 6,
//          STAMPED = 7,
//          EXCHANGE = 8,
//          EXCHANGE_BLOCK = 9
//        };
    switch (t.type()) {

        case TransType::NAME: {
            auto nt = t.GetExtension(fantasybit::NameTrans::name_trans);

            if (!verify_name(st, nt, sig, digest)) {
                qDebug() << " !verify name";
                return;
            }

            mNameData.AddNewName(nt.fantasy_name(), nt.public_key() );
            qDebug() <<  "verified " << FantasyName::name_hash(nt.fantasy_name()) << " adding name";
            if ( AllNamesRepPtr->names_size() >= MaxNames) {
                qDebug() << " post purge " << AllNamesRepPtr->names_size() << " > " << MaxNames;

                int endpurge = MaxNames/10;
                if (endpurge > 1)
                    AllNamesRepPtr->mutable_names()->DeleteSubrange(0,endpurge);
                qDebug() << " post purge " << AllNamesRepPtr->names_size();
            }
            AllNamesRepPtr->add_names(nt.fantasy_name());
    //            AllNamesRep.add_names(nt.fantasy_name());
    //            AllNamesRep2.add_names(nt.fantasy_name());
    //            if ( AllNamesRepPtr->names_size() >= 1000) {
    //                AllNamesRepPtr->clear_names();
    //                AllNamesRepPtr = (AllNamesRep.names_size() > AllNamesRep2.names_size()) ? &AllNamesRep : &AllNamesRep2;
    //            }

            break;
        }

#ifdef PROD_SEASON_TRADING
        case TransType::EXCHANGE: {
            auto emdg = t.GetExtension(fantasybit::ExchangeOrder::exchange_order);
            mExchangeData.OnNewOrderMsg(emdg,++mySeq,fn);
            break;
        }
#endif

        default:
            break;
      }

//    StampedTrans mStampedTrans;
//    Transaction trans{};
//    trans.set_version(Commissioner::TRANS_VERSION);
//    trans.set_type(TransType::STAMPED);

//    trans.SerializeAsString();
//    trans.MutableExtension(StampedTrans::stamped_trans)->CopyFrom(stt);

//    trans.SetAllocatedExtension(StampedTrans::stamped_trans,mStampedTrans);

//    mStampedTrans.set_allocated_signed_orig();


//    trans.SerializeToString()
    return;
}

std::shared_ptr<FantasyName> TxServer::getFNverifySt(const SignedTransaction &st) {

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

bool TxServer::verify_name(const fantasybit::SignedTransaction &st, const NameTrans &nt,
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

    if ( !Commissioner::verify(sig, digest, pk) ) {
        qDebug() << " bad signed sig";
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



void TxServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "socketDisconnected:" << pClient  << " Reason: " << pClient->closeReason ();
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}


void LiteServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "socketDisconnected:" << pClient  << " Reason: " << pClient->closeReason ();
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
void LiteServer::handleError(const QString err)
{
    qDebug() << "LiteServer ProRoto Error " << err ;
}

void TxServer::handleError(const QString err)
{
    qDebug() << "TxServer ProRoto Error " << err ;
}


fantasybit::GetAllNamesRep Server::AllNamesRep{};
