#ifndef LAPIWORKER_H
#define LAPIWORKER_H

#include <QObject>
#include <QSharedDataPointer>
#include <QNetworkAccessManager>
#include <QThread>
#include <QNetworkReply>
#include <QEventLoop>
#include <QRunnable>
#include <QApplication>
#include <QWaitCondition>
#include "RestFullCall.h"
#include "globals.h"
#include "NodeWorker.h"
#include <QTimer.h>
#include <algorithm>
#include "Processor.h"
#include "FantasyAgent.h"
#include "ProtoData.pb.h"

extern QWaitCondition waitForGUI;
class MainLAPIWorker : public QObject
{
    Q_OBJECT
public:
    MainLAPIWorker(QObject * parent=0):  QObject(parent)
    {
        timer = new QTimer(this);
        node.thread()->connect(node.thread(),
                                SIGNAL(started()),
                                node.object(),
                                SLOT(init()));

        myNodeWorker = node.object();
        QObject::connect(myNodeWorker,SIGNAL(InSync(int)),this,SLOT(OnInSync(int)));
        QObject::connect(myNodeWorker,SIGNAL(SeenBlock(int)),this,SLOT(OnSeenBlock(int)));
        //QObject::connect(this,SIGNAL(Timer()),myNodeWorker,SLOT(TryNext()));
        QObject::connect(this,SIGNAL(ProcessNext()),this,SLOT(ProcessBlock()));
        QObject::connect(timer,SIGNAL(timeout()),this,SLOT(Timer()));

        QObject::connect(this,SIGNAL(GetNext()),myNodeWorker,SLOT(TryNext()));
        //QObject::connect(this,SIGNAL(GetNext()),qApp,SLOT(aboutQt()));
        //QObject::connect(timer,SIGNAL(timeout()),myNodeWorker,SLOT(Timer()));

        node.thread()->start();
    }
    ThreadedObject<NodeWorker> node;

private:
    int intervalmax = 60000;
    int intervalstart = 500;
    int bcount =0;
    int pcount =0;
    int count =0;
    int last_block=0;
    NodeWorker *myNodeWorker;
    int numto = std::numeric_limits<int>::max();
    bool amlive = false;
    QTimer * timer;
    fantasybit::BlockProcessor processor;
    fantasybit::FantasyAgent agent{};
    //DeltaData deltadata;
public:
    ~MainLAPIWorker(){}


    void GoLive() {
        emit OnLive();
        numto = 0;
        intervalstart = 500;
        timer->start(intervalstart);

        //ToDo: convert names with a status OnLive()
        QMultiMap<QString,QString> qm{};
        for(auto p : agent.getMyNames()) {
            qm.insertMulti(QString::fromStdString(p.first),
                           QString::fromStdString(p.second));
        }

    }

signals:
    void sendNotificationWithData(const QVariant & notificationData);
    void ProcessNext();
    void GetNext();
    void OnData(const fantasybit::DeltaData &);
    void OnLive();

public slots:
    void processGUIRequest(const QVariant & requestData){
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

    void getPlayers(int ){
        QUrl url("http://api.nfldata.apiphany.com/nfl/v2/JSON/AreAnyGamesInProgress");
        RestfullCall rest;
        rest.moveToThread(QThread::currentThread());
        rest.restFullSynchrounousCallGet(url,"","");
        emit sendNotificationWithData(QVariant(rest.lastReply()));
    }

    void startPoint(){
        qDebug("Main Core Thread started");
        /*
        int height = GetRealHeight();
        Task doInAnotherThreadAndForget;
        doInAnotherThreadAndForget.setAutoDelete(true);
        doInAnotherThreadAndForget.run(height);
        */
        // initialize things
        //.....
        //wait for the MainForm (the GUI controller) to start
        QMutex mutex;
        mutex.lock();
        waitForGUI.wait(&mutex);
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

    void OnInSync(int num) {
        numto = num;
        intervalstart = 2000;
        emit ProcessNext();
    }

    void ProcessBlock() {
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

    void OnSeenBlock(int num) {
        if (amlive)
            numto = num;
        timer->start(intervalstart);
        count = bcount = 0;
    }

    void Timer() {
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

private:
    bool Process(fantasybit::Block &b) {
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


    void doNewDelta() {
        auto deltasnap = processor.GetandClear();
        qDebug(deltasnap.DebugString().c_str());
        emit OnData(deltasnap);
    }

};


#endif // LAPIWORKER_H
