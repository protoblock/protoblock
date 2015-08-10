#ifndef NODEWORKER_H
#define NODEWORKER_H

#include <QObject>
#include <QSharedDataPointer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QRunnable>
#include <QApplication>
#include <QWaitCondition>
#include "globals.h"
#include "Node.h"
#include <QTimer.h>
#include <algorithm>


class NodeWorker : public QObject
{
    Q_OBJECT
public:
    NodeWorker() {
    }
    //QObject * parent=0):QObject(parent){}
    fantasybit::Node node;

signals:
    void InSync(int height);
    void SeenBlock(int height);

public slots:

    void init(){
        qDebug("NodeWorker Thread started");

        if ( node.Sync() )
            emit InSync(node.getLastLocalBlockNum());
    }

    void TryNext() {
        auto gnum = node.getLastGlobalBlockNum();
        if ( !gnum )
            qDebug("Error getLastGlobalBlockNum");
        else {
            emit SeenBlock(*gnum);
            node.SyncTo(*gnum);
        }
    }
};


#endif
