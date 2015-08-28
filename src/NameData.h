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
#include <set>
#include <mutex>
#include <memory>

using std::string;
namespace fantasybit
{

class FantasyNameData : public QObject {

    Q_OBJECT

    std::shared_ptr<leveldb::DB> namestore;
    std::shared_ptr<leveldb::DB> projstore;

    leveldb::WriteOptions write_sync{};

    std::unordered_map<std::string,
            std::unordered_map<std::string,int>> FantasyNameProjections;
    std::unordered_map<std::string,
            std::unordered_map<std::string,int>> PlayerIDProjections;

    std::set<std::string> mSubscribed{};
    bool amlive = false;
    int week = 0;

    std::recursive_mutex data_mutex{};


signals:
    void ProjectionLive(FantasyBitProj &);
    void FantasyNameFound(string);
    void FantasyNameBalance(FantasyNameBal &);

public slots:
    void OnLive(bool subscribe) {
        amlive = true;
    }

    void OnWeekOver(int week);
    void OnWeekStart(int week);


public:
    FantasyNameData() {}
    void init();
    void AddNewName(std::string name, std::string pubkey);
    void AddBalance(const std::string name,uint64_t amount);

    void AddProjection(const std::string &name, const std::string &player, uint32_t proj);
    void OnProjection(const std::string &name, const std::string &player, uint32_t proj);
    void OnFantasyName(std::string &name);
    void OnFantasyNameBalance(FantasyNameBal &fn);

    //ToDo: the place to store game time IN/OUt status of NFL player
    //void UpdatePlayerGameStatus(const std::string &player, PlayerGameStatus status);

    std::unordered_map<std::string,int> GetProjById(const std::string &pid);
    std::unordered_map<std::string,int> GetProjByName(const std::string &nm);

    void Subscribe(std::string );
    void UnSubscribe(std::string );

    std::string filedir(const std::string &in);
};

}

#endif /* defined(__fantasybit__NameData__) */
