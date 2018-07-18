#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

class PeerWireClient;

#include <QByteArray>
#include <QSet>

class ConnectionManager
{
public:
    static ConnectionManager *instance();

    bool canAddConnection() const;
    void addConnection(PeerWireClient *connection);
    void removeConnection(PeerWireClient *connection);
    int maxConnections() const;
    QByteArray clientId() const;

 private:
    QSet<PeerWireClient *> connections;
    mutable QByteArray id;
};

#endif
