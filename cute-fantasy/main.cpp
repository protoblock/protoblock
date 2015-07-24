//
//  main.cpp
//  cute-fantasy
//
//  Created by Jay Berg on 4/8/14.
//
//

#include "dataagentui.h"
#include "fantasyplayerui.h"
#include "sfgui.h"
#include "client.h"
#include <iostream>
#include "singleapplication.h"
#include "gamesoftheweekdialog.h"
#include "Node.h"
#include "Server.h"

#include "ClientUI.h"
#include "Processor.h"

#define LOG(logger, severity) LOGIT(logger, severity,  __FILE__, __LINE__, __FUNCTION__)

using namespace fantasybit;

int domain(int argc, char *argv[]) {

    initBoostLog();
    string gui_address{ "inproc://fantasygui" };


    Node node{};
    //node.getMyIp();

    try {
        node.init();
        node.runHandShake();
    }
    catch (std::exception &e)
    {
        LOG(lg,fatal) <<  e.what();
        return -1;
    }

    string delta_address{ "inproc://deltaserver" };

    string server_address{ "inproc://rcpserver" };
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


    //sfGUI widget;
#ifdef DATAAGENTGUI
    DataAgentUI widget;
#else
    FantasyPlayerUI widget;
#endif

    QThread clientthread;
    Client *client = new Client {gui_address};
    client->moveToThread(&clientthread);
    QObject::connect(&clientthread, SIGNAL(finished()), client, SLOT(deleteLater()));
    QObject::connect(&widget,SIGNAL(fromGUI(InData)), client, SLOT(toServer(InData)));
    QObject::connect(client, SIGNAL(onData(DeltaData)), &widget, SLOT(fromServer(DeltaData)) );
    //QObject::connect(client, SIGNAL(onData(DeltaData)), &widget, SLOT(refreshViews(DeltaData)) );
    QObject::connect(&widget, SIGNAL(onClose()), client, SLOT(quit()) );


    clientthread.start();
    widget.show();
    int ret = a.exec();


    //QThread::sleep(5);
//    dlg.show();


    clientui.stop();
    //nn::term();
   /**/
    server.stop();
    node.stop();
    processor.stop();
    nn::term();

    //QThread::sleep(5);

    clientt.join();
    servert.join();
    syncRequest_.join();
    syncService_.join();
    runLive_.join();
    pendingTransactions_.join();    
    return ret;
}

int main(int argc, char *argv[])
{
    try {
        domain(argc, argv);
    }
    catch (...) {
        return -1;
    }
}

