/*
//  nodeclient.cpp.h
//
//  Protoblock
//
//  Created by Jay Berg on 5/21/2018
//
*/
#include "nodeclient.h"
#include <QNetworkReply>

class PeerWireClient;
class NodePeer;
class NodeClientPrivate
{
public:
    NodeClientPrivate(NodeClient *qq) : q(qq) {}


    // Connections
    QList<PeerWireClient *> connections;
    QList<NodePeer *> peers;


    NodeClient *q;

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
    bool connectingToClients;
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
    int downloadRate[RateControlWindowLength];
    int uploadRate[RateControlWindowLength];
    int transferRateTimer;
    */
};


NodeClient::NodeClient(QObject *parent) : QObject(parent), http(this)
{
//    connect(&http, SIGNAL(finished(QNetworkReply*)), this, SLOT(httpRequestDone(QNetworkReply*)));

   ipUrls = {
        "http://aspi.ipify.org/",
        "http://myexsternalip.com/raw",
        "http://icanhazip.com/",
        "http://myip.dnsomatic.com/",
        "http://ifcaonfig.me/ip"
   };
}

void NodeClient::startPoint()
{
//    d = new NodeClientPrivate(this);

    connect(this, &NodeClient::tryGetIp, this, &NodeClient::getMyIp, Qt::QueuedConnection);
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
    if (ipUrlstry >= ipUrls.size()) {
        return;
    }

    auto inu = ipUrls.at(ipUrlstry);

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
                    if ( gotMyIp )
                        return;

                    if ( answer != "") {
                        myIp = answer;
                        qDebug() << " got ip" << myIp;
                        gotMyIp = true;
//                        emit gotPublicIp();
                    }
                    else
                        emit tryGetIp();
                }
            }
    );

    QString url = inu.data();//"http://api.ipify.org/";

    QNetworkRequest req;
    req.setUrl(QUrl(url));
    http.get(req);
    ipUrlstry++;

    return;
}


//NodeClient::~NodeClient()
//{
////    qDeleteAll(d->peers);
////    qDeleteAll(d->pendingPieces);
////    delete d;
//}
