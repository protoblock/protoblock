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
    //loader.loadProjFromFile();
    //Commissioner::makeGenesisBlock();
    //return 0;
    /*
    SqlStuff sql;
    int i = sql.getpidT("cfc93f5e-105e-4a5e-88d3-f4279893cfa8");
    i = sql.maxPid();
    */
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



