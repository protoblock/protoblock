#ifndef LITE_SERVER_H
#define LITE_SERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QWebSocketServer>

#include "NameData.h"
#include "Data.h"
#include "StateData.pb.h"
//#include "ldbhashreadertool.h"
//#include "pbjson.hpp"
#include <iostream>
#include <stdexcept>
#include "ExData.pb.h"
#include <stack>
#include "ExchangeData.h"
#include <utility>
#include <functional>

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
    ~LiteServer();
    enum Incoming{
        GetBlockHeader,
        GetLeaderBoardTree,
        GetLeaderBoardItem,
        GetTicker,
        GetStats,
        GetPlayerInfo,
        GetAwardsItem,
        GetMerkleTree,
        GetDrillMerkle,
        UnKnown
    };

    Incoming setEnum(const QString &string);

    QStringList createCommandArgument(const QString &cmd);


public slots:
    void OnDepthDelta(fantasybit::DepthFeedDelta *df);
    void OnNewOO(fantasybit::FullOrderDelta);
Q_SIGNALS:
    void closed();
    void error(QString);

private Q_SLOTS:
    void onNewConnection();
    void processBinaryMessage(const QByteArray &message);
    void socketDisconnected();
    void handleError(const QString err);
    void OnMarketSnapShot(fantasybit::MarketSnapshot*);
    void OnMarketTicker(fantasybit::MarketTicker *);
    void OnTradeTick(fantasybit::TradeTic*);
private:
    void processBinaryTxMessage(const QByteArray &message);
    QString m_errorString;
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
    bool m_debug;
    fantasybit::BlockMeta bm;
    std::string forPbJSon;
    quint16 mport;
    std::string mRepstr;
    fantasybit::GetROWMarketRep mROWMarketRep;
    std::unordered_map<std::string, ROWMarket *> mPidROWMarket;
    std::unordered_map<std::string, GetDepthRep *> mPidGetDepthRep;

//    std::unordered_map<std::string,std::unordered_map<std::string, AllOdersSymbol *>> mAllOdersFname;
//    std::unordered_map<int32_t,std::unordered_map<std::string, AllOdersSymbol *>> mAllOdersFname;

    ROWMarket *getRowmarket(const std::string &pid);
    GetDepthRep * getDepthRep(const std::string &playerid);

    std::unordered_map<QWebSocket *, std::vector<std::string>> mSocketSubscribed;
    std::unordered_map<std::string, std::set<QWebSocket *>> mFnameSubscribed;

//    std::unordered_map< std::pair<std::string, std::string> , std::vector<Order *>>  reservedOrder;
//    std::unordered_map< std::pair<std::string, std::string> , std::vector<AllOdersSymbol *>>  reserveAllOdersSymbol;

//    std::unordered_map< std::string, std::unordered_map<std::string, AllOdersSymbol *>>  fname2symbolAllOrderss;

    std::unordered_map< std::string, AllOdersFname *>  fname2sAllOdersFname;

    std::unordered_map< AllOdersSymbol *, std::stack<Order *>>  openOrderSlots;

    std::unordered_map< AllOdersFname *, std::stack<AllOdersSymbol *>>  openOrderSymbolSlot;

    std::unordered_map< pair<string,string> ,AllOdersSymbol *>  fnamesymbolAllOrders;

    std::unordered_map<int32_t,Order *> mSeqOrderMap;


    void getFnameSnap(const std::string &fname);
    Order *addOrder(AllOdersSymbol *allords, Order *orderin);
    AllOdersSymbol *getAllOdersSymbol(AllOdersFname *aofp, const std::string &symbol);
    AllOdersFname *getAllOdersFname(const std::string &fname);
};

#endif //LITE_SERVER_H
