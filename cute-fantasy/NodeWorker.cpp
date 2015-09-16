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


    void NodeWorker::preinit() {
        node.init();
        //node.Cleaner();

    }

    void NodeWorker::init(){
        qDebug("NodeWorker Thread started");

        bool ret = node.Sync();
        int last = node.getLastLocalBlockNum();

        if ( last > hi ) hi = last;

        if ( ret )
            emit InSync(last);
        else {
            auto gnum = node.getLastGlobalBlockNum();

            if ( last > *gnum  && last > 1 && *gnum > 1) {
                node.BackSync(*gnum);
                hi = node.getLastLocalBlockNum();
                emit ResetIndex();
                int last = node.getLastLocalBlockNum();
                emit InSync(last);
            }
            else
                emit BlockError(last);
        }
    }

    void NodeWorker::TryNext() {
        qDebug() << this->thread();
        auto gnum = node.getLastGlobalBlockNum();
        if ( !gnum )
            qDebug("Error getLastGlobalBlockNum");
        else {
            if ( *gnum > hi ) {
                hi = *gnum;
                emit SeenBlock(*gnum);
                if ( !node.SyncTo(*gnum) ) {
                    int last = node.getLastLocalBlockNum();
                    emit BlockError(last);
                }
            }
        }
    }
