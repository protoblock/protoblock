//
//  Source.h
//  fantasybit
//
//  Created by Jay Berg on 8/24/14.
//
//

#ifndef __fantasybit__Processor__
#define __fantasybit__Processor__

#include "blockrecorder.h"
#include <fc/filesystem.hpp>
#include <fc/crypto/elliptic.hpp>


namespace fantasybit
{

class BlockProcessor
{
    BlockRecorder mRecorder{};
	int realHeight = 0;
	int lastidprocessed = 0;
	DeltaData outDelta{};
	GlobalState mGlobalState{};
    bool verify_name(const SignedTransaction &, const NameTrans &, const fc::ecc::signature&, const fc::sha256 &);

public:
    int init();

    DeltaData GetandClear();
    bool process(Block &sblock);
    bool processDataBlock(const Block &sblock);
    bool isInWeekGame(const std::string &id, int week );
    bool sanity(const FantasyPlayerPoints &fpp);
	void process(decltype(DataTransition::default_instance().data()) in, 
                const std::string &blocksigner );
    void process(const DataTransition &indt);
    bool isValidTx(const SignedTransaction &st);
    void processTxfrom(const Block &b,int start = 0);
    void setPreGameWeek(int week);
    static bool verifySignedBlock(const Block &sblock);
    static bool verifySignedTransaction(const SignedTransaction &st);
};

}


#endif

