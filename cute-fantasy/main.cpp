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

#include "globals.h"
#include <iostream>
#include "singleapplication.h"
#include "LAPIWorker.h"
#include "threadedqobject.h"
#include "TestCoreGUIForm.h"
#include "core.h"



int domain(int argc, char *argv[]){

    SingleApplication a(argc, argv);
    Core::instance()->bootstrap();

    ThreadedQObject<MainLAPIWorker> coreApi;
    coreApi.thread()->connect(coreApi.thread(),
                              SIGNAL(started()),
                              coreApi.object(),
                              SLOT(startPoint()));

    coreApi.thread()->start();
    TestCoreGUIForm form(coreApi.object());
    form.show();
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



