//
//  NameData.h
//  fantasybit
//
//  Created by Jay Berg on 8/17/15.
//
//
#ifndef __fantasybit__NameData__
#define __fantasybit__NameData__


#include <QObject>
#include "NameData.pb.h"
#include <leveldb/db.h>
#include <unordered_map>
namespace fantasybit
{

class NameData : public QObject {

    Q_OBJECT

    leveldb::DB *namestore;
    leveldb::DB *projstore;

    leveldb::WriteOptions write_sync{};

    std::unordered_map<std::string,
            std::unordered_map<std::string,int>> FantasyNameProjections;
    std::unordered_map<std::string,
            std::unordered_map<std::string,int>> PlayerIDProjections;
    bool amlive = false;
public slots:
    void OnSubscribeLive() {
        amlive = true;
    }

public:
    NameData() {}
    void init();
    void AddNewName(const std::string &pubkey,const std::string &name);
    void AddBalance(const std::string name,uint64_t amount);

    void AddProjection(const std::string &name, const std::string &player, uint32_t proj);
    void OnProjection(const std::string &name, const std::string &player, uint32_t proj);
    void OnFantasyName(const std::string name);

    std::unordered_map<std::string,int> GetProjById(const std::string &pid) {
        return PlayerIDProjections[pid];
    }

    std::string filedir(const std::string &in);
};

}

#endif /* defined(__fantasybit__NameData__) */
