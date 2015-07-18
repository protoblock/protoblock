//
//  main.cpp
//  cute-fantasy
//
//  Created by Jay Berg on 4/8/14.
//
//

#include "sfgui.h"
#include <QApplication>
#include "client.h"
#include <iostream>
#include "Node.h"
#include "Server.h"
#include "ClientUI.h"
#include "Processor.h"

#define LOG(logger, severity) LOGIT(logger, severity,  __FILE__, __LINE__, __FUNCTION__)

using namespace fantasybit;
int main(int argc, char *argv[])
{
    string gui_address{ "ipc:///tmp/fantasygui.ipc" };


    Node node{};
    //node.getMyIp();
    node.init();
    node.runHandShake();

    string delta_address{ "ipc://deltaserver" };

    string server_address{ "ipc://rcpserver" };
    Server server{ server_address };;

    ClientUI clientui{ server_address, gui_address, delta_address };

    thread servert{ &Server::run, &server };
    thread clientt{ &ClientUI::run, &clientui };

    thread syncRequest_{ &Node::syncRequest, &node };
    thread syncService_{ &Node::syncService, &node };

    BlockProcessor processor{ delta_address };
    thread processor_{ &BlockProcessor::run, &processor };

    thread runLive_{ &Node::runLive, &node };
    thread pendingTransactions_{ &Node::pendingTransactions, &node };

    //std::string ipc{"ipc:///tmp/fantasygui.ipc"};
    QApplication a(argc, argv);

    qRegisterMetaType<DeltaData>("DeltaData");
    qRegisterMetaType<InData>("InData");
    sfGUI widget;

    QThread clientthread;
    Client *client = new Client {gui_address};
    client->moveToThread(&clientthread);
    QObject::connect(&clientthread, SIGNAL(finished()), client, SLOT(deleteLater()));
    QObject::connect(&widget,SIGNAL(fromGUI(InData)), client, SLOT(toServer(InData)));
    QObject::connect(client, SIGNAL(onData(DeltaData)), &widget, SLOT(fromServer(DeltaData)) );
    QObject::connect(client, SIGNAL(onData(DeltaData)), &widget, SLOT(refreshViews(DeltaData)) );
    QObject::connect(&widget, SIGNAL(onClose()), client, SLOT(quit()) );


    clientthread.start();
    widget.show();
    int ret = a.exec();


    clientui.stop();
    nn::term();
   /**/
    server.stop();

   LOG(lg, debug) << " 4 ";

    node.stop();
    LOG(lg, debug) << " 5 ";

    processor.stop();
    //nn::term();
    LOG(lg, debug) << " 6 ";

    clientt.join();
    servert.join();
    syncRequest_.join();
    syncService_.join();
    runLive_.join();
    pendingTransactions_.join();

    return ret;
}
