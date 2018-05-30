/*
//  peerwire.cpp
//
//  Protoblock
//
//  Created by Jay Berg on 5/21/2018
//
*/
#include "PeerWire.h"
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/message.h>

#include <socketcopyinginputstream.h>
#include "P2PData.pb.h"
#include "node.h"
#include <QTimerEvent>

using namespace  fantasybit;
using namespace  pb;

static const int ConnectTimeout = 8000;//60 * 1000;
static const int ClientTimeout = 10000;//120 * 1000;
//static const int ActionTimeout = 10000;


PeerWire::PeerWire(PeerWireState state, QObject *parent)
    : mPWstate(state), QObject(parent) {}

bool PeerWire::setSocketDescriptor(qintptr socketDescriptor) {
    bool ret =  m_socket.setSocketDescriptor(socketDescriptor);
    if ( !ret ) {
        qDebug() << " PeerWire::setSocketDescriptor " << ret;
    }
    else {
//        mSessionId.set_start_time(int(QDateTime::currentDateTime().toTime_t())- GENEIS_EPOCH);
        qDebug() << " setSocketDescriptor " << m_socket.peerAddress() << m_socket.peerPort();
        m_peer->set_address(m_socket.peerAddress().toString().toStdString());
        SocketConnected ();
    }

    return ret;
}

void PeerWire::init() {
//    mNextAction = None;
    connect(this, &PeerWire::NewAction, this, &PeerWire::OnNewAction, Qt::QueuedConnection);

    inittime = (std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::system_clock::now().time_since_epoch()).count());

    connect(&m_socket, &QTcpSocket::stateChanged, this, &PeerWire::SocketStateChange);

    connect(&m_socket, &QTcpSocket::connected, this, &PeerWire::SocketConnected);
    connect(&m_socket, &QTcpSocket::readyRead, this, &PeerWire::onReadyRead);

    connect(&m_socket, &QTcpSocket::disconnected, this, &PeerWire::SocketDisconnected);
    connect(&m_socket, &QTcpSocket::hostFound, this, &PeerWire::SocketHostFound);
//    connect(&m_socket, &QTcpSocket::error, this, &PeerWire::OnSocketError);
    connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(OnSocketError(QAbstractSocket::SocketError)));


    qDebug() << "init " <<  m_peer->address().data() << "PeerWire::socketStateChanged" << m_socket.state() << mPWstate << inittime;
    m_timeoutTimer = startTimer(ConnectTimeout);
//    mActionTimer = startTimer(ActionTimeout);
}

void PeerWire::SocketHostFound() {
    qDebug() << "PeerWire::SocketHostFound Found Host" << peer()->address ().data () << mPWstate;
}

void PeerWire::OnSocketError(QAbstractSocket::SocketError err) {
    if ( err == QAbstractSocket::ConnectionRefusedError) {
        if ( m_timeoutTimer ) {
            killTimer(m_timeoutTimer);
            emit OnTimeout();
        }
    }
    qDebug() << "PeerWire::OnSocketError" << peer()->address ().data () << mPWstate << err;
}

void PeerWire::SocketStateChange(QAbstractSocket::SocketState state) {
    qDebug() << "PeerWire::SocketStateChange" << peer()->address ().data ();

    if ( this->m_peer != NULL )
        qDebug() << inittime << "PeerWire::socketStateChanged" << state << mPWstate;
    else
        qDebug() << "null PeerWire::socketStateChanged" << state << mPWstate;
}

void PeerWire::SocketConnected() {

    if ( m_timeoutTimer )
        killTimer(m_timeoutTimer);

    if ( mSessionId.start_time () == 0 ) {
        mSessionId.set_start_time (
            (int(QDateTime::currentDateTime().toTime_t())-GENEIS_EPOCH));

        emit OnConnected ();
    }

//    if ( m_peer != NULL )  {
//        m_peer->set_is_listening(Peer::YES);
//        mSessionId.set_start_time(int(QDateTime::currentDateTime().toTime_t())-GENEIS_EPOCH);
//        qDebug() << m_peer->address().data() << "PeerWire::connected" << mPWstate;
//        if (!m_sendIntro && m_socket.state() == QAbstractSocket::ConnectedState)
//            sendIntro();
//    }
//    else
//        qDebug() << "NULL PeerWire::connected";

}

void PeerWire::SocketDisconnected () {
    if ( m_timeoutTimer )
        killTimer(m_timeoutTimer);

    emit OnDisconnected ();
}

void PeerWire::NatTimeout()
{
    emit NewAction(PeerWire::NatNo);
}



void PeerWire::OnNewAction(DoAction act)
{
    qDebug() << " PeerWire::OnnewAction " << act;
    if ( act == Intro ) {
        sendIntro(false);
    }
    else if ( act == Disconnect) {
        diconnectFromHost();
    }
    else if ( act == NatIntro) {
        sendIntro(true);
    }
    else if ( act == NatNo ) {
        sendNatNo();
    }

}

void PeerWire::setPeer(fantasybit::Peer *peer) {
    m_peer = peer;
}

void PeerWire::sendNatNo()
{
    if (m_sendIntro || m_socket.state() != QAbstractSocket::ConnectedState) {
        qDebug() << " sendNatNo " << m_socket.state();
        return;
    }

    WireMsg wiremsg;
    wiremsg.set_type(MsgType::INTRO);

    MsgIntro &natmsg = *wiremsg.mutable_intro();
    natmsg.mutable_youare()->CopyFrom(mInIntro.iam());
    natmsg.mutable_iam()->CopyFrom(mInIntro.youare());
    natmsg.mutable_iam()->mutable_session_id()->set_uuid("NAT");
    natmsg.mutable_youare()->mutable_peer()->set_is_listening(Peer::NO);
    doWrite(wiremsg);
}

