#include "torrentserializer.h"

#include <QDebug>
#include <QSettings>
#include <QTimer>

#include "torrentmodel.h"
#include "torrentmodelitem.h"

int TorrentSerializer::SaveTimeSecDefault = 60;

TorrentSerializer::TorrentSerializer(TorrentModel *model, QObject *parent)
    : QObject( parent )
    , m_model( model )
    , m_saveTimer( nullptr )
{
    Q_ASSERT( m_model != nullptr );
    m_settings      = new QSettings( this );
    m_saveTimeSec   = SaveTimeSecDefault;
}

void TorrentSerializer::initSaveTimer()
{
    Q_ASSERT( m_saveTimer == nullptr );
    m_saveTimer     = new QTimer( this );
    m_saveTimer->setInterval( m_saveTimeSec * 1000 );
    connect( m_saveTimer, &QTimer::timeout,            this, &TorrentSerializer::save );
    connect( m_model,     &QObject::destroyed,         this, &TorrentSerializer::onModelDestroyed );
}

TorrentSerializer::~TorrentSerializer()
{
    m_saveTimer->stop();
    m_settings->sync();
}

void TorrentSerializer::save()
{
    int rowCount = m_model->rowCount();
    m_settings->beginWriteArray( "TorrentData", rowCount );
    for ( int i = 0; i < rowCount; ++i ){
        auto index       = m_model->index(i, 0);
        auto variantData = m_model->data( index, Qt::DisplayRole );
        auto torrentItem = variantData.value< TorrentModelItemStar >();
        m_settings->setArrayIndex( i );
        m_settings->setValue("torrentFilePath"  , torrentItem->torrentFilePath() );
        m_settings->setValue("destDir"          , torrentItem->destDir() );
        m_settings->setValue("uploadedBytes"    , torrentItem->client()->uploadedBytes() );
        m_settings->setValue("downloadedBytes"  , torrentItem->client()->downloadedBytes() );
        m_settings->setValue("dumpedState"      , torrentItem->client()->dumpedState() );

    }
    m_settings->endArray();
}

void TorrentSerializer::load()
{
    int rowCount = m_settings->beginReadArray( "TorrentData" );
    for ( int i = 0; i < rowCount; ++i ){
        m_settings->setArrayIndex( i );
        auto torFilePath     = m_settings->value("torrentFilePath").toUrl();
        auto destDir         = m_settings->value("destDir").toString();
        auto dumpedState     = m_settings->value("dumpedState").toByteArray();
        auto uploadedBytes   = m_settings->value("uploadedBytes" ).toLongLong();
        auto downloadedBytes = m_settings->value("downloadedBytes" ).toLongLong();

        int oldCount = m_model->rowCount();
        m_model->addTorrent( torFilePath.toLocalFile(), destDir, dumpedState );
        int newCount = m_model->rowCount();
        if ( oldCount == newCount ){
            qWarning() << Q_FUNC_INFO << "oldCount == newCount";
            continue;
        }

        auto indexLast = m_model->index( m_model->rowCount() - 1, 0 );
        auto itemLast  = m_model->data( indexLast, Qt::DisplayRole ).value< TorrentModelItemStar >();
        auto addClient = itemLast->client();
        addClient->setUploadedBytes( uploadedBytes );
        addClient->setDownloadedBytes( downloadedBytes );
    }
    m_settings->endArray();
}

void TorrentSerializer::onModelDestroyed()
{
    m_saveTimer->stop();
    this->disconnect();

}

int TorrentSerializer::saveTimeSec() const
{
    return m_saveTimeSec;
}

void TorrentSerializer::setSaveTimeSec(int saveTimeSec)
{
    if ( m_saveTimeSec == saveTimeSec )
        return;

    m_saveTimer->setInterval( (m_saveTimeSec = saveTimeSec) * 1000 );
}


TorrentModel *TorrentSerializer::model() const
{
    return m_model;
}

void TorrentSerializer::startAutoSave()
{
    if ( m_saveTimer == nullptr ){
        initSaveTimer();
        connect( m_model,     &TorrentModel::countChanged, this, &TorrentSerializer::save );
    }

    if ( !m_saveTimer->isActive() )
        m_saveTimer->start();
}


