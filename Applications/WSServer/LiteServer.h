#ifndef LITE_SERVER_H
#define LITE_SERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QWebSocketServer>
#include "StateData.pb.h"
#include <iostream>
#include <stdexcept>
#include <stack>
#include <utility>
#include <functional>
#include <unordered_map>

using namespace fantasybit;
using namespace std;
QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

template <typename T> struct hash<pair<T,T> > {
    size_t operator()(pair<T,T> const &p) {
        return hash<T>()(p.first) ^ hash<T>()(p.second);
    }

};

class LiteServer : public QObject
{
    Q_OBJECT
public:
    explicit LiteServer(quint16 port, bool debug = false, QObject *parent = Q_NULLPTR);
//    ~LiteServer();


//    Incoming setEnum(const QString &string);

//    QStringList createCommandArgument(const QString &cmd);

    void doSendLeaders(QWebSocket *pClient);
public slots:
    void onNewProj(const std::string &name ) {
        auto it = mFnameSubscribed.find(name);
        if ( it == end(mFnameSubscribed))
            return;

        qDebug()  << " socket yes " << name.data();
        doSendProjections(it->second,name);
//        doSendLeaders(it->second);
    }

    void GameStart(string gameid);
    void OnLive();

Q_SIGNALS:
    void closed();
    void error(QString);

private Q_SLOTS:
    void onNewConnection();
    void processBinaryMessage(const QByteArray &message);
    void socketDisconnected();
    void handleError(const QString err);

private:
    QString m_errorString;
    QWebSocketServer *m_pWebSocketServer;
    bool m_debug;
    quint16 mport;
    std::string mRepstr;
    WSReply gameStart;


//    fantasybit::GetROWMarketRep mROWMarketRep;
//    std::unordered_map<std::string, ROWMarket *> mPidROWMarket;
//    std::unordered_map<std::string, GetDepthRep *> mPidGetDepthRep;

//    std::unordered_map<std::string,std::unordered_map<std::string, AllOdersSymbol *>> mAllOdersFname;
//    std::unordered_map<int32_t,std::unordered_map<std::string, AllOdersSymbol *>> mAllOdersFname;

//    ROWMarket *getRowmarket(const std::string &pid);
//    GetDepthRep * getDepthRep(const std::string &playerid);

//    std::unordered_map<QWebSocket *, std::vector<std::string>> mSocketSubscribed;
//    std::unordered_map<std::string, std::set<QWebSocket *>> mFnameSubscribed;

    std::unordered_map<std::string, QWebSocket *> mFnameSubscribed;
    std::unordered_map<QWebSocket *, std::string> mSocketSubscribed;

//    std::unordered_map< std::pair<std::string, std::string> , std::vector<Order *>>  reservedOrder;
//    std::unordered_map< std::pair<std::string, std::string> , std::vector<AllOdersSymbol *>>  reserveAllOdersSymbol;

//    std::unordered_map< std::string, std::unordered_map<std::string, AllOdersSymbol *>>  fname2symbolAllOrderss;



//    std::unordered_map< std::string, fnameptrs>  fnameptrsmap;

//    void getFnameSnap(const std::string &fname);
//    AllOdersSymbol * getAllOdersSymbol(fnameptrs &fptr,const std::string &symbol);
//    AllOdersFname *getAllOdersFname(const std::string &fname);

    GetProjectionRep *mGetProjectionRep;
    WSReply mWSReplyGetProjectionRep;
    QByteArray mWSReplybyteArray;
    quint16 mPort;
    void doSendProjections(QWebSocket *pClient, const std::string &fname);
};

#endif //LITE_SERVER_H
