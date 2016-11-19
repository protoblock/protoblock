#include "LAPIWorker.h"


#include <QWaitCondition>
#include "RestFullCall.h"
#include "globals.h"
#include <algorithm>
#include "ProtoData.pb.h"
#include "core.h"
#include <vector>
#include "Commissioner.h"


using namespace std;
using namespace fantasybit;

MainLAPIWorker::MainLAPIWorker(QObject * parent):  QObject(parent),
    data{}, namedata{}, exchangedata{}, processor(data,namedata, exchangedata)
{
    timer = new QTimer(this);
#ifndef NOSYNC
    node.thread()->connect(node.thread(),
                           SIGNAL(started()),
                           node.object(),
                           SLOT(init()));
    myNodeWorker = node.object();


    //QObject::connect(this,SIGNAL(Timer()),myNodeWorker,SLOT(TryNext()));
    //QObject::connect(this,SIGNAL(GetNext()),qApp,SLOT(aboutQt()));
    //QObject::connect(timer,SIGNAL(timeout()),myNodeWorker,SLOT(Timer()));


    //block sync
    QObject::connect(myNodeWorker,SIGNAL(InSync(int32_t)),this,SLOT(OnInSync(int32_t)));
    QObject::connect(myNodeWorker,SIGNAL(SeenBlock(int32_t)),this,SLOT(OnSeenBlock(int32_t)));
    QObject::connect(myNodeWorker,SIGNAL(BlockError(int32_t)),this,SLOT(OnBlockError(int32_t)));
    QObject::connect(myNodeWorker,SIGNAL(ResetIndex()),this,SLOT(ResetIndex()));

    QObject::connect(this,SIGNAL(GetNext()),myNodeWorker,SLOT(TryNext()));
#endif

    QObject::connect(this,SIGNAL(ProcessNext()),this,SLOT(ProcessBlock()));
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(Timer()));


    //data processing
    QObject::connect(this,SIGNAL(LiveData(bool)),&exchangedata,SLOT(OnLive(bool)));
    QObject::connect(this,SIGNAL(LiveData(bool)),&data,SLOT(OnLive(bool)));
    QObject::connect(this,SIGNAL(LiveData(bool)),&namedata,SLOT(OnLive(bool)));
    QObject::connect(this,SIGNAL(LiveData(bool)),&processor,SLOT(OnLive(bool)));

    /*
    QObject::connect(&exchangedata,SIGNAL(NewFantasyNameOrder(fantasybit::Order&)),
                     &namedata,SLOT(NewFantasyNameOrder(fantasybit::Order&)));
    */

    //data to data signals
    QObject::connect(&processor,SIGNAL(WeekStart(int)),this,SIGNAL(NewWeek(int)));

    //QObject::connect(&data,SIGNAL(NewGameResult(string)),this,SIGNAL(GameOver(string)));


    //delt data
    QObject::connect(&data,SIGNAL(PlayerStatusChange(pair<string,fantasybit::PlayerStatus>)),
                     this,SIGNAL(PlayerStatusChange(pair<string,fantasybit::PlayerStatus>)));

    QObject::connect(&data,SIGNAL(GameStart(string)),
                     this,SIGNAL(GameStart(string)));

    //QObject::connect(&data,SIGNAL(GlobalStateChange(GlobalState)),
    //                 this,SLOT(OnGlobalStateChange(GlobalState)));

    QObject::connect(&data,SIGNAL(GlobalStateChange(fantasybit::GlobalState)),
                     this,SIGNAL(GlobalStateChange(fantasybit::GlobalState)));

    QObject::connect(&namedata,SIGNAL(FantasyNameFound(string)),
                     this,SLOT(OnFoundName(string)));

    QObject::connect(&namedata,SIGNAL(ProjectionLive(fantasybit::FantasyBitProj)),
                     this,SLOT(OnProjLive(fantasybit::FantasyBitProj)));

    QObject::connect(&namedata,SIGNAL(FantasyNameBalance(fantasybit::FantasyNameBal)),
                     this,SIGNAL(NameBal(fantasybit::FantasyNameBal)));

    QObject::connect(&processor,SIGNAL(onControlMessage(QString)),
                     this,SIGNAL(onControlMessage(QString)));

    QObject::connect(&namedata,SIGNAL(FantasyNameFound(string)),
                     this,SLOT(OnFoundName(string)));

    QObject::connect(&namedata,SIGNAL(NewFantasyName(fantasybit::FantasyNameBal)),
                     this,SIGNAL(NewFantasyName(fantasybit::FantasyNameBal)));

    QObject::connect(&namedata,SIGNAL(AnyFantasyNameBalance(fantasybit::FantasyNameBal)),
                     this,SIGNAL(AnyFantasyNameBalance(fantasybit::FantasyNameBal)));

    connect(&processor,&BlockProcessor::FinishedResults,this, &MainLAPIWorker::FinishedResults);

}

