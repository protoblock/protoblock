#include "ldbwriter.h"

using namespace std;
namespace fantasybit {

void LdbWriter::init(leveldb::DB * db) {
    //qDebug() << BlockMeta::descriptor()->file()->DebugString().data();

//    write_sync.sync = true;
    this->db = db;

/*

    leveldb::Options opt;
    opt.create_if_missing = true;
    opt.error_if_exists = false;

    leveldb::Status status;

    leveldb::DB *db;
#ifdef JAYDESK
    status = leveldb::DB::Open(opt, "D:/data/working-explorer/block/meta", &db);

#else
    status = leveldb::DB::Open(opt, StaticUtil::DATAPATH+"/meta", &db);
#endif

    if (!status.ok()) {
        std::cout << "!!! error opening db block/meta" << std::endl;
        std::cout << "!!! " << status.ToString() << std::endl;
        exit(1);
    }

    this->db = db;
    */
}

std::string LdbWriter::read(const std::string &id) {
    string value;
    db->Get(leveldb::ReadOptions(),id,&value);
    return value;
}

std::string LdbWriter::read(const std::string &id, GOOGLE_NAMESPACE::protobuf::Message &msg) {
    auto str = read(id);
    msg.ParseFromString(str);
    return str;
}

}
