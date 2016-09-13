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
#include <QDir>
#include "StateData.pb.h"
#include "ldbwriter.h"
#include "PeerNode.h"
#include "pbutils.h"

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

    std::unordered_map<std::string,long> PlayerIDSumProj;


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
    int GetAvgProj(const string &playerid) {
        std::lock_guard<std::recursive_mutex> lockg{ data_mutex };
        int count = PlayerIDProjections[playerid].size();
        long sum = PlayerIDSumProj[playerid];

//        qDebug() << " sum " << sum << " count " << count;
        if  (count > 0 && sum > 0)
            return  sum / count;
        else
            return 0;
    }


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
    void AddProjection(const std::string &name, const std::string &player, uint32_t proj,int32_t);
    void OnProjection(const std::string &name, const std::string &player, uint32_t pro,int32_t,int);
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

    void OnSeasonStart(int season) {
//        bootStrap(season,1);
    }

    void OnSeasonEnd(int season) {
        seasonFreeze(season);
    }

    void addBootStrap(Bootstrap *in) {
        LdbWriter ldb;
        ldb.init(Node::bootstrap.get());
        Bootstrap &bs = *in;

        bs.set_fnamemetaroot(BootStrapFantasyName(ldb));

        ldb.write(in->key(),ldb.write(bs));
        return;
    }

    std::string BootStrapFantasyName(LdbWriter &ldb) {
        auto *it = namestore->NewIterator(leveldb::ReadOptions());
        FantasyNameBalMeta fnm;
        MerkleTree tree;

        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            FantasyNameBal fn;
            fn.ParseFromArray(it->value().data(),it->value().size());
            fnm.set_name(fn.name());
            fnm.set_public_key(fn.public_key());
            fnm.set_bits(fn.bits());
            fnm.set_stake(fn.stake());
            tree.add_leaves(ldb.write(fnm));
        }
        delete it;

        tree.set_root(pb::makeMerkleRoot(tree.leaves()));
        return ldb.write(tree.root(),tree.SerializeAsString());
    }

    void seasonFreeze(int season) {
        return;
        closeAll();
        string moveto = GET_ROOT_DIR() + "freeze-" + std::to_string(season);
        QDir dir(moveto.data());
        if ( !dir.exists() )
            dir.mkdir(moveto.data());
        dir.rename(filedir("namestore").data(), (moveto + "/namestore").data());
        init();
    }
};

}

#endif /* defined(__fantasybit__NameData__) */
