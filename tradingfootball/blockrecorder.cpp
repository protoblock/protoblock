#include <blockrecorder.h>
#include <QDebug>
#include <leveldb/db.h>

using namespace fantasybit ;


void BlockRecorder::InitCheckpoint(int32_t lastblock) {
    leveldb::DB *db1;
    leveldb::Status status;
    leveldb::DB::Open(leveldb::Options(), filedir("blockstatus"), &db1);
    leveldb::Slice value((char*)&lastblock, sizeof(int32_t));
    db1->Put(leveldb::WriteOptions(), "lastblock", value);
    delete db1;
}

void BlockRecorder::init() {
    write_sync.sync = true;
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::DB *db1;
    leveldb::Status status;

    status = leveldb::DB::Open(options, filedir("blockstatus"), &db1);
    qDebug() << filedir("blockstatus");
    if (!status.ok()) {
        std::string err = status.ToString();
        qWarning() << err;
    }
    blockstatus.reset(db1);
    std::string value;
    status = blockstatus->Get(leveldb::ReadOptions(), "lastblock", &value);
    if (!status.ok()) {
        lastBlock =  0;
        qWarning() << "!ok no blocks";
    }
    else {
        lastBlock = *(reinterpret_cast<const int32_t *>(value.data()));
        qInfo() << "lastBLock: " << lastBlock;
    }
}


void BlockRecorder::startBlock(int32_t num) {
    leveldb::Slice value((char*)&num, sizeof(int32_t));
    blockstatus->Put(write_sync, "processing", value);
    blockstatus->Put(write_sync, "lastblock", value);
    qInfo() << "starting block: " << num;
}


int32_t BlockRecorder::endBlock(int32_t num) {
    int32_t none = -1;
    leveldb::Slice value((char*)&none, sizeof(int32_t));
    blockstatus->Put(write_sync, "processing", value);
    qInfo() << "end block: " << num;
    return num;
}


bool BlockRecorder::isValid() {
    std::string value;
    if (blockstatus->Get(leveldb::ReadOptions(), "processing", &value).IsNotFound())
        return true;

    int32_t num = *(reinterpret_cast<const int32_t *>(value.data()));
    return num < 0;
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


