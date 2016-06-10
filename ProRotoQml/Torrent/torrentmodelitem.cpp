#include "torrentmodelitem.h"
#include "torrentmodel.h"

void TorrentModelItem::setupConnections()
{
    connect(m_client, &TorrentClient::stateChanged,       this, &TorrentModelItem::updateState);
    connect(m_client, &TorrentClient::peerInfoUpdated,    this, &TorrentModelItem::updatePeerInfo);
    connect(m_client, &TorrentClient::progressUpdated,    this, &TorrentModelItem::updateProgress);
    connect(m_client, &TorrentClient::downloadRateUpdated,this, &TorrentModelItem::updateDownloadRate);
    connect(m_client, &TorrentClient::uploadRateUpdated,  this, &TorrentModelItem::updateUploadRate);
    //connect(m_client, &TorrentClient::stopped,            this, &TorrentModelItem::torrentStopped);
    //connect(m_client, &TorrentClient::sigError,           this, &TorrentModelItem::torrentError);
}

TorrentModelItem::TorrentModelItem(QObject *parent)
    : QObject ( parent )
    , m_client( nullptr )
{
}

TorrentModelItem::TorrentModelItem(TorrentClient *client, const QString &destDir,
                                   const QString &name, QObject *parent)
    : QObject( parent )
    , m_client( client )
{
    Q_ASSERT( m_client != nullptr );

    m_destDir   = destDir;
    m_name      = name;
    m_downloadRate  = 0;
    m_uploadRate    = 0;

    if ( m_name.isEmpty() ){
        auto metaInfo = m_client->metaInfo();
        m_name = metaInfo.fileForm() == MetaInfo::SingleFileForm ? metaInfo.singleFile().name : metaInfo.name();
    }
    setupConnections();
}

TorrentClient *TorrentModelItem::client() const
{
    return m_client;
}

void TorrentModelItem::setClient(TorrentClient *client)
{
    Q_ASSERT( m_client == nullptr );
    m_client = client;
    //m_client->setParent( this );
}

QString TorrentModelItem::name() const
{
    return m_name;
}

void TorrentModelItem::setName(const QString &name)
{
    m_name = name;
}

QString TorrentModelItem::destDir() const
{
    return m_destDir;
}

void TorrentModelItem::setDestDir(const QString &destDir)
{
    m_destDir = destDir;
}

quint64 TorrentModelItem::uploadRate() const
{
    return m_uploadRate;
}

quint64 TorrentModelItem::downloadRate() const
{
    return m_downloadRate;
}

void TorrentModelItem::start()
{
    Q_ASSERT( m_client != nullptr );
    m_client->start();
}

QUrl TorrentModelItem::torrentFilePath() const
{
    return m_torrentFilePath;
}

void TorrentModelItem::setTorrentFilePath(const QUrl &torrentFilePath)
{
    m_torrentFilePath = torrentFilePath;
}


/// private slots start
void TorrentModelItem::updateState(TorrentClient::State newState)
{
    Q_UNUSED( newState );
    QVector<int> v( 2 );
                 v[ 0 ] = TorrentModel::TorStateStr;
                 v[ 1 ] = TorrentModel::TorState;
    emit dataChanged( v );
}

void TorrentModelItem::updatePeerInfo()
{
    QVector<int> v(2);
                 v[0] = TorrentModel::TorPeerCount;
                 v[1] = TorrentModel::TorSeedCount;
    emit dataChanged( std::move(v) );
}

void TorrentModelItem::updateProgress(int progress)
{
    Q_UNUSED( progress );
    emit dataChanged( QVector<int>(1, TorrentModel::TorProgress) );
}

void TorrentModelItem::updateDownloadRate(int dRate)
{
    if ( dRate != m_downloadRate ){
        m_downloadRate = dRate;
        emit dataChanged( QVector<int>(1, TorrentModel::TorDownSpeed) );
    }
}

void TorrentModelItem::updateUploadRate(int uRate)
{
    if ( uRate != m_uploadRate ){
        m_uploadRate = uRate;
        emit dataChanged( QVector<int>(1, TorrentModel::TorUpSpeed) );
    }
}

void TorrentModelItem::torrentStopped()
{
}

void TorrentModelItem::torrentError(TorrentClient::Error error)
{
    Q_UNUSED( error );
}

/// private slots end
