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
#include "ApiData.pb.h"
#include "FantasyName.h"

using std::string;
namespace fantasybit
{

class FantasyNameData : public QObject {

    Q_OBJECT

    std::shared_ptr<leveldb::DB> namestore;
    std::shared_ptr<leveldb::DB> projstore;

    leveldb::WriteOptions write_sync{};

    typedef std::unordered_map<std::string,std::unordered_map<std::string,int>> mapmapi;

    mapmapi FantasyNameProjections;
    std::unordered_map<std::string,
            std::unordered_map<std::string,int>> PlayerIDProjections;

    std::set<std::string> mSubscribed{};
    bool amlive = false;
    int week = 0;

    std::recursive_mutex data_mutex{};


signals:
    void ProjectionLive(fantasybit::FantasyBitProj);
    void FantasyNameFound(string);
    void FantasyNameBalance(fantasybit::FantasyNameBal);
    void FantasyNamePnl(fantasybit::FantasyNameBal);
    void new_dataFantasyNameHash(fantasybit::FantasyNameHash);
    void NewFantasyNameOrder(fantasybit::Order&);

public slots:
    void OnLive(bool subscribe) {
        amlive = true;
    }

    //void OnFantasyNameOrder(fantasybit::Order&);

public:
    void OnWeekOver(int week);
    void OnWeekStart(int week);

public:
    FantasyNameData() {}
    void init();
    void closeAll();

    /*
    void IsSubscribed(const std::string &fname) {
        return amlive && (mSubscribed.find(name) != end(mSubscribed));
    }
    */

    void AddNewName(std::string name, std::string pubkey);
    void AddBalance(const std::string name,uint64_t amount);
    void AddPnL(const std::string name, int64_t pnl);
    void AddProjection(const std::string &name, const std::string &player, uint32_t proj);
    void OnProjection(const std::string &name, const std::string &player, uint32_t proj);
    void OnFantasyName(std::shared_ptr<FantasyName> fn);

    void OnFantasyNameBalance(FantasyNameBal &fn);

    void OnFantasyNamePnl(FantasyNameBal &fn);

    //ToDo: the place to store game time IN/OUt status of NFL player
    //void UpdatePlayerGameStatus(const std::string &player, PlayerGameStatus status);

    std::unordered_map<std::string,int> GetProjById(const std::string &pid);
    std::unordered_map<std::string,int> GetProjByName(const std::string &nm);

    void Subscribe(std::string );
    void UnSubscribe(std::string );

    void OnGameStart(std::string gid,
                     std::vector<std::string> &home,
                     std::vector<std::string> &away
                     );

    GameFantasyBitProj GetGameProj(const std::string &gid);

    std::string filedir(const std::string &in);

    void dump(mapmapi &mm);

    void dump(std::unordered_map<std::string,int> &m);

    void dumpProj();

};

}

#endif /* defined(__fantasybit__NameData__) */
