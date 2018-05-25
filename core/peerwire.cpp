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

using namespace  fantasybit;
using namespace  pb;

static const int ConnectTimeout = 30000;//60 * 1000;
static const int ClientTimeout = 120 * 1000;


PeerWire::PeerWire(QObject *parent) : QObject(parent) {}

void PeerWire::init(fantasybit::Peer *peer) {
    m_peer = peer;

    connect(&m_socket, &QTcpSocket::stateChanged, this,
            [&](QAbstractSocket::SocketState state){
        if ( m_peer != NULL )
            qDebug() << m_peer->DebugString().data();
        qDebug() << "PeerWire::socketStateChanged" << state;
    });

    connect(&m_socket, &QTcpSocket::connected, this,
            [&](){
        if ( m_peer != NULL )  {
            m_peer->set_is_listening(Peer::YES);
            qDebug() << m_peer->DebugString().data() << "PeerWire::connected";
            if (!m_sendIntro && m_socket.state() == QAbstractSocket::ConnectedState)
                sendIntro();
        }
        else
            qDebug() << "NULL PeerWire::connected";
    });

    connect(&m_socket, &QTcpSocket::readyRead, this, &PeerWire::onReadyReady);


    m_timeoutTimer = startTimer(ConnectTimeout);
    if (!m_sendIntro && m_socket.state() == QAbstractSocket::ListeningState)
        sendIntro();
}

void PeerWire::sendIntro()
{
    m_sendIntro = true;

    // Restart the timeout
    if (m_timeoutTimer)
        killTimer(m_timeoutTimer);
    m_timeoutTimer = startTimer(ClientTimeout);

    WireMsg wiremsg;
    wiremsg.set_type(MsgType::INTRO);

    MsgIntro *intro = wiremsg.mutable_intro();
    WirePeer *me = intro->mutable_iam();
    SessionId *sid = me->mutable_session_id();
    sid->CopyFrom(Node::mSessionId);
    me->set_allocated_peer(&Node::mPeer);

    intro->mutable_youare()->set_allocated_peer(m_peer);
    SessionId *psid = intro->mutable_youare()->mutable_session_id();
    psid->set_start_time(m_startTime);

    doWrite(wiremsg);

    me->release_peer();
}

void PeerWire::onReadyReady()
{
    qDebug() << "ProtobufSocketReader onReadyReady " << m_socket.bytesAvailable() << sizeof(uint32_t);

    SocketCopyingInputStream ssis(&m_socket);
    google::protobuf::io::CopyingInputStreamAdaptor cisa(&ssis);

    for (;;)
    {
        fantasybit::WireMsg myMessage;
        if ( !readDelimitedFrom(&cisa, &myMessage) )
            return;
        qDebug() << m_peer->DebugString().data() << myMessage.DebugString().data();
    }
}

void PeerWire::doWrite(const WireMsg &msg)
{
    SocketCopyingOutputStream scos(&m_socket);
    google::protobuf::io::CopyingOutputStreamAdaptor cos_adp(&scos);

    if (!writeDelimitedTo(msg, &cos_adp) ) return;
    // Now we have to flush, otherwise the write to the socket won't happen until enough bytes accumulate
    cos_adp.Flush();
}

void PeerWire::timerEvent(QTimerEvent *event) {
    if ( m_peer != NULL )
        qDebug() << m_peer->DebugString().data() << " PeerWire::timerEvent " << m_socket.state();
    else
        qDebug() << "NULL PeerWire::timerEvent " << m_socket.state();
}

bool PeerWire::readDelimitedFrom(google::protobuf::io::ZeroCopyInputStream *rawInput, google::protobuf::MessageLite *message) {

    // We create a new coded stream for each message.  Don't worry, this is fast,
    // and it makes sure the 64MB total size limit is imposed per-message rather
    // than on the whole stream.  (See the CodedInputStream interface for more
    // info on this limit.)
    google::protobuf::io::CodedInputStream input(rawInput);

    // Read the size.
    uint32_t size;
    if (!input.ReadVarint32(&size)) return false;

    if ( size == 0 ) return false;
    // Tell the stream not to read beyond that size.
    google::protobuf::io::CodedInputStream::Limit limit =
            input.PushLimit(size);

    // Parse the message.
    if (!message->MergeFromCodedStream(&input)) return false;
    if (!input.ConsumedEntireMessage()) return false;

    // Release the limit.
    input.PopLimit(limit);

    return true;
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