void MainLAPIWorker::GoLive() {
    amlive = true;
    numto = 0;
    intervalstart = 1000;
#ifdef LIGHT_CLIENT_ONLY
    intervalstart = 500;
#endif


#ifdef TESTING_PRE_ROW_TRADE_FEATURE
    justwentlive = true;
#endif

    timer->start(intervalstart);

    qDebug() << "emit LiveData(true)";
    emit LiveData(true);
    emit LiveGui(data.GetGlobalState());
    qDebug() << "emit LiveGui(data.GetGlobalState())" << data.GetGlobalState().DebugString();
    OnGetMyNames();

}

void MainLAPIWorker::startPoint(){

    qDebug("Main Core Thread started");

//    Core::instance()->waitForGui();

#ifndef NOSYNC
    auto h = myNodeWorker->preinit();
    emit Height(h);
    node.thread()->start();
#endif
    {
        std::lock_guard<std::recursive_mutex> lockg{ last_mutex };
        last_block = processor.init();
        qDebug() << " startPoint last_block " << last_block;
#ifdef NOSYNC
        Node node;
        node.init();
        numto = Node::getLastLocalBlockNum();
#ifdef BLOCK_STEP
        int gonum = 2187 ;
        numto = (gonum >= last_block) ? gonum : last_block;
#endif
        emit Height(numto);
        OnInSync(numto);
#endif

        if ( last_block < 0 ) {
            //emit OnError();
            qDebug() << " lapi last_block < 0 ";
            last_block = BlockRecorder::zeroblock -1;
        }
        emit BlockNum(last_block);
    }

    intervalstart = 5000;
    timer->start(intervalstart);
}

void MainLAPIWorker::ResetIndex() {
    std::lock_guard<std::recursive_mutex> lockg{ last_mutex };
    processor.hardReset();
    last_block = processor.init();
    if ( last_block < 0 ) {
        last_block = BlockRecorder::zeroblock;
    }
}

//blockchain
void MainLAPIWorker::OnInSync(int32_t num) {
    qDebug() << "OnInSync" << num;
    bool myamlive;
    {
        std::lock_guard<std::recursive_mutex> lockg{ last_mutex };
        myamlive = (!amlive && num == last_block);
    }
    qDebug() << "myamlive" << myamlive;

    if ( myamlive )
        GoLive();
    else {
        numto = num;
        intervalstart = 2000;
        emit ProcessNext();
    }
}

bool MainLAPIWorker::doProcessBlock() {
    int32_t next;
    {
        std::lock_guard<std::recursive_mutex> lockg{ last_mutex };
        next = last_block+1;
    }
    auto b = fantasybit::Node::getLocalBlock(next);
    if (!b) {
        qWarning() << " !b";
        return false;
    }
    if ( !Process(*b) ) {
        qWarning() << " !Process";
        return false;
    }

    return true;
}

