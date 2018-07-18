#include "qqmlwebsockets.h"
#include <QtWebSockets/QWebSocket>

QQmlWebSocket::QQmlWebSocket(QObject *parent) :
    QObject(parent),
    m_webSocket(),
    m_status(Closed),
    m_url(),
    m_isActive(false),
    m_componentCompleted(true),
    m_errorString()
{
}

QQmlWebSocket::QQmlWebSocket(QWebSocket *socket, QObject *parent) :
    QObject(parent),
    m_status(Closed),
    m_url(socket->requestUrl()),
    m_isActive(true),
    m_componentCompleted(true),
    m_errorString(socket->errorString())
{
    setSocket(socket);
    onStateChanged(socket->state());
}

QQmlWebSocket::~QQmlWebSocket()
{
}

qint64 QQmlWebSocket::sendTextMessage(const QString &message)
{
    if (m_status != Open) {
        setErrorString(tr("Messages can only be sent when the socket is open."));
        setStatus(Error);
        return 0;
    }
    return m_webSocket->sendTextMessage(message);
}

QUrl QQmlWebSocket::url() const
{
    return m_url;
}

void QQmlWebSocket::setUrl(const QUrl &url)
{
    if (m_url == url) {
        return;
    }
    if (m_webSocket && (m_status == Open)) {
        m_webSocket->close();
    }
    m_url = url;
    Q_EMIT urlChanged();
    if (m_webSocket) {
        m_webSocket->open(m_url);
    }
}

QQmlWebSocket::Status QQmlWebSocket::status() const
{
    return m_status;
}

QString QQmlWebSocket::errorString() const
{
    return m_errorString;
}

void QQmlWebSocket::classBegin()
{
    m_componentCompleted = false;
    m_errorString = tr("QQmlWebSocket is not ready.");
    m_status = Closed;
}

void QQmlWebSocket::componentComplete()
{
    setSocket(new QWebSocket);

    m_componentCompleted = true;

    open();
}

void QQmlWebSocket::setSocket(QWebSocket *socket)
{
    m_webSocket.reset(socket);
    if (m_webSocket) {
        // explicit ownership via QScopedPointer
        m_webSocket->setParent(Q_NULLPTR);
        connect(m_webSocket.data(), &QWebSocket::textMessageReceived,
                this, &QQmlWebSocket::textMessageReceived);
        connect(m_webSocket.data(), &QWebSocket::binaryMessageReceived,
            this, &QQmlWebSocket::binaryMessageReceived);
        typedef void (QWebSocket::* ErrorSignal)(QAbstractSocket::SocketError);
        connect(m_webSocket.data(), static_cast<ErrorSignal>(&QWebSocket::error),
                this, &QQmlWebSocket::onError);
        connect(m_webSocket.data(), &QWebSocket::stateChanged,
                this, &QQmlWebSocket::onStateChanged);
    }
}

void QQmlWebSocket::onError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    setErrorString(m_webSocket->errorString());
    setStatus(Error);
}

void QQmlWebSocket::onStateChanged(QAbstractSocket::SocketState state)
{
    switch (state)
    {
        case QAbstractSocket::ConnectingState:
        case QAbstractSocket::BoundState:
        case QAbstractSocket::HostLookupState:
        {
            setStatus(Connecting);
            break;
        }
        case QAbstractSocket::UnconnectedState:
        {
            setStatus(Closed);
            break;
        }
        case QAbstractSocket::ConnectedState:
        {
            setStatus(Open);
            break;
        }
        case QAbstractSocket::ClosingState:
        {
            setStatus(Closing);
            break;
        }
        default:
        {
            setStatus(Connecting);
            break;
        }
    }
}

void QQmlWebSocket::setStatus(QQmlWebSocket::Status status)
{
    if (m_status == status) {
        return;
    }
    m_status = status;
    if (status != Error) {
        setErrorString();
    }
    Q_EMIT statusChanged(m_status);
}

void QQmlWebSocket::setActive(bool active)
{
    if (m_isActive == active) {
        return;
    }
    m_isActive = active;
    Q_EMIT activeChanged(m_isActive);
    if (!m_componentCompleted) {
        return;
    }
    if (m_isActive) {
        open();
    } else {
        close();
    }
}

bool QQmlWebSocket::isActive() const
{
    return m_isActive;
}

void QQmlWebSocket::open()
{
    if (m_componentCompleted && m_isActive && m_url.isValid() && Q_LIKELY(m_webSocket)) {
        m_webSocket->open(m_url);
    }
}

void QQmlWebSocket::close()
{
    if (m_componentCompleted && Q_LIKELY(m_webSocket)) {
        m_webSocket->close();
    }
}

void QQmlWebSocket::setErrorString(QString errorString)
{
    if (m_errorString == errorString) {
        return;
    }
    m_errorString = errorString;
    Q_EMIT errorStringChanged(m_errorString);
}

