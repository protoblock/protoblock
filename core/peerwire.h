/*
//  peerwire.h
//
//  Protoblock
//
//  Created by Jay Berg on 5/21/2018
//
*/

#ifndef PEERWIRE_H
#define PEERWIRE_H

#include <QBitArray>
#include <QTcpSocket>
#include <QHostAddress>
#include "P2PData.pb.h"
#include <chrono>





//https://cmiclab.cs.ucl.ac.uk/mespak/NiftyLink/tree/master/Code/Libraries/NetworkQt
class PeerWire : public QObject
{
    Q_OBJECT

public:
    enum PeerWireStateFlag {
        _none_ = 0x0,
        Outgoing = 0x1,
        Incoming = 0x2
    };
    Q_DECLARE_FLAGS(PeerWireState, PeerWireStateFlag)

    enum DoAction {
        None,
        Intro,
        IntroThenDisconnect,
        Hello,
        Disconnect
    };
    Q_ENUM(DoAction)

//    void setAction(DoAction in) {
//        mNextAction = in;
//        qDebug() << " new action " << in;
//    }

    PeerWire(PeerWireState state, QObject *parent = 0);
    fantasybit::Peer *peer() const { return m_peer;}

    QHostAddress setSocketDescriptor(qintptr socketDescriptor);

    void connectToHost() {
        m_socket.connectToHost(peer()->address().data(), peer()->port());
    }

    void diconnectFromHost() {
//        m_socket.abort();
        killTimer(mActionTimer);
        m_socket.disconnectFromHost();
    }

    void init();

    void onReadyRead();

    void doWrite(const fantasybit::WireMsg &msg);

    static int readDelimitedFrom(
        google::protobuf::io::ZeroCopyInputStream* rawInput,
        google::protobuf::MessageLite* message);

    static bool writeDelimitedTo(
       const google::protobuf::MessageLite& message,
       google::protobuf::io::ZeroCopyOutputStream* rawOutput);

    inline PeerWireState PeerState() const { return mPWstate; }
    void SetPeerState(PeerWireState in) { mPWstate = in; }


    void sendIntro();
    void setPeer(fantasybit::Peer *peer);

    fantasybit::SessionId mSessionId;
    fantasybit::MsgIntro mInIntro;
    fantasybit::MsgIntro mOutIntro;

//    std::string mKnownpeer;

signals:
    void NewWireMsg(const fantasybit::WireMsg &);
    void NewAction(DoAction act);

protected:
    void timerEvent(QTimerEvent *event) override;

public slots:
    void SocketStateChange(QAbstractSocket::SocketState state);
    void SocketConnected();

    void OnNewAction(DoAction act);

private slots:
//    void sendHandShake();
//    void processIncomingData();
//    void socketStateChanged(QAbstractSocket::SocketState state);

private:
    // Data waiting to be read/written
    QByteArray incomingBuffer;
    QByteArray outgoingBuffer;

    bool receivedHandShake;
    bool m_sendIntro = false;

    // Timeout handling
    int m_timeoutTimer;
    int pendingRequestTimer;
    bool invalidateTimeout;
    int mActionTimer;

    //state
    PeerWireState mPWstate;

//    uint64_t m_startTime;
    fantasybit::Peer *m_peer;

//    DoAction mNextAction;
    uint64_t inittime;
    QTcpSocket m_socket;
};

Q_DECLARE_METATYPE(fantasybit::WireMsg)
Q_DECLARE_OPERATORS_FOR_FLAGS(PeerWire::PeerWireState)


#endif
