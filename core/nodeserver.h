/*
//  nodeserver.h
//
//  Protoblock
//
//  Created by Jay Berg on 5/23/2018
//
*/

#ifndef NODESERVER_H
#define NODESERVER_H

#include <QList>
#include <QTcpServer>

class NodeClient;

class NodeServer : public QTcpServer
{
    Q_OBJECT

public:
    inline NodeServer() {}
    static NodeServer *instance();

    void addNodeClient(NodeClient *client);

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

private slots:
    void removeClient();
//    void processInfoHash(const QByteArray &infoHash);

private:
    NodeClient  *nodeclient;
};

#endif
