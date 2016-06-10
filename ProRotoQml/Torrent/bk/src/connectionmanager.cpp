#include "connectionmanager.h"

#include <QByteArray>
#include <QDateTime>

static const int MaxConnections = 250;

Q_GLOBAL_STATIC(ConnectionManager, connectionManager)

ConnectionManager *ConnectionManager::instance()
{
    return connectionManager();
}

bool ConnectionManager::canAddConnection() const
{
    return (m_connections.size() < MaxConnections);
}

void ConnectionManager::addConnection(PeerWireClient *client)
{
    m_connections << client;
}

void ConnectionManager::removeConnection(PeerWireClient *client)
{
    m_connections.remove(client);
}

int ConnectionManager::maxConnections() const
{
    return MaxConnections;
}

QByteArray ConnectionManager::clientId() const
{
    if (m_id.isEmpty()) {
        // Generate peer id
        int startupTime = int(QDateTime::currentDateTime().toTime_t());

        QString s;
        s.sprintf("-QT%04x-", (QT_VERSION % 0xffff00) >> 8);
        m_id += s.toLatin1();
        m_id += QByteArray::number(startupTime, 10);
        m_id += QByteArray(20 - m_id.size(), '-');
    }
    return m_id;
}
