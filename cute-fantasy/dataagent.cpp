#include "dataagent.h"

using namespace fantasybit;


SignedTransaction dataagent::signTx(Transaction &tx) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->Agent().makeSigned(tx);
}

fc::optional<Block> dataagent::makeNewBlockAsDataAgent(Transaction &st) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    auto b = worker->Agent().makeNewBlockAsDataAgent(st,mLastBlock);
    if (b)
        mLastBlock = (*b).signedhead().head();
    return b;
}

GameResult dataagent::getGameResult(int week, GameInfo &gi ) {
    GameResult gr{};
    vector<GameInfo> v = { gi };
    auto vg = gameloader.loadGameStatsFromTradeRadar(week,v);
    if ( vg.size() > 0 )
        gr = vg[0];

    return gr;

}

MyFantasyName dataagent::importMnemonic(string in) {
    MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
    return worker->Agent().UseMnemonic(in);

}

