#include "requesttoservermanager.h"
#include "torrentclient.h"
#include <QUrlQuery>
#include <QByteArray>
#include <QUrl>
#include <QDebug>
#include <QHostInfo>
#include <QTimer>

static const uint WAITING_TIME = 500;   // 0.5 sec

RequestToServerManager::RequestToServerManager(Torrent *torrent, QObject *parent)
    : QObject(parent)
{
    if ( torrent == nullptr ){
        qCritical() << Q_FUNC_INFO << "torFileInfo is null";
        return;
    }

    m_torrent = torrent;
    m_torrent->setParent( this );

    m_connectionId = 0;
    m_transactionId = 0;
    m_wantPeersCount = -1;
    m_requestsAmount = 0;
    m_wasFetch = false;
    m_lastAction = ActionError;

    connect( &m_udpSocket, &QUdpSocket::stateChanged, [this](QAbstractSocket::SocketState state){
        qDebug() << "SOCKET STATE IS CHANGED : " << state;
    } );
    connect( &m_udpSocket, &QUdpSocket::disconnected, [](){
        qDebug() << "SOCKET IS DISCONNECTED!";
    } );
}

const QSet< PeerInfo >& RequestToServerManager::GetPeers(qint32 peersCount /*= -1*/)
{
    if ( peersCount < -1 ){
        qWarning() << Q_FUNC_INFO << "peersCount < -1";
        peersCount = -1;
    }

    m_wantPeersCount = peersCount;
    auto torFleInfo = &m_torrent->GetTorrentFileInfo();
    connect( &m_udpSocket, &QIODevice::readyRead, this, &RequestToServerManager::responseHandler );

    for ( auto url : torFleInfo->GetTrackerUrlList() ){
        m_fetchedPeers.clear();
        m_wasFetch = false;
        bool wasFail = false;
        m_udpSocket.connectToHost( url.host(), url.port(), QIODevice::ReadWrite, QUdpSocket::IPv4Protocol );

        if ( !m_udpSocket.waitForConnected() ){
            m_udpSocket.abort();
            continue;
        }

        int requestsAmount = 0;
        do{
            ++requestsAmount;
            connectRequest();
            if ( requestsAmount > 4 ){
                m_udpSocket.abort();
                wasFail = true;
                break;
            }
        }while( !m_udpSocket.waitForReadyRead(WAITING_TIME) );
        if ( wasFail ){
            continue;
        }

        requestsAmount = 0;
        do{
            ++requestsAmount;
            announceRequest();
            if ( requestsAmount > 4 ){
                m_udpSocket.abort();
                wasFail = true;
                break;
            }
        }while( !m_udpSocket.waitForReadyRead(WAITING_TIME) );
        if ( wasFail ){
            continue;
        }

        if ( m_wasFetch ){
            break;
        }
    }

    return m_fetchedPeers;
}

//// Requests

void RequestToServerManager::connectRequest()
{
    m_lastAction = ActionConnect;
    m_transactionId = QDateTime::currentDateTime().toTime_t();
    qDebug() << "CONNECT REQUEST :" << qint64(0x41727101980) << qint32( m_lastAction ) << qint32( m_transactionId );

    QByteArray messege;
    QDataStream( &messege, QIODevice::WriteOnly ) << qint64( 0x41727101980 ) << qint32( m_lastAction )
                                                  << qint32( m_transactionId );
    qDebug() << "WRITTEN BYTES : " << m_udpSocket.write( messege );
}

