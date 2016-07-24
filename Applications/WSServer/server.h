#ifndef SERVER_H
#define SERVER_H

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

using namespace fantasybit;
QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(quint16 port, bool debug = false, QObject *parent = Q_NULLPTR);
    ~Server();
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

    int32_t mySeq = 0;

    QStringList createCommandArgument(const QString &cmd);




    fantasybit::FantasyNameData mNameData;
#ifdef PROD_SEASON_TRADING
    fantasybit::NFLStateData mNFLStateData;
#endif

    bool verify_name(const fantasybit::SignedTransaction &st, const NameTrans &nt,
                     const secp256k1_ecdsa_signature &sig, const pb::sha256 &digest);
    std::shared_ptr<FantasyName> getFNverifySt(const SignedTransaction &st);

    static fantasybit::GetAllNamesRep AllNamesRep;
//    fantasybit::GetAllNamesRep AllNamesRep2;
    fantasybit::GetAllNamesRep *AllNamesRepPtr;

    fantasybit::GetROWMarketRep ROWMarketRep;
    std::unordered_map<std::string, ROWMarket *> mPidROWMarket;

Q_SIGNALS:
    void closed();
    void error(QString);

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(const QByteArray &message);
    void socketDisconnected();
    void handleError(const QString err);
private:
    void processBinaryTxMessage(const QByteArray &message);
    QString m_errorString;
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
//    LdbHashReaderTool *pLdbHashReaderTool;
    bool m_debug;
    fantasybit::BlockMeta bm;
    std::string forPbJSon;
    quint16 mport;
    std::string mRepstr;
    const uint MaxNames = ((MAX_NAMES_LB + 0) > 10 ) ? (MAX_NAMES_LB + 0) : std::numeric_limits<int>::max();

};

#endif //ECHOSERVER_H
