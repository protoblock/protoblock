#ifndef FULLGATEWAY_H
#define FULLGATEWAY_H

#include "pbgateways.h"
#include "LAPIWorker.h"
#include "dataservice.h"
#include "mediator.h"
#include "Commissioner.h"
#include "playerquoteslicemodel.h"

namespace pb {
using namespace std;

using namespace fantasybit;

class FullGateway : public QObject, public IPBGateway, public ITradingProxy {
    Q_OBJECT
    Q_INTERFACES(pb::IPBGateway)
    Q_INTERFACES(pb::ITradingProxy)

    IPBGateway *gatepass;
    IDataService *datapass;
    MainLAPIWorker *mlapi;
    DataService *dataservice;

public:
    FullGateway //(IPBGateway *gatein, IDataService *datain) :
                (MainLAPIWorker *immlapi, DataService *datain) :
                                    mlapi(immlapi),
                                    dataservice(datain),
                                    gatepass{qobject_cast<pb::IPBGateway *>(immlapi)},
                                    datapass{datain} ,
                                    m_PlayerQuoteSliceModel{} {

//        mlapi->dataService = dataservice;
        this->dataService = datain;
        this->tradingProxy = this;
        connect(mlapi, &MainLAPIWorker::PlayName,
                this, [this](string inname) {
            qDebug() << " FullGateway PlayName" << inname.data();
            emit PlayName(inname);
        });

        connect(mlapi, &MainLAPIWorker::NameStatus,
                this, [this](fantasybit::MyFantasyName inname) {
            qDebug() << " FullGateway namestatus" << inname.DebugString().data();
            myName = inname;
            emit NameStatus(inname);
        });


        connect( mlapi, &MainLAPIWorker::LiveProj,
                this, &FullGateway::LiveProj);

        connect( mlapi, &MainLAPIWorker::MyNames,
                this,  [this](vector<fantasybit::MyFantasyName> mynames) {
            m_mynames = mynames;
            if ( heslive )
                emit MyNames(mynames);

            for ( auto name : m_mynames) {
                 std::shared_ptr<FantasyName> &s = mynamebal[name.name()];
                 if ( s ) continue;
                 s = fantasybit::Commissioner::getName(name.name());
                 if ( !s ) continue;
//                 if ( myName.name() == "" ) {
//                     myName.set_name(s->alias());
//                     myName.set_status(MyNameStatus::confirmed);
//                     emit OnUseName(myName.name().data());
//                 }
                 FantasyNameBal fnb;
                 fnb.set_name(s->alias());
                 fnb.set_public_key(fantasybit::Commissioner::pk2str(s->pubkey()));
                 fnb.set_bits(s->getBalance());
                 fnb.set_chash(s->hash());
                 if ( heslive )
                    emit NameBal(fnb);
                 else
                    holdfresh.push_back(fnb);
            }
        });

        connect( mlapi, &MainLAPIWorker::LiveGui,
                this,   &FullGateway::OnLiveGui);

        connect( mlapi, &MainLAPIWorker::NameBal,
                this,   &FullGateway::NameBal);

        connect( mlapi, &MainLAPIWorker::PlayerStatusChange,
                this,   &FullGateway::PlayerStatusChange);

        connect( mlapi, &MainLAPIWorker::GlobalStateChange,
                this,   &FullGateway::GlobalStateChange);

        connect( mlapi, &MainLAPIWorker::NewWeek,
                this,   &FullGateway::NewWeek);

        connect( mlapi, &MainLAPIWorker::NewSeason,
                Mediator::instance(),   &Mediator::NewSeason);

        connect( mlapi, &MainLAPIWorker::GameStart,
                this,   &FullGateway::GameStart);

        connect( mlapi, &MainLAPIWorker::GameOver,
                this,   &FullGateway::GameOver);

        connect( mlapi, &MainLAPIWorker::onControlMessage,
                this,   &FullGateway::onControlMessage);

        connect( mlapi, &MainLAPIWorker::NewFantasyName,
                this,   &FullGateway::NewFantasyName);

        connect( mlapi, &MainLAPIWorker::AnyFantasyNameBalance,
                this,   &FullGateway::AnyFantasyNameBalance);

        connect( mlapi, &MainLAPIWorker::Height,
                this,   &FullGateway::Height);


        connect( mlapi, &MainLAPIWorker::BlockNum,
                this,   &FullGateway::BlockNum);

        connect( mlapi, &MainLAPIWorker::FinishedResults,
                this,   &FullGateway::FinishedResults);


//        connect( mlapi, SIGNAL(  GlobalStateChange(fantasybit::GlobalState)  ),
//                this,      SLOT(    GlobalStateChange(fantasybit::GlobalState)        ));

//        connect( mlapi, SIGNAL   ( LiveGui(fantasybit::GlobalState)     ),
//                this,      SLOT     (  OnLiveGui(fantasybit::GlobalState)     ));
//        connect( mlapi, SIGNAL   (  NewWeek(int)    ),
//                this,      SLOT     (  NewWeek(int)     ));
//        connect( mlapi, SIGNAL   (  GameStart(string)    ),
//                this,      SLOT     (  GameStart(string)     ));
//        connect( mlapi, SIGNAL   (  GameOver(string)    ),
//                this,      SLOT     (  GameOver(string)     ));
//        connect( mlapi, SIGNAL   (  onControlMessage(QString)    ),
//                this,      SLOT     (  onControlMessage(QString)     ));

//        QObject::connect(this,SIGNAL(OnClaimName(QString)),mlapi,SLOT(OnClaimName(QString)));

        connect( this,   &FullGateway::doOnClaimName,
                 mlapi, &MainLAPIWorker::OnClaimName);

        connect( this, &FullGateway::OnUseName,
                 mlapi, &MainLAPIWorker::OnUseName);

        connect(Mediator::instance(),&Mediator::ready,this, &FullGateway::ClientReady);
//                [this](){101
//                       if ( this->amLive ) {
//                           emit LiveGui(m_gs);
//                       }

//                       if ( !m_mynames.empty())
//                            emit MyNames(m_mynames);
//                       for( auto &v : holdfresh)
//                            emit NameBal(v);
//                       holdfresh.clear();

//                       heslive = true;
//                 });

//        ExchangeData *exchangedata = &(mlapi->ExData());

//        //trading
//        QObject::connect(exchangedata,SIGNAL(NewMarketSnapShot(fantasybit::MarketSnapshot*)),
//                          this,SLOT(OnMarketSnapShot(fantasybit::MarketSnapshot*)));

//        QObject::connect(exchangedata,SIGNAL(NewMarketTicker(fantasybit::MarketTicker *)),
//                          Mediator::instance(),SLOT(OnMarketTicker(fantasybit::MarketTicker *)));

//        QObject::connect(exchangedata,SIGNAL(NewTradeTic(fantasybit::TradeTic*)),
//                          this,SLOT(OnTradeTick(fantasybit::TradeTic*)));

//        QObject::connect(exchangedata,SIGNAL(NewDepthDelta(fantasybit::DepthFeedDelta*)),
//                          this,SLOT(OnDepthDelta(fantasybit::DepthFeedDelta*)));

//        QObject::connect(exchangedata,
//                         SIGNAL(NewOO(fantasybit::FullOrderDelta)),
//                          this,SLOT(OnNewOO(fantasybit::FullOrderDelta)));

//        QObject::connect(exchangedata,
//                         SIGNAL(NewPos(fantasybit::FullPosition)),
//                          this,SLOT(OnNewPos(fantasybit::FullPosition)));

//        connect(exchangedata,&ExchangeData::FinishMarketSnapShot,
//                this, &FullGateway::OnFinishMarketSnapShot);

//        connect(exchangedata,&ExchangeData::StartMarketSnapShot,
//                this, &FullGateway::OnStartMarketSnapShot);

        QObject::connect(Mediator::instance(),SIGNAL(NewHeightStop(int)),mlapi,SLOT(OnSeenBlock(int32_t)));

    }

signals:
    void PlayName(string);
    void NameStatus(fantasybit::MyFantasyName);
    void LiveProj(fantasybit::FantasyBitProj);
    void MyNames(vector<fantasybit::MyFantasyName>);
    void NameBal(fantasybit::FantasyNameBal);
    void PlayerStatusChange(pair<string,fantasybit::PlayerStatus>);
    void GlobalStateChange(fantasybit::GlobalState);
    void LiveGui(fantasybit::GlobalState);
    void NewWeek(int);
    void GameStart(string);
    void GameOver(string);
    void onControlMessage(QString);
    void OnUseName(QString);
    void doOnClaimName(QString);