#include <QAbstractEventDispatcher>
void MainLAPIWorker::ProcessBlock() {

    int docount = 0;
    bool catchingup;
    do {
        if ( !doProcessBlock() ) return;
        emit BlockNum(last_block);
        count = pcount = 0;
        emit BlockNum(last_block);
        {
            std::lock_guard<std::recursive_mutex> lockg{ last_mutex };
            catchingup = !amlive && last_block < numto;
        }

        if ( catchingup )
        {

            if ( docount++ == 50 ) {
                QThread::currentThread()->eventDispatcher()->processEvents(QEventLoop::AllEvents);
                docount = 0;
            }
        }

        else if ( !amlive ) {
            {
                std::lock_guard<std::recursive_mutex> lockg{ last_mutex };
                amlive = true;
            }

            GoLive();
        }
//        else if ( docount < numto) {
//            docount = numto;
//            emit BlockNum(last_block);
//        }
#ifdef BLOCK_STEP
        if ( last_block < numto)
            if ( docount++ < 10) continue;
        docount=0;
#endif

    } while( catchingup );
}

void MainLAPIWorker::OnSeenBlock(int32_t num) {
    {
        std::lock_guard<std::recursive_mutex> lockg{ last_mutex };
        if (amlive)
            numto = num;
    }
    timer->start(intervalstart);
    count = bcount = 0;
    emit Height(num);
}

void MainLAPIWorker::OnBlockError(int32_t last) {
    {
        std::lock_guard<std::recursive_mutex> lockg{ last_mutex };
        numto = last;
    }
    emit BlockError(last);
}

void MainLAPIWorker::Timer() {
#ifdef TESTING_PRE_ROW_TRADE_FEATURE
    if ( justwentlive ) {
        justwentlive = false;
    }
#endif

    //qDebug() << " Timer ";
    bool numtogtlast;
    {
        std::lock_guard<std::recursive_mutex> lockg{ last_mutex };
        numtogtlast = (numto > last_block);
    }
    bcount++;
    pcount++;
    if ( !amlive ) {
        emit ProcessNext();
        if ( bcount > 10 && pcount < 2)
            emit GetNext();
        else if ( bcount < 3 && numto < std::numeric_limits<int32_t>::max())
            emit GetNext();
    }
    else if ( numtogtlast ) {
        emit ProcessNext();
        if ( bcount < 3)
            emit GetNext();
    }
#ifdef LIGHT_CLIENT_ONLY
    emit GetNext();
#else
    else {
        count++;
        emit GetNext();
        //emit ProcessNext();
        if ( count%10 == 0 ) {
            int interval = count/10*intervalstart*2;
    //        qInfo() << " timerr " << interval;
            if ( interval < intervalmax && interval > 500) {
                timer->start(interval);
    //            qInfo() << " timeout ";
    //            if ( true )
    //                emit GameStart("201600110");
            }
        }
    }
#endif
}

bool MainLAPIWorker::Process(fantasybit::Block &b) {
    int32_t last = processor.process(b);
    if ( last == -1 ) {
        //emit OnError();
        timer->start(5000);
        return false;
    }

    //if ( last != last_block+1) {
        //emit OnError
        //should never be here
    //    return false;
    //}

    {
        std::lock_guard<std::recursive_mutex> lockg{ last_mutex };
        if ( last == last_block+1)
            last_block = last;
        else
            qDebug() << " shoud bever be here! ? reorg? fork? ";
    }
    return true;
}

/*
//Data
void MainLAPIWorker::OnPlayerStatusChange(pair<string,fantasybit::PlayerStatus> in) {
    emit PlayerStatusChange(in);
}

void MainLAPIWorker::OnNameBal(fantasybit::FantasyNameBal bal) {
    emit NameBalance(bal);
}
*/