void PeerWire::sendIntro(bool nattest)
{
    if (m_sendIntro || m_socket.state() != QAbstractSocket::ConnectedState) {
        qDebug() << " sendIntro " << m_socket.state();
        return;
    }

    m_sendIntro = true;

    // Restart the timeout
//    m_timeoutTimer = startTimer(ClientTimeout);

    WireMsg wiremsg;
    wiremsg.set_type(MsgType::INTRO);

    MsgIntro *intro = &mOutIntro;
    WirePeer *me = intro->mutable_iam();
    SessionId *sid = me->mutable_session_id();
    sid->CopyFrom(Node::mSessionId);
    me->mutable_peer()->CopyFrom(Node::mPeer);
    if ( nattest )
        sid->set_uuid (sid->uuid () + "-NAT");

    intro->mutable_youare()->mutable_peer ()->CopyFrom (*m_peer);
    SessionId *psid = intro->mutable_youare()->mutable_session_id();
    psid->CopyFrom (mSessionId);
//    psid->set_start_time(m_startTime);

    wiremsg.set_allocated_intro (intro);
    doWrite(wiremsg);

    wiremsg.release_intro ();
}

void PeerWire::onReadyRead()
{
    qDebug() << "ProtobufSocketReader onReadyRead ";// << m_socket.bytesAvailable() << m_peer->address().data() << mPWstate;

    SocketCopyingInputStream ssis(&m_socket);
    google::protobuf::io::CopyingInputStreamAdaptor cisa(&ssis);

    int size = 0;
    for (;;)
    {
        fantasybit::WireMsg myMessage;
        size = readDelimitedFrom(&cisa, &myMessage);
        if  (size == -1) {
            qDebug() << "onReadyReady no more data read" << inittime;//m_peer->address().data();
            return;
        }
        else {
            qDebug() << "onReadyReady" << inittime;
            //m_peer->address().data() << myMessage.type() << " flags " << mPWstate;
            emit NewWireMsg(myMessage);
        }
    }
}

void PeerWire::doWrite(const WireMsg &msg)
{
    SocketCopyingOutputStream scos(&m_socket);
    google::protobuf::io::CopyingOutputStreamAdaptor cos_adp(&scos);

    qDebug() << "doWrite sending " << msg.DebugString ().data ();
    if (!writeDelimitedTo(msg, &cos_adp) ) {
        qDebug() << " error in write ";
        return;
    }
    // Now we have to flush, otherwise the write to the socket won't happen until enough bytes accumulate
    cos_adp.Flush();
}

void PeerWire::timerEvent(QTimerEvent *event) {
//    if ( m_peer != NULL )
//        qDebug() << m_peer->DebugString().data() << " PeerWire::timerEvent " << m_socket.state();
//    else
//        qDebug() << "NULL PeerWire::timerEvent " << m_socket.state();

    if ( event->timerId() == m_timeoutTimer ) {
        qDebug() << " m_timeoutTimer ";

        if ( m_socket.state() == QAbstractSocket::UnconnectedState) {
            if ( m_timeoutTimer ) {
                killTimer(m_timeoutTimer);
                emit OnTimeout();
            }
        }

        qDebug() << m_peer->address().data() << m_socket.state() << mPWstate << inittime;


//        if ( mNextAction == Intro ) {
//            mNextAction = None;
//            sendIntro();
//        }
//        else if ( mNextAction == IntroThenDisconnect) {
//            mNextAction = Disconnect;
//            sendIntro();
//        }
//        else if ( mNextAction == Disconnect) {
//            mNextAction = None;
//            diconnectFromHost();
//        }
    }
}

int PeerWire::readDelimitedFrom(google::protobuf::io::ZeroCopyInputStream *rawInput, google::protobuf::MessageLite *message) {

    // We create a new coded stream for each message.  Don't worry, this is fast,
    // and it makes sure the 64MB total size limit is imposed per-message rather
    // than on the whole stream.  (See the CodedInputStream interface for more
    // info on this limit.)
    google::protobuf::io::CodedInputStream input(rawInput);

    // Read the size.
    uint32_t size;
    if (!input.ReadVarint32(&size)) return -1;

    if ( size == 0 ) return -1;

    // Tell the stream not to read beyond that size.
    google::protobuf::io::CodedInputStream::Limit limit =
            input.PushLimit(size);

    // Parse the message.
    if (!message->MergeFromCodedStream(&input)) return -1;
    if (!input.ConsumedEntireMessage()) return -1;

    // Release the limit.
    input.PopLimit(limit);

    return size;
}

bool PeerWire::writeDelimitedTo(const google::protobuf::MessageLite &message, google::protobuf::io::ZeroCopyOutputStream *rawOutput) {

    // We create a new coded stream for each message.  Don't worry, this is fast.
    google::protobuf::io::CodedOutputStream output(rawOutput);

    // Write the size.
    const int size = message.ByteSize();
    output.WriteVarint32(size);

    uint8_t* buffer = output.GetDirectBufferForNBytesAndAdvance(size);
    if (buffer != NULL) {
        // Optimization:  The message fits in one buffer, so use the faster
        // direct-to-array serialization path.
        message.SerializeWithCachedSizesToArray(buffer);
    }
    else {
        // Slightly-slower path when the message is multiple buffers.
        message.SerializeWithCachedSizes(&output);
        if (output.HadError()) return false;
    }

    return true;
}