    void nameAvail(QString &, bool);

    void NewFantasyName(fantasybit::FantasyNameBal);
    void AnyFantasyNameBalance(fantasybit::FantasyNameBal);
    void Height(int);
    void BlockNum(int);
    void FinishedResults();
    //trading
    void GotMarketSnaps();

public slots:
    void OnLiveGui(fantasybit::GlobalState gs) {
        qDebug() << "FullGateway received Livegui ";
        amLive = true;
        m_gs = gs;
        if ( heslive ) {
            initBothLive();
            emit LiveGui(gs);
            if ( !holdfresh.empty() ) {
                if ( !m_mynames.empty())
                     emit MyNames(m_mynames);
                for( auto &v : holdfresh)
                     emit NameBal(v);
                holdfresh.clear();
            }

            if ( gotAllSnaps )
                emit GotMarketSnaps();
        }
        else
            emit LiveGui(gs);

    }

    void OnClaimName(QString s) {
        emit doOnClaimName(s);
    }

    void nameCheck(QString s) {
        emit nameAvail(s,Commissioner::isAliasAvailable(s.toStdString()));
    }

    void OnGetMyNames() {}
    void UseName(QString s) {
        emit OnUseName(s);
    }

    void initBothLive() {
        connect(Mediator::instance(),&Mediator::NewProjection,
                mlapi,&MainLAPIWorker::OnProjTX);

        connect(Mediator::instance(),&Mediator::NewOrder,
                mlapi,&MainLAPIWorker::OnNewOrder);

        connect(Mediator::instance(),&Mediator::NewTransfer,
                mlapi,&MainLAPIWorker::OnNewTransfer);

        connect(Mediator::instance(),&Mediator::NewSwapAsk,
                mlapi,&MainLAPIWorker::OnNewSwapAsk);

        connect(Mediator::instance(),&Mediator::NewSwapBid,
                mlapi,&MainLAPIWorker::OnNewSwapBid);

        connect(Mediator::instance(),&Mediator::NewSwapSent,
                mlapi,&MainLAPIWorker::OnNewSwapSent);

        connect(Mediator::instance(),&Mediator::NewSwapFill,
                mlapi,&MainLAPIWorker::OnNewSwapFill);

        connect(Mediator::instance(),&Mediator::NewProofOfDoubleSpend,
                mlapi,&MainLAPIWorker::OnNewProofOfDoubleSpend);



        Mediator *med = Mediator::instance();
        ExchangeData *exchangedata = &(mlapi->ExData());
        SwapStateData *swapdata = &(mlapi->SwapState());

        //trading
        QObject::connect(exchangedata,SIGNAL(NewMarketTicker(fantasybit::MarketTicker,int32_t)),
                          med,SLOT(OnMarketTicker(fantasybit::MarketTicker,int32_t)));

        QObject::connect(exchangedata,SIGNAL(NewTradeTic(fantasybit::TradeTic*)),
                          med,SLOT(OnTradeTick(fantasybit::TradeTic*)));

        QObject::connect(exchangedata,SIGNAL(NewDepthDelta(fantasybit::DepthFeedDelta*)),
                          med,SLOT(OnDepthDelta(fantasybit::DepthFeedDelta*)));

        QObject::connect(exchangedata,
                         SIGNAL(NewOO(fantasybit::FullOrderDelta)),
                          med,SLOT(OnNewOO(fantasybit::FullOrderDelta)));

        QObject::connect(exchangedata,
                         SIGNAL(NewPos(fantasybit::FullPosition)),
                          med,SLOT(OnNewPos(fantasybit::FullPosition)));

        QObject::connect(swapdata,SIGNAL(NewSwapData(fantasybit::SwapOrder)),
                          med,SLOT(OnSwapData(fantasybit::SwapOrder)));

    }

