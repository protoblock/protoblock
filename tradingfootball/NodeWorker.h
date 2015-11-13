#ifndef NODEWORKER_H
#define NODEWORKER_H

#include <QObject>
//#include <QSharedDataPointer>
//#include <QNetworkAccessManager>
//#include <QNetworkReply>
#include <QEventLoop>
//#include <QRunnable>
#include <QApplication>
//#include <QWaitCondition>
//#include "globals.h"
#include "PeerNode.h"
//#include <QTimer.h>
//#include <algorithm>


class NodeWorker : public QObject
{
    Q_OBJECT

    int32_t hi = 0;
public:
    NodeWorker(QObject * parent=0);
    fantasybit::Node node;
    int32_t preinit();

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
