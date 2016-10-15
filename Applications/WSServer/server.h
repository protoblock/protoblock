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

#ifdef TESTING_ONLY
    QTimer testGameStart;
    vector<GameData> holdGameData;
#endif

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

    ProjByName *avgProjByName, zeroProjByName;
    std::unordered_map<std::string,ProjByName *> mProjByNames;
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
    void GlobalStateChange(fantasybit::GlobalState);
    void NewWeek(int);
    void GameStart(string);
    void GameOver(string);
    void onControlMessage(QString);
    */

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

            AddNames(FantasyName::toFantasyNameBal(*fnptr));
            emit onNewProj(name);
        }
        blocknames.clear();
        currblock = n;

    }

#ifdef TESTING_ONLY
    void setTestGameStart() {
        for ( auto &it : holdGameData) {
            if ( it.status().status() == GameStatus_Status_SCHEDULED) {
                it.mutable_status()->set_status(GameStatus_Status_INGAME);
                emit GameStart(it.gameid());
                return;
            }
        }

//        emit WeekOver(6);
    }

#endif


signals:
    void onNewProj(const std::string &name );
    void GameStart(string);

};


#endif //SERVER_H
