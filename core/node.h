/*
//  nodeclient.h
//
//  Protoblock
//
//  Created by Jay Berg on 5/21/2018
//
*/
#ifndef NODE_H
#define NODE_H
#include <QNetworkAccessManager>
#include <QObject>
#include <QNetworkReply>
#include <QHostAddress>
#include <QTcpServer>

#include <queue>

#include "P2PData.pb.h"
#include "peerstore.h"
#include <unordered_set>
#include "peerwire.h"

static int PORT_DEFAULT = 10130;
static bool IS_TEST_NET = true;
static const int WIRE_VERSION = 1;
static const int MAX_OUTBOUND = 8;

static int GENEIS_EPOCH = 1397433600;

using fantasybit::Peer;

namespace pb {

class Node : public QTcpServer
{
    Q_OBJECT

public:
    explicit Node(QObject *parent = 0);
    static Node *instance();
    ~Node();

    void getMyIp();
    fantasybit::SessionId mSessionId;
    fantasybit::Peer mPeer;
    std::string PeerIpPort(const fantasybit::Peer &p) {
        return p.address() + FB_PORT(p.port());
    }

    void init();
signals:
    void tryGetIp();
    void NewPeer(const std::string &);
    void ListeningStateChange();

public slots:
    void startPoint();
    void connectToPeers();
    void OnNewWireMsg(const fantasybit::WireMsg &msg);
    void WireConnected();
    void WireDisconnected();
    void WireTimeout() {}

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *event) override;

private:
    void AddKnownPeer(const std::string &pap, fantasybit::Peer *p) {
        knownpeers.insert({pap,p});
        emit NewPeer(pap);
    }

    //get self ip
    bool gotMyIp = false;
    QNetworkAccessManager http;
    QNetworkReply *qreply;
    std::string mMyAddress;
    int ipUrlsTry = 0;
    static std::vector<std::string> IP_URLS;
    void gotPublicIp(bool);
    void startServer();
    //
    fantasybit::PeerChainStatus mPeerChainStatus;
    bool connectingToClients = false;
    void callPeerConnector();

    //timer
    int m_pendingNatTimer = 0;

    pb::PeerStore pstore;
    std::string m_selfkey;
    std::unordered_map<std::string,Peer *> knownpeers;
    std::unordered_map<std::string,Peer *> m_connectedUUID;
    std::unordered_map<Peer *,PeerWire *> m_connections;

    std::unordered_set<Peer *> testingnat;
    std::queue<PeerWire *> m_pending_nat_test;

    int m_numoutgoing = 0;
    int m_numincomming = 0;

    fantasybit::PeerChainStatus::ChainState getChainState() const;
    void setChainState(const fantasybit::PeerChainStatus::ChainState state);
    PeerWire * newPeerWire(Peer *p,bool isOutgoing);
    };

}

Q_DECLARE_METATYPE(fantasybit::Peer)

#endif // NODE_H
