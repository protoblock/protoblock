#ifndef SERVER_H
#define SERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QWebSocketServer>

#include "NameData.h"
#include "StateData.pb.h"
//#include "ldbhashreadertool.h"
//#include "pbjson.hpp"

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



    QStringList createCommandArgument(const QString &cmd);


    fantasybit::FantasyNameData mNameData;


    bool verify_name(const fantasybit::SignedTransaction &st, const NameTrans &nt,
                     const secp256k1_ecdsa_signature &sig, const pb::sha256 &digest);
    std::shared_ptr<FantasyName> getFNverifySt(const SignedTransaction &st);

    static fantasybit::GetAllNamesRep AllNamesRep;
Q_SIGNALS:
    void closed();
    void error(QString);

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
    void handleError(const QString err);
private:
    QString m_errorString;
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
//    LdbHashReaderTool *pLdbHashReaderTool;
    bool m_debug;
    fantasybit::BlockMeta bm;
    std::string forPbJSon;
    quint16 mport;


};

#endif //ECHOSERVER_H
