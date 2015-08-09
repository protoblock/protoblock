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

/**
 * @brief The Task class : a task to run in a separate thread and forget about it.
 */
class Task : public QRunnable {
public:
 void run() {
  qDebug("I am doing something in my own thread. I have no way to notify my creator i'm done");
 }
};


class LAPIWorkerData : public QSharedData
{
public:
 LAPIWorkerData(){}
 ~LAPIWorkerData(){}
};

template <class T>
class ThreadedObject {
public:
    ThreadedObject(){
        myObjectInstance.moveToThread(&myWorkerThread);        
    }
    ~ThreadedObject(){
        if (myWorkerThread.isRunning())
            myWorkerThread.quit();
        myWorkerThread.wait();//AKA join
    }

    T * object() { return & myObjectInstance; }
    QThread * thread()  { return & myWorkerThread; }

protected :
    QThread myWorkerThread;
    T myObjectInstance;
};


extern QWaitCondition waitForGUI;
class MainLAPIWorker : public QObject {
    Q_OBJECT
public:
    MainLAPIWorker(QObject * parent=0):QObject(parent){}
    ~MainLAPIWorker(){}

signals:
    void sendNotificationWithData(const QVariant & notificationData);
public slots:
    void processGUIRequest(const QVariant & requestData){
        //GUI sent us a request process it
        //  processing requestData
        //and then reply back a kind of result
        if (requestData.type()== QVariant::String){
            if (requestData.toString()=="PING")
                emit sendNotificationWithData(QString("PONG"));
        }
    }

    void getPlayers(int /*week*/){
        QUrl url("http://api.nfldata.apiphany.com/nfl/v2/JSON/AreAnyGamesInProgress");
        RestfullCall rest;
        rest.moveToThread(QThread::currentThread());
        rest.restFullSynchrounousCallGet(url,"","");
        emit sendNotificationWithData(QVariant(rest.lastReply()));
    }

    void startPoint(){
        qDebug("Main Core Thread started");
        Task doInAnotherThreadAndForget;
        doInAnotherThreadAndForget.setAutoDelete(true);
        doInAnotherThreadAndForget.run();
        // initialize things
        //.....
        //wait for the MainForm (the GUI controller) to start
        QMutex mutex;
        mutex.lock();
        waitForGUI.wait(&mutex);
        //send one way notification
        emit sendNotificationWithData(QVariant("We notify you that something happened"));
    } 
};




#endif // LAPIWORKER_H
