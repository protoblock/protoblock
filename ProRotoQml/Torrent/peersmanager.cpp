#include "peersmanager.h"

#include "peerconnection.h"
#include "requesttoservermanager.h"
#include "torrent.h"

#include <QThread>
#include <QTimer>

static uint TIMEOUT_FETCH = 2500;           // 2.5 sec
static uint TIMER_FETCH_TIMEOUT_MAX = 5;

static uint TIMEOUT_UPDATE_SPEED = 6000;    // 6 sec

PeersManager::PeersManager(RequestToServerManager *requestManager, const QByteArray &infoHash,
                           uint piecesCount, QObject *parent)
    : QObject(parent)
    , m_piecesCount( piecesCount )
    , m_infoHash( infoHash )
{
    m_timerFetchTimeoutCount = 0;
    m_timerFetch = new QTimer( this );
    m_timerFetch->setInterval( TIMEOUT_FETCH );
    m_timerFetch->setSingleShot( true );
    connect( m_timerFetch, &QTimer::timeout, this, &PeersManager::timerFetchHandler );

    m_downloadSpeed = 0;
    m_uploadSpeed   = 0;
    m_timerUpdateSpeed = new QTimer( this );
    m_timerUpdateSpeed->setInterval( TIMEOUT_UPDATE_SPEED );
    m_timerUpdateSpeed->setSingleShot( false );
    connect( m_timerUpdateSpeed, &QTimer::timeout, this, &PeersManager::timerUpdateSpeedHandler );

    m_requestManager = requestManager;
    m_needPeersCount = -1;
    m_fetchComplited = false;
    Q_ASSERT( m_requestManager != nullptr );
}

PeersManager::~PeersManager()
{
    if ( m_requestManager != nullptr )
        delete m_requestManager;
}

const QHash<PeerInfo, PeerConnection *> &PeersManager::GetConnections() const
{
    return m_complitedConnections;
}

void PeersManager::SetPeersCount(int peersCount)
{
    m_needPeersCount = peersCount;
}

void PeersManager::StartFetchPeers()
{
    m_fetchComplited = false;
    m_allPeers = m_requestManager->GetPeers( 100 );      // получаем от сервера сразу много пиров
    m_timerFetch->start();
    for ( auto &peerInfo : m_allPeers ){
        auto peer = new PeerConnection();
        m_unknownConnections.insert( peerInfo, peer );
        connect( peer, &PeerConnection::handshakeFailed, this, &PeersManager::removePeer );
        connect( peer, &PeerConnection::handshakeIsDone, this, &PeersManager::handshakedPeer );
        peer->connectToPeer( peerInfo, m_infoHash, m_piecesCount );
    }
}

void PeersManager::abortAllUnknown()
{
    for ( auto peerConnection : m_unknownConnections ){
        peerConnection->abort();
        peerConnection->deleteLater();
    }
    m_unknownConnections.clear();
}

void PeersManager::removePeer()
{
    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();
    auto senderPeer = qobject_cast< PeerConnection * >( sender() );
    if ( senderPeer == nullptr ){
        qWarning() << Q_FUNC_INFO << "senderPeer == nullptr";
        return;
    }
    auto peerInfo = senderPeer->GetPeerInfo();
    auto failedPeer = m_unknownConnections.take( peerInfo );
    if ( failedPeer == nullptr ){
        qDebug() << Q_FUNC_INFO << "failedPeer == nullptr";
        return;
    }
    failedPeer->deleteLater();
    failedPeer->abort();
}

void PeersManager::handshakedPeer()
{
    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();
    auto senderPeer = qobject_cast< PeerConnection * >( sender() );
    if ( senderPeer == nullptr ){
        qCritical() << Q_FUNC_INFO << "senderPeer == nullptr";
        return;
    }

    auto peer = m_unknownConnections.take( senderPeer->GetPeerInfo() );
    if ( peer == nullptr ){
        qDebug() << Q_FUNC_INFO << "peer == nullpt";
        return;
    }
    m_handshakedConnections.insert( peer->GetPeerInfo(), peer );

    if ( isNeedMorePeers() ){
        connect( peer, &PeerConnection::IsPeerChokingChanged,
                 this, &PeersManager::newSuccessConnection );
        peer->sendInterested();
    }
//    else {
//        abortAllUnknown();
//    }
}

void PeersManager::newSuccessConnection(bool isPeerChoking)
{
    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();
    if ( !isNeedMorePeers() ){
        setFetchComplited();
        return;
    }

    auto senderPeer = qobject_cast< PeerConnection * >( sender() );
    if ( senderPeer == nullptr ){
        qCritical() << Q_FUNC_INFO << "senderPeer == nullptr";
        return;
    }

    if ( isPeerChoking ){
        return;
    }

    auto peerConnection = m_handshakedConnections.take( senderPeer->GetPeerInfo() );
    if ( peerConnection == nullptr ){
        qDebug() << Q_FUNC_INFO << "peerConnection == nullptr" << QThread::currentThreadId();
        return;
    }
    m_complitedConnections.insert( peerConnection->GetPeerInfo(), peerConnection);
//    quint64 pieceCount = m_torrentFileInfo.GetPieceLength() / MaxBlockSize4Request;

//    auto piece2Block = m_downloadingPiece2ComplitedBlocks.insert( m_nextPiece2Download, QBitArray( pieceCount ) );
//    auto piece2NextBlock = m_downloadingPiece2NextBlock.insert( m_nextPiece2Download, 0 );

//    (*peerConnection)->sendRequest( piece2Block.key(), piece2NextBlock.value() * MaxBlockSize4Request,
//                                    MaxBlockSize4Request );
    //    ++m_nextPiece2Download;
}

void PeersManager::timerFetchHandler()
{
    if ( !isNeedMorePeers() || m_handshakedConnections.size() <= 0
         || m_timerFetchTimeoutCount++ > TIMER_FETCH_TIMEOUT_MAX )
    {
        setFetchComplited();
        return;
    }

    for ( auto peer : m_unknownConnections ){
        peer->sendInterested();
    }

    m_timerFetch->start();
}

void PeersManager::timerUpdateSpeedHandler()
{
    quint64 downloadSpeed = 0;
    quint64 uploadSpeed = 0;
    for ( PeerConnection *peer : m_complitedConnections ){
        downloadSpeed   += peer->GetDownloadSpeed();
        uploadSpeed     += peer->GetUploadSpeed();
    }
    if ( downloadSpeed != m_downloadSpeed ){
        emit DownloadSpeedChanged( m_downloadSpeed = downloadSpeed );
    }
    if ( uploadSpeed != m_uploadSpeed ){
        emit UploadSpeedChanged( m_uploadSpeed = uploadSpeed );
    }
}

bool PeersManager::isNeedMorePeers() const
{
    if ( m_fetchComplited )
        return false;

    if ( m_needPeersCount < 0 )
        return true;

    return m_complitedConnections.size() < m_needPeersCount;
}

void PeersManager::setFetchComplited()
{
    if ( m_fetchComplited )
        return;

    m_fetchComplited = true;
    abortAllUnknown();
    emit FetchComplited();
    m_timerUpdateSpeed->start();
}
