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

using namespace fantasybit;
int main(int argc, char *argv[])
{
    std::string ipc{"ipc:///tmp/"};
    std::string arg = ( argc > 1) ? argv[1] :  "temp";
    ipc+= arg + ".ipc";
    ipc = "ipc:///tmp/temp.ipc";
    //std::string arg = ( argc > 1) ? argv[1] :  "56633";
    //ipc = "tcp://127.0.0.1:" + arg;
    std::cout << " using " << ipc << "\n";

    QApplication a(argc, argv);

    qRegisterMetaType<OutData>("OutData");
    qRegisterMetaType<InData>("InData");
    sfGUI widget;

    QThread clientthread;
    Client *client = new Client {ipc,arg};
    client->moveToThread(&clientthread);
    QObject::connect(&clientthread, SIGNAL(finished()), client, SLOT(deleteLater()));
    QObject::connect(&widget,SIGNAL(fromGUI(InData)), client, SLOT(toServer(InData)));
    QObject::connect(client, SIGNAL(onData(OutData)), &widget, SLOT(fromServer(OutData)) );
    QObject::connect(&widget, SIGNAL(onClose()), client, SLOT(quit()) );


    clientthread.start();
    widget.show();

    return a.exec();
}
