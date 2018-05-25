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
    explicit PeerWire(QObject *parent = 0);


    fantasybit::Peer *peer() const { return m_peer;}

    QHostAddress setSocketDescriptor(qintptr socketDescriptor) {
        bool ret =  m_socket.setSocketDescriptor(socketDescriptor);
        if ( ret )
            m_startTime = (std::chrono::duration_cast<std::chrono::milliseconds>
                           (std::chrono::system_clock::now().time_since_epoch()).count());
        qDebug() << " setSocketDescriptor " << m_socket.peerAddress() << m_socket.peerPort();
        return m_socket.peerAddress();
    }

    void connectToHost() {
        m_socket.connectToHost(peer()->address().data(), peer()->port());
    }

    void diconnectFromHost() {}

    void init(fantasybit::Peer *peer);

    void onReadyReady();

    void doWrite(const fantasybit::WireMsg &msg);

    static bool readDelimitedFrom(
        google::protobuf::io::ZeroCopyInputStream* rawInput,
        google::protobuf::MessageLite* message);

    static bool writeDelimitedTo(
       const google::protobuf::MessageLite& message,
       google::protobuf::io::ZeroCopyOutputStream* rawOutput);



    void sendIntro();
signals:

protected:
    void timerEvent(QTimerEvent *event) override;


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
    int keepAliveTimer;

    uint64_t m_startTime;
    fantasybit::Peer *m_peer;

    QTcpSocket m_socket;
};



#endif
