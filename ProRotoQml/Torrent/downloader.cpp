#include "downloader.h"

#include "peerconnection.h"
#include "torrentclient.h"
#include "requesttoservermanager.h"
#include "peersmanager.h"

#include <QMessageBox>

//// start static
const uint Downloader::_MaxPiecesDownloading = 5;
const uint Downloader::_MaxBlockSize4Request = 8000;

const QHash< Downloader::States, QString > Downloader::_State2Name = Downloader::formState2Name();

const QString Downloader::getStateName(States state)
{
    if ( !_State2Name.contains( state ) ){
        return _State2Name[ StateInvalid ];
    }

    return _State2Name[ state ];
}

const QHash<Downloader::States, QString> Downloader::formState2Name()
{
    QHash<Downloader::States, QString> state2name;
    state2name[ StatePeersProcessing ] = QStringLiteral( "Getting list of peers" );
    state2name[ StateFileVerification ] = QStringLiteral( "Files checking" );
    state2name[ StateDownloading ] = QStringLiteral( "Downloading" );
    state2name[ StateDownloaded ] = QStringLiteral( "Downloaded" );
    state2name[ StateInvalid ] = QStringLiteral( "Invalid state" );
    return state2name;
}
//// end static


Downloader::Downloader(const TorrentFileInfo &info, QObject *parent)
    : QObject(parent)
    , m_torrentFileInfo( info )
{
    m_nextPiece2Download = -1;

    auto requestManager = new RequestToServerManager( new Torrent(info) );
    m_peersManager = new PeersManager( requestManager, m_torrentFileInfo.GetInfoHashSHA1(),
                                       m_torrentFileInfo.GetPieces().size(), this );
    connect( m_peersManager, &PeersManager::FetchComplited, this, &Downloader::fetchComplited );
    connect( m_peersManager, &PeersManager::DownloadSpeedChanged, this, &Downloader::downloadSpeedChanged );
    connect( m_peersManager, &PeersManager::UploadSpeedChanged, this, &Downloader::uploadSpeedChanged );

    m_fileManager = new FileManager( this );
    connect( m_fileManager, &FileManager::verificationProgress, this, &Downloader::progressChanged );
    connect( m_fileManager, &FileManager::verificationDone, this, &Downloader::startDownload );
    connect( m_fileManager, &FileManager::pieceVerified, this, [this]( int index, bool isVer ){
        Q_UNUSED( index );
        if ( isVer ){
            int verPiecesCount = m_fileManager->completedPieces().count( true );
            int piecesCount = m_torrentFileInfo.GetPieces().size();
            emit progressChanged( verPiecesCount * 100 / piecesCount );
        }
    } );

    m_fileManager->setDestinationFolder("D:/");
    m_fileManager->setTorrentFileInfo( info );
    m_fileManager->start();
}

Downloader::~Downloader()
{
}

void Downloader::startTorrent()
{
    setState( StateFileVerification );
    m_fileManager->startDataVerification();
}

void Downloader::startDownload()
{
    setState( StatePeersProcessing );
    m_peersManager->SetPeersCount( 50 );
    m_peersManager->StartFetchPeers();

    const auto complitedPieces = m_fileManager->completedPieces();
    m_nextPiece2Download = -1;
    for ( int i = 0; i < complitedPieces.size(); ++i ){
        if ( !complitedPieces.testBit( i ) ){
            m_nextPiece2Download = i;
            break;
        }
    }
    if ( m_nextPiece2Download < 0 ){
        setState( StateDownloaded );
    }

}

qint32 Downloader::getFirstNotDownloadedPiece() const
{
    const auto complitedPieces = m_fileManager->completedPieces();

    if ( complitedPieces.count( false ) <= 0 ){
        return -1;
    }

    for ( int i = 0; i < complitedPieces.size(); ++i ){
        if ( !complitedPieces.testBit( i ) ){
            return i;
        }
    }

    return -1;
}

qint32 Downloader::getNextPiece()
{
    const auto complitedPieces = m_fileManager->completedPieces();

    if ( complitedPieces.count( false ) <= 0 ){
        return -1;
    }

    auto prepareNewPiece = [this](){
        m_downloadingPiece2ComplitedBlocks[ m_nextPiece2Download ].resize( getBlocksCount() );
        m_downloadingPiece2NextBlock[ m_nextPiece2Download ] = 0;
    };

    if ( m_nextPiece2Download >= complitedPieces.size() ){
        m_nextPiece2Download = getFirstNotDownloadedPiece();
        prepareNewPiece();
        return m_nextPiece2Download++;
    }

    while( complitedPieces.testBit( m_nextPiece2Download ) ){
        if ( ++m_nextPiece2Download >= complitedPieces.size() ){
            m_nextPiece2Download = getFirstNotDownloadedPiece();
            prepareNewPiece();
            return m_nextPiece2Download++;
        }
    }

    prepareNewPiece();
    return m_nextPiece2Download++;;
}

