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

    int hi = 0;
public:
    NodeWorker(QObject * parent=0);
    fantasybit::Node node;
    void preinit();

signals:
    void InSync(int height);
    void SeenBlock(int height);
    void BlockError(int height);
    void ResetIndex();

public slots:

    void init();
    void TryNext();
};


#endif
