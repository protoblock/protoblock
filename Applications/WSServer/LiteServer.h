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

using namespace fantasybit;
QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

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

    ROWMarket *getRowmarket(const std::string &pid);
    GetDepthRep * getDepthRep(const std::string &playerid);

};

#endif //LITE_SERVER_H
