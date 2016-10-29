#ifndef PROTOBLOCKAPI_H
#define PROTOBLOCKAPI_H

#include <QObject>
#include <QStringList>
#include <QNetworkSession>
#include<QNetworkAccessManager>
#include<QTcpServer>


QT_BEGIN_NAMESPACE
class QTcpServer;
class QNetworkSession;
QT_END_NAMESPACE

class ProtoBlobkApi : public QObject
{
    Q_OBJECT
public:
    explicit ProtoBlobkApi(QObject *parent = 0);

private slots:
    void sessionOpened();
    void sendFortune();

private:
    QTcpServer *tcpServer;
    QStringList fortunes;
    QNetworkSession *networkSession;
};


#endif
