#ifndef QQMLWEBSOCKET_H
#define QQMLWEBSOCKET_H

#include <QObject>
#include <QQmlParserStatus>
#include <QtQml>
#include <QScopedPointer>
#include <QtWebSockets/QWebSocket>


class QQmlWebSocket : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_DISABLE_COPY(QQmlWebSocket)
    Q_INTERFACES(QQmlParserStatus)

    Q_ENUMS(Status)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)

public:
    explicit QQmlWebSocket(QObject *parent = 0);
    explicit QQmlWebSocket(QWebSocket *socket, QObject *parent = 0);
    virtual ~QQmlWebSocket();

    enum Status
    {
        Connecting  = 0,
        Open        = 1,
        Closing     = 2,
        Closed      = 3,
        Error       = 4
    };

    QUrl url() const;
    void setUrl(const QUrl &url);
    Status status() const;
    QString errorString() const;

    void setActive(bool active);
    bool isActive() const;

    Q_INVOKABLE qint64 sendTextMessage(const QString &message);


Q_SIGNALS:
    void textMessageReceived(QString message);
    void binaryMessageReceived(QByteArray message);
    void statusChanged(Status status);
    void activeChanged(bool isActive);
    void errorStringChanged(QString errorString);
    void urlChanged();

public:
    void classBegin() Q_DECL_OVERRIDE;
    void componentComplete() Q_DECL_OVERRIDE;

private Q_SLOTS:
    void onError(QAbstractSocket::SocketError error);
    void onStateChanged(QAbstractSocket::SocketState state);

private:
    QScopedPointer<QWebSocket> m_webSocket;
    Status m_status;
    QUrl m_url;
    bool m_isActive;
    bool m_componentCompleted;
    QString m_errorString;

    // takes ownership of the socket
    void setSocket(QWebSocket *socket);
    void setStatus(Status status);
    void open();
    void close();
    void setErrorString(QString errorString = QString());
};


#endif // QQMLWEBSOCKET_H
