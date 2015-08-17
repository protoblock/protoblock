//
//  NameData.h
//  fantasybit
//
//  Created by Jay Berg on 8/17/15.
//
//
#ifndef __fantasybit__NameData__
#define __fantasybit__NameData__

#include "NameData.pb.h"
#include <leveldb/db.h>

namespace fantasybit
{

class NameData {
    leveldb::DB *namestore;
    leveldb::DB *projstore;

    leveldb::WriteOptions write_sync{};

    std::map<std::string, std::map<std::string,uint32_t>> FantasyNameProjections;
    std::map<std::string, std::map<std::string,uint32_t>> PlayerIDProjections;

public:
    NameData();
    void init();
    void AddNewName(const std::string &pubkey,const std::string &name);
    void AddBalance(uint64_t amount);

    void AddProjection(const std::string &name, const std::string player, uint32_t proj);

    std::string filedir(const std::string &in);
};

}

#endif /* defined(__fantasybit__NameData__) */
