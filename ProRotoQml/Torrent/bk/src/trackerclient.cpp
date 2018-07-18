#include "bencodeparser.h"
#include "connectionmanager.h"
#include "torrentclient.h"
#include "torrentserver.h"
#include "trackerclient.h"

#include <QtCore>
#include <QNetworkRequest>

TrackerClient::TrackerClient(TorrentClient *downloader, QObject *parent)
    : QObject(parent), m_torrentDownloader(downloader)
{
    m_uploadedBytes = 0;
    m_length = 0;
    m_requestIntervalSec = 10 * 60;
    m_requestIntervalTimer = -1;
    m_firstTrackerRequest = true;
    m_lastTrackerRequest = false;
    m_firstSeeding = true;

    m_lastAction = ActionError;
    m_connectionId = 0;
    m_transactionId = 0;
    m_requestTryNum = 0;
    m_maxTryNum = 3;
    m_isLastRequestSuccessful = false;
    m_fetchTimeout = 8 * 1000;
    m_fetchTimer.setInterval( m_fetchTimeout );
    m_fetchTimer.setSingleShot( true );
    connect( &m_fetchTimer, &QTimer::timeout, this, &TrackerClient::fetchTimeoutHandler );
    connect( this, &TrackerClient::peerListUpdated, &m_fetchTimer, &QTimer::stop );

    connect( &m_udpSocket, &QAbstractSocket::stateChanged, this, []( QAbstractSocket::SocketState state ) {
        qDebug() << state;
    });
    connect( &m_udpSocket, &QIODevice::readyRead,           this, &TrackerClient::responseHandler );
    connect( &m_udpSocket, &QAbstractSocket::connected,     this, &TrackerClient::connectRequest );
}

TrackerClient::~TrackerClient()
{
    m_fetchTimer.stop();
    m_udpSocket.abort();
}

void TrackerClient::start(const MetaInfo &info)
{
    m_metaInfo = info;
    m_currentAnnounceIt = m_metaInfo.announceList().begin();
    qDebug() << *m_currentAnnounceIt;
    QTimer::singleShot(0, this, &TrackerClient::fetchPeerList);

    if (m_metaInfo.fileForm() == MetaInfo::SingleFileForm) {
        m_length = m_metaInfo.singleFile().Length;
    } else {
        QList<MetaInfoMultiFile> files = m_metaInfo.multiFiles();
        for (int i = 0; i < files.size(); ++i)
            m_length += files.at(i).Length;
    }
}

void TrackerClient::startSeeding()
{
    m_firstSeeding = true;
    fetchPeerList();
}

void TrackerClient::stop()
{
    m_fetchTimer.stop();
    m_lastTrackerRequest = true;
    fetchPeerList();
}

void TrackerClient::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_requestIntervalTimer) {
        fetchPeerList();
    } else {
        QObject::timerEvent(event);
    }
}

//// Requests

void TrackerClient::connectRequest()
{
    m_fetchTimer.start();
    m_lastAction = ActionConnect;
    m_transactionId = QDateTime::currentDateTime().toTime_t();
    qDebug() << "CONNECT REQUEST :" << qint64(0x41727101980) << qint32( m_lastAction ) << qint32( m_transactionId );

    QByteArray messege;
    QDataStream( &messege, QIODevice::WriteOnly ) << qint64( 0x41727101980 ) << qint32( m_lastAction )
                                                  << qint32( m_transactionId );
    m_udpSocket.write( messege );
}

void TrackerClient::announceRequest()
{
    m_fetchTimer.start();
    m_lastAction = ActionAnnounce;
    m_transactionId = QDateTime::currentDateTime().toTime_t();

    QByteArray infoHash = m_torrentDownloader->infoHash();
    if ( infoHash.size() != 20 ){
        qWarning() << Q_FUNC_INFO << "INVALID INFOHASH";
    }

    bool seeding = (m_torrentDownloader->state() == TorrentClient::Seeding);

    qint32  event = 2;  // Started
    qint32  key = 0;
    auto    peerId = ConnectionManager::instance()->clientId();
    qint64  downloaded = 0;
    qint64  left = 0;
    qint64  uploaded = m_torrentDownloader->uploadedBytes();
    quint16 port = TorrentServer::instance()->serverPort();

    if ( m_firstSeeding ) {
        downloaded = m_torrentDownloader->downloadedBytes();
        left = seeding ? 0 : qMax<int>(0, m_metaInfo.totalSize() - m_torrentDownloader->downloadedBytes());
    }

    if ( seeding && m_firstSeeding ){
        event = 1;  // Complited;
        m_firstSeeding = false;
    } else if ( m_firstTrackerRequest ) {
        m_firstTrackerRequest = false;
        event = 2;  // Started
    } else if ( m_lastTrackerRequest ){
        event = 1;  // Stopped
    }

    QByteArray messege;
    QDataStream out( &messege, QIODevice::WriteOnly );
    out << qint64( m_connectionId ) << qint32( m_lastAction ) << qint32( m_transactionId );

    QByteArray messegeEnd;
    QDataStream out2( &messegeEnd, QIODevice::WriteOnly );
    out2<< qint64( downloaded ) << qint64( left )
        << qint64( uploaded ) << qint32( event ) << qint32( 0 )
        << qint32( key ) << qint32( -1 ) << qint16( port );

    messege.append( infoHash ).append( peerId ).append( messegeEnd );
    m_udpSocket.write( messege );
}

void TrackerClient::scrapeRequest()
{
    m_fetchTimer.start();
    m_lastAction = ActionScrape;

    //QByteArray messege;
    //QDataStream out( &messege, QIODevice::WriteOnly );

    //m_udpSocket.write( messege );
}