    void ClientReady() {
        heslive = true;
        if ( amLive ) {
            initBothLive();

            emit LiveGui(m_gs);
            if ( gotAllSnaps )
                emit GotMarketSnaps();
        }

        if ( !holdfresh.empty() ) {
            if ( !m_mynames.empty())
                 emit MyNames(m_mynames);
            for( auto &v : holdfresh)
                 emit NameBal(v);
            holdfresh.clear();
        }
    }

/**** trading
 *
 *
 * /
 */
//    void OnMarketTicker(fantasybit::MarketTicker *);
//    void OnMarketSnapShot(fantasybit::MarketSnapshot*);
//    void OnDepthDelta(fantasybit::DepthFeedDelta*);
//    void OnTradeTick(fantasybit::TradeTic*);
//    void OnMyNewOrder(fantasybit::Order& ord);
//    void OnNewPos(fantasybit::FullPosition);
//    void OnNewOO(fantasybit::FullOrderDelta);

    void OnFinishMarketSnapShot(int week) {
        gotAllSnaps = true;
        if ( heslive && amLive )
            emit GotMarketSnaps();
    }

    void OnStartMarketSnapShot(int week) {
        gotAllSnaps = false;
        m_PlayerQuoteSliceModel.initWeek(week);
        m_PlayerQuoteSliceModel.clear();
    }

private:
    bool amLive = false, heslive = false;
    fantasybit::GlobalState m_gs;
    vector<fantasybit::MyFantasyName> m_mynames;
    std::unordered_map<std::string, std::shared_ptr<FantasyName>> mynamebal;
    vector<FantasyNameBal> holdfresh;
    MyFantasyName myName;

