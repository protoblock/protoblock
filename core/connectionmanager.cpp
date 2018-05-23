/*
//  connectionmanager.cpp
//
//  Protoblock
//
//  Created by Jay Berg on 5/21/2018
//
*/
#include "connectionmanager.h"

#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <openssl/rand.h>

static const int MaxConnections = 250;

Q_GLOBAL_STATIC(ConnectionManager, connectionManager)

ConnectionManager *ConnectionManager::instance()
{
    return connectionManager();
}

bool ConnectionManager::canAddConnection() const
{
    return (connections.size() < MaxConnections);
}

void ConnectionManager::addConnection(PeerWireClient *client)
{
    connections << client;
}

void ConnectionManager::removeConnection(PeerWireClient *client)
{
    connections.remove(client);
}

int ConnectionManager::maxConnections() const
{
    return MaxConnections;
}

QByteArray ConnectionManager::clientId() const
{
    if (id.isEmpty()) {
        // Generate peer id
        // Monday, April 14, 2014 12:00:00 AM
        int genesisepoch = 1397433600;
        int startupTime = int(QDateTime::currentDateTime().toTime_t());
        startupTime = startupTime - genesisepoch;
//        startupTime /= 60;

        id += QString::asprintf("-%c%02d-",TEST_OR_PROD,WIRE_VERSION);
//        id += QString::asprintf("-QT%03x-", (QT_VERSION % 0xffff00) >> 8).toLatin1();
        id += QByteArray::number(startupTime, 10);
        id += "-";

        int left = 19-id.size();
        if ( left > 0) {
            unsigned char buf[19];
            RAND_bytes(buf, left);
            id += QByteArray(QByteArray((const char *)buf,left).toBase64().data(),left);
        }
        id += "-";
    }
    return id;
}
