#ifndef LDBWRITER_H
#define LDBWRITER_H

#include <leveldb/db.h>
#include <QDebug>
//#include "../utils/utils.h"
#include <string>
#include <generated/StateData.pb.h>

namespace fantasybit {

class LdbWriter {
    leveldb::DB *db;
    leveldb::WriteOptions write_sync{};

public:
    void init();
    std::string write(const std::string &key, const std::string &val);
    std::string write(const std::string &val);
    std::string write(const google::protobuf::Message &msg);
    std::string read(const std::string &id) ;
    //std::string read( const  QString &qid) ;

    /**
     * @brief read and parse protobuf
     * @param id
     * @param msg
     * @return
     */
    std::string read(const std::string &id,
                     google::protobuf::Message &msg);

    ~LdbWriter();
};

}

#endif // LDBWRITER_H