void TrackerClient::sendLastActionRequest()
{
    switch ( m_lastAction ) {
    case ActionConnectToHost :
        m_udpSocket.abort();
        fetchPeerList();
        break;

    case ActionConnect :
        connectRequest();
        break;

    case ActionAnnounce :
        announceRequest();
        break;

    case ActionScrape :
        scrapeRequest();
        break;

    case ActionError :
        qCritical() << Q_FUNC_INFO << "ERROR ACTION";
        break;

    default:
        qCritical() << Q_FUNC_INFO << "UNKNOWN ACTION";
        break;
    }
}

//// Handlers

void TrackerClient::connectResponseHandler()
{
    if ( m_udpSocket.bytesAvailable() < 16 ){
        qWarning() << Q_FUNC_INFO << "INCORRECT RESPONSE SIZE";
    }
    qint32 action = -1;
    qint32 transactionId = -1;
    qint64 connectionId = -1;

    auto buff = m_udpSocket.readAll();
    QDataStream( &buff, QIODevice::ReadOnly ) >> action >> transactionId >> connectionId;
    qDebug() << "CONNECT RESPONSE : " << action << transactionId << connectionId;

    if ( action != (qint32)m_lastAction ){
        qCritical() << Q_FUNC_INFO << "WRONG ACTION";
        return;
    }

    if ( m_transactionId != transactionId ){
        qCritical() << Q_FUNC_INFO << "TRANSACTION ID IS NOT EQUAL";
        return;
    }

    m_lastAction = ActionAnnounce;              // Не обязательно
    m_connectionId = connectionId;
    announceRequest();
}

void TrackerClient::announceResponseHandler()
{
    if ( m_lastTrackerRequest ) {
        m_udpSocket.readAll();
        emit stopped();
        return;
    }

    if ( m_udpSocket.bytesAvailable() < 20 ){
        emit warning( tr("Incorrect response size from %1").arg( *m_currentAnnounceIt ) );
        qWarning() << Q_FUNC_INFO << "INCORRECT RESPONSE SIZE";
    }
    qint32 action = -1;
    qint32 transactionId = -1;
    qint32 interval = -1;
    qint32 leechers = -1;
    qint32 seeders = -1;
    quint32 ipAddress = -1;
    quint16 tcpPort = -1;

    auto buff = m_udpSocket.readAll();
    QDataStream in( &buff, QIODevice::ReadOnly );
    in >> action >> transactionId >> interval >> leechers >> seeders;
    //qDebug() << "ANNOUNCE RESPONSE : " << action << transactionId << interval << leechers << seeders;

    if ( action != (qint32)m_lastAction ){
        emit warning( tr("Wrong action from %1").arg( *m_currentAnnounceIt ) );
        qCritical() << Q_FUNC_INFO << "WRONG ACTION";
        fetchTimeoutHandler();
    }

    if ( m_transactionId != transactionId ){
        emit warning( tr("Transaction ID is not equal from %1").arg( *m_currentAnnounceIt ) );
        qCritical() << Q_FUNC_INFO << "TRANSACTION ID IS NOT EQUAL";
        fetchTimeoutHandler();
    }

    if ( interval > 0 && (m_requestIntervalSec != interval || m_requestIntervalTimer == -1) ){
        m_requestIntervalSec = interval;
        if ( m_requestIntervalTimer != -1 )
            killTimer( m_requestIntervalTimer );

        m_requestIntervalTimer = startTimer( m_requestIntervalSec * 1000 );
    }

    while ( !in.atEnd() ){
        in >> ipAddress >> tcpPort;
        TorrentPeer peer = {};
        peer.address.setAddress( ipAddress );
        peer.port = tcpPort;
        m_peerList << peer;
        //qDebug() << QHostAddress(ipAddress).toString() << tcpPort << ++i;
    }
    emit peerListUpdated(m_peerList);
}

void TrackerClient::scrapeResponseHandler()
{
    qDebug() << "SCRAPE RESPONSE HANDLER";
}

void TrackerClient::responseHandler()
{
    m_requestTryNum = 0;
    switch ( m_lastAction ) {
    case ActionConnect :
        connectResponseHandler();
        break;

    case ActionAnnounce :
        announceResponseHandler();
        break;

    case ActionScrape :
        scrapeResponseHandler();
        break;

    case ActionError :
        qCritical() << Q_FUNC_INFO << "ERROR ACTION";
        break;

    default:
        qCritical() << Q_FUNC_INFO << "UNKNOWN ACTION";
        qDebug() << Q_FUNC_INFO << m_udpSocket.readAll();
        break;
    }
}

void TrackerClient::fetchPeerList()
{
    m_peerList.clear();
    if ( m_udpSocket.state() == QAbstractSocket::ConnectedState ){
        connectRequest();
        return;
    }

    QUrl url( *m_currentAnnounceIt );
    m_lastAction = ActionConnectToHost;
    m_fetchTimer.start();
    m_udpSocket.connectToHost( url.host(), url.port(), QIODevice::ReadWrite, QUdpSocket::IPv4Protocol );
}

void TrackerClient::fetchTimeoutHandler()
{
    if ( ++m_requestTryNum >= m_maxTryNum ){
        if ( ++m_currentAnnounceIt == m_metaInfo.announceList().end() ){
            emit failure( QStringLiteral("All trackers are not responding") );
            m_currentAnnounceIt = m_metaInfo.announceList().begin();
        }
        fetchPeerList();
        return;
    }
    sendLastActionRequest();
}
