#ifndef REQUESTTOSERVERMANAGER_H
#define REQUESTTOSERVERMANAGER_H

#include <QObject>
#include <QSharedPointer>
#include <QByteArray>
#include <QUdpSocket>
#include <QSet>

#include "torrentfileinfo.h"
#include "torrent.h"
#include "peerinfo.h"

class TorrentClient;

class RequestToServerManager : public QObject
{
    Q_OBJECT
public:
    explicit RequestToServerManager(Torrent *torrent, QObject *parent = 0);
    ~RequestToServerManager(){}

    const QSet<PeerInfo> &GetPeers(qint32 peersCount = -1 );

signals:

private slots:

//// Requests :
    void connectRequest();
    void announceRequest();
    void scrapeRequest();

    void sendLastActionRequest();

//// Handlers :
    void connectResponseHandler();
    void announceResponseHandler();
    void scrapeResponseHandler();
    void responseHandler();

private:
    enum ActionTypes{
        ActionConnect   = 0,
        ActionAnnounce  = 1,
        ActionScrape    = 2,
        ActionError     = 3
    };
    ActionTypes             m_lastAction;

    QUdpSocket              m_udpSocket;
    Torrent                *m_torrent;

    qint32                  m_transactionId;
    qint64                  m_connectionId;

    QByteArray              m_response;

    qint32                  m_wantPeersCount;
    uint                    m_requestsAmount;        // После каждого успешного действия сбрасывается

    bool                    m_wasFetch;
    QSet< PeerInfo >        m_fetchedPeers;
};

#endif // REQUESTTOSERVERMANAGER_H
