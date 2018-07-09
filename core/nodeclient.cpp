/*
//  nodeclient.cpp
//
//  Protoblock
//
//  Created by Jay Berg on 5/21/2018
//
*/
#include "nodeclient.h"
#include <QNetworkReply>
#include <connectionmanager.h>
#include <peerstore.h>
#include <set>
#include <unordered_set>
#include <peerwireclient.h>
#include <QTimer>
#include "nodeserver.h"

static const int MaxOutbound = 8;
static const int MinimumTimeBeforeRevisit = 30;
class NodePeer;
class NodeClientPrivate
{
public:
    NodeClientPrivate(NodeClient *qq) : q(qq) {
        pstore.init();
        for ( const auto pp : pstore.peersmm ) {
            NodePeer *np = new NodePeer;
            np->address.setAddress(pp.second.address().data());
            np->port = 10130;//pp.second.port();
            np->ipport = pp.first;
//            np->interesting = true;
            np->lastVisited = 0;
            np->connectStart = 0;
            np->connectTime = 999999;
            np->noConnectTries = 0;
//            np->upordown = 0;
            knownpeers.insert(np);
        }
    }
    NodePeer selfPeer;
    pb::PeerStore pstore;

    // Connections
    std::unordered_set<PeerWireClient *> connections;
//    std::vector<PeerWireClient *> outconnections;
    std::unordered_set<NodePeer *> knownpeers;
    std::unordered_map<std::string,PeerWireClient *> ipportconnected;
    std::unordered_map<std::string,PeerWireClient *> idconnected;

    int numoutgoing = 0;

    NodeClient *q;
    NodeClient::State state;
    NodeClient::Error error;

    bool connectingToClients;


    /*
    // State / error
    void setError(TorrentClient::Error error);
    void setState(TorrentClient::State state);
    TorrentClient::Error error;
    TorrentClient::State state;
    QString errorString;
    QString stateString;

    // Where to save data
    QString destinationFolder;
    MetaInfo metaInfo;

    // Announce tracker and file manager
    QByteArray peerId;
    QByteArray infoHash;
    TrackerClient trackerClient;
    FileManager fileManager;

    // Connections
    QList<PeerWireClient *> connections;
    QList<TorrentPeer *> peers;
    bool schedulerCalled;
    void callScheduler();
    void callPeerConnector();
    int uploadScheduleTimer;

    // Pieces
    QMap<int, PeerWireClient *> readIds;
    QMultiMap<PeerWireClient *, TorrentPiece *> payloads;
    QMap<int, TorrentPiece *> pendingPieces;
    QBitArray completedPieces;
    QBitArray incompletePieces;
    int pieceCount;

    // Progress
    int lastProgressValue;
    qint64 downloadedBytes;
    qint64 uploadedBytes;
    int downloadRate[RateControlWindowLength];F
    int uploadRate[RateControlWindowLength];
    int transferRateTimer;
    */
    void setState(NodeClient::State state)
    {
        this->state = state;
        emit q->stateChanged(state);
    }

    void callPeerConnector()
    {
        if (!connectingToClients) {
            connectingToClients = true;
            QTimer::singleShot(10000, q, SLOT(connectToPeers()));
        }
    }

    void setError(NodeClient::Error errorCode)
    {
        this->error = errorCode;
//        switch (error) {
//        case TorrentClient::UnknownError:
//            errorString = QT_TRANSLATE_NOOP(TorrentClient, "Unknown error");
//            break;
//        case TorrentClient::TorrentParseError:
//            errorString = QT_TRANSLATE_NOOP(TorrentClient, "Invalid torrent data");
//            break;
//        case TorrentClient::InvalidTrackerError:
//            errorString = QT_TRANSLATE_NOOP(TorrentClient, "Unable to connect to tracker");
//            break;
//        case TorrentClient::FileError:
//            errorString = QT_TRANSLATE_NOOP(TorrentClient, "File error");
//            break;
//        case TorrentClient::ServerError:
//            errorString = QT_TRANSLATE_NOOP(TorrentClient, "Unable to initialize server");
//            break;
//        }
        emit q->error(errorCode);
    }
};

NodeClient::NodeClient(QObject *parent) : QObject(parent), http(this)
{
//    connect(&http, SIGNAL(finished(QNetworkReply*)), this, SLOT(httpRequestDone(QNetworkReply*)));

   ipUrls = {
        "http://api.ipify.org/",
        "http://myexternalip.com/raw",
        "http://icanhazip.com/",
        "http://myip.dnsomatic.com/",
        "http://ifcaonfig.me/ip"
   };

//   connect(&d->trackerClient, SIGNAL(peerListUpdated(QList<TorrentPeer>)),
//           this, SLOT(addToPeerList(QList<TorrentPeer>)));

}