void Downloader::pieceDownloaded(qint32 pieceIndex)
{
    if ( m_downloadingPiece2ComplitedBlocks.remove( pieceIndex ) != 1 ){
        qCritical() << Q_FUNC_INFO << "m_downloadingPiece2ComplitedBlocks.remove";
    }
    if ( m_downloadingPiece2NextBlock.remove( pieceIndex ) != 1 ){
        qCritical() << Q_FUNC_INFO << "m_downloadingPiece2NextBlock.remove";
    }
}

void Downloader::fetchComplited()
{
    // Временно, пока не поддерживается сидирование
    if ( m_state == StateDownloaded ){
        return;
    }

    setState( StateDownloading );
    const auto& peers = m_peersManager->GetConnections();
    qDebug() << Q_FUNC_INFO << QThread::currentThreadId() << peers.size();

    // Каждый пир будет качать свой кусок.
    for ( auto peer : peers ){
        auto pieceIndex = getNextPiece();
        m_downloadingPiece2ComplitedBlocks[ pieceIndex ].resize( getBlocksCount() );

        connect( peer, &PeerConnection::BlockDownloaded, this, &Downloader::writeIncomingBlock );
        //peer->SetPiecesCount( m_torrentFileInfo.GetPieces().size() );
        peer->DownloadBlock( pieceIndex , 0, getBlockLength( 0 ) );
    }
}

quint32 Downloader::getBlockLength( quint32 numOfBlock ) const
{
    if ( numOfBlock == getBlocksCount() - 1 ){
        quint32 lastBlockLen = m_torrentFileInfo.GetPieceLength() % _MaxBlockSize4Request;
        if ( lastBlockLen > 0 )
            return lastBlockLen;
    }
    return _MaxBlockSize4Request;
}

quint32 Downloader::getBlocksCount() const
{
    quint32 blocksCount = m_torrentFileInfo.GetPieceLength() / _MaxBlockSize4Request;
    if ( (m_torrentFileInfo.GetPieceLength() % _MaxBlockSize4Request) > 0 ){
        ++blocksCount;
    }
    return blocksCount;
}

quint32 Downloader::getBlockNum(quint32 begin) const
{
    return begin / _MaxBlockSize4Request;
}

quint32 Downloader::getBlockBegin(quint32 blockNum) const
{
    return blockNum * _MaxBlockSize4Request;
}

void Downloader::writeIncomingBlock( quint32 index, quint32 begin, const QByteArray &block )
{
    qDebug() << "INCOMING BLOCK" << index << begin << block.size();
    m_fileManager->write( index, begin, block );

    auto senderPeer = qobject_cast< PeerConnection * >( sender() );
    if ( senderPeer == nullptr ){
        qCritical() << Q_FUNC_INFO << "senderPeer == nullptr";
        return;
    }

    const auto& peers = m_peersManager->GetConnections();
    auto peerConnection = peers.find( senderPeer->GetPeerInfo() );
    if ( peerConnection == peers.end() ){
        qCritical() << Q_FUNC_INFO << "peerConnection == m_peers.end()";
        return;
    }

    quint32 blockBit = getBlockNum( begin );
    if ( blockBit >= getBlocksCount() - 1 ){        // NEW PIECE
        m_fileManager->verifyPiece( index );

        pieceDownloaded( index );
        (*peerConnection)->DownloadBlock( getNextPiece(), 0, getBlockLength( 0 ) );
        qDebug() << "NEW INDEX!!!\nBLOCK NUM " << 0;

        return;
    }
    auto &complBlocks = m_downloadingPiece2ComplitedBlocks[ index ];
    if ( complBlocks.testBit( blockBit ) )
        return;

    complBlocks.setBit( blockBit );

    quint32 nextBlock = ++m_downloadingPiece2NextBlock[ index ];
    qDebug() << "BLOCK NUM " << nextBlock;
    (*peerConnection)->DownloadBlock( index, getBlockBegin(nextBlock), getBlockLength( nextBlock ) );
}

void Downloader::setState(Downloader::States state)
{
    if ( m_state != state ){
        emit stateIsChanged( m_state = state );
    }
}
