/*
//  nodeclient.h
//
//  Protoblock
//
//  Created by Jay Berg on 5/21/2018
//
*/
#ifndef NODECLIENT_H
#define NODECLIENT_H
#include <QNetworkAccessManager>
#include <QObject>
#include <QNetworkReply>
#include <QHostAddress>

class NodeClientPrivate;
class PeerWireClient;

/*
 *  Upstream - PeerA connects to PeerB listening on port
 *             PeerB is upstream from PeerA
 *             PeerA is downstream from PeerB
 *
 *  Downstream - PeerB listening on port - PeerA connects
 *             PeerA is downstream to PeerB
 *             PeerB is upstream to PeerA
 * */
enum NodePeerDirection {
    Upstream = 0x1,
    Downstream = 0x2
};

//NodePeer has an id, and will use this is to find same peer listening and connecting
// will only add new peers when confirmed listening
// so try to connect to peer, even though he is already conencted to you
// when connected twice, the original connection should stay alive
// and mark this peer as both upstream and downstream
//
class NodePeer {
public:
    QHostAddress address;
    quint16 port;
    QString id;
    QString context;
//    bool interesting;
//    bool seed;
    uint lastVisited;
    uint connectStart;
    uint connectTime;
    uint noConnectTries;
    NodePeerDirection upordown;
//    QBitArray pieces;
//    int numCompletedPieces;
    //ToDo: blockheight - block measure

    std::string ipport;

    inline bool operator==(const NodePeer &other)
    {
        return port == other.port
            && address == other.address
            && id == other.id;
    }
};

class NodeClient : public QObject
{
    Q_OBJECT
public:
    enum State {
        Starting,
        Searching,
        Connecting,
        Syncing,
        EndingSync,
        Running,
        Reorging,
        Stopping
    };

    enum Error {
        UnknownError,
        TorrentParseError,
        InvalidTrackerError,
        FileError,
        ServerError
    };

//    enum PeerWireStateFlag {
//        ChokingPeer = 0x1,
//        InterestedInPeer = 0x2,
//        ChokedByPeer = 0x4,
//        PeerIsInterested = 0x8
//    };

//    enum Error {
//        UnknownError,
//        TorrentParseError,
//        InvalidTrackerError,
//        FileError,
//        ServerError
//    };

    explicit NodeClient(QObject *parent = 0);
    ~NodeClient();

//    std::string getMyIp();

//    ~NodeClient();
    void getMyIp();
signals:
    void tryGetIp();
    void gotPublicIp(bool);
    void stateChanged(NodeClient::State state);
    void peerInfoUpdated();
    void error(NodeClient::Error error);


public slots:
    void startPoint();

private slots:
    // File management
//    void sendToPeer(int readId, int pieceIndex, int begin, const QByteArray &data);
//    void fullVerificationDone();
//    void pieceVerified(int pieceIndex, bool ok);
//    void handleFileError();

    // Connection handling
    void connectToPeers();
//    QList<TorrentPeer *> weighedFreePeers() const;
    void setupOutgoingConnection();
    void initializeConnection(PeerWireClient *client);
    void removeClient();
//    void peerPiecesAvailable(const QBitArray &pieces);
//    void peerRequestsBlock(int pieceIndex, int begin, int length);
//    void blockReceived(int pieceIndex, int begin, const QByteArray &data);
//    void peerWireBytesWritten(qint64 bytes);
//    void peerWireBytesReceived(qint64 bytes);
//    int blocksLeftForPiece(const TorrentPiece *piece) const;

    // Scheduling
//    void scheduleUploads();
//    void scheduleDownloads();
//    void schedulePieceForClient(PeerWireClient *client);
//    void requestMore(PeerWireClient *client);
//    int requestBlocks(PeerWireClient *client, TorrentPiece *piece, int maxBlocks);
//    void peerChoked();
//    void peerUnchoked();

    // Tracker handling
//    void addToPeerList(const QList<TorrentPeer> &peerList);
//    void trackerStopped();

    // Progress
//    void updateProgress(int progress = -1);


private:
    NodeClientPrivate *d;
    friend class NodeClientPrivate;

    QNetworkAccessManager http;
    QNetworkReply *qreply;

    std::vector<std::string> ipUrls;
    QString myIp;
    bool gotMyIp = false;
    int ipUrlsTry = 0;
    std::vector<NodePeer *> FreePeers() const;
    void startServer();
};

#endif // NODECLIENT_H
