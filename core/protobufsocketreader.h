/*
//  protobufsocketreader.h.h
//
//  Protoblock
//
//  Created by Jay Berg on 5/17/2018
//
*/
#ifndef PROTOBUFSOCKETREADER_H
#define PROTOBUFSOCKETREADER_H

#include <QObject>
#include <QTcpSocket>

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/message.h>

#include <P2PData.pb.h>
#include <stdint.h>

#include <socketcopyinginputstream.h>
#include <qthread.h>
#include <qabstracteventdispatcher.h>



class PBData : public QSharedData {

};

namespace pb {

class ProtobufSocketReader : public QObject
{
    Q_OBJECT


public:
    ProtobufSocketReader(QObject *parent = 0) {
        m_socket = new QTcpSocket(this);


        connect(m_socket, &QTcpSocket::connected, this, &ProtobufSocketReader::onConnected);

        connect(m_socket, &QTcpSocket::readyRead, this, &ProtobufSocketReader::onReadyReady);
//        connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
    }

//    ProtobufSocketReader(const ProtobufSocketReader &);
//    ProtobufSocketReader &operator=(const ProtobufSocketReader &);
//    ~ProtobufSocketReader();

signals:

public slots:

private:
//    QSharedDataPointer<ProtobufSocketReaderData> data;


    google::protobuf::io::CopyingInputStreamAdaptor *_CopyingInputStreamAdaptor = nullptr;
    google::protobuf::io::CodedInputStream *_CodedInputStream = nullptr;
    SocketCopyingInputStream *_SocketCopyingInputStream;
    fantasybit::Peer msg;

public:
    QTcpSocket *m_socket;

    void connectTO(std::string peer) {
        qDebug() << "QTcpSocket::connectto" << peer.data();
        m_socket->connectToHost(peer.data(),10130);
    }

    void sendTo() {
        qDebug() << " xxx sendto";

    }

    void onConnected() {
        qDebug() << "ProtobufSocketReader connected ";
    }

    void onReadyReady()
    {
        qDebug() << "ProtobufSocketReader onReadyReady " << m_socket->bytesAvailable() << sizeof(uint32_t);

        SocketCopyingInputStream ssis(m_socket);
        google::protobuf::io::CopyingInputStreamAdaptor cisa(&ssis);

        for (;;)
         {
             fantasybit::Peer myMessage;
             if ( !readDelimitedFrom(&cisa, &myMessage) )
                 return;
             qDebug() << myMessage.DebugString().data();
         }

        //Some maximum size of a message to protect from malformed
        //packets or bugs. This value should be above the size of the largest
        //protobuf message you ever expect to receive.
        static const uint32_t MAX_ALLOWED_SIZE = 1024 * 1024; //1MB

        uint32_t msgSize;
        while ( m_socket->bytesAvailable() >= sizeof(msgSize) )
        {
    //        char buffer[4];
    //        m_socket->peek(buffer,4);

    //        google::protobuf::uint32 size;
    //        google::protobuf::io::ArrayInputStream ais(buffer,4);
    //        google::protobuf::io::CodedInputStream coded_input(&ais);
    //        coded_input.ReadVarint32(&size);//Decode the HDR and get the size
    //        qDebug() <<"size of payload is "<<size;
    //        msgSize = size + coded_input.CurrentPosition();
    //        qDebug() << " msgSize " << msgSize;

            //OK, we have read in a 32-bit message size, now verify that
            //it is valid before we try to read that many bytes on the socket

            if ( msgSize > MAX_ALLOWED_SIZE )
            {
                //You might throw an exception here or do some other error
                //handling. Like most Qt code, though, this app doesn't
                //use exceptions, so we handle a malformed packet by just
                //closing the socket. In a real app you'd probably at least
                //want to do some logging here, as this should be a very
                //abnormal case and signifies a bug on the sending end.
                m_socket->close();
                break;
            }

            //Now check to see if we have that many bytes available to read in.
            //If we do, we have at least one full message waiting to process, if not,
            //we'll process the message on the next call to onReadyRead.
            if ( m_socket->bytesAvailable() >= msgSize )
            {
                _SocketCopyingInputStream = new SocketCopyingInputStream(m_socket);
                _CopyingInputStreamAdaptor = new google::protobuf::io::CopyingInputStreamAdaptor(
                          _SocketCopyingInputStream );
                //_CopyingInputStreamAdaptor->SetOwnsCopyingStream(true);
                google::protobuf::io::CodedInputStream mm(_CopyingInputStreamAdaptor);
                _CodedInputStream = &mm;//new google::protobuf::io::CodedInputStream(_CopyingInputStreamAdaptor);

                qDebug() << " got all bytes " << m_socket->bytesAvailable();

                auto limit = _CodedInputStream->PushLimit(msgSize);
                uint32_t size;
                bool ret = _CodedInputStream->ReadVarint32(&size);
                qDebug() << " ok noew read" << ret << size;
                // Parse the message.
                 if (!msg.MergeFromCodedStream(_CodedInputStream)) return;
                 if (!_CodedInputStream->ConsumedEntireMessage()) return;

                 // Release the limit.
                 _CodedInputStream->PopLimit(limit);
                return;

                 ret = msg.ParseFromCodedStream(_CodedInputStream);
                qDebug() << " ok noew ret" << ret;

                _CodedInputStream->PopLimit(limit);
                qDebug() << msg.DebugString().data();

    //            m_socket->read(reinterpret_cast<char*>(&msg_size), sizeof(msg_size));
    //            QScopedArrayPointer<char> buffer(new char[msgSize]);
    //            m_socket->read(&buffer[0], msgSize);
    //            processMessage(buffer, msgSize);
            }
            else
            {
                break;
            }
        }
    }

