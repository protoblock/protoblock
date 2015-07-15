//
//  client.h
//  cute-fantasy
//
//  Created by Jay Berg on 4/8/14.
//
//

#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <nn.hpp>
#include <MsgSock.h>
#include <ProtoData.pb.h>
#include <string>
#include <QTimer>
#include <QEvent>

namespace fantasybit
{

class Client : public QObject
{
    std::string address;
    Q_OBJECT
    nn::socket sock;
    Receiver receiver;
    QTimer * timer, *timer2;
    InData indata{};
    DeltaData outdata{};
    InData hbdata{};
public:
    explicit Client(const std::string &,QObject *parent = 0);

    virtual bool event(QEvent *ev);

    ~Client()
    {
        nn::term();
    }

signals:
    void onData(const DeltaData &data);
    void doPoll();

public slots:
    void toServer(const InData &in);
    void pollServer();
    void heartbeat();
    void start();
    void quit();
};

}

#endif // CLIENT_H
