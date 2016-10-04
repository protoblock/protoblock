#ifndef TX_SERVER_H
#define TX_SERVER_H

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


class TxServer : public QObject
{
    Q_OBJECT
public:
    explicit TxServer(quint16 port, bool debug = false, QObject *parent = Q_NULLPTR);
    ~TxServer();
    int32_t mySeq = 0; //todo

    bool verify_name(const fantasybit::SignedTransaction &st, const NameTrans &nt,
                     const secp256k1_ecdsa_signature &sig, const pb::sha256 &digest);
    std::shared_ptr<FantasyName> getFNverifySt(const SignedTransaction &st);


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
    QList<QWebSocket *> m_clients;
    quint16 mport;
    bool m_debug;
    fantasybit::GetAllNamesRep *AllNamesRepPtr;
//    fantasybit::FantasyNameData mNameData;


    const uint MaxNames = ((MAX_NAMES_LB + 0) > 10 ) ? (MAX_NAMES_LB + 0) : std::numeric_limits<int>::max();

};

#endif //TX_SERVER_H
