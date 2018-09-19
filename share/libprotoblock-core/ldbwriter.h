#ifndef LDBWRITER_H
#define LDBWRITER_H

#include <leveldb/db.h>
#include <QDebug>
#include "utils/utils.h"
#include <string>
#include "StateData.pb.h"

namespace fantasybit {

class LdbWriter {
    leveldb::DB *db;
    leveldb::WriteOptions write_sync{};

public:

    void init(leveldb::DB * = nullptr);


    std::string write(const std::string &key, const std::string &val) {
        //qDebug() << key.data() << val.data();
        if (db->Put(write_sync,key,val).ok())
            return key;
        else return "";
    }
    std::string write(const std::string &val) {
        auto ret = pb::hashit(val).str();
        write(ret,val);
        return ret;
    }

    std::string write(const GOOGLE_NAMESPACE::protobuf::Message &msg) {
        return write(msg.SerializeAsString());
    }

    std::string write(const fantasybit::MerkleTree &tree) {
        return write(tree.root(),tree.SerializeAsString());
    }

    std::string write(const fantasybit::KeyValue &kv) {
        return write(kv.key(),kv.value());
    }


    std::string read(const std::string &id) ;

    /**
     * @brief read and parse protobuf
     * @param id
     * @param msg
     * @return
     */
    std::string read(const std::string &id,
                     GOOGLE_NAMESPACE::protobuf::Message &msg);

    ~LdbWriter() {
//        if ( db ) delete db;
    }
};

}

#endif // LDBWRITER_H

/*
 *
 * class LdbWriter {
    leveldb::DB *db;
    leveldb::WriteOptions write_sync{};

public:

    void LdbWriter::init() {

        //for(auto fd : BlockMeta::descriptor()->DebugString().data())
        write_sync.sync = true;

        leveldb::Options opt;
        opt.create_if_missing = true;
        opt.error_if_exists = false;

        leveldb::Status status;

        leveldb::DB *db;
        status = leveldb::DB::Open(opt, "./block/meta", &db);
        if (!status.ok()) {
            cout << "!!! error opening db block/meta" << endl;
            cout << "!!! " << status.ToString() << endl;
            exit(1);
        }

        this->db = db;
    }


    void write(const string &key, const string &val) {
        //qDebug() << key.data() << val.data();
        db->Put(write_sync,key,val);
    }
    void write(const string &val) {
        write(fc::sha256::hash(val).str(),val);
    }

    void write(const google::protobuf::Message &msg) {
        write(msg.SerializeAsString());
    }

    string read(const string &id) {
        string value;
        db->Get(leveldb::ReadOptions(),id,&value);
        return value;
    }

    ~LdbWriter() {
        if ( db ) delete db;
    }
};
*/
