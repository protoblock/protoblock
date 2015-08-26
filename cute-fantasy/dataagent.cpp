#include "dataagent.h"

using namespace fantasybit;


SignedTransaction dataagent::signTx(Transaction &tx) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->Agent().makeSigned(tx);
}

Block dataagent::makeNewBlockAsDataAgent(const SignedTransaction &st) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    Block b = worker->Agent().makeNewBlockAsDataAgent(st,mLastBlock);
    mLastBlock = b.signedhead().head();
    return b;
}

