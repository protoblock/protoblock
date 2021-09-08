#include "NodeWorker.h"

#include <QObject>
#include <QThread>

NodeWorker::NodeWorker(QObject * parent):  QObject(parent) {}

int32_t NodeWorker::preinit() {
    node.init();
    return global_height();
}

void NodeWorker::init(){
    qDebug("NodeWorker Thread started");

    syncing = true;
    bool ret = node.Sync();
    int32_t last = node.getLastLocalBlockNum();

    if ( last > hi ) hi = last;

    if ( ret )
        emit InSync(last);
    else {
        int32_t ret_h = global_height();

        if ( last > ret_h  && last > 1 && ret_h > 1) {
            node.BackSync(ret_h);
            hi = node.getLastLocalBlockNum();
            emit ResetIndex();
            int32_t last = node.getLastLocalBlockNum();
            emit InSync(last);
        }
        else if ( last == ret_h && last > 1) {
            emit ResetIndex();
            emit InSync(last);
        }
        else
            emit BlockError(last);
    }
    syncing = false;
}

void NodeWorker::TryNext() {
    if ( syncing ) return;
    syncing = true;
//        qDebug() << this->thread();
    int32_t ret_h = global_height();
    if ( ret_h == 0)
        qDebug("Error getLastGlobalBlockNum");
    else {
        if ( ret_h > hi ) {
            hi = ret_h;
            emit SeenBlock(ret_h);
            if ( !node.SyncTo(ret_h) ) {
                int32_t last = node.getLastLocalBlockNum();
                if ( last == ret_h && last > 1) {
                    emit ResetIndex();
                    emit InSync(last);
                }
                else
                    emit BlockError(last);
            }
        }
    }
    syncing = false;
}

int32_t NodeWorker::global_height() {
    int32_t ret_h = 0;
    int count = 0;
    while ( true ) {
        auto h = node.getLastGlobalBlockNum();
        ret_h = h ? *h : 0;
        if ( ret_h > 0 )
            return ret_h;

        int sleept = count++ * 100;
        if ( sleept > 5000) sleept = 5000;
        QThread::currentThread()->msleep(sleept);
    }
}
