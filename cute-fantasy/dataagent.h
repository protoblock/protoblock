#ifndef DATAAGENT_H
#define DATAAGENT_H

#include <QMutex>
#include "genericsingleton.h"
#include "core.h"
#include "LAPIWorker.h"

class dataagent : public GenericSingleton<dataagent>  {
    friend class GenericSingleton<dataagent>;
    dataagent(){}
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

    fc::optional<BlockHeader> mLastBlock;

};

#endif // DATAAGENT_H
