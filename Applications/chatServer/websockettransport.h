#ifndef WEBSOCKETTRANSPORT_H
#define WEBSOCKETTRANSPORT_H

#include <QtWebChannel/QWebChannelAbstractTransport>

QT_BEGIN_NAMESPACE

class QWebSocket;
class WebSocketTransport : public QWebChannelAbstractTransport
{
    Q_OBJECT
public:
    explicit WebSocketTransport(QWebSocket *socket);
    virtual ~WebSocketTransport();

    void sendMessage(const QJsonObject &message) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void textMessageReceived(const QString &message);

private:
    QWebSocket *m_socket;
};

QT_END_NAMESPACE

#endif // WEBSOCKETTRANSPORT_H
