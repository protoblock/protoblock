#ifndef LAPIWORKER_H
#define LAPIWORKER_H

#include "globals.h"
#include <QTimer.h>
#include "threadedqobject.h"
#include "NodeWorker.h"
#include "FantasyAgent.h"
#include "Processor.h"
#include "FantasyAgent.h"
#include "NameData.h"
#include "Data.h"

class MainLAPIWorker : public QObject
{
    Q_OBJECT

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
    fantasybit::FantasyAgent agent{};
    fantasybit::DataData data;
    fantasybit::NameData namedata;
    fantasybit::BlockProcessor processor;

    fantasybit::DeltaData deltadata{};

public:
    MainLAPIWorker(QObject * parent=0);
    ~MainLAPIWorker(){}
    ThreadedQObject<NodeWorker> node;
    void GoLive();

signals:

    void sendNotificationWithData(const QVariant & notificationData);
    void ProcessNext();
    void GetNext();
    void OnData(const fantasybit::DeltaData &);
    void OnLive(const fantasybit::DeltaData &);
    void SubscribeLive();

public slots:

    void processGUIRequest(const QVariant & requestData);

    void getPlayers(int );

    void startPoint();

    void OnInSync(int num);

    void ProcessBlock();

    void OnSeenBlock(int num);

    void Timer();

private:

    bool Process(fantasybit::Block &b);

    void doNewDelta();

};


#endif // LAPIWORKER_H
