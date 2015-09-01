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
    data{}, namedata{}, processor(data,namedata)
{
    timer = new QTimer(this);
    node.thread()->connect(node.thread(),
                           SIGNAL(started()),
                           node.object(),
                           SLOT(init()));
    myNodeWorker = node.object();

    //QObject::connect(this,SIGNAL(Timer()),myNodeWorker,SLOT(TryNext()));
    //QObject::connect(this,SIGNAL(GetNext()),qApp,SLOT(aboutQt()));
    //QObject::connect(timer,SIGNAL(timeout()),myNodeWorker,SLOT(Timer()));


    //block sync
    QObject::connect(myNodeWorker,SIGNAL(InSync(int)),this,SLOT(OnInSync(int)));
    QObject::connect(myNodeWorker,SIGNAL(SeenBlock(int)),this,SLOT(OnSeenBlock(int)));
    QObject::connect(myNodeWorker,SIGNAL(BlockError(int)),this,SLOT(OnBlockError(int)));

    QObject::connect(this,SIGNAL(ProcessNext()),this,SLOT(ProcessBlock()));
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(Timer()));
    QObject::connect(this,SIGNAL(GetNext()),myNodeWorker,SLOT(TryNext()));

    //data processing
    QObject::connect(this,SIGNAL(LiveData(bool)),&data,SLOT(OnLive(bool)));
    QObject::connect(this,SIGNAL(LiveData(bool)),&namedata,SLOT(OnLive(bool)));
    //QObject::connect(this,SIGNAL(Live(bool)),&processor,SLOT(OnLive(bool)));

    //data to data signals
    QObject::connect(&processor,SIGNAL(WeekOver(int)),&data,SLOT(OnWeekOver(int)));
    QObject::connect(&processor,SIGNAL(WeekOver(int)),&namedata,SLOT(OnWeekOver(int)));
    QObject::connect(&processor,SIGNAL(WeekStart(int)),&namedata,SLOT(OnWeekStart(int)));
    QObject::connect(&processor,SIGNAL(WeekStart(int)),&data,SLOT(OnWeekStart(int)));
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

    QObject::connect(&namedata,SIGNAL(FantasyNameBalance(fantasybit::FantasyNameBal&)),
                     this,SIGNAL(NameBal(fantasybit::FantasyNameBal&)));
}

void MainLAPIWorker::GoLive() {
    amlive = true;
    numto = 0;
    intervalstart = 1000;
    timer->start(intervalstart);

    OnGetMyNames();
    qDebug() << "emit LiveData(true)";
    emit LiveData(true);
    emit LiveGui(data.GetGlobalState());
    qDebug() << "emit LiveGui(data.GetGlobalState())" << data.GetGlobalState().DebugString();

}

void MainLAPIWorker::startPoint(){
    qDebug("Main Core Thread started");
    Core::instance()->waitForGui();
    node.thread()->start();
    last_block = processor.init();
    if ( last_block < 0 ) {
        //emit OnError();
        last_block = 0;
    }   

    intervalstart = 5000;
    timer->start(intervalstart);
}

//blockchain
void MainLAPIWorker::OnInSync(int num) {
    if ( !amlive && num == last_block )
        GoLive();
    else {
        numto = num;
        intervalstart = 2000;
        emit ProcessNext();
    }
}

#include <QAbstractEventDispatcher>
void MainLAPIWorker::ProcessBlock() {

    auto b = fantasybit::Node::getLocalBlock(last_block+1);
    if (!b) {
        //emit OnError();
        return;
    }
    if ( !Process(*b) ) return;

    count = pcount = 0;
    if (!amlive && last_block < numto )
    {
        //emit ProcessNext(); //catching up
        QThread::currentThread()->eventDispatcher()->processEvents(QEventLoop::AllEvents);
        ProcessNext();
    }
    else {
        //doNewDelta();
        if ( !amlive ) {
            amlive = true;
            GoLive();
        }
    }
}

void MainLAPIWorker::OnSeenBlock(int num) {   
    if (amlive)
        numto = num;
    timer->start(intervalstart);
    count = bcount = 0;
}

void MainLAPIWorker::OnBlockError(int last) {
    numto = last;
    emit BlockError(last);
}

void MainLAPIWorker::Timer() {
    //qDebug() << " Timer ";
    bcount++;
    pcount++;
    if ( !amlive ) {
        emit ProcessNext();
        if ( bcount > 10 && pcount < 2)
            emit GetNext();
        else if ( bcount < 3 && numto < std::numeric_limits<int>::max())
            emit GetNext();
    }
    else if ( numto > last_block ) {
        emit ProcessNext();
        if ( bcount < 3)
            emit GetNext();
    }
    else {
        count++;
        emit GetNext();
        //emit ProcessNext();
        int interval = count*intervalstart*2;
        if ( interval < intervalmax && interval > 10)
            timer->start(interval);
    }

}

