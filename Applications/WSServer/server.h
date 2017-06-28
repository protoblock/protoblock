#ifndef SERVER_H
#define SERVER_H

#include "StateData.pb.h"
//#include "TxServer.h"
#include "ExchangeData.h"
#include "Data.h"
#include "pbgateways.h"
#include "dataservice.h"
#include "Commissioner.h"
#include <unordered_set>
#include <stack>

namespace fantasybit {

struct fnameptrs {
    fnameptrs(AllOdersFname *ao = nullptr) : fnameAllOdersFname(ao) {}
    AllOdersFname *fnameAllOdersFname;
    std::unordered_map<int32_t,Order *> mSeqOrderMap;
    std::unordered_map<string, AllOdersSymbol *>    fnamesymbolAllOrders;
    std::unordered_map< AllOdersSymbol *, std::stack<Order *>>  openOrderSlots;
    std::stack<AllOdersSymbol *> openOrderSymbolSlot;
};

class Server : public QObject {
    Q_OBJECT

    explicit Server(QObject *parent = 0) :  QObject(parent) {}

    static Server *myInstance;
    void setupConnection(pb::IPBGateway *ingateway);
    void initData();

    void resetRosterProjScheduleData();

    pb::IPBGateway *mGateway = nullptr;
    QObject *mOGateway;
    bool amLive = false;

    std::unordered_map<std::string,int> mPlayerId2Index;
    std::unordered_set<std::string> blocknames;
    std::unordered_set<std::string> posnames;
    std::unordered_set<std::string> depthsymbols;

    int currblock = 0;

public:
    static Server *instance();

    void init();
    void setContext(pb::IPBGateway *ingateway) {
        qDebug() << " setContext " << bool(ingateway == nullptr);
        mGateway = ingateway;
        setupConnection(mGateway);
        qDebug() << " setContext2 " << bool(mGateway == nullptr);
    }






public:
    WSReply mGetCurrRostersReply;
    fantasybit::GetAllNamesRep mAllNamesRep;
    fantasybit::GetCurrRostersRep *mGetCurrRostersRep;
    QByteArray mGetCurrRostersRepStrBytesWSReply;
    std::string mGetCurrRostersRepStrWSreply;

    GetGlobalStateRep GlobalStateRep;
    GetScheduleRep ScheduleRep;

    ProjByName *avgProjByName, zeroProjByName;
    std::unordered_map<std::string,ProjByName *> mProjByNames;

    std::unordered_map<std::string,FantasyNameBal *> mPk2Bal;

    ROWMarket *getRowmarket(const std::string &pid);
    GetDepthRep * getDepthRep(const std::string &playerid);

    fantasybit::GetROWMarketRep mROWMarketRep;

    fnameptrs &getfnameptrs(const std::string &fname, bool clean = false);

    void cleanIt(fnameptrs &fptr) {
    //    for ( auto &all : fptr.fnamesymbolAllOrders ) {
    //        all.second->orders.clear();
    //    }
        fptr.fnameAllOdersFname->clear_pidorders();
        fptr.mSeqOrderMap.clear();
        fptr.fnamesymbolAllOrders.clear();
        fptr.openOrderSlots.clear();
        fptr.openOrderSymbolSlot = std::stack<AllOdersSymbol *> {};
    }

    void cleanIt(const std::string &fname) {
        auto it = fnameptrsmap.find(fname);
        if ( it != end(fnameptrsmap))
            cleanIt(it->second);
    }

    void UnSubscribe(const std::string &fname) {
        qDebug() << " unsubscribe " << fname.data();
        emit DoSubscribe(fname,false);
        cleanIt(fname);
    }

    void Subscribe(const std::string &fname) {
        qDebug() << " subscribe " << fname.data();
        emit DoSubscribe(fname,true);
        getFnameSnap(fname);
    }


    AllOdersSymbol *getAllOdersSymbol(fnameptrs &fptr, const std::string &symbol);
    Order *addOrder(fnameptrs &fptr, AllOdersSymbol *allords, const Order &orderin);
    void ProcessMarketSnapShot(fantasybit::MarketSnapshot *mt);
private:

//    static fantasybit::GetROWMarketRep ROWMarketRep;
//    static NFLStateData  NFLData;
    void getFnameSnap(const std::string &fname);

    std::unordered_map<std::string, ROWMarket *> mPidROWMarket;
    std::unordered_map<std::string, GetDepthRep *> mPidGetDepthRep;

    std::unordered_map<std::string,int> mPk2Index;

    std::unordered_map< std::string, fnameptrs>  fnameptrsmap;

    void AddNames(const FantasyNameBal &fnb) {
//        qDebug() << " server::addnames " << fnb.DebugString().data();
        FantasyNameBal *p;
        auto it = mPk2Index.find(fnb.public_key());
        if ( it == end(mPk2Index)) {
            p = mAllNamesRep.add_fnb();
            mAllNamesRep.add_names(fnb.name());
            mPk2Index.insert({fnb.public_key(),mAllNamesRep.fnb_size()-1});
        }
        else
            p = mAllNamesRep.mutable_fnb(it->second);

        // = mAllNamesRep.add_fnb();
        p->CopyFrom(fnb);
        mPk2Bal.insert({fnb.public_key(),p});
        auto it2 = mProjByNames.find(fnb.name());
        if ( it2 == end(mProjByNames))
            mProjByNames.insert({fnb.name(),ProjByName::default_instance().New()});
        ProjByName *pbn = mProjByNames[fnb.name()];
        pbn->set_count(fnb.count());
        pbn->set_block(fnb.block());
        pbn->set_name(fnb.name());
    }

