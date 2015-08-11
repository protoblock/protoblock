//
//  main.cpp
//  cute-fantasy
//
//  Created by Jay Berg on 4/8/14.
//
//

#ifdef DATAAGENTGUI
#include "dataagentui.h"
#elif Jay2015PrePreSeasonTestDemo
#include "demoui.h"
#else
#include "fantasyplayerui.h"
#endif

#include "sfgui.h"
#include "client.h"
#include <iostream>
#include "singleapplication.h"
#include "gamesoftheweekdialog.h"
#include "PeerNode.h"
#include "Processor.h"
#include "ClientUI.h"
#include "Processor.h"
#include "LAPIWorker.h"
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>


#define LOG(logger, severity) LOGIT(logger, severity,  __FILE__, __LINE__, __FUNCTION__)


int domain(int argc, char *argv[]) {

    LOG(lg,trace) << GET_ROOT_DIR();

    QApplication a(argc, argv);

#ifdef DATAAGENTGUI
//    if (! createConnection()) {
//        LOG(lg,error) << "db errror";
//        return -1;
//    }
#endif

    string gui_address{ "inproc://fantasygui" };



    //BlockRecorder mRecorder{};


    string delta_address{ "inproc://deltaserver" };

    string server_address{ "inproc://rcpserver" };

    ClientUI clientui{ server_address, gui_address, delta_address };


    //std::string ipc{"ipc:///tmp/fantasygui.ipc"};




    //qRegisterMetaType<DeltaData>("DeltaData");
    //qRegisterMetaType<InData>("InData");


    //sfGUI widget;
#ifdef DATAAGENTGUI
    DataAgentUI widget;
#elif Jay2015PrePreSeasonTestDemo
        DemoUI widget;
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

    //QThread::sleep(5);

    return ret;
}

int domain2(int argc, char *argv[]) {
    string gui_address{ "inproc://fantasygui" };

    QApplication a(argc, argv);


    qRegisterMetaType<DeltaData>("DeltaData");
    qRegisterMetaType<InData>("InData");


#ifdef DATAAGENTGUI
    DataAgentUI widget;
#elif Jay2015PrePreSeasonTestDemo
    DemoUI widget;
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

    return ret;
}

#include "TestCoreGUIForm.h"
#include <QWaitCondition>
QWaitCondition waitForGUI;
int domain1(int argc, char *argv[]){
    initBoostLog();
    QApplication a(argc, argv);
    ThreadedObject<MainLAPIWorker> coreApi;
    coreApi.thread()->connect(coreApi.thread(),
                                 SIGNAL(started()),
                                 coreApi.object(),
                                 SLOT(startPoint()));

    coreApi.thread()->start();
    TestCoreGUIForm form(coreApi.object(),&waitForGUI);
    form.show();
    return a.exec();
}


int main(int argc, char *argv[])
{
    //Jay2015PrePreSeasonTestDemo();
    //initBoostLog();


    try {
        domain1(argc, argv);
    }
    catch (...) {
        return -1;
    }
}



