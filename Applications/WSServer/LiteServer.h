#ifndef LITE_SERVER_H
#define LITE_SERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QWebSocketServer>
#include "StateData.pb.h"
#include <iostream>
#include <stdexcept>
#include "ExData.pb.h"
#include <set>
#include <utility>
#include <functional>
#include <unordered_map>
#include "server.h"


QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

namespace fantasybit {

class LiteServer : public QObject {
    Q_OBJECT
public:
    explicit LiteServer(quint16 port, bool debug = false, QObject *parent = Q_NULLPTR);
    ~LiteServer();

public slots:
    void onNewProj(const std::string &name ) {
        auto it = mFnameSubscribed.find(name);
        if ( it == end(mFnameSubscribed))
            return;

        qDebug()  << " socket yes " << name.data();
        for ( const auto &s : it->second)
            doSendProjections(s,name);
    }

    void newDepth(const string &name) {
        const auto it = mSymbolSubscribed.find(name);
        if ( it == end(mSymbolSubscribed))
            return;

//        qDebug()  << "newDepth socket yes " << name.data();
//        for ( const auto &s : it->second)
        doSendDepth(name,nullptr,&it->second);
    }

    void newRow();

    void newPos(const string &name) {
        auto it = mFnameSubscribed.find(name);
        if ( it == end(mFnameSubscribed))
            return;

        for ( const auto &s : it->second)
            doSendOrders(s,name);
    }

    void GameStart(std::string gameid);
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
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
    quint16 mport;
    std::string mRepstr;
    WSReply gameStart;

    std::unordered_map<QWebSocket *, std::vector<std::string>> mSocketSubscribed;
    std::unordered_map<std::string, std::set<QWebSocket *>> mFnameSubscribed;
    std::unordered_map<std::string, std::set<QWebSocket *>> mSymbolSubscribed;
    std::unordered_map<QWebSocket *, std::string> mSocketSymbol;

    GetProjectionRep *mGetProjectionRep;
    WSReply mWSReplyGetProjectionRep;
    QByteArray mWSReplybyteArray;
    quint16 mPort;
    void doSendProjections(QWebSocket *pClient, const std::string &fname);


    WSReply mRepDepth;
    GetDepthRep *mGetDepthRep;
    QByteArray mRepDepthArray;
    void doSendDepth(const std::string &symbol,QWebSocket *pClient =nullptr,
                     const std::set<QWebSocket *> * = nullptr);

    WSReply rowRep;
    QByteArray mGetROWMarketRepArray;

    GetOrdersRep *mGetOrdersRep;
    WSReply mWSReplyGetOrdersRep;
    QByteArray mGetOrdersRepArray;
    void doSendOrders(QWebSocket *pClient, const std::string &fname);

    Server *mServer;
};

}


#endif //LITE_SERVER_H



/*

template <typename T> struct hash<pair<T,T> > {
    size_t operator()(pair<T,T> const &p) {
        return hash<T>()(p.first) ^ hash<T>()(p.second);
    }
};




    void doSendLeaders(QWebSocket *pClient);
public slots:
    Order *addOrder(fnameptrs &fptr, AllOdersSymbol *allords, const Order &orderin);
    void cleanIt(fnameptrs &fptr);

    void cleanIt(const std::string &fname);
public slots:
    void OnDepthDelta(fantasybit::DepthFeedDelta *df);
    void OnNewOO(const fantasybit::FullOrderDelta &);
    void OnNewPos(const fantasybit::FullPosition &);





    void OnMarketSnapShot(fantasybit::MarketSnapshot*);
    void OnMarketTicker(fantasybit::MarketTicker *);
    void OnTradeTick(fantasybit::TradeTic*);

private:
    void processBinaryTxMessage(const QByteArray &message);

    QString m_errorString;






//    std::unordered_map<std::string,std::unordered_map<std::string, AllOdersSymbol *>> mAllOdersFname;
//    std::unordered_map<int32_t,std::unordered_map<std::string, AllOdersSymbol *>> mAllOdersFname;





//    std::unordered_map< std::pair<std::string, std::string> , std::vector<Order *>>  reservedOrder;
//    std::unordered_map< std::pair<std::string, std::string> , std::vector<AllOdersSymbol *>>  reserveAllOdersSymbol;

//    std::unordered_map< std::string, std::unordered_map<std::string, AllOdersSymbol *>>  fname2symbolAllOrderss;






    AllOdersFname *getAllOdersFname(const std::string &fname);

*/

