/*
//  connectionmanager.h
//
//  Protoblock
//
//  Created by Jay Berg on 5/21/2018
//
*/
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
    static const int WIRE_VERSION = 1;
    static const char TEST_OR_PROD = 'T';
};

#endif
