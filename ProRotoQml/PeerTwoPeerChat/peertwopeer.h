#ifndef PEERTWOPEER_H
#define PEERTWOPEER_H

#include <QQuickItem>

class PeerTwoPeer : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(PeerTwoPeer)

public:
    PeerTwoPeer(QQuickItem *parent = 0);
    ~PeerTwoPeer();
};

#endif // PEERTWOPEER_H
