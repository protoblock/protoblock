#ifndef TORRENTMODEL_H
#define TORRENTMODEL_H

#include <QAbstractListModel>

#include "metainfo.h"
#include "torrentclient.h"

#include <QUrl>

class TorrentModelItem;

class TorrentModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS( TorrentClient::State )
    Q_ENUMS( TorrentRoles )
    Q_PROPERTY( int count READ rowCount NOTIFY countChanged )

    typedef QAbstractListModel MyBase;

public:
    enum TorrentRoles{
        TorName = Qt::UserRole + 1,
        TorStateStr,
        TorState,
        TorProgress,
        TorDownSpeed,
        TorUpSpeed,
        TorBytesDownloaded,
        TorBytesUploaded,
        TorSeedCount,
        TorPeerCount
    };

    explicit TorrentModel(QObject *parent = 0);
    ~TorrentModel();

    Q_INVOKABLE QVariant data( int index, TorrentRoles role );

/// QAbstractListModel implementation start
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    // don't use these methods.
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
/// QAbstractListModel implementation end

signals:
    void countChanged( int newCount );

public slots:
    void addTorrent(const QUrl &fileUrl, const QUrl &destinationFolder,
                    const QByteArray &resumeState = QByteArray());
    void addTorrent(const QString &fileName, const QString &destinationFolder,
                    const QByteArray &resumeState = QByteArray());

    void setPause( int index, bool value );
    void setStop ( int index );

private:
    TorrentClient  *getTorrentClientSender( QObject *sender ) const;
    int             client2row( TorrentClient *client ) const;

    QList< TorrentModelItem* > m_items;
    const TorrentModelItem &at(int index ) const;
    TorrentClient *atClient( int index ) const;

private slots:
    void itemChanged( QVector<int> roles );
};

#endif // TORRENTMODEL_H
