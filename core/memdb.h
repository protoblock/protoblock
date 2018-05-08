/*
//  memdb.h
//
//  Protoblock
//
//  Created by Jay Berg on 5/7/2018
//
*/
#ifndef MEMDB_H
#define MEMDB_H

namespace pb {

static int PORT_HAND = 10130;
static int PORT_SYNC_SERV = 10125;
static int PORT_LIVE_BLOCK = 10126;
static int PORT_LIVE_TX = 10127;
static int PORT_LIVE_TX_NAT = 10128;

static std::string GET_ROOT_DIR() {
    return ROOT_DIR_;
}

static std::string FB_PORT(int port) {
    return std::string(":").append(std::to_string(port));
}

template <typename T>
class memdb {

    std::auto_ptr<leveldb::DB> ldbp;
    std::mutex ldb_mutex;
    leveldb::WriteOptions write_sync;
    typename T::memtype memmap;
    typename T::pbufty pbufmsg;

public:
    memdb() : write_sync{ sync = true } {}

    void init() {
        leveldb::Options options;
        options.create_if_missing = true;
        leveldb::Status status;
        leveldb::DB *db;
        if ( leveldb::DB::Open(options, filedir(dname()), &db).ok() ) {
            ldbp.reset(db);

            auto *it = ldbp->NewIterator(leveldb::ReadOptions());
            for (it->SeekToFirst(); it->Valid(); it->Next()) {
                pbufmsg.ParseFromArray(it->value().size(),it->value().data());
                add2mem(memmap,it->key(),pbufmsg);
            }
        }
    }

    virtual std::string filedir(const std::string &in) {
        return GET_ROOT_DIR() + in;
    }

    bool add2Memdb(const std::string &key,const T::pbufty &data) {
        ldbp->Put(write_sync, key, data.SerializeAsString());

        add2mem(memmap,it->key(),pbufmsg);
    }
};

}
#endif // MEMDB_H