    bool readDelimitedFrom(
        google::protobuf::io::ZeroCopyInputStream* rawInput,
        google::protobuf::MessageLite* message) {
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
};


class ProtobufSocketWriter : public QObject
{
    Q_OBJECT


public:
    ProtobufSocketWriter(QObject *parent = 0) {}
public slots:
    void setTcp(QTcpSocket *in) {
        m_socket = in;
    }

//        _CopyingOutputStreamAdaptor = new google::protobuf::io::CopyingOutputStreamAdaptor(
//                    new SocketCopyingOutputStream(m_socket));
////        _CopyingOutputStreamAdaptor->SetOwnsCopyingStream(true);
//        _CodedOutputStream = new google::protobuf::io::CodedOutputStream(_CopyingOutputStreamAdaptor);


//        connect(m_socket, &QTcpSocket::readyRead, this, &ProtobufSocketReader::onReadyReady);
//        connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),


//    ProtobufSocketReader(const ProtobufSocketReader &);
//    ProtobufSocketReader &operator=(const ProtobufSocketReader &);
//    ~ProtobufSocketReader();

signals:

public slots:

private:
//    QSharedDataPointer<ProtobufSocketReaderData> data;


    google::protobuf::io::CopyingOutputStreamAdaptor *_CopyingOutputStreamAdaptor = nullptr;
    google::protobuf::io::CodedOutputStream *_CodedOutputStream = nullptr;
    fantasybit::Peer msg;

public:
    QTcpSocket *m_socket;

    void sendTo()
    {
        SocketCopyingOutputStream scos(m_socket);
        google::protobuf::io::CopyingOutputStreamAdaptor cos_adp(&scos);

        int i = 0;
        do {
            {
            fantasybit::Peer myMessage;
            myMessage.set_address("hello world");
//            myMessage.set_lastsync(i);
            if (!writeDelimitedTo(myMessage, &cos_adp) ) return;
            // Now we have to flush, otherwise the write to the socket won't happen until enough bytes accumulate
            cos_adp.Flush();
            QThread::currentThread()->eventDispatcher()->processEvents(QEventLoop::AllEvents);
            }
        } while (i++ < 100000);

        return;

        /*
        msg.set_address("test send");
        std::string msgs =  "test 0";
        for ( int i =0; i<10; i++)
            msgs.append(" test " + std::to_string(i));
        msg.set_address(msgs);

//        char buffer[4000];
//        google::protobuf::io::ArrayOutputStream aos(buffer,4000);
//        _CodedOutputStream = new google::protobuf::io::CodedOutputStream(&aos);

        _CodedOutputStream->WriteVarint32(msg.ByteSize());
        qDebug() << "first aos bt" << _CodedOutputStream->ByteCount() << msg.ByteSize();

        uint8_t* buffer = _CodedOutputStream->GetDirectBufferForNBytesAndAdvance(msg.ByteSize());
        if (buffer != NULL) {
            // Optimization:  The message fits in one buffer, so use the faster
            // direct-to-array serialization path.
            msg.SerializeWithCachedSizesToArray(buffer);
        }
        else {
            // Slightly-slower path when the message is multiple buffers.
            msg.SerializeWithCachedSizes(_CodedOutputStream);
            if (_CodedOutputStream->HadError()) return;
        }
        qDebug() << " aos bt" << _CodedOutputStream->ByteCount() << msg.ByteSize() << _CopyingOutputStreamAdaptor->ByteCount();

        _CopyingOutputStreamAdaptor->Flush();
        return;

        if ( !msg.SerializeToCodedStream(_CodedOutputStream) )
            qDebug() << " serial err";
        else
            qDebug() << " aos bt" << _CodedOutputStream->ByteCount() << msg.ByteSize() << _CopyingOutputStreamAdaptor->ByteCount();

        _CopyingOutputStreamAdaptor->Flush();
        qDebug() << " aos bt" << _CodedOutputStream->ByteCount() << msg.ByteSize() << _CopyingOutputStreamAdaptor->ByteCount();
        delete _CopyingOutputStreamAdaptor;
//        delete _CodedOutputStream;
//        m_socket->write(buffer,_CodedOutputStream->ByteCount());

//        uint32_t msgSize;
//        memcpy(reinterpret_cast<char*>(&msgSize), buffer, sizeof(msgSize));
//        qDebug() << msgSize;

//        google::protobuf::uint32 size;
//        google::protobuf::io::ArrayInputStream ais(buffer,4);
//        google::protobuf::io::CodedInputStream coded_input(&ais);
//        coded_input.ReadVarint32(&size);//Decode the HDR and get the size
//        qDebug() <<"size of payload is "<<size << ais.ByteCount() << coded_input.CurrentPosition();
        //return size;

//        QByteArray qb;
 //       qb.append()


        */

    }

    bool writeDelimitedTo(
           const google::protobuf::MessageLite& message,
           google::protobuf::io::ZeroCopyOutputStream* rawOutput) {
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
};

}

//https://caffe2.ai/doxygen-c/html/proto__utils_8cc_source.html

#endif // PROTOBUFSOCKETREADER_H