    bool gotAllSnaps = false;
public:
    PlayerQuoteSliceModel & GetPlayerQuoteSliceModel() {
        return  m_PlayerQuoteSliceModel;
   }

    PlayerQuoteSliceModel m_PlayerQuoteSliceModel;

//    ThreadedQObject<BitcoinWalletWorker> myBitcoinWalletThread;


    /*
    std::unordered_map<std::string,PlayerDetail>
        DataService::GetTeamRoster(const std::string &teamid){
        MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
        return worker->NFLState().GetTeamRoster(teamid);
    }

    std::vector<fantasybit::GameRoster> GetCurrentWeekGameRosters(){
        return dataservice->GetCurrentWeekGameRosters();
    }

    std::vector<fantasybit::GameResult> GetPrevWeekGameResults(int week){
        //QMutexLocker(&DataService::instance()->myMutex);
        MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
        return dataservice->GetPrevWeekGameResults(week);
    }

    fantasybit::WeeklySchedule DataService::GetWeeklySchedule(int week) {
    //QMutexLocker(&DataService::instance()->myMutex);
        MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
        return worker->NFLState().GetWeeklySchedule(week);
    }

    fantasybit::PlayerBase DataService::GetPlayerBase(std::string playerId) {
        MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
        return worker->NFLState().GetPlayerBase(playerId);
    }


    fantasybit::GameStatus DataService::GetGameStatus(string gid) {
        MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
        return worker->NFLState().GetUpdatedGameStatus(gid);
    }


    std::vector<std::shared_ptr<fantasybit::FantasyName>> DataService::GetLeaderBoard() {
        auto v = Commissioner::GetFantasyNames();

        std::sort(v.begin(), v.end(),
            [](shared_ptr<FantasyName> f1, shared_ptr<FantasyName> f2){
                return (f2->getBalance() == f1->getBalance()) ? (f1->hash() < f2->hash() ) :
                                                           (f2->getBalance() < f1->getBalance());
            });

        return v;
    }

    // curent week
    unordered_map<std::string,int> DataService::GetProjByName(const std::string &fname) {
        MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
        return worker->NameData().GetProjByName(fname);
    }

    //current week
    unordered_map<std::string,int> DataService::GetProjById(const std::string &pid) {
        MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
        return worker->NameData().GetProjById(pid);
    }

    fantasybit::GlobalState DataService::GetGlobalState() {
        MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
        return worker->NFLState().GetGlobalState();
    }


    MyFantasyName DataService::importMnemonic(std::string &in) {
        MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
        return worker->Agent().UseMnemonic(in);
    }

    string DataService::exportMnemonic(std::string &in) {
        MainLAPIWorker* worker = Core::resolveByName<MainLAPIWorker>("coreapi");
        return worker->Agent().getMnemonic(in);
    }
    */
};

}
#endif // FULLGATEWAY_H
