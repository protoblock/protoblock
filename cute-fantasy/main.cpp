//
//  main.cpp
//  cute-fantasy
//
//  Created by Jay Berg on 4/8/14.
//
//
#include "globals.h"
#include <iostream>
#include "singleapplication.h"
#include "mainwindow.h"
#include "core.h"
#include "testingwindow.h"
#include <QMetaType>

//using namespace fantasybit;

//Q_DECLARE_METATYPE(fantasybit::GlobalState)
//Q_DECLARE_METATYPE(fantasybit::MyFantasyName)
//Q_DECLARE_METATYPE(fantasybit::FantasyBitProj)
//Q_DECLARE_METATYPE(vector<fantasybit::MyFantasyName>)



int domain(int argc, char *argv[]){
    SingleApplication a(argc, argv);

    Core::instance()->bootstrap();
    MainWindow form;

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



