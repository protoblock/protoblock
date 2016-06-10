#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QHash>

#include "peerinfo.h"
#include "torrentfileinfo.h"
#include "filemanager.h"

class PeerConnection;
class TorrentClient;
class RequestToServerManager;
class PeersManager;

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(const TorrentFileInfo &info, QObject *parent = 0);
    ~Downloader();

    enum States{
        StatePeersProcessing,
        StateFileVerification,
        StateDownloading,
        StateDownloaded,
        StateInvalid
    };

signals:
    void progressChanged(double percent);
    void stateIsChanged( States newState );
    void downloadSpeedChanged( quint64 newSpeed );
    void uploadSpeedChanged( quint64 newSpeed );

public slots:
    void startTorrent();
    void fetchComplited();
    void writeIncomingBlock(quint32 index, quint32 begin, const QByteArray &block);

private slots:
    void setState( States state );
    void startDownload();

private:
    States                              m_state;

    QSet< PeerInfo >                    m_peers;
    QHash< PeerInfo, PeerConnection *>  m_peerConnections;
    QHash< quint32, QBitArray >         m_downloadingPiece2ComplitedBlocks;
    QHash< quint32, quint32 >           m_downloadingPiece2NextBlock;

    FileManager                        *m_fileManager;
    PeersManager                       *m_peersManager;
    TorrentFileInfo                     m_torrentFileInfo;

    // Временно. Потом номер куска для скачки будет определяться по частоте встречания этого куска у пиров
    qint32                              m_nextPiece2Download;

private:
    qint32  getFirstNotDownloadedPiece() const;
    qint32  getNextPiece();
    void    pieceDownloaded( qint32 pieceIndex );

    quint32 getBlockLength(quint32 numOfBlock) const;
    quint32 getBlocksCount() const;
    quint32 getBlockNum(quint32 begin) const;
    quint32 getBlockBegin(quint32 blockNum) const;

//// Static public members
public:
    static const QString getStateName( States state );

//// Static consts
private:
    static const uint _MaxPiecesDownloading;
    static const uint _MaxBlockSize4Request;

    static const QHash< States, QString > _State2Name;

//// Static private members
private:
    static const QHash< States, QString > formState2Name();
};

#endif // DOWNLOADER_H
