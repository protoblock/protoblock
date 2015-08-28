#include <blockrecorder.h>
#include <QDebug>
#include <leveldb/db.h>

using namespace fantasybit ;

void BlockRecorder::init() {
    write_sync.sync = true;
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::DB *db1;
    leveldb::Status status;
    status = leveldb::DB::Open(options, filedir("blockstatus"), &db1);
    blockstatus.reset(db1);
    std::string value;
    status = blockstatus->Get(leveldb::ReadOptions(), "lastblock", &value);
    if (!status.ok()) {
        lastBlock =  0;
        qWarning() << "!ok no blocks";
    }
    else {
        lastBlock = *(reinterpret_cast<const int *>(value.data()));
        qInfo() << "lastBLock: " << lastBlock;
    }
}


void BlockRecorder::startBlock(int num) {
    leveldb::Slice value((char*)&num, sizeof(int));
    blockstatus->Put(write_sync, "processing", value);
    blockstatus->Put(leveldb::WriteOptions(), "lastblock", value);
    qInfo() << "starting block: " << num;
}


int BlockRecorder::endBlock(int num) {
    int none = -1;
    leveldb::Slice value((char*)&none, sizeof(int));
    blockstatus->Put(write_sync, "processing", value);
    qInfo() << "end block: " << num;
    return num;
}


bool BlockRecorder::isValid() {
    std::string value;
    if (blockstatus->Get(leveldb::ReadOptions(), "processing", &value).IsNotFound())
        return true;

    int num = *(reinterpret_cast<const int *>(value.data()));
    return num < 0;
}


int BlockRecorder::getLastBlockId() {
    return lastBlock;
}

std::string BlockRecorder::filedir(const std::string &in) {
    return GET_ROOT_DIR() + "index/" + in;
}

void BlockRecorder::closeAll() {
    blockstatus.reset();
}


