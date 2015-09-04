//
//  Source.h
//  fantasybit
//
//  Created by Jay Berg on 8/24/14.
//
//

#ifndef __fantasybit__Processor__
#define __fantasybit__Processor__

#include <QObject>

#include "blockrecorder.h"
#include "Data.h"
#include "NameData.h"
#include <fc/crypto/sha256.hpp>
#include <fc/crypto/elliptic.hpp>
#include "ProtoData.pb.h"
#include "ApiData.pb.h"

namespace fantasybit
{

class BlockProcessor : public QObject {

    Q_OBJECT


    BlockRecorder mRecorder{};
    NFLStateData &mData;
    FantasyNameData &mNameData;
	int realHeight = 0;
	int lastidprocessed = 0;
    //GlobalState mGlobalState{};
    bool verify_name(const SignedTransaction &, const NameTrans &,
                     const fc::ecc::signature&, const fc::sha256 &);
    bool amlive = false;
public slots:
    void OnSubscribeLive() {
        amlive = true;
    }

signals:
    void WeekStart(int);
    void WeekOver(int);
    void InvalidState(int);   
    void new_dataDistribution(fantasybit::Distribution);


public:
    BlockProcessor(NFLStateData &data, FantasyNameData &namedata) : mData(data), mNameData(namedata) {}
    int init();

    int process(Block &sblock);
    bool processDataBlock(const Block &sblock);
    //bool isInWeekGame(const std::string &id, int week );
    //bool sanity(const FantasyPlayerPoints &fpp);
	void process(decltype(DataTransition::default_instance().data()) in, 
                const std::string &blocksigner );
    void process(const DataTransition &indt);
    bool isValidTx(const SignedTransaction &st);
    void processTxfrom(const Block &b,int start = 0);
    static bool verifySignedBlock(const Block &sblock);
    static bool verifySignedTransaction(const SignedTransaction &st);
    void processResultProj(PlayerResult* playerresult,
                           std::unordered_map<std::string,int> &proj,
                           const std::string &blocksigner);

    void BlockProcessor::OnWeekOver(int week);
    void BlockProcessor::OnWeekStart(int week);

};

}

#endif

