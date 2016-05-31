#include "socketclient.h"
#include <QtCore/QDebug>
SocketClient::SocketClient(const QUrl &url, bool debug, QObject *parent) :
    QObject(parent),
    m_url(url),
    m_debug(debug)
{
    if (m_debug)
        qDebug() << "WebSocket server:" << url;
    connect(&m_webSocket, &QWebSocket::connected, this, &SocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &SocketClient::closed);
    m_webSocket.open(QUrl(url));
}

qint64 SocketClient::sendBinaryMessage(const QByteArray &data)
{
    return m_webSocket.sendBinaryMessage(data);
}

qint64 SocketClient::sendTextMessage(const QString &message)
{
    return  m_webSocket.sendTextMessage (message);
}

QHostAddress SocketClient::peerAddress() const
{
    return m_webSocket.peerAddress ();
}

QAbstractSocket::SocketState SocketClient::state() const
{
    return m_webSocket.state ();
}

QWebSocketProtocol::Version SocketClient::version() const
{
    return m_webSocket.version ();
}
void SocketClient::onConnected()
{
    if (m_debug)
        qDebug() << "WebSocket connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &SocketClient::onTextMessageReceived);
    m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}


void SocketClient::onTextMessageReceived(QString message)
{
    if (m_debug)
        qDebug() << "Message received:" << message;
    m_webSocket.close();
}


void SocketClient::onBinaryMessageRecived( QByteArray bArray)
{
    if (m_debug)
        qDebug() << "Message received:" << bArray;
    m_webSocket.close();
}
