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
    dataagent() : gameloader("REG") {}
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
    fc::optional<Block> makeNewBlockAsDataAgent(Transaction &);
    GameResult getGameResult(int week, GameInfo &gi );

    MyFantasyName importMnemonic(string in);
    fc::optional<BlockHeader> mLastBlock;
    GameStatsLoader gameloader;
};

#endif // DATAAGENT_H