    void SaveProj(const FantasyBitProj &fbp) {
//        if ( fbp.block() > currblock) {
//            for ( auto name : blocknames) {
//                auto fnptr = Commissioner::getName(name);
//                if ( fnptr )
//                    AddNames(FantasyName::toFantasyNameBal(*fnptr));
//            }
//            blocknames.clear();
//            currblock = fbp.block();
//        }

        qDebug() << "SaveProj blocknames.insert" << fbp.name().data();
        blocknames.insert(fbp.name());
//        = {fbp.count(),fbp.block()};
    }

    void saveMarket(const std::string &symbol) {

    }

    void saveDepth(const std::string &symbol) {
        depthsymbols.insert(symbol);
    }

    void savePos(const std::string &fname) {
        posnames.insert(fname);
    }

    std::unordered_map<std::string,std::string> mSymb2Pid;

//    static void AddNames(FantasyNameBal *pFn) {
//        mAllNamesRep.add_names(pFn->name());
//        mAllNamesRep.mutable_fnb()->AddAllocated(pFn);
//        Pk2Bal.insert({pFn->public_key(),pFn});

//    }

//    bool goodPid(const std::string &pid) {
//        auto pb = DataService::instance()->GetPlayerBase(pid);
//        return pb.has_last() && pb.last() != "";
//    }


protected slots:
    void OnNewFantasyName(fantasybit::FantasyNameBal fnb) {
        AddNames(fnb);
    }

    void OnAnyFantasyNameBalance(fantasybit::FantasyNameBal fnb) {
//        AddNames(fnb);
        blocknames.insert(fnb.name());
        qDebug() << "OnAnyFantasyNameBalance blocknames.insert" << fnb.name().data();
    }

    void LiveGui(fantasybit::GlobalState gs);

    void LiveProj(fantasybit::FantasyBitProj);

    /*
    void NameStatus(fantasybit::MyFantasyName);
    void MyNames(vector<fantasybit::MyFantasyName>);
    void NameBal(fantasybit::FantasyNameBal);
    void PlayerStatusChange(pair<string,fantasybit::PlayerStatus> in);

    void NewWeek(int);
    void GameStart(string);
    void GameOver(string);
    void onControlMessage(QString);
    */

    void GlobalStateChange(fantasybit::GlobalState gs) {
        GlobalStateRep.mutable_globalstate()->CopyFrom(gs);
    }

    void NewWeek(int) {
        if ( amLive )
            initData();
    }

    void ResetData() { resetRosterProjScheduleData(); }

    void Height(int h) {
        qDebug() << " height " << h;
//        setheight(h);
    }

    void BlockNum(int n) {
        qDebug() << "slot BlockNum " << n;
//        setblocknum(n);
        if ( !amLive ) return;

        for ( auto name : blocknames) {
            auto fnptr = Commissioner::getName(name);
            if ( !fnptr ) continue;

            FantasyNameBal &fff = FantasyName::toFantasyNameBal(*fnptr);
            qDebug() << fff.DebugString().data();
            AddNames(fff);
            emit onNewProj(name);
        }
        blocknames.clear();

        if ( depthsymbols.size() > 0) {
            emit newRow();
            for ( const auto &symb : depthsymbols) {
                emit newDepth(symb);
            }
        }
        depthsymbols.clear();

        for ( const auto &name : posnames) {
            emit newPos(name);
        }
        posnames.clear();


        currblock = n;
    }

#ifdef TESTING_ONLY
    void setTestGameStart() {

        for ( auto &it : holdGameData) {
            if ( it.second.status().status() == GameStatus_Status_SCHEDULED) {
                it.second.mutable_status()->set_status(GameStatus_Status_INGAME);
                qDebug() << " in game start " << it.second.DebugString().data();
                emit GameStart(it.second.gameid());
                testGameStart.setInterval(20000);
                ResetData();
                return;
            }
        }

//        emit WeekOver(6);
    }

#endif


    void OnMarketTicker(fantasybit::MarketTicker,int32_t);
//    void OnMarketSnapShot(fantasybit::MarketSnapshot*);
    void OnDepthDelta(fantasybit::DepthFeedDelta*);
    void OnTradeTick(fantasybit::TradeTic*);
    void OnNewPos(fantasybit::FullPosition);
    void OnNewOO(fantasybit::FullOrderDelta);

signals:
    void onNewProj(const std::string &name );
    void GameStart(string);
    void GoLive();    
    void ready();
    void DoSubscribe(const string &name, bool suborun);
    void newDepth(const string &name );
    void newPos(const string &name );
    void newRow();
};


}
#endif //SERVER_H
/*
 *
 *
 *
class Server {
public:
    fantasybit::GetAllNamesRep mAllNamesRep;
    static fantasybit::ExchangeData TheExchange;
//    static fantasybit::GetROWMarketRep ROWMarketRep;
    static NFLStateData  NFLData;
    std::unordered_map<std::string,FantasyNameBal *> mPk2Bal;

    void AddNames(const FantasyNameBal &fnb) {
        mAllNamesRep.add_names(fnb.name());
        FantasyNameBal *p = Server::AllNamesRep.add_fnb();
        p->CopyFrom(fnb);
        Pk2Bal.insert({fnb.public_key(),p});
    }

    void AddNames(FantasyNameBal *pFn) {
        Server::AllNamesRep.add_names(pFn->name());
        Server::AllNamesRep.mutable_fnb()->AddAllocated(pFn);
        Pk2Bal.insert({pFn->public_key(),pFn});

    }

    static bool goodPid(const std::string &pid) {
        auto pb = NFLData.GetPlayerBase(pid);
        return pb.has_last() && pb.last() != "";
    }

    static std::unordered_map<std::string,FantasyNameBal> myNewNames;

};



 */
