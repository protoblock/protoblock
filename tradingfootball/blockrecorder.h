#ifndef BLOCKRECORDER_H
#define BLOCKRECORDER_H

#include <leveldb/db.h>
#include "fbutils.h"
#include "ProtoData.pb.h"

namespace fantasybit {

class BlockRecorder {

    std::unique_ptr<leveldb::DB> blockstatus;  // lastblock -> status // last block processed
    leveldb::WriteOptions write_sync{};
    int32_t lastBlock = 0;
public:
    static void InitCheckpoint(int32_t);

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
    static std::string filedir(const std::string &in);
    void closeAll();

#ifdef CLEAN_BLOCKS
    std::unique_ptr<leveldb::DB> cleanblocks;

    fantasybit::Block cleanBlock;
    void newBlock(const fantasybit::Block &inb) {
        cleanBlock.Clear();
        cleanBlock.mutable_signedhead()->CopyFrom(inb.signedhead());
        cleanBlock.mutable_signed_transactions()->Add()->CopyFrom(inb.signed_transactions(0));
    }

    void addTx(const fantasybit::SignedTransaction &st) {
        cleanBlock.mutable_signed_transactions()->Add()->CopyFrom(st);
    }

    void endBlock() {
        int32_t bnum = cleanBlock.signedhead().head().num();
        leveldb::Slice value((char*)&bnum, sizeof(int32_t));
        cleanblocks->Put(write_sync, value, cleanBlock.SerializeAsString());
    }

#endif
};
}

#endif // BLOCKRECORDER_H

