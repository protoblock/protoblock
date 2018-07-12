#include "NodeWorker.h"

#include <QObject>

int32_t NodeWorker::preinit() {
    node.init();
    auto h = node.getLastGlobalBlockNum();
    return h ? *h : 0;
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
        auto gnum = node.getLastGlobalBlockNum();

        if ( last > *gnum  && last > 1 && *gnum > 1) {
            node.BackSync(*gnum);
            hi = node.getLastLocalBlockNum();
            emit ResetIndex();
            int32_t last = node.getLastLocalBlockNum();
            emit InSync(last);
        }
        else if ( last == *gnum && last > 1) {
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
    auto gnum = node.getLastGlobalBlockNum();
    if ( !gnum )
        qDebug("Error getLastGlobalBlockNum");
    else {
        if ( *gnum > hi ) {
            hi = *gnum;
            emit SeenBlock(*gnum);
            if ( !node.SyncTo(*gnum) ) {
                int32_t last = node.getLastLocalBlockNum();
                if ( last == *gnum && last > 1) {
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
