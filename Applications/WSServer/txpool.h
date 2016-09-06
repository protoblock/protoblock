#ifndef TXPOOL_H
#define TXPOOL_H

//
//  txpool.h
//  fantasybit
//
//  Created by Jay Berg on 7/23/16
//
//



#include <mutex>
#include "fbutils.h"
#include <leveldb/db.h>
#include "ProtoData.pb.h"

namespace fantasybit
{

class TxPool
{
public:
    static void init() {
        leveldb::Options options;
        options.create_if_missing = true;
        leveldb::Status status;

        leveldb::DB *db4;
        std::string fname = "C:/work/prodWsserver0905/wsserver/storage/tx/txpool";
        status = leveldb::DB::Open(options, fname, &db4);
        txpool.reset(db4);
        if ( !status.ok() ) {
            qCritical() << " cant open " << GET_ROOT_DIR().data() << "tx/txpool";
            //todo emit fatal
            return;
        }

        {
        auto *it = txpool->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            SignedTransaction st;
            if ( !st.ParseFromString(it->value().ToString())) {
                 qCritical() << "NameData error reading names";
                 break;
            }

            if ( st.trans().type() == TransType::NAME) {

                if ( myTxSt.find(st.fantasy_name()) != end(myTxSt))
                    qDebug() << "fatal error " << st.DebugString().data();

                myTxSt[st.fantasy_name()] = st;
            }

            qDebug() << "txpool" << it->key().ToString().data() << st.DebugString().data();
        }
        }

        leveldb::DB *db5;
        status = leveldb::DB::Open(options, GET_ROOT_DIR() + "tx/meta", &db5);
        meta.reset(db5);
        if ( !status.ok() ) {
            qCritical() << " cant open " << GET_ROOT_DIR().data() << "tx/meta";
            //todo emit fatal
            return;
        }

        {
        auto *it = meta->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            qDebug() << "meta " << it->key().ToString().data() << it->value().ToString().data();
        }
        }



    }

    leveldb::WriteOptions write_sync{};

    static std::shared_ptr<leveldb::DB> txpool;
    static std::shared_ptr<leveldb::DB> meta;

    static std::unordered_map<std::string,SignedTransaction> myTxSt;
    static bool addTxPool(const std::string &id, std::string &txstr)  {
        uint64_t timestamp = (std::chrono::duration_cast<std::chrono::milliseconds>
                          (std::chrono::system_clock::now().time_since_epoch()).count());

        qDebug() << " txpool " << id.data();
        if (txpool->Put(leveldb::WriteOptions(), id, txstr ).ok()) {
           qDebug() << " timestamp " << timestamp;
           return (meta->Put(leveldb::WriteOptions(),std::to_string(timestamp),id)).ok();
        }

        return false;
    }
};
}

#endif // TXPOOL_H
