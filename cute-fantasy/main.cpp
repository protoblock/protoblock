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
#include "PeerNode.h"
#include "playerloader.h"
#include "testingwindow.h"

using namespace fantasybit;
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



