#ifndef TORRENTMODELITEM_H
#define TORRENTMODELITEM_H

#include <QObject>
#include <QVector>
#include <QUrl>

#include "torrentclient.h"

class TorrentModelItem : public QObject
{
    Q_OBJECT
public:
    explicit TorrentModelItem(QObject *parent = 0);
    TorrentModelItem(TorrentClient *client, const QString &destDir,
                     const QString &name = QString(), QObject *parent = 0);
    ~TorrentModelItem(){}

signals:
    void dataChanged( QVector< int > fields );

public:
    TorrentClient *client() const;
    void setClient(TorrentClient *client);

    QString name() const;
    void setName(const QString &name);

    QString destDir() const;
    void setDestDir(const QString &destDir);

    quint64 uploadRate() const;
    quint64 downloadRate() const;

    QUrl torrentFilePath() const;
    void setTorrentFilePath(const QUrl &torrentFilePath);

public slots:
    void start();

private:
    TorrentClient*      m_client;
    QString             m_name;
    QString             m_destDir;
    quint64             m_downloadRate;
    quint64             m_uploadRate;
    QUrl                m_torrentFilePath;

private slots:
    void updateState( TorrentClient::State newState );
    void updatePeerInfo();
    void updateProgress( int progress );
    void updateDownloadRate( int dRate );
    void updateUploadRate( int uRate );
    void torrentStopped();
    void torrentError( TorrentClient::Error error );

private:
    void setupConnections();
};

typedef TorrentModelItem* TorrentModelItemStar;
Q_DECLARE_METATYPE( TorrentModelItemStar )

#endif // TORRENTMODELITEM_H
