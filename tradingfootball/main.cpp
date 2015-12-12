//
//  main.cpp
//  cute-fantasy
//
//  Created by Jay Berg on 4/8/14.
//
//
#include "globals.h"
#include <iostream>
#include "RunGuard.h"
#include "mainwindow.h"
#include "core.h"
#include <QDesktopWidget>

#ifdef DATAAGENTGUI
    #include "testingwindow.h"
#endif

#include <QMetaType>

//using namespace fantasybit;

//Q_DECLARE_METATYPE(fantasybit::GlobalState)
//Q_DECLARE_METATYPE(fantasybit::MyFantasyName)
//Q_DECLARE_METATYPE(fantasybit::FantasyBitProj)
//Q_DECLARE_METATYPE(vector<fantasybit::MyFantasyName>)

#include "playerloader.h"

int domain(int argc, char *argv[]){

    RunGuard guard( "tradingfootballprodapp" );
        if ( !guard.tryToRun() )
            return 0;

    QApplication a(argc, argv);


    //MikeClayLoader loader;
    //loader.loadProjFromLink(14);
    //Commissioner::makeGenesisBlock();
    //return 0;
    /*
    SqlStuff sql;
    int i = sql.getpidT("cfc93f5e-105e-4a5e-88d3-f4279893cfa8");
    i = sql.maxPid();
    */

    //ExchangeData ed;
    //ed.init();
    //ExchangeOrder eo;
    //eo.
    //ed.OnNewOrderMsg();
    //return 0;

    Core::instance()->bootstrap();
    MainWindow form;

    if ( false ){
    Node node;
    node.init();
    node.Cleaner();
    return 0;
    }

#ifdef DATAAGENTGUI
    TestingWindow dform;
#endif

    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-form.width()) / 2;
    int y = (screenGeometry.height()-form.height()) / 2;
    form.move(x, y);
    form.show();

#ifdef DATAAGENTGUI
    dform.show();
#endif

    return a.exec();
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



