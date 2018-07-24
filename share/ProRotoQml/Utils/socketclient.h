#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QString>
#include <QByteArray>

class SocketClient : public QObject
{
    Q_OBJECT
public:
    explicit SocketClient(const QUrl &url, bool debug = false, QObject *parent = Q_NULLPTR);

    qint64 sendBinaryMessage(const QByteArray &data);
    qint64 sendTextMessage(const QString &message);

    QHostAddress  peerAddress() const;
    QAbstractSocket::SocketState  state()const;
    QWebSocketProtocol::Version  version()const;


signals:
    void closed();

protected slots :
    void onConnected();
    void onTextMessageReceived( QString message);
    void onBinaryMessageRecived(QByteArray bArray);

private:
    QWebSocket m_webSocket;
    QUrl m_url;
    bool m_debug;
};

#endif // SOCKETCLIENT_H