void NodeClient::startPoint()
{
    d = new NodeClientPrivate(this);

    connect(this, &NodeClient::tryGetIp, this, &NodeClient::getMyIp, Qt::QueuedConnection);
    connect(this, &NodeClient::gotPublicIp,
            this, [=](bool gotip) {
                QHostAddress me;
                if ( gotip ) {
                    me.setAddress(myIp);
                    d->selfPeer.address = me;
                }
                d->selfPeer.id = ConnectionManager::instance()->clientId();
                startServer();
            });

    getMyIp();
//    // Connect the file manager
//    connect(&d->fileManager, SIGNAL(dataRead(int,int,int,QByteArray)),
//            this, SLOT(sendToPeer(int,int,int,QByteArray)));
//    connect(&d->fileManager, SIGNAL(verificationProgress(int)),
//            this, SLOT(updateProgress(int)));
//    connect(&d->fileManager, SIGNAL(verificationDone()),
//            this, SLOT(fullVerificationDone()));
//    connect(&d->fileManager, SIGNAL(pieceVerified(int,bool)),
//            this, SLOT(pieceVerified(int,bool)));
//    connect(&d->fileManager, SIGNAL(error()),
//            this, SLOT(handleFileError()));

//    // Connect the tracker client
//    connect(&d->trackerClient, SIGNAL(peerListUpdated(QList<TorrentPeer>)),
//            this, SLOT(addToPeerList(QList<TorrentPeer>)));
//    connect(&d->trackerClient, SIGNAL(stopped()),
    //            this, SIGNAL(stopped()));
}

void NodeClient::getMyIp()
{
    if (ipUrlsTry >= ipUrls.size()) {
        emit gotPublicIp(false);
        return;
    }

    auto inu = ipUrls.at(ipUrlsTry);

//http://api.ipify.org/
//http://myexternalip.com/raw
//http://icanhazip.com/
//http://myip.dnsomatic.com/
//http://ifconfig.me/ip
    qDebug() << " getMyIp " << inu.data();

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
                            myIp = answer;
                            qDebug() << " got ip" << myIp;
                            gotMyIp = true;
                            emit gotPublicIp(true);
                        }
                    }
                }
            }
    );

    QString url = inu.data();//"http://api.ipify.org/";

    QNetworkRequest req;
    req.setUrl(QUrl(url));
    http.get(req);
    ipUrlsTry++;

    return;
}

NodeClient::~NodeClient()
{
////    qDeleteAll(d->peers);
////    qDeleteAll(d->pendingPieces);
/// for
///
///


//    std::vector<NodePeer *> vknown(d->knownpeers.begin(),d->knownpeers.end());
//    d->knownpeers.clear();
//    qDeleteAll(vknown);

//    std::vector<d->connections.value_type> vcon(d->connections.begin(),d->connections.end());
//    d->connections.clear();
//    qDeleteAll(vcon);

//    for(auto it = d->knownpeers.begin(); it != d->knownpeers.end(); ) {
//        auto p = *it;
//        it = d->knownpeers.erase(it);
//        if ( p )
//           delete p;
//    }

//    for(auto it = d->connections.begin(); it != d->connections.end(); ) {
//        auto p = *it;
//        it = d->connections.erase(it);
//        if ( p )
//           delete p;
//    }

//    d->ipportconnected.clear();
//    d->idconnected.clear();

//    for(auto p : d->connections )
//        p->deleteLater();



//    if ( d ) delete d;
}

void NodeClient::startServer() {
    // Start the server
    qDebug() << " startserver ";
    NodeServer *server = NodeServer::instance();
//    connect(server,NodeServer::)
    if (!server->isListening()) {
        // Set up the peer wire server
        server->listen(QHostAddress::Any, pb::PORT_DEFAULT);
        if (!server->isListening()) {
            d->setError(ServerError);
            return;
        }
    }

    d->setState(Searching);
    connectToPeers();
}

std::vector<NodePeer *> NodeClient::FreePeers() const
{
    std::vector<NodePeer *> freePeers;

    // Generate a list of peers that we want to connect to.
    uint now = QDateTime::currentDateTime().toTime_t();
//    std::vector<NodePeer *> freePeers;
    for (NodePeer *np : d->knownpeers) {
        bool busy = false;
        auto it = d->ipportconnected.find(np->ipport);
        if ( it != end(d->ipportconnected) ) {
            PeerWireClient *client = it->second;
            if (client->state() == PeerWireClient::ConnectedState)
                busy = true;
        }

        if (!busy && (now - np->lastVisited) > uint(MinimumTimeBeforeRevisit * np->noConnectTries))
            freePeers.push_back(np);
    }

    return freePeers;
}

