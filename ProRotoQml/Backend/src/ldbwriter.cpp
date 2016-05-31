#include "ldbwriter.h"
#include <QCryptographicHash>
#include <iostream>
using namespace std;
namespace fantasybit {

void LdbWriter::init() {
    //qDebug() << BlockMeta::descriptor()->file()->DebugString().data();
    write_sync.sync = true;

    leveldb::Options opt;
    opt.create_if_missing = true;
    opt.error_if_exists = false;

    leveldb::Status status;
    leveldb::DB *db;

#ifdef Q_OS_WIN
    status = leveldb::DB::Open(opt, "D:/data/block/meta", &db);
#else
    status = leveldb::DB::Open(opt, "/Users/satoshi/work/tmp/tmp/meta", &db);
#endif

    if (!status.ok()) {
        std::cout << "!!! error opening db block/meta" << std::endl;
        std::cout << "!!! " << status.ToString() << std::endl;
        exit(1);
    }

    this->db = db;
}

string LdbWriter::write(const string &key, const string &val) {
    //qDebug() << key.data() << val.data();
    db->Put(write_sync,key,val);
    return key;
}

string LdbWriter::write(const string &val) {
    QString valToQString  = QString::fromStdString (val);
    QByteArray results = QCryptographicHash::hash( valToQString.toLatin1() , QCryptographicHash::Sha256 );
    QString hashResult(results.toHex());

    /// back to the std::string
    auto ret = hashResult.toStdString ();

    write(ret,val);
    return ret;

    // auto ret = fc::sha256::hash(val).str();
//    write(ret,v//al);
//    return ret;
}

string LdbWriter::write(const google::protobuf::Message &msg) {
    return write(msg.SerializeAsString());
}

std::string LdbWriter::read(const std::string &id) {
    string value;
    db->Get(leveldb::ReadOptions(),id,&value);
    return value;
}

string LdbWriter::read(const string &id, google::protobuf::Message &msg) {
    auto str = read(id);
    msg.ParseFromString(str);
    return str;
}

//std::string LdbWriter::read(const QString &qid) {
//    return read(qid.toStdString());
//}


LdbWriter::~LdbWriter() {
    if ( db ) delete db;
}

}
