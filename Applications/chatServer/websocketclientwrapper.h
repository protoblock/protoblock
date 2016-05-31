
#ifndef WEBSOCKETTRANSPORTSERVER_H
#define WEBSOCKETTRANSPORTSERVER_H

#include <QObject>

QT_BEGIN_NAMESPACE

class QWebSocketServer;
class WebSocketTransport;

class WebSocketClientWrapper : public QObject
{
    Q_OBJECT

public:
    WebSocketClientWrapper(QWebSocketServer *server, QObject *parent = 0);

Q_SIGNALS:
    void clientConnected(WebSocketTransport* client);

private Q_SLOTS:
    void handleNewConnection();

private:
    QWebSocketServer *m_server;
};

QT_END_NAMESPACE

#endif // WEBSOCKETTRANSPORTSERVER_H
