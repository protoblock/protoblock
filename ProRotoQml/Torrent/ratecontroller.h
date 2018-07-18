#ifndef RATECONTROLLER_H
#define RATECONTROLLER_H

#include <QObject>
#include <QSet>
#include <QTime>

class PeerWireClient;

class RateController : public QObject
{
    Q_OBJECT

public:
    inline RateController(QObject *parent = 0)
        : QObject(parent), transferScheduled(false) { }
    static RateController *instance();

    void addSocket(PeerWireClient *socket);
    void removeSocket(PeerWireClient *socket);

    inline int uploadLimit() const { return upLimit; }
    inline int downloadLimit() const { return downLimit; }
    inline void setUploadLimit(int bytesPerSecond) { upLimit = bytesPerSecond; }
    void setDownloadLimit(int bytesPerSecond);

public slots:
    void transfer();
    void scheduleTransfer();

private:
    QTime stopWatch;
    QSet<PeerWireClient *> sockets;
    int upLimit;
    int downLimit;
    bool transferScheduled;
};

#endif
