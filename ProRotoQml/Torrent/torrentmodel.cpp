#include "torrentmodel.h"
#include <QFileInfo>
#include <QThread>
#include <QUrl>

#include "torrentmodelitem.h"
#include "ratecontroller.h"
#include "torrentserializer.h"

TorrentModel::TorrentModel(QObject *parent)
    : QAbstractListModel(parent)
{

    TorrentSerializer   serializer( this );
                        serializer.load();
                        serializer.startAutoSave();
}

TorrentModel::~TorrentModel()
{
}

QVariant TorrentModel::data(int index, TorrentRoles role)
{
    return data( this->index( index ), role );
}

QVariant TorrentModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() || index.row() < 0 || index.row() >= m_items.size() )
        return QVariant();

    auto &item = at( index.row() );
    if ( item.client() == nullptr ){
        qWarning() << Q_FUNC_INFO << "item.client() == nullptr";
        return QVariant();
    }

    switch ( role ){
        case Qt::DisplayRole    : return QVariant::fromValue( m_items[ index.row() ] );
        case TorName            : return item.name();
        case TorStateStr        : return item.client()->stateString();
        case TorState           : return item.client()->state();
        case TorProgress        : return item.client()->progress() < 0 ? 0 : item.client()->progress();
        case TorDownSpeed       : return item.downloadRate();
        case TorUpSpeed         : return item.uploadRate();
        case TorBytesUploaded   : return item.client()->uploadedBytes();
        case TorBytesDownloaded : return item.client()->downloadedBytes();
        case TorSeedCount       : return item.client()->seedCount();
        case TorPeerCount       : return item.client()->connectedPeerCount();
    }

    return QVariant();
}

int TorrentModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.size();
}

Qt::ItemFlags TorrentModel::flags(const QModelIndex &index) const
{
    return MyBase::flags( index );
}

QHash<int, QByteArray> TorrentModel::roleNames() const
{
    QHash<int, QByteArray> roles = MyBase::roleNames();

    roles[ TorName ]        = "TorName";
    roles[ TorStateStr ]    = "TorStateStr";
    roles[ TorState ]       = "TorState";
    roles[ TorProgress ]    = "TorProgress";
    roles[ TorUpSpeed ]     = "TorUpSpeed";
    roles[ TorDownSpeed ]   = "TorDownSpeed";
    roles[ TorBytesDownloaded ] = "TorBytesDownloaded";
    roles[ TorBytesUploaded ] = "TorBytesUploaded";
    roles[ TorSeedCount ]   = "TorSeedCount";
    roles[ TorPeerCount ]   = "TorPeerCount";

    return roles;
}

bool TorrentModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED( index );
    Q_UNUSED( value );
    Q_UNUSED( role );
    return false;
}

bool TorrentModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if ( row != m_items.size() )
        return false;

    if ( count == 0 )
        return true;

    beginRemoveRows(parent, row, row + count - 1);
    for ( int i = 0; i  < count; ++i ){
        m_items.push_back( new TorrentModelItem(this) );
    }
    endRemoveRows();
    emit countChanged( rowCount() );

    return true;
}

bool TorrentModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if ( row >= m_items.size() || row < 0 || count < 0 )
        return false;

    if ( row + count > m_items.size() )
        return false;

    if ( count == 0 )
        return true;

    beginInsertRows(parent, row, row + count - 1);
    for ( int i = row; i < row + count; ++i ){
        delete m_items[ i ];
        m_items.removeAt( i );
    }
    endInsertRows();
    emit countChanged( rowCount() );

    return true;
}

void TorrentModel::addTorrent(const QUrl &fileUrl, const QUrl &destinationFolder, const QByteArray &resumeState)
{
    if ( !fileUrl.isLocalFile() ){
        Q_ASSERT( false );
        return;
    }

    if ( !destinationFolder.isLocalFile() ){
        Q_ASSERT( false );
        return;
    }

    addTorrent( fileUrl.toLocalFile(), destinationFolder.toLocalFile(), resumeState );
}

void TorrentModel::addTorrent(const QString &fileName, const QString &destinationFolder,
                              const QByteArray &resumeState)
{
    // Create a new torrent client and attempt to parse the torrent data.
    auto client = new TorrentClient;
    if ( !client->setTorrent( fileName ) ) {
        delete client;
        return ;
    }

    // Check if the torrent is already being downloaded.
    auto findRes = std::find_if( m_items.begin(), m_items.end(), [&]( TorrentModelItem* item ){
        return item->client()->infoHash() == client->infoHash();
    } );
    if ( findRes != m_items.end() ){
        return ;
    }

    client->setDestinationFolder( destinationFolder );
    client->setDumpedState( resumeState );

    auto newItem = new TorrentModelItem( client, destinationFolder );
    newItem->setTorrentFilePath( QUrl::fromLocalFile(fileName) );
    connect( newItem, &TorrentModelItem::dataChanged, this, &TorrentModel::itemChanged );

    beginInsertRows( QModelIndex(), rowCount(), rowCount() );
    m_items << newItem;
    endInsertRows();
    emit countChanged( rowCount() );

    client->start();
}

void TorrentModel::setPause(int index, bool value)
{
    atClient(index)->setPaused( value );
}

void TorrentModel::setStop(int index)
{
    beginRemoveRows( QModelIndex(), index, index );

    auto client = atClient( index );
    //connect( client, &TorrentClient::stopped, client, &QObject::deleteLater );
    connect( this, &QObject::destroyed, client, &QObject::deleteLater );
    client->stop();
    delete m_items[ index ];
    m_items.removeAt( index );

    endRemoveRows();
    emit countChanged( rowCount() );
}

TorrentClient *TorrentModel::getTorrentClientSender(QObject *sender) const
{
    auto torClient = qobject_cast< TorrentClient * >( sender );
    Q_ASSERT( torClient != nullptr );
    return torClient;
}

int TorrentModel::client2row(TorrentClient *client) const
{
    Q_ASSERT( client != nullptr );
    auto res = std::find_if( m_items.begin(), m_items.end(), [=]( TorrentModelItem *item ){
        return item->client() == client;
    });
    if ( res != m_items.end() )
        return res - m_items.begin();
    else
        return -1;
}

const TorrentModelItem &TorrentModel::at(int index) const
{
    Q_ASSERT( index < m_items.size() && index >= 0 );
    if ( index >= m_items.size() ){
        return *m_items.last();
    }
    if ( index < 0 ){
        return *m_items.first();
    }
    return *m_items[ index ];
}

TorrentClient *TorrentModel::atClient(int index) const
{
    Q_ASSERT( index >= 0 );
    if ( index < 0 )
        return nullptr;

    auto client = at( index ).client();
    Q_ASSERT( client != nullptr );
    return client;
}

/// private slots
void TorrentModel::itemChanged(QVector<int> roles)
{
    int row = client2row( qobject_cast< TorrentModelItem *>( sender() )->client() );
    Q_ASSERT( row >= 0 );
    if ( row < 0 )
        return;

    emit dataChanged( index(row), index(row), roles );
}

