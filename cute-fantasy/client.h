#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

#include <nn.hpp>
#include <MsgSock.h>
#include <fb/ProtoData.pb.h>
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
    OutData outdata{};
    InData hbdata{};
    //QThread thread;
public:
    explicit Client(const std::string &,QObject *parent = 0);

    virtual bool event(QEvent *ev);

    ~Client()
    {
        nn::term();
    }

signals:
    void onData(const OutData &data);
    void doPoll();

public slots:
    void fromGUI(const QString &in);

    void toServer(const InData &in);

    void pollServer();

    void heartbeat();

    void start();

    void quit();
};

}

#endif // CLIENT_H