bool MainLAPIWorker::Process(fantasybit::Block &b) {
    int last = processor.process(b);
    if ( last == -1 ) {
        //emit OnError();
        timer->start(5000);
        return false;
    }
    if ( last != last_block+1) {
        //emit OnError
        //should never be here
        return false;
    }

    last_block = last;
    return true;
}

/*
//Data
void MainLAPIWorker::OnPlayerStatusChange(pair<string,fantasybit::PlayerStatus> in) {
    emit PlayerStatusChange(in);
}

void MainLAPIWorker::OnNameBal(fantasybit::FantasyNameBal &bal) {
    emit NameBalance(bal);
}
*/

void MainLAPIWorker::OnGetMyNames() {
    vector<MyFantasyName> my;
    myfantasynames = agent.getMyNamesStatus();
    for(auto p : myfantasynames) {
        if ( amlive )
        if ( p.second.status() < MyNameStatus::confirmed )
            namedata.Subscribe(p.first);

        my.push_back(p.second);
    }

    qDebug() << " MyNames(my)";
    emit MyNames(my);
}

//from Gui
void MainLAPIWorker::OnUseName(QString name) {
    myCurrentName.set_name(name.toStdString());
    myCurrentName.set_status(MyNameStatus::none);


    auto it = myfantasynames.find(name.toStdString());
    if ( it == end(myfantasynames)) {
        myfantasynames = agent.getMyNamesStatus();
        it = myfantasynames.find(name.toStdString());
    }

    if ( it != end(myfantasynames)) {
        if ( agent.UseName(myCurrentName.name()) ) {
            myCurrentName = it->second;
        }
    }

    if ( myCurrentName.status() < MyNameStatus::confirmed)
        namedata.Subscribe(myCurrentName.name());

    qDebug() << "NameStatus(myCurrentName)" << myCurrentName.DebugString();
    emit NameStatus(myCurrentName);
}


//tx
void MainLAPIWorker::OnClaimName(QString name) {
    if ( !amlive ) return;

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
        namedata.Subscribe(myCurrentName.name());
        DoPostTx(sn);
    }

    qDebug() << "NameStatus(myCurrentName)" << myCurrentName.DebugString();
    emit NameStatus(myCurrentName);
}

void MainLAPIWorker::OnProjTX(fantasybit::FantasyBitProj inp) {
    if ( !amlive ) return;

    if ( !agent.HaveClient() ) return;

    ProjectionTrans pj{};
    pj.set_playerid(inp.playerid());
    pj.set_points(inp.proj());

    auto gs = data.GetGlobalState();
    pj.set_week(gs.week());
    pj.set_season(gs.season());

    Transaction trans{};
    trans.set_version(Commissioner::TRANS_VERSION);
    trans.set_type(TransType::PROJECTION);
    trans.MutableExtension(ProjectionTrans::proj_trans)->CopyFrom(pj);
    SignedTransaction sn = agent.makeSigned(trans);
    agent.onSignedTransaction(sn);
    DoPostTx(sn);
    namedata.Subscribe(myCurrentName.name());

}

//tx status
void MainLAPIWorker::OnFoundName(string name) {
    if (myCurrentName.name() == name)
        myCurrentName.set_status(MyNameStatus::confirmed);

    auto it = myfantasynames.find(name);
    if ( it == end(myfantasynames)) {
        myfantasynames = agent.getMyNamesStatus();
        it = myfantasynames.find(name);
    }

    if ( it != end(myfantasynames)) {
        it->second.set_status(MyNameStatus::confirmed);
        qDebug() << "NameStatus(it->second)" << it->second.DebugString();
        emit NameStatus(it->second);
    }
    else {
        qWarning() << "it != end(myfantasynames)";
    }
}

void MainLAPIWorker::OnProjLive(fantasybit::FantasyBitProj proj) {
    emit LiveProj(proj);
}


void MainLAPIWorker::BeOracle() {
    if ( agent.beDataAgent() ) {
        auto ns = agent.getCurrentNamesStatus();
        emit NameStatus(ns);
    }
}

void MainLAPIWorker::DoPostTx(SignedTransaction &st) {
    auto txstr = st.SerializeAsString();
    RestfullClient rest(QUrl("http://192.96.159.216:4545"));
    rest.postRawData("tx","shit",txstr.data(),((size_t)txstr.size()));
}



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
