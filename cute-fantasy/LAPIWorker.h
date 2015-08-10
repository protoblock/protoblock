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


extern QWaitCondition waitForGUI;
class MainLAPIWorker : public QObject
{
    Q_OBJECT
public:
    MainLAPIWorker(QObject * parent=0):  QObject(parent)
    {
        timer = new QTimer(this);
        ThreadedObject<NodeWorker> node;
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
public:
    ~MainLAPIWorker(){}

    bool Process(fantasybit::Block &b) {
        return true;
    }

    void GoLive() {
        //emit OnLive(deltasnap);
        //deltasnap.clear();
        numto = 0;
        timer->start(intervalstart);
    }

signals:
    void sendNotificationWithData(const QVariant & notificationData);
    void ProcessNext();
    void GetNext();

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
        timer->start(intervalstart);
        emit sendNotificationWithData(QVariant("We notify you that something happened"));
    } 

    void OnInSync(int num) {
        numto = num;
        emit ProcessNext();
    }

    void ProcessBlock() {
        auto b = fantasybit::Node::getLocalBlock(last_block+1);
        if (b) {
            count = pcount = 0;
            if ( Process(*b) ) {
                last_block++;
                if ( !amlive ) {
                    if ( last_block >= numto ) {
                        amlive = true;
                        GoLive();
                    }
                    emit ProcessNext();
                }
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
};


#endif // LAPIWORKER_H
