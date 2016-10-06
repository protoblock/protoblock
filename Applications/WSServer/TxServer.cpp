#include "txserver.h"
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
#include "server.h"

#include "RestfullCall.h"

QT_USE_NAMESPACE

TxServer::TxServer(quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("WS TxServer"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_clients(),
    m_debug(debug)
{

//    mNameData.init();
    TxPool::init();
#ifdef PROD_SEASON_TRADING
    Server::NFLData.init();
#endif

    Server::TheExchange.init();
    mySeq = Server::TheExchange.MAXSEQ;

    mport = port;

    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {

        QHostAddress hInf = m_pWebSocketServer->serverAddress();

        qDebug() << "WS TxServer " << hInf.toString() << " listening on port" << port << m_pWebSocketServer->serverName() << " ";
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &TxServer::onNewConnection);

        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &TxServer::closed);
    }


    connect (this,SIGNAL(error(QString)),this,SLOT(handleError(QString)));
    AllNamesRepPtr = &Server::instance()->AllNamesRep;
    if ( AllNamesRepPtr->names_size() > MaxNames)
        AllNamesRepPtr->mutable_names()->DeleteSubrange(0,AllNamesRepPtr->names_size() - MaxNames);
}

TxServer::~TxServer() {
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}


void TxServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &TxServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &TxServer::socketDisconnected);
    m_clients << pSocket;
}

void TxServer::processBinaryMessage(const QByteArray &message) {
    RestfullClient rest(QUrl(PAPIURL.data()));
    rest.postRawData("tx","octet-stream",message);

    /*
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
//            AllNamesRepPtr->add_names(nt.fantasy_name());
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
            auto fn = getFNverifySt(st);//Commissioner::getName(st.fantasy_name());//getFNverifySt(st);
            if ( !fn ) {
                qDebug() << "Error bad fn ";
                break;
            }

            auto emdg = t.GetExtension(fantasybit::ExchangeOrder::exchange_order);
            qDebug() << emdg.DebugString().data();
            Server::TheExchange.OnNewOrderMsg(emdg,++mySeq,fn);
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
*/
    return;
}

std::shared_ptr<FantasyName> TxServer::getFNverifySt(const SignedTransaction &st) {

    std::shared_ptr<FantasyName> ret;
    if (st.trans().version() != Commissioner::TRANS_VERSION) {
        qCritical() << " !verifySignedTransaction wrong trans version! ";
        return ret;
    }

    pb::sha256 digest = pb::hashit(st.trans().SerializeAsString());
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
#ifdef VERIFY_NAME_TX
        return false;
#endif
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


void TxServer::handleError(const QString err)
{
    qDebug() << "TxServer ProRoto Error " << err ;
}
