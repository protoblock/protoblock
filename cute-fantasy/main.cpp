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

int domain(int argc, char *argv[]){

    SingleApplication a(argc, argv);
    Core::instance()->bootstrap();
    MainWindow form;
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



