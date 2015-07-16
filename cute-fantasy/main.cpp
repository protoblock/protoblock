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
    std::string ipc{"ipc:///tmp/fantasygui.ipc"};
    QApplication a(argc, argv);

    qRegisterMetaType<DeltaData>("DeltaData");
    qRegisterMetaType<InData>("InData");
    sfGUI widget;

    QThread clientthread;
    Client *client = new Client {ipc};
    client->moveToThread(&clientthread);
    QObject::connect(&clientthread, SIGNAL(finished()), client, SLOT(deleteLater()));
    QObject::connect(&widget,SIGNAL(fromGUI(InData)), client, SLOT(toServer(InData)));
    QObject::connect(client, SIGNAL(onData(DeltaData)), &widget, SLOT(fromServer(DeltaData)) );
    QObject::connect(client, SIGNAL(onData(DeltaData)), &widget, SLOT(refreshViews(DeltaData)) );
    QObject::connect(&widget, SIGNAL(onClose()), client, SLOT(quit()) );


    clientthread.start();
    widget.show();

    return a.exec();
}