void MainLAPIWorker::OnGetMyNames() {
    vector<MyFantasyName> my;

    my = agent.getMyNamesStatus();
    for(auto p : my) {
        /*
        if ( amlive )
        if ( p.second.status() < MyNameStatus::confirmed )
            namedata.Subscribe(p.first);
        */
        myfantasynames[p.name()] = p;
    }


    qDebug() << " MyNames(my)";
    emit MyNames(my);
	/*
    if ( my.size() > 0 )  {
        myCurrentName = my.at(my.size()-1);
        agent.UseName(myCurrentName.name());
        emit NameStatus(myCurrentName);
        namedata.Subscribe(myCurrentName.name());
    }
	*/
}

//from Gui
void MainLAPIWorker::OnUseName(QString name) {
    qDebug() << " OnUseName " << name;
    myCurrentName.set_name(name.toStdString());
    myCurrentName.set_status(MyNameStatus::requested);

    //myfantasynames = agent.getMyNamesStatus();

    auto it = myfantasynames.find(name.toStdString());
    if ( it != end(myfantasynames)) {
        if ( agent.UseName(myCurrentName.name()) ) {
            myCurrentName = it->second;
        }
    }

    if ( myCurrentName.status() >= MyNameStatus::requested)
        DoSubscribe(myCurrentName.name(),true);

    qDebug() << "malpi NameStatus(myCurrentName)" << myCurrentName.DebugString().data();
    emit NameStatus(myCurrentName);
}


//tx
void MainLAPIWorker::OnClaimName(QString name) {
    if ( !amlive ) return;

    qDebug() << " OnClaimName " << name;

    myCurrentName.set_name(name.toStdString());
    myCurrentName.set_status(MyNameStatus::none);

    auto ret = agent.signPlayer(name.toStdString());

    switch ( ret ) {
        case FantasyAgent::AVAIL:
            myCurrentName.set_status(MyNameStatus::requested);
        break;
        case FantasyAgent::NOTAVAILABLE:
            myCurrentName.set_status(MyNameStatus::notavil);
        break;
        case FantasyAgent::OWNED:
            myCurrentName.set_status(MyNameStatus::confirmed);
        break;
    }

    if ( myCurrentName.status() == MyNameStatus::requested)  {
        NameTrans nt{};
        nt.set_public_key(agent.pubKeyStr());
        nt.set_fantasy_name(name.toStdString());


        Transaction trans{};
        trans.set_version(Commissioner::TRANS_VERSION);
        trans.set_type(TransType::NAME);
        trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nt);
        SignedTransaction sn = agent.makeSigned(trans);
        agent.onSignedTransaction(sn);
        DoSubscribe(myCurrentName.name(),true);
        DoPostTx(sn);
        count = bcount = 0;
        timer->start(intervalstart);
    }

    myfantasynames[myCurrentName.name()] = myCurrentName;
    qDebug() << "NameStatus(myCurrentName)" << myCurrentName.DebugString();
    emit NameStatus(myCurrentName);
}

void MainLAPIWorker::OnProjTX(vector<fantasybit::FantasyBitProj> vinp) {
    if ( !amlive ) return;

    if ( !agent.HaveClient() ) return;

    Transaction trans{};

    if ( vinp.size() == 0) return;
    if ( vinp.size() == 1) {
        FantasyBitProj &inp = vinp[0];
        ProjectionTrans pj{};
        pj.set_playerid(inp.playerid());
        pj.set_points(inp.proj());

        auto gs = data.GetGlobalState();
        pj.set_week(gs.week());
        pj.set_season(gs.season());

        trans.set_version(Commissioner::TRANS_VERSION);
        trans.set_type(TransType::PROJECTION);
        trans.MutableExtension(ProjectionTrans::proj_trans)->CopyFrom(pj);
    }
    else {
        ProjectionTransBlock pj{};

        auto gs = data.GetGlobalState();
        pj.set_week(gs.week());
        pj.set_season(gs.season());

        for (auto fbj : vinp ) {
            PlayerPoints pp;
            pp.set_playerid(fbj.playerid());
            pp.set_points(fbj.proj());
            pj.add_player_points()->CopyFrom(pp);
        }

        trans.set_version(Commissioner::TRANS_VERSION);
        trans.set_type(TransType::PROJECTION_BLOCK);
        trans.MutableExtension(ProjectionTransBlock::proj_trans_block)->CopyFrom(pj);
    }

    SignedTransaction sn = agent.makeSigned(trans);
    agent.onSignedTransaction(sn);
    DoPostTx(sn);
    DoSubscribe(myCurrentName.name(),true);
    count = bcount = 0;
//    timer->stop();
//    Timer();
    timer->start(intervalstart);
}

