#include "dataagent.h"

using namespace fantasybit;


SignedTransaction dataagent::signTx(Transaction &tx) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->Agent().makeSigned(tx);
}

Block dataagent::makeNewBlockAsDataAgent(const SignedTransaction &st) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->Agent().makeNewBlockAsDataAgent(st);
}

