#ifndef TORRENTSERVER_H
#define TORRENTSERVER_H

#include <QList>
#include <QTcpServer>

class TorrentClient;

class TorrentServer : public QTcpServer
{
    Q_OBJECT

public:
    inline TorrentServer() {}
    static TorrentServer *instance();

    void addClient(TorrentClient *client);
    void removeClient(TorrentClient *client);

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

private slots:
    void removeClient();
    void processInfoHash(const QByteArray &infoHash);

private:
    QList<TorrentClient *> clients;
};

#endif
