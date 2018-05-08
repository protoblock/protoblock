/*
//  main.cpp
//
//  Protoblock
//
//  Created by Jay Berg on 5/8/2018
//
*/
#include <QCoreApplication>
#include <QDebug>

#include "peer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << " live compile protoblock core!! ";

    pb::peer myPeer;
    myPeer.init();
    return a.exec();
}
