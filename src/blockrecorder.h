#ifndef BLOCKRECORDER_H
#define BLOCKRECORDER_H

#include "ProtoData.pb.h"
#include <leveldb/db.h>

namespace fantasybit {
using hash_t = uint64_t;

class BlockRecorder
{
    leveldb::DB *blockstatus;  // lastblock -> status // last block processed
    leveldb::DB *namehashpub;  // FantasyName::Hash -> public_key
    leveldb::DB *pubfantasyname; // public_key ->FantasyName
    leveldb::DB *pubbalance; // public_key -> fantasybit balance
    leveldb::DB *state; // GlobalState and TeamState
    leveldb::DB *players; // NFL playerid -> PlayerData
    leveldb::DB *teams; // NFL teamid -> TeamData
    leveldb::DB *projections; // playerid:FantasyName -> projection

    //leveldb::DB *gamesids;
    //leveldb::DB *projections;
    leveldb::WriteOptions write_sync{};

    //std::map<Uid, leveldb::DB*> game_projections_db{};
    //std::vector<leveldb::DB*> alldbs{};
    int lastBlock = 0;
public:
    void init();
    void initAllData();
    void initFantasyNames();
    void initProjections();
    void initData();    DeltaData DeltaSnap();
    void OnGlobalState(const GlobalState &gs);
    GlobalState GetGlobalState();
    void OnTeamState(const TeamState &gs);
    TeamState GetTeamState(const std::string &teamid);
    void OnPlayerData(const PlayerData &gs);
    PlayerData GetPlayerData(const std::string &pid);
    void OnTeamData(const TeamData &gs);
    TeamData GetTeamData(const std::string &pid);
    void startBlock(int num);
    int endBlock(int num);
    bool isValid();
    int getLastBlockId();
    void recordName(const hash_t &hash,const std::string &pubkey,const std::string &name);
    void addProjection(const std::string &fname, const FantasyPlayerPoints &fpp);
    void clearProjections();
    void addBalance(std::string &pubkey,uint64_t add);
    std::string filedir(const std::string &in);

    void closeAll();
};
}

#endif // BLOCKRECORDER_H

