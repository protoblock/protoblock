#ifndef DATAAGENT_H
#define DATAAGENT_H

#include <QMutex>
#include "genericsingleton.h"
#include "core.h"
#include "LAPIWorker.h"
#include "playerloader.h"
using namespace fantasybit;
class dataagent : public GenericSingleton<dataagent>  {
    friend class GenericSingleton<dataagent>;
    dataagent() : gameloader("PRE") {}
public:

    void getExternalGameResults();
    void stageData();
    void markgameStart();
    void processingPendingTx();
    void getTx();
    void makeBlock();
    void sendBlock();
    void oracleSign();

    SignedTransaction signTx(Transaction &tx);
    Block makeNewBlockAsDataAgent(const SignedTransaction &);
    GameResult getGameResult(int week, GameInfo &gi );

    fc::optional<BlockHeader> mLastBlock;
    GameStatsLoader gameloader;
};

#endif // DATAAGENT_H
