/*
//  main.cpp
//
//  Protoblock
//
//  Created by Jay Berg on 5/8/2018
//
*/
//#include <QCoreApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include <nng/nng.h>
#include <nng/protocol/reqrep0/rep.h>
#include <nng/protocol/reqrep0/req.h>

#include <ws2tcpip.h>

#include "peer.h"
#include "mesh.h"

#ifndef _WIN32
#include <arpa/inet.h>blo
#endif
#ifdef _WIN32
#include <windows.h>
#else
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#endif
#include <QHostAddress>
#include <iostream>

#include "console.h"
#include <protobufsocketreader.h>
#include <testserver.h>
#include <threadedqobject.h>

#include <QNetworkInterface>
#include <QDateTime>

#include <nodeclient.h>
pb::mesh myMesh;

using namespace pb;
//static const char *
//ip4tostr(void *addr)
//{
//    static char buf[256];

//#ifdef _WIN32
//    return (const char *)(InetNtop(AF_INET, addr, (PWSTR)buf, sizeof(buf)));

//#else
//    return (inet_ntop(AF_INET, addr, buf, sizeof(buf)));

//#endif
//}

//static const char *
//ip6tostr(void *addr)
//{
//    static char buf[256];

//#ifdef _WIN32
//    return (const char *)(InetNtop(AF_INET6, addr, (PWSTR)buf, sizeof(buf)));

//#else
//    return (inet_ntop(AF_INET6, addr, buf, sizeof(buf)));

