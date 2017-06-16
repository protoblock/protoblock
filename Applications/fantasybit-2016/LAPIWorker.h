#ifndef LAPIWORKER_H
#define LAPIWORKER_H

#include "globals.h"
#include <QTimer.h>
#include "threadedqobject.h"
#include "NodeWorker.h"
#include "FantasyAgent.h"
#include "Processor.h"
#include "NameData.h"
#include "Data.h"
#include "NameData.pb.h"
#include "StaticData.pb.h"
#include "StatusData.pb.h"
#include "ProtoData.pb.h"
#include <vector>
#include <mutex>
#include "iresolvable.h"
#ifdef USE_PB_GATEWAYS
#endif
#include "pbgateways.h"
using fantasybit::GlobalState;
using namespace fantasybit;
using namespace std;
class MainLAPIWorker : public QObject , public IResolvable
        , public pb::IPBGateway

{
    Q_OBJECT
    Q_INTERFACES(pb::IPBGateway)

    int intervalmax = 30000;
    int intervalstart = 500;
    int bcount =0;
    int pcount =0;
    int count =0;
    int32_t last_block=0;
#ifndef NOSYNC
    NodeWorker *myNodeWorker;
#endif
    int32_t numto = std::numeric_limits<int32_t>::max();
    bool amlive = false;
    bool quitting = false;
    QTimer * timer;
    fantasybit::FantasyAgent agent{};
    fantasybit::NFLStateData data;
    fantasybit::FantasyNameData namedata;
    fantasybit::BlockProcessor processor;

    fantasybit::ExchangeData exchangedata;

    //fantasybit::DeltaData deltadata{};
    std::map<std::string,fantasybit::MyFantasyName> myfantasynames{};
    fantasybit::MyFantasyName myCurrentName{};

    void DoPostTx(SignedTransaction &st);
    void DoPostTr(SignedTransaction &st);

    std::recursive_mutex last_mutex{};

#ifdef TESTING_PRE_ROW_TRADE_FEATURE
    bool justwentlive = false;
#endif
public:
    MainLAPIWorker(QObject * parent=0);
    ~MainLAPIWorker(){}
#ifndef NOSYNC
    ThreadedQObject<NodeWorker> node;
#endif

    fantasybit::NFLStateData &NFLState() { return data; }

    fantasybit::FantasyNameData &NameData() { return namedata; }

    fantasybit::FantasyAgent &Agent() { return agent; }

    fantasybit::ExchangeData &ExData() { return exchangedata; }

    /*
    std::vector<fantasybit::GameRoster> getWeekGameRosters(int week){
        std::vector<fantasybit::GameRoster> vector;
        vector.push_back(fantasybit::GameRoster());
        return vector;
    }
    */
    void GoLive();

signals:

    //void OnData(const fantasybit::DeltaData &);
    //void sendNotificationWithData(const QVariant & notificationData);
    //sync blocks
    void ProcessNext();
    void GetNext();

    //to data
    //void SubscribeLive();
    void LiveData(bool);


    //to GUI
    void NameStatus(fantasybit::MyFantasyName);
    void LiveProj(fantasybit::FantasyBitProj);
    void MyNames(vector<fantasybit::MyFantasyName>);
    void NameBal(fantasybit::FantasyNameBal);
    void PlayerStatusChange(pair<string,fantasybit::PlayerStatus> in);
    void GlobalStateChange(fantasybit::GlobalState);
    void LiveGui(fantasybit::GlobalState);
    void NewWeek(int);
    void NewSeason(int);
    void GameStart(string);
    void GameOver(string);
    void onControlMessage(QString);

    void BlockError(int32_t last);

    void Height(int);
    void BlockNum(int);

    void NewFantasyName(fantasybit::FantasyNameBal);

    void AnyFantasyNameBalance(fantasybit::FantasyNameBal);

    void FinishedResults();


public slots:

    //void getLivePlayers(int );
    //void processGUIRequest(const QVariant & requestData);

    void startPoint();

    void OnInSync(int32_t num);
    void ProcessBlock();
    void OnSeenBlock(int32_t num);
    void Timer();
    void OnBlockError(int32_t last);
    void ResetIndex();

    //void OnPlayerChange(std::string);

    //from gui
    void OnGetMyNames();
    void OnUseName(QString);

    //tx
    void OnFoundName(string);
    void OnProjLive(fantasybit::FantasyBitProj);
    void OnClaimName(QString);
    void OnProjTX(vector<fantasybit::FantasyBitProj>);

    void OnNewOrder(fantasybit::ExchangeOrder);

    void Quit();
    //data
    //void OnGlobalStateChange(fantasybit::GlobalState);
    //void OnNameBal(fantasybit::FantasyNameBal);
    //void OnPlayerStatusChange(pair<string,fantasybit::PlayerStatus>);

    //dataagent
#ifdef DATAAGENT
    void BeOracle();
#endif
private:
    void DoSubscribe(const string &name, bool suborun);

    bool Process(fantasybit::Block &b);
    bool doProcessBlock();
    string m_txstr;
    //void doNewDelta();

};


#endif // LAPIWORKER_H
