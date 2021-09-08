#ifndef NODEWORKER_H
#define NODEWORKER_H

#include <QObject>
#include "PeerNode.h"
#include <QTimer>

class NodeWorker : public QObject
{
    Q_OBJECT

    int32_t hi = 0;
    bool syncing = false;
public:
    NodeWorker(QObject * parent=0);
    fantasybit::Node node;
    int32_t preinit();
    int32_t global_height();
signals:
    void InSync(int32_t height);
    void SeenBlock(int32_t height);
    void BlockError(int32_t height);
    void ResetIndex();

public slots:
    void init();
    void TryNext();
};


#endif
