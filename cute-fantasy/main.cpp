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

using namespace fantasybit;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<OutData>("OutData");
    qRegisterMetaType<InData>("InData");
    sfGUI widget;

    QThread clientthread;
    Client *client = new Client {"ipc:///tmp/test.ipc"};
    client->moveToThread(&clientthread);
    QObject::connect(&clientthread, SIGNAL(finished()), client, SLOT(deleteLater()));
    QObject::connect(&widget,SIGNAL(fromGUI(InData)), client, SLOT(toServer(InData)));
    QObject::connect(client, SIGNAL(onData(OutData)), &widget, SLOT(fromServer(OutData)) );
    QObject::connect(&widget, SIGNAL(onClose()), client, SLOT(quit()) );

    clientthread.start();
    widget.show();

    return a.exec();
}
