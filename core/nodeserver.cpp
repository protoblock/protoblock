/*
//  nodeserver.cpp
//
//  Protoblock
//
//  Created by Jay Berg on 5/23/2018
//
*/
#include "connectionmanager.h"
#include "peerwireclient.h"
//#include "ratecontroller.h"
#include "nodeclient.h"
#include "nodeserver.h"

Q_GLOBAL_STATIC(NodeServer, nodeServer)

NodeServer *NodeServer::instance()
{
    return nodeServer();
}

void NodeServer::addNodeClient(NodeClient *client)
{
    nodeclient = client;
}


void NodeServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << " incommming ";
    PeerWireClient *client =
        new PeerWireClient(ConnectionManager::instance()->clientId(), this);

    if (client->setSocketDescriptor(socketDescriptor)) {
        if (ConnectionManager::instance()->canAddConnection() && nodeclient != NULL) {
            connect(client, SIGNAL(infoHashReceived(QByteArray)),
                    this, SLOT(processInfoHash(QByteArray)));
            connect(client, SIGNAL(error(QAbstractSocket::SocketError)),
                    this, SLOT(removeClient()));
//            RateController::instance()->addSocket(client);
            ConnectionManager::instance()->addConnection(client);
            return;
        }
    }
    client->abort();
    delete client;
}

void NodeServer::removeClient()
{
    PeerWireClient *peer = qobject_cast<PeerWireClient *>(sender());
//    RateController::instance()->removeSocket(peer);
    ConnectionManager::instance()->removeConnection(peer);
    peer->deleteLater();
}

//void NodeServer::processInfoHash(const QByteArray &infoHash)
//{
//    PeerWireClient *peer = qobject_cast<PeerWireClient *>(sender());
//    foreach (TorrentClient *client, clients) {
//        if (nodeclient->state() >= TorrentClient::Searching && client->infoHash() == infoHash) {
//            peer->disconnect(peer, 0, this, 0);
//            client->setupIncomingConnection(peer);
//            return;
//        }
//    }
//    removeClient();
//}