//#endif
//}
/*
void notify(nng_pipe p, nng_pipe_action act, void *arg) {
    qDebug() << "server  notify ";

    switch (act) {
    case NNG_PIPE_ADD:
        qDebug() << "add";
        {
        nng_sockaddr obuf;
        nng_pipe_getopt_sockaddr(p,NNG_OPT_REMADDR,&obuf);
        qDebug() << NNG_OPT_REMADDR << obuf.s_family;
        nng_sockaddr_in6 in = obuf.s_in6;
//        std::string address = ip4tostr(&in);

        QHostAddress qha(in.sa_addr);
        auto s1 = qha.toString();

        QHostAddress qha2(qha.toIPv4Address());
        auto s2 = qha2.toString();

        myMesh.onNewSynced(qha2.toString().toStdString());

        }
        break;
    case NNG_PIPE_REM:
        qDebug() << "remove";
        break;
    default:
        return;
    }

    {
        nng_sockaddr obuf;
        nng_pipe_getopt_sockaddr(p,NNG_OPT_REMADDR,&obuf);
        qDebug() << obuf.s_family;

        nng_sockaddr_in6 in = obuf.s_in6;
        qDebug() << NNG_OPT_REMADDR << in.sa_addr[12] << "." <<  in.sa_addr[13] << "." <<  in.sa_addr[14] << "." << in.sa_addr[15];
    }
    {
        char* obuf;
        nng_pipe_getopt_string(p,NNG_OPT_URL,&obuf);
        qDebug() << NNG_OPT_URL << "option::" << obuf << ":: done";
    }
    {
        nng_sockaddr obuf;
        nng_pipe_getopt_sockaddr(p,NNG_OPT_LOCADDR,&obuf);
        qDebug() << NNG_OPT_LOCADDR << obuf.s_family;

        nng_sockaddr_in6 in = obuf.s_in6;
        qDebug() << NNG_OPT_LOCADDR << in.sa_addr[12] << "." <<  in.sa_addr[13] << "." <<  in.sa_addr[14] << "." << in.sa_addr[15];

    }

    {
        int obuf;
        nng_pipe_getopt_int(p,NNG_OPT_PEER,&obuf);
        qDebug() << NNG_OPT_PEER << "option::" << obuf << ":: done";
    }
    {
        char* obuf;
        nng_pipe_getopt_string(p,NNG_OPT_PEERNAME,&obuf);
        qDebug() << NNG_OPT_PEERNAME << "option::" << obuf << ":: done";
    }
    {
        char* obuf;
        nng_pipe_getopt_string(p,NNG_OPT_SOCKNAME,&obuf);
        qDebug() << NNG_OPT_SOCKNAME << "option::" << obuf << ":: done";
    }
    qDebug() << "";

}

int domain1(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << " live compile protoblock core!! ";

    pb::peer myPeer;
    myPeer.init();

    myMesh.init();
    myMesh.recvAio();

    nng_socket   sock;
    nng_rep0_open_raw(&sock);
    nng_pipe_notify(sock, notify, NULL);
    nng_listener listener;
    int rv = nng_listen(sock, "tcp://[]:10128", &listener, 0);

    Console console;
    console.run();
    QObject::connect(&console, SIGNAL(quit()), &a, SLOT(quit()));
    QObject::connect(&console, &Console::command, &myMesh, &pb::mesh::connectTO);
    QObject::connect(&console, &Console::sendto, &myMesh, &pb::mesh::putto);

    return a.exec();
}
*/
int main(int argc, char *argv[]) {
    QGuiApplication a(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/main.qml"));

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    foreach (const QNetworkInterface &netInterface, QNetworkInterface::allInterfaces()) {
        QNetworkInterface::InterfaceFlags flags = netInterface.flags();
        if( (bool)(flags & QNetworkInterface::IsRunning) && !(bool)(flags & QNetworkInterface::IsLoopBack)){
            foreach (const QNetworkAddressEntry &address, netInterface.addressEntries()) {
                if(address.ip().protocol() == QAbstractSocket::IPv4Protocol)
                    qDebug() << address.ip().toString();
            }
        }
    }

//    QTcpSocket dnsTestSocket;
//     QString localIP="127.0.0.1";    //fall back
//     QString googleDns = "8.8.8.83";  //try google DNS or sth. else reliable first
//     dnsTestSocket.connectToHost(googleDns, 53);
//     if (dnsTestSocket.waitForConnected(3000))
//     {
//         localIP = dnsTestSocket.localAddress().toString();
//     }
//     else
//     {
//         foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
//         {
//             QString guessedGatewayAddress = address.toString().section( ".",0,2 ) + ".1";

//             if (address.protocol() == QAbstractSocket::IPv4Protocol
//                 && address != QHostAddress(QHostAddress::LocalHost)
//                 )
//             {
//                 dnsTestSocket.connectToHost(guessedGatewayAddress, 53);
//                 if (dnsTestSocket.waitForConnected(3000))
//                 {
//                     localIP = dnsTestSocket.localAddress().toString();
//                     break;
//                 }
//             }
//         }
//     }

//     qDebug() <<localIP;

     ThreadedQObject<NodeClient> node;

     QObject::connect(node.thread(),SIGNAL(started()),node.object(),SLOT(startPoint()));

     node.thread()->start();
     QObject::connect(node.thread(),&QThread::finished,node.object(),&QObject::deleteLater);

//     Console console2;
//     QObject::connect(&console2, SIGNAL(quit()), &a, SLOT(quit()));
//     console2.run();

//     nc.getMyIp("http://ss.me/ip");

//     for ( auto str : {
//           "http://api.ipify.org/",
//           "http://myexternalip.com/raw",
//           "http://icanhazip.com/",
//           "http://myip.dnsomatic.com/",
//           "http://ifcaonfig.me/ip"} )

//         nc.getMyIp(str);

//     http://api.ipify.org/
//     http://myexternalip.com/raw
//     http://icanhazip.com/
//     http://myip.dnsomatic.com/
//     http://ifconfig.me/ip

     qDebug() << "main" << QThread::currentThreadId();


     QObject::connect(&a, SIGNAL(lastWindowClosed()),
                      node.thread(), SLOT(quit()));
     QObject::connect(node.thread(),&QThread::finished,node.object(),&QObject::deleteLater);


//    QObject::connect(&a, &QGuiApplication::lastWindowClosed, node.thread(), &QThread::quit );


    return a.exec();
#ifdef XXXX

    ThreadedQObject<Console> console;
    QObject::connect(console.thread(),SIGNAL(started()),console.object(),SLOT(run()));
    console.thread()->start();

    QObject::connect(console.object(), SIGNAL(quit()), &a, SLOT(quit()));


#ifndef CLIENT
    pb::ProtobufSocketReader psr;
    QObject::connect(console.object(), &Console::command, &psr, &pb::ProtobufSocketReader::connectTO);
#else
    ThreadedQObject<TestServer> server;
    QObject::connect(server.thread(),SIGNAL(started()),server.object(),SLOT(start()));
    server.thread()->start();

    pb::ProtobufSocketWriter psw;


    //QObject::connect(&server, &pb::TestServer::newConnection, &server, &pb::TestServer::sendTo);
    QObject::connect(server.object(), &pb::TestServer::newConnection, server.object(), &pb::TestServer::onXXNewConnection);
    QObject::connect(console.object(), &Console::sendto, &psw, &pb::ProtobufSocketWriter::sendTo);

    QObject::connect(server.object(), &pb::TestServer::connected, &psw, &pb::ProtobufSocketWriter::setTcp);

#endif

    qRegisterMetaType<std::string>("std::string");

    return a.exec();

#endif
}
