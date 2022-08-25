#include <blockrecorder.h>
#include <QDebug>
#include <leveldb/db.h>
#include "PeerNode.h"
#include <pbutils.h>

using namespace fantasybit ;

int BlockRecorder::zeroblock(-1);
int BlockRecorder::BlockTimestamp(0);

void BlockRecorder::InitCheckpoint(int32_t lastblock) {
    pb::make_all(filedir(""));

    leveldb::DB *db1{};
    leveldb::Status status{};
    leveldb::Options options{};
    options.create_if_missing = true;

    leveldb::DB::Open(options, filedir("blockstatus"), &db1);
    leveldb::Slice value((char*)&lastblock, sizeof(int32_t));
    db1->Put(leveldb::WriteOptions(), "lastblock", value);
    delete db1;
}

int32_t BlockRecorder::GetInitLastBlockProccsed() {
    pb::make_all(filedir(""));

    leveldb::DB *db1{};
    leveldb::Status status{};
    leveldb::Options options{};
    options.create_if_missing = true;

    leveldb::DB::Open(options, filedir("blockstatus"), &db1);
    int32_t ret = -1;
    std::string value;
    if (db1->Get(leveldb::ReadOptions(), "processing", &value).IsNotFound()) {
        status = db1->Get(leveldb::ReadOptions(), "lastblock", &value);
        if (status.ok()) {
            ret =  *(reinterpret_cast<const int32_t *>(value.data()));
        }
    }
    delete db1;
    return ret;
}

void BlockRecorder::init() {
    pb::make_all(filedir(""));

//    write_sync.sync = true;
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::DB *db1;
    leveldb::Status status;

    status = leveldb::DB::Open(options, filedir("blockstatus"), &db1);
    qDebug() << filedir("blockstatus").data();
    if (!status.ok()) {
        std::string err = status.ToString();
        qWarning() << err.data();
    }
    blockstatus.reset(db1);
    std::string value;
    status = blockstatus->Get(leveldb::ReadOptions(), "lastblock", &value);
    if (!status.ok()) {
        lastBlock =  BlockRecorder::zeroblock;
        qWarning() << "!ok no blocks start from " << lastBlock;
    }
    else {
        lastBlock = *(reinterpret_cast<const int32_t *>(value.data()));
        qInfo() << "BlockRecorder::init lastBLock: " << lastBlock;

    }

    #ifdef CLEAN_BLOCKS
    Int32Comparator *cmp = new Int32Comparator();
    leveldb::Options optionsInt;
    optionsInt.create_if_missing = true;
    optionsInt.comparator = cmp;

    leveldb::DB *db2;
    status = leveldb::DB::Open(optionsInt, filedir("cleanblock"), &db2);
    cleanblocks.reset(db2);
    #endif

}


void BlockRecorder::startBlock(int32_t num) {
#ifdef TRACEDEBUG
        qDebug() << " startBlock " << num;
#endif

    leveldb::Slice value((char*)&num, sizeof(int32_t));
    blockstatus->Put(write_sync, "processing", value);
    blockstatus->Put(write_sync, "lastblock", value);
    //qInfo() << "starting block: " << num;
}


int32_t BlockRecorder::endBlock(int32_t num) {
#ifdef TRACEDEBUG
        qDebug() << " endBlock " << num;
#endif
    int32_t none = -1;
    leveldb::Slice value((char*)&none, sizeof(int32_t));
    blockstatus->Put(write_sync, "processing", value);
    //qInfo() << "end block: " << num;
    return num;
}


bool BlockRecorder::isValid(int32_t height) {
    std::string value;
    if (blockstatus->Get(leveldb::ReadOptions(), "processing", &value).IsNotFound())
        return true;

    int32_t num = *(reinterpret_cast<const int32_t *>(value.data()));
    return num < 0 && lastBlock > BlockRecorder::zeroblock && lastBlock <= height;
}


int32_t BlockRecorder::getLastBlockId() {
    return lastBlock;
}

std::string BlockRecorder::filedir(const std::string &in) {
    return GET_ROOT_DIR() + "index/" + in;
}

void BlockRecorder::closeAll() {
    blockstatus.reset();
}


