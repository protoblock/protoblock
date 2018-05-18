/*
//  testserver.h
//
//  Protoblock
//
//  Created by Jay Berg on 5/17/2018
//
*/
#ifndef TESTSERVER_H
#define TESTSERVER_H
#include <QTcpServer>
#include <protobufsocketreader.h>

namespace  pb {

class TestServer : public QTcpServer {
     Q_OBJECT

public:
    TestServer(QObject *parent = 0) {}
//        connect(this,&TestServer::newConnection,this, &TestServer::onNewConnection);
//        qDebug() << "TestServer this";
//    }

//    void incomingConnection(qintptr socketDescriptor) override {
//        qDebug() << "incoming connection";
//    }

    void onXXNewConnection() {
        qDebug() << "TestServer onNewConnection connection";
        socket = this->nextPendingConnection();
        qDebug() << " connected to "  << socket->peerAddress().toString();

//        _ProtobufSocketWriter = new ProtobufSocketWriter(socket,this);
        //_ProtobufSocketWriter->sendTo();
//        connect(this,&TestServer::sendTo,_ProtobufSocketWriter,&ProtobufSocketWriter::sendTo);
        emit connected(socket);
    }

//    void send() {
//        emit sendTo(socket);
//    }

public slots:
    void start() {
        listen(QHostAddress::Any, 10130);
    }

signals:
    void connected(QTcpSocket *in);


//{
//        qDebug() << isListening() << " sendTo ";

////        _ProtobufSocketWriter->sendTo();
//    }
public:
    QTcpSocket *socket;
    ProtobufSocketWriter *_ProtobufSocketWriter;

protected:
//    QTcpSocket *nextPendingConnection();

};

}
#endif // TESTSERVER_H
