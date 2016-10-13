#ifndef SERVER_H
#define SERVER_H

#include "StateData.pb.h"
//#include "TxServer.h"
#include "LiteServer.h"
#include "ExchangeData.h"
#include "Data.h"
#include "pbgateways.h"
#include "dataservice.h"
#include "Commissioner.h"
#include <unordered_set>

using namespace fantasybit;

class Server : public QObject {
    Q_OBJECT

    explicit Server(QObject *parent = 0) :  QObject(parent) {}

    static Server *myInstance;
    void setupConnection(pb::IPBGateway *ingateway);
    void initData();
public:
    static Server *instance();
    WSReply mGetCurrRostersReply;
    fantasybit::GetAllNamesRep mAllNamesRep;
    fantasybit::GetCurrRostersRep *mGetCurrRostersRep;
    QByteArray mGetCurrRostersRepStrBytesWSReply;
    std::string mGetCurrRostersRepStrWSreply;

    static fantasybit::ExchangeData TheExchange;

    GetGlobalStateRep GlobalStateRep;
    GetScheduleRep ScheduleRep;

    ProjByName avgProjByName, zeroProjByName;
    std::unordered_map<std::string,ProjByName> mProjByNames;
    std::unordered_map<std::string,int> mPlayerId2Index;

//    std::unordered_map<std::string,std::pair<int,int>> mFname2BlockCount;
    std::unordered_set<std::string> blocknames;
    int currblock = 0;


//    static fantasybit::GetROWMarketRep ROWMarketRep;
//    static NFLStateData  NFLData;
    std::unordered_map<std::string,FantasyNameBal *> mPk2Bal;
    std::unordered_map<std::string,int> mPk2Index;


    void AddNames(const FantasyNameBal &fnb) {
//        qDebug() << " server::addnames " << fnb.DebugString().data();
        FantasyNameBal *p;
        auto it = mPk2Index.find(fnb.public_key());
        if ( it == end(mPk2Index)) {
            p = mAllNamesRep.add_fnb();
            mAllNamesRep.add_names(fnb.name());
        }
        else
            p = mAllNamesRep.mutable_fnb(it->second);

        // = mAllNamesRep.add_fnb();
        p->CopyFrom(fnb);
        mPk2Bal.insert({fnb.public_key(),p});
        ProjByName &pbn = mProjByNames[fnb.name()];
        pbn.set_count(fnb.count());
        pbn.set_block(fnb.block());
        pbn.set_name(fnb.name());
    }

    void SaveProj(const FantasyBitProj &fbp) {
        if ( fbp.block() > currblock) {
            for ( auto name : blocknames) {
                auto fnptr = Commissioner::getName(name);
                if ( fnptr )
                    AddNames(FantasyName::toFantasyNameBal(*fnptr));
            }
            blocknames.clear();
            currblock = fbp.block();
        }

        blocknames.insert(fbp.name());
//        = {fbp.count(),fbp.block()};
    }


//    static void AddNames(FantasyNameBal *pFn) {
//        mAllNamesRep.add_names(pFn->name());
//        mAllNamesRep.mutable_fnb()->AddAllocated(pFn);
//        Pk2Bal.insert({pFn->public_key(),pFn});

//    }

    static bool goodPid(const std::string &pid) {
        auto pb = DataService::instance()->GetPlayerBase(pid);
        return pb.has_last() && pb.last() != "";
    }

    pb::IPBGateway *mGateway = nullptr;
    void setContext(pb::IPBGateway *ingateway) {
        mGateway = ingateway;
        setupConnection(mGateway);
    }

    bool amLive = false;
protected slots:
    void OnNewFantasyName(fantasybit::FantasyNameBal fnb) {
        AddNames(fnb);
    }

    void OnAnyFantasyNameBalance(fantasybit::FantasyNameBal fnb) {
        AddNames(fnb);
    }

    void LiveGui(fantasybit::GlobalState gs);

    void LiveProj(fantasybit::FantasyBitProj);

    /*
    void NameStatus(fantasybit::MyFantasyName);
    void MyNames(vector<fantasybit::MyFantasyName>);
    void NameBal(fantasybit::FantasyNameBal);
    void PlayerStatusChange(pair<string,fantasybit::PlayerStatus> in);
    void GlobalStateChange(fantasybit::GlobalState);
    void NewWeek(int);
    void GameStart(string);
    void GameOver(string);
    void onControlMessage(QString);
    */
};


#endif //SERVER_H
