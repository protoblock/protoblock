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
#include "NameData.pb.h"
#include "StaticData.pb.h"
#include "StatusData.pb.h"
#include "ProtoData.pb.h"
#include <vector>

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
    std::map<std::string,fantasybit::MyFantasyName> myfantasynames{};
    fantasybit::MyFantasyName myCurrentName{};
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
    void OnLive();
    void SubscribeLive();
    void NameStatus(const fantasybit::MyFantasyName &);
    void LiveProj(fantasybit::FantasyBitProj &);
    void OnMyNames(std::vector<fantasybit::MyFantasyName> &);


public slots:

    void processGUIRequest(const QVariant & requestData);

    void OnGetMyNames();

    void getLivePlayers(int );

    void startPoint();

    void OnInSync(int num);

    void ProcessBlock();

    void OnSeenBlock(int num);

    void Timer();

    void OnPlayerChange(std::string);
    void OnFoundName(std::string &);
    void OnProjLive(fantasybit::FantasyBitProj &);
    void OnNameBal(fantasybit::FantasyNameBal &);

    void OnUseName(QString);
    void OnClaimName(QString);

    void OnProjTX(fantasybit::FantasyBitProj);
private:

    bool Process(fantasybit::Block &b);

    void doNewDelta();

};


#endif // LAPIWORKER_H
