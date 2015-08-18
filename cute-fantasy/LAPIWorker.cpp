#include "LAPIWorker.h"


#include <QWaitCondition>
#include "RestFullCall.h"
#include "globals.h"
#include <algorithm>
#include "ProtoData.pb.h"
#include "core.h"


MainLAPIWorker::MainLAPIWorker(QObject * parent):  QObject(parent),
    data{}, namedata{}, processor(data,namedata)
{
    timer = new QTimer(this);
    node.thread()->connect(node.thread(),
                           SIGNAL(started()),
                           node.object(),
                           SLOT(init()));
    myNodeWorker = node.object();
    node.thread()->start();
    QObject::connect(myNodeWorker,SIGNAL(InSync(int)),this,SLOT(OnInSync(int)));
    QObject::connect(myNodeWorker,SIGNAL(SeenBlock(int)),this,SLOT(OnSeenBlock(int)));
    //QObject::connect(this,SIGNAL(Timer()),myNodeWorker,SLOT(TryNext()));
    QObject::connect(this,SIGNAL(ProcessNext()),this,SLOT(ProcessBlock()));
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(Timer()));
    QObject::connect(this,SIGNAL(GetNext()),myNodeWorker,SLOT(TryNext()));
    //QObject::connect(this,SIGNAL(GetNext()),qApp,SLOT(aboutQt()));
    //QObject::connect(timer,SIGNAL(timeout()),myNodeWorker,SLOT(Timer()));
    QObject::connect(this,SIGNAL(SubscribeLive()),&data,SLOT(OnSubscribeLive()));
    QObject::connect(this,SIGNAL(SubscribeLive()),&namedata,SLOT(OnSubscribeLive()));
    QObject::connect(this,SIGNAL(SubscribeLive()),&processor,SLOT(OnSubscribeLive()));

}

void MainLAPIWorker::GoLive() {
    numto = 0;
    intervalstart = 500;
    timer->start(intervalstart);

    //ToDo: convert names with a status OnLive()
    for(auto p : agent.getMyNamesStatus()) {
        deltadata.add_myfantasyname()->CopyFrom(p.second);
    }
    deltadata.mutable_globalstate()->CopyFrom(data.GetGlobalState());

    emit OnLive(deltadata);
    emit SubscribeLive();

}

void MainLAPIWorker::processGUIRequest(const QVariant & requestData){
    //GUI sent us a request process it
    //  processing requestData
    //and then reply back a kind of result
    /*
        if (requestData.type()== QVariant::String){
            if (requestData.toString()=="PING")
                emit sendNotificationWithData(QString("PONG"));
        }
        */
}

void MainLAPIWorker::getPlayers(int week){
    if ( amlive ) {
        auto allgames = data.GetWeeklyGameRosters(week);
        //emit sendNotificationWithData(allgames);
    }
    /*
    QUrl url("http://api.nfldata.apiphany.com/nfl/v2/JSON/AreAnyGamesInProgress");
    RestfullCall rest;
    rest.moveToThread(QThread::currentThread());
    rest.restFullSynchrounousCallGet(url,"","");
    */
}

void MainLAPIWorker::startPoint(){

    qDebug("Main Core Thread started");
    QMutex mutex;
    mutex.lock();
    qDebug("Waiting for gui to show up it's first window");
    Core::waitForGUI.wait(&mutex);
    //send one way notification
    last_block = processor.init();
    if ( last_block < 0 ) {
        //emit OnError();
        last_block = 0;
    }

    //emit sendNotificationWithData(QVariant(qm));

    intervalstart = 5000;
    timer->start(intervalstart);
}

void MainLAPIWorker::OnInSync(int num) {
    numto = num;
    intervalstart = 2000;
    emit ProcessNext();
}

void MainLAPIWorker::ProcessBlock() {
    auto b = fantasybit::Node::getLocalBlock(last_block+1);
    if (!b) {
        //emit OnError();
        return;
    }
    if ( !Process(*b) ) return;

    count = pcount = 0;
    if ( !amlive && last_block < numto )
        emit ProcessNext(); //catching up
    else {
        doNewDelta();
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

void MainLAPIWorker::Timer() {
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
        emit ProcessNext();
        int interval = std::min(intervalmax,count*intervalstart);
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

void MainLAPIWorker::doNewDelta() {
    //auto deltasnap = processor.GetandClear();
    //qDebug(deltasnap.DebugString().c_str());
    //emit OnData(deltasnap);
}
