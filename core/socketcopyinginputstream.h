/*
//  socketcopyinginputstream.h
//
//  Protoblock
//
//  Created by Jay Berg on 5/17/2018
//
*/
#ifndef SOCKETCOPYINGINPUTSTREAM_H
#define SOCKETCOPYINGINPUTSTREAM_H
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <QTcpSocket>

class SocketCopyingInputStream : public google::protobuf::io::CopyingInputStream {
    QTcpSocket *_sock;

public:
    SocketCopyingInputStream(QTcpSocket *insock) : _sock(insock) {}

//    int Skip(int count) override {
//        return _sock->skip(count);
//    }

    int Read(void * buffer,int size) override {
        int ret = _sock->read((char *)buffer,size);
        qDebug() << " SocketCopyingInputStream read " << size << "read " << ret;
        return ret;
    }
};


class SocketCopyingOutputStream : public google::protobuf::io::CopyingOutputStream {
    QTcpSocket *_sock;

public:
    SocketCopyingOutputStream(QTcpSocket *insock) : _sock(insock) {}

//    int Skip(int count) override {
//        return _sock->skip(count);
//    }

    bool Write(const void * buffer,int size) override {
        qDebug() << " inside Write " << size;
        return _sock->write((char *)buffer,size);
    }
};
#endif // SOCKETCOPYINGINPUTSTREAM_H