void NodeClient::connectToPeers()
{
    d->connectingToClients = false;

    if (d->state == Stopping || d->state == Reorging)
        return;

    if (d->state == Searching || d->state == Starting)
        d->setState(Connecting);

//    // Find the list of peers we are not currently connected to, where
//    // the more interesting peers are listed more than once.
    std::vector<NodePeer *> freePeers = FreePeers();
    std::vector<bool> used;
    used.assign(freePeers.size(),false);
    int remaining = freePeers.size();
    // Start as many connections as we can
    while (!d->numoutgoing < MaxOutbound && remaining > 0) {
        PeerWireClient *client = new PeerWireClient(ConnectionManager::instance()->clientId(), this);
//        RateController::instance()->addSocket(client);
        ConnectionManager::instance()->addConnection(client);

        initializeConnection(client);
        d->connections.insert(client);

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

        NodePeer *peer = freePeers.at(index);
        peer->connectStart = QDateTime::currentDateTime().toTime_t();
        peer->lastVisited = peer->connectStart;

        // Connect to the peer.
        qDebug() << peer->address << peer->port;
        peer->noConnectTries++;
        client->setPeer(peer);
        client->connectToHost(peer->address, peer->port);
    }

    if ( freePeers.size() == 0 )
        d->callPeerConnector();
}

void NodeClient::initializeConnection(PeerWireClient *client)
{
    connect(client, SIGNAL(connected()),
            this, SLOT(setupOutgoingConnection()));
    connect(client, SIGNAL(disconnected()),
            this, SLOT(removeClient()));
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(removeClient()));
//    connect(client, SIGNAL(piecesAvailable(QBitArray)),
//            this, SLOT(peerPiecesAvailable(QBitArray)));
//    connect(client, SIGNAL(blockRequested(int,int,int)),
//            this, SLOT(peerRequestsBlock(int,int,int)));
//    connect(client, SIGNAL(blockReceived(int,int,QByteArray)),
//            this, SLOT(blockReceived(int,int,QByteArray)));
//    connect(client, SIGNAL(choked()),
//            this, SLOT(peerChoked()));
//    connect(client, SIGNAL(unchoked()),
//            this, SLOT(peerUnchoked()));
//    connect(client, SIGNAL(bytesWritten(qint64)),
//            this, SLOT(peerWireBytesWritten(qint64)));
//    connect(client, SIGNAL(bytesReceived(qint64)),
//            this, SLOT(peerWireBytesReceived(qint64)));
}

void NodeClient::setupOutgoingConnection()
{
    PeerWireClient *client = qobject_cast<PeerWireClient *>(sender());

    // Update connection statistics.
    client->peer()->connectTime = client->peer()->lastVisited - client->peer()->connectStart;
    client->peer()->noConnectTries = 0;
    qDebug() << client->peer()->ipport.data() << " connected";


    // Send handshake and piece list
//    client->initialize(d->infoHash, d->pieceCount);
//    client->sendPieceList(d->completedPieces);

    emit peerInfoUpdated();

    if (d->state == Searching || d->state == Connecting) {
//        int completed = d->completedPieces.count(true);
//        if (completed == 0)
//            d->setState(WarmingUp);
//        else if (d->incompletePieces.count(true) < 5 && d->pendingPieces.size() > d->incompletePieces.count(true))
//            d->setState(Endgame);
    }
}

void NodeClient::removeClient()
{
    PeerWireClient *client = static_cast<PeerWireClient *>(sender());

    qDebug() << " remove client" << client->peerAddress() << client->peer()->ipport.data();

    // Remove the host from our list of known peers if the connection
    // failed.
    if (client->peer() && client->error() == QAbstractSocket::ConnectionRefusedError)
        d->knownpeers.erase(client->peer());

    // Remove the client from RateController and all structures.
//    RateController::instance()->removeSocket(client);
    d->connections.erase(client);

    /*
    QMultiMap<PeerWireClient *, TorrentPiece *>::Iterator it = d->payloads.find(client);
    while (it != d->payloads.end() && it.key() == client) {
        TorrentPiece *piece = it.value();
        piece->inProgress = false;
        piece->requestedBlocks.fill(false);
        it = d->payloads.erase(it);
    }

    // Remove pending read requests.
    QMapIterator<int, PeerWireClient *> it2(d->readIds);
    while (it2.findNext(client))
        d->readIds.remove(it2.key());
    */

    // Delete the client later.
    disconnect(client, SIGNAL(disconnected()), this, SLOT(removeClient()));
    client->deleteLater();
    ConnectionManager::instance()->removeConnection(client);

    emit peerInfoUpdated();
    d->callPeerConnector();
}

