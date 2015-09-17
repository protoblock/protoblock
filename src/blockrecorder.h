#ifndef BLOCKRECORDER_H
#define BLOCKRECORDER_H

#include <leveldb/db.h>
#include "fbutils.h"

namespace fantasybit {

class BlockRecorder {

    std::unique_ptr<leveldb::DB> blockstatus;  // lastblock -> status // last block processed
    leveldb::WriteOptions write_sync{};
    int32_t lastBlock = 0;

//public slots:

//signals:
//    //void BadState(int);

public:

    void init();
    /*
    void initAllData();
    void initFantasyNames();
    void initProjections();
    void initData();
    DeltaData DeltaSnap();
    void OnGlobalState(const GlobalState &gs);
    GlobalState GetGlobalState();
    void OnTeamState(const TeamState &gs);
    TeamState GetTeamState(const std::string &teamid);
    void OnPlayerData(const PlayerData &gs);
    PlayerData GetPlayerData(const std::string &pid);
    void OnTeamData(const TeamData &gs);
    TeamData GetTeamData(const std::string &pid);
*/
    void startBlock(int32_t num);
    int32_t endBlock(int32_t num);
    bool isValid();
    int32_t getLastBlockId();
    /*
    void recordName(const hash_t &hash,const std::string &pubkey,const std::string &name);
    void addProjection(const std::string &fname, const FantasyPlayerPoints &fpp);
    void clearProjections();
    void addBalance(std::string &pubkey,uint64_t add);
    */
    std::string filedir(const std::string &in);
    void closeAll();
};
}

#endif // BLOCKRECORDER_H

