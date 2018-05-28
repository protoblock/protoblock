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

#include <vector>

#include "P2PData.pb.h"
#include "peerstore.h"
#include <unordered_set>
#include "peerwire.h"

static int PORT_DEFAULT = 10130;
static bool IS_TEST_NET = true;
static const int WIRE_VERSION = 1;
static const int MAX_OUTBOUND = 8;

static int GENEIS_EPOCH = 1397433600;


namespace pb {

class Node : public QTcpServer
{
    Q_OBJECT

public:
    explicit Node(QObject *parent = 0);
    static Node *instance();
    ~Node();

    void getMyIp();
    static fantasybit::SessionId mSessionId;
    static fantasybit::Peer mPeer;

signals:
    void tryGetIp();
    void newPeer(fantasybit::Peer &);

public slots:
    void startPoint();
    void connectToPeers();
    void OnNewWireMsg(const fantasybit::WireMsg &msg);

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

private:
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

    std::vector<std::string> m_pending_nat_test;

    int m_numoutgoing = 0;

    fantasybit::PeerChainStatus::ChainState getChainState() const;
    void setChainState(const fantasybit::PeerChainStatus::ChainState state);
};

}

Q_DECLARE_METATYPE(fantasybit::Peer)

#endif // NODE_H
