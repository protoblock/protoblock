#include "NodeWorker.h"

#include <QObject>
//#include <QSharedDataPointer>
//#include <QNetworkAccessManager>
//#include <QNetworkReply>
//#include <QEventLoop>
//#include <QRunnable>
//#include <QApplication>
//#include <QWaitCondition>
//#include "globals.h"
//#include "Node.h"
//#include <QTimer.h>
//#include <algorithm>

    NodeWorker::NodeWorker(QObject * parent):  QObject(parent)
    {
    }


    void NodeWorker::init(){
        qDebug("NodeWorker Thread started");

        if ( node.Sync() )
            emit InSync(node.getLastLocalBlockNum());
        else
            emit BlockError(node.getLastLocalBlockNum());

    }

    void NodeWorker::TryNext() {
        auto gnum = node.getLastGlobalBlockNum();
        if ( !gnum )
            qDebug("Error getLastGlobalBlockNum");
        else {
            emit SeenBlock(*gnum);
            node.SyncTo(*gnum);
        }
    }
