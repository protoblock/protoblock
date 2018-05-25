/*
//  Node.cpp
//
//  Protoblock
//
//  Created by Jay Berg on 5/21/2018
//
*/
#include "Node.h"
#include <QNetworkReply>
#include <openssl/rand.h>
#include <QTimer>
#include <peerwire.h>

using namespace pb;
using namespace fantasybit;

Node::Node(QObject *parent) : QTcpServer(parent), http(this)
{
    mPeer.set_is_listening(Peer::NOTSURE);
    mPeer.set_port(PORT_DEFAULT);

    mSessionId.set_network_id(IS_TEST_NET ? SessionId::TEST : SessionId::PROD);
    mSessionId.set_wire_version(WIRE_VERSION);

    int genesisepoch = 1397433600;
    int startupTime = int(QDateTime::currentDateTime().toTime_t());
    mSessionId.set_start_time(startupTime - genesisepoch);
    unsigned char buf[6];
    int ret = RAND_bytes(buf, 6);

    if ( ret != 1 )
        qDebug() << " error generating uuid";
    else
        mSessionId.set_uuid(QByteArray((const char *)buf,6).toBase64().toStdString());

    qDebug() << mSessionId.DebugString().data();

    mPeerChainStatus.set_chain_state(PeerChainStatus::STARTING);

    pstore.init();
    for ( const auto pp : pstore.peersmm ) {
        Peer *np = new Peer();
        np->CopyFrom(pp.second);
        if ( np->port() == 0 )
            np->set_port(PORT_DEFAULT);
        knownpeers.insert(np);
    }
}

Node::~Node() {

}

void Node::startPoint()
{
    connect(this, &Node::tryGetIp, this, &Node::getMyIp, Qt::QueuedConnection);

    getMyIp();
}

void Node::getMyIp()
{
    if (ipUrlsTry >= IP_URLS.size()) {
        gotPublicIp(false);
        return;
    }

    auto inu = IP_URLS.at(ipUrlsTry);

    QObject::connect(&http, &QNetworkAccessManager::finished,
        this, [=](QNetworkReply *qreply) {

                if (qreply->error()) {
                    qDebug() << "err" << qreply->errorString();
                    qreply->deleteLater();
                    qreply->abort();
                    emit tryGetIp();
                }

                else if ( qreply->bytesAvailable() > 0 ) {
                    QString answer = qreply->readAll();

                    qDebug() << "answer" << answer << qreply->isFinished() << answer.trimmed();

                    qreply->deleteLater();
                    qreply->abort();
                    if ( !gotMyIp ) {
                        if ( answer == "")
                            emit tryGetIp();
                        else {
                            mMyAddress = answer.toStdString();
                            qDebug() << " got ip" << mMyAddress.data();
                            gotMyIp = true;
                            gotPublicIp(true);
                        }
                    }
                }
            }
    );

    QString url = inu.data();
    QNetworkRequest req;
    req.setUrl(QUrl(url));
    http.get(req);
    ipUrlsTry++;

    return;
}

void Node::gotPublicIp(bool gotip)  {
    if ( !gotip ) {
        qDebug() << "dont know my ip!";
        mPeer.set_address("");
    }
    else
        mPeer.set_address(mMyAddress);

    startServer();
}


void Node::startServer() {
    connect(this,&QTcpServer::acceptError,
            this, [this] (QAbstractSocket::SocketError socketError) {
                qDebug() << socketError << " tcpserver" << this->errorString();
            });

    // Start the server
    qDebug() << " startserver ";
    if (!this->isListening()) {
        // Set up the peer wire server
        this->listen(QHostAddress::AnyIPv4, PORT_DEFAULT);
        if (!this->isListening()) {
            qDebug() << " !listening " << errorString();
            mPeer.set_is_listening(Peer::NO);
            mPeer.set_port(0);
            return;
        }
        else {
            qDebug() << "server listening " <<
            this->serverAddress() << this->serverPort();
        }
    }

    mPeerChainStatus.set_chain_state(PeerChainStatus::SEARCHING);
    callPeerConnector();
}

void Node::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << " incomingConnection ";
    PeerWire *client = new PeerWire(this);
    QHostAddress qh = client->setSocketDescriptor(socketDescriptor);
    Peer *p = new fantasybit::Peer;
    p->set_is_listening(Peer::NOTSURE);
    p->set_address(QHostAddress(qh.toIPv4Address()).toString().toStdString());
    p->set_port(0);
    client->init(p);
    m_connections.insert(client);
}

void Node::callPeerConnector()
{
    if (!connectingToClients) {
        connectingToClients = true;
        QTimer::singleShot(10000, this, SLOT(connectToPeers()));
    }
}

inline PeerChainStatus::ChainState Node::getChainState() const {
    return mPeerChainStatus.chain_state();
}

inline void Node::setChainState(const PeerChainStatus::ChainState state) {
    return mPeerChainStatus.set_chain_state(state);
}

void Node::connectToPeers()
{
    connectingToClients = false;

    if (    getChainState() == PeerChainStatus::REORG ||
            getChainState() == PeerChainStatus::STOPPING ||
            getChainState() == PeerChainStatus::ABORTING )
        return;

    if (getChainState() == PeerChainStatus::STARTING || getChainState() == PeerChainStatus::SEARCHING)
        setChainState(PeerChainStatus::CONNECTING);

//    // Find the list of peers we are not currently connected to, where
//    // the more interesting peers are listed more than once.
    std::vector<Peer *> freePeers{knownpeers.begin(),knownpeers.end()};
    std::vector<bool> used;
    used.assign(freePeers.size(),false);
    int remaining = freePeers.size();
    // Start as many connections as we can
    while (m_numoutgoing < MAX_OUTBOUND && remaining > 0) {
        PeerWire *client = new PeerWire(this);
//        RateController::instance()->addSocket(client);
//        ConnectionManager::instance()->addConnection(client);

//        initializeConnection(client);
//        d->connections.insert(client);

        // Pick a random peer from the list of weighed peers.
        int index = 0;
        do {
            index = (qrand() % freePeers.size());
            if ( used.at(index) ) {
                index++;
                if ( index >= freePeers.size())
                    index = 0;
            }
        } while (used.at(index));
        remaining--;
        used.at(index) = true;

//        client->setPeer();
//        peer->connectStart = QDateTime::currentDateTime().toTime_t();
//        peer->lastVisited = peer->connectStart;

        // Connect to the peer.
//        qDebug() << peer->address << peer->port;
//        peer->noConnectTries++;
//        client->setPeer(peer);
        client->init(freePeers.at(index));
        client->connectToHost();
    }

    if ( freePeers.size() == 0 )
        callPeerConnector();
}



decltype(pb::Node::IP_URLS) pb::Node::IP_URLS {
     "http://api.ipify.org/",
     "http://myexternalip.com/raw",
     "http://icanhazip.com/",
     "http://myip.dnsomatic.com/",
     "http://ifcaonfig.me/ip"
};

decltype (pb::Node::mSessionId) pb::Node::mSessionId{};
decltype (pb::Node::mPeer)      pb::Node::mPeer{};
