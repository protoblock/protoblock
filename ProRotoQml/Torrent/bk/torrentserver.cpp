#include "connectionmanager.h"
#include "peerwireclient.h"
#include "ratecontroller.h"
#include "torrentclient.h"
#include "torrentserver.h"

Q_GLOBAL_STATIC(TorrentServer, torrentServer)

TorrentServer *TorrentServer::instance()
{
    return torrentServer();
}

void TorrentServer::addClient(TorrentClient *client)
{
    clients << client;
}

void TorrentServer::removeClient(TorrentClient *client)
{
    clients.removeAll(client);
}

void TorrentServer::incomingConnection(qintptr socketDescriptor)
{
    PeerWireClient *client =
        new PeerWireClient(ConnectionManager::instance()->clientId(), this);

    if (client->setSocketDescriptor(socketDescriptor)) {
        if (ConnectionManager::instance()->canAddConnection() && !clients.isEmpty()) {
            connect(client, SIGNAL(infoHashReceived(QByteArray)),
                    this, SLOT(processInfoHash(QByteArray)));
            connect(client, SIGNAL(error(QAbstractSocket::SocketError)),
                    this, SLOT(removeClient()));
            RateController::instance()->addSocket(client);
            ConnectionManager::instance()->addConnection(client);
            return;
        }
    }
    client->abort();
    delete client;
}

void TorrentServer::removeClient()
{
    PeerWireClient *peer = qobject_cast<PeerWireClient *>(sender());
    RateController::instance()->removeSocket(peer);
    ConnectionManager::instance()->removeConnection(peer);
    peer->deleteLater();
}

void TorrentServer::processInfoHash(const QByteArray &infoHash)
{
    PeerWireClient *peer = qobject_cast<PeerWireClient *>(sender());
    foreach (TorrentClient *client, clients) {
        if (client->state() >= TorrentClient::Searching && client->infoHash() == infoHash) {
            peer->disconnect(peer, 0, this, 0);
            client->setupIncomingConnection(peer);
            return;
        }
    }
    removeClient();
}