void RequestToServerManager::announceRequest()
{
    auto torFileInfo = &m_torrent->GetTorrentFileInfo();

    m_lastAction = ActionAnnounce;
    m_transactionId = QDateTime::currentDateTime().toTime_t();

    const auto &infoHash = torFileInfo->GetInfoHashSHA1();
    if ( infoHash.size() != 20 ){
        qWarning() << Q_FUNC_INFO << "INVALIS INFOHASH";
    }

    auto clientId = TorrentClient::instance()->GetClientId();
    if ( clientId.size() != 20 ){
        qWarning() << Q_FUNC_INFO << "INVALIS CLIENT ID";
    }

    qint32 event = 2;
    qint32 key = 0;
    auto downlInf = m_torrent->GetTorrentDownloadInfo();
    QByteArray messege;
    QDataStream out( &messege, QIODevice::WriteOnly );
    out << qint64( m_connectionId ) << qint32( m_lastAction ) << qint32( m_transactionId );

    auto ipAddress = TorrentClient::instance()->GetIpAddress();
    QByteArray messegeEnd;
    QDataStream out2( &messegeEnd, QIODevice::WriteOnly );
    out2<< qint64( downlInf->Downloaded ) << qint64( downlInf->Left )
        << qint64( downlInf->Uploaded ) << qint32( event ) << qint32(ipAddress.toIPv4Address())
        << qint32( key ) << qint32( m_wantPeersCount ) << qint16( TorrentClient::instance()->GetPort() );

    qDebug() << "ANNOUNCE REQUEST IS :" << qint64( m_connectionId ) << qint32( m_lastAction )
             << qint32( m_transactionId ) << infoHash << clientId << qint64( downlInf->Downloaded )
             << qint64( downlInf->Left ) << qint64( downlInf->Uploaded )
             << qint32( event ) << qint32( 0 )
             << qint32( key ) << qint32( m_wantPeersCount ) << qint16( TorrentClient::instance()->GetPort() );

    messege.append( infoHash ).append( clientId ).append(messegeEnd);
    qDebug() << "ANNOUNCE REQUEST SIZE : " << m_udpSocket.write( messege );
}

void RequestToServerManager::scrapeRequest()
{
    m_lastAction = ActionScrape;

    QByteArray messege;
    QDataStream out( &messege, QIODevice::WriteOnly );

    qDebug() << "SCRAPE REQUEST SIZE : " << m_udpSocket.write( messege );
}

void RequestToServerManager::sendLastActionRequest()
{
    switch ( m_lastAction ) {
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

void RequestToServerManager::connectResponseHandler()
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
}

void RequestToServerManager::announceResponseHandler()
{
    if ( m_udpSocket.bytesAvailable() < 20 ){
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
    qDebug() << "ANNOUNCE RESPONSE : " << action << transactionId << interval << leechers << seeders;

    if ( action != (qint32)m_lastAction ){
        qCritical() << Q_FUNC_INFO << "WRONG ACTION";
        return;
    }

    if ( m_transactionId != transactionId ){
        qCritical() << Q_FUNC_INFO << "TRANSACTION ID IS NOT EQUAL";
        return;
    }

    m_wasFetch = true;
    int i = 0;
    while ( !in.atEnd() ){
        in >> ipAddress >> tcpPort;
        m_fetchedPeers.insert( PeerInfo( QHostAddress(ipAddress), tcpPort ) );
        qDebug() << QHostAddress(ipAddress).toString() << tcpPort << ++i;
    }
}

void RequestToServerManager::scrapeResponseHandler()
{
    qDebug() << "SCRAPE RESPONSE HANDLER";
}

void RequestToServerManager::responseHandler()
{
    qDebug() << "RESPONSE HANDLER";
    switch ( m_lastAction ) {
    case ActionConnect :
        connectResponseHandler();
        m_requestsAmount = 0;
        break;

    case ActionAnnounce :
        announceResponseHandler();
        m_requestsAmount = 0;
        break;

    case ActionScrape :
        scrapeResponseHandler();
        m_requestsAmount = 0;
        break;

    case ActionError :
        qCritical() << Q_FUNC_INFO << "ERROR ACTION";
        break;

    default:
        qCritical() << Q_FUNC_INFO << "UNKNOWN ACTION";
        break;
    }
}