//tx status
void MainLAPIWorker::OnFoundName(string name) {
    if ( !myCurrentName.has_name())
        myCurrentName.set_name(name);

    if (myCurrentName.name() == name) {
        if ( !agent.UseName(name) ) {
            myCurrentName.set_status(MyNameStatus::notavil);
            DoSubscribe(name,false);
            emit NameStatus(myCurrentName);
            myCurrentName.Clear();
            return;
        }
        else {
            MyFantasyName mfn = agent.getCurrentNamesStatus();
            myCurrentName.set_status(mfn.status());
            if (myCurrentName.status() == MyNameStatus::none )
                myCurrentName.set_status(MyNameStatus::notavil);
            if ( myCurrentName.status() ==  MyNameStatus::notavil ) {
                DoSubscribe(name, false);
                emit NameStatus(myCurrentName);
                myCurrentName.Clear();
                return;
            }
            else {
                emit NameStatus(myCurrentName);
            }
        }

        return;
    }
}

void MainLAPIWorker::OnProjLive(fantasybit::FantasyBitProj proj) {
    emit LiveProj(proj);
}


#ifdef DATAAGENT

void MainLAPIWorker::BeOracle() {
    if ( agent.beDataAgent() ) {
        auto ns = agent.getCurrentNamesStatus();
        emit NameStatus(ns);
    }
}
#endif

void MainLAPIWorker::DoPostTx(SignedTransaction &st) {
    auto txstr = st.SerializeAsString();
    RestfullClient rest(QUrl(PAPIURL.data()));
    //rest.postRawData("tx","octet-stream",txstr.data(),((size_t)txstr.size()));
    rest.postRawData("tx","octet-stream",txstr.data(),((size_t)txstr.size()));
}

void MainLAPIWorker::DoPostTr(SignedTransaction &st) {
    auto txstr = st.SerializeAsString();
    RestfullClient rest(QUrl(PAPIURL.data()));
    //rest.postRawData("tx","octet-stream",txstr.data(),((size_t)txstr.size()));
    rest.postRawData("trade","octet-stream",txstr.data(),((size_t)txstr.size()));
}

void MainLAPIWorker::DoSubscribe(const string &name, bool suborun) {

    if ( suborun ) {
        namedata.Subscribe(name);
        exchangedata.Subscribe(name);
    }
    else {
        namedata.UnSubscribe(name);
        exchangedata.UnSubscribe(name);
    }
}

/*
void MainLAPIWorker::OnNewOrder(fantasybit::ExchangeOrder eo) {

    Transaction trans{};
    trans.set_version(Commissioner::TRANS_VERSION);
    trans.set_type(TransType::EXCHANGE);
    trans.MutableExtension(ExchangeOrder::exchange_order)->CopyFrom(eo);
    SignedTransaction sn = agent.makeSigned(trans);
    agent.onSignedTransaction(sn);
    DoPostTr(sn);
    //namedata.Subscribe(myCurrentName.name());
}
*/
/*ys
//ToDo: convert names with a status OnLive()
myfantasynames = agent.getMyNamesStatus();
for(auto p : myfantasynames) {
    if ( p.second.status() < MyNameStatus::confirmed )
        namedata.Subscribe(p.first);

    deltadata.add_myfantasyname()->CopyFrom(p.second);
}
deltadata.mutable_globalstate()->CopyFrom(data.GetGlobalState());
*/
