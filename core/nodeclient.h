/*
//  nodeclient.h
//
//  Protoblock
//
//  Created by Jay Berg on 5/21/2018
//
*/
#ifndef NODECLIENT_H
#define NODECLIENT_H
#include <QNetworkAccessManager>
#include <QObject>
#include <QNetworkReply>

class NodeClientPrivate;

class NodeClient : public QObject
{
    Q_OBJECT
public:
    explicit NodeClient(QObject *parent = 0);


//    std::string getMyIp();

//    ~NodeClient();
    void getMyIp();
signals:
    void tryGetIp();
    void gotPublicIp();

public slots:
    void startPoint();

private:
    NodeClientPrivate *d;
    friend class NodeClientPrivate;

    QNetworkAccessManager http;
    QNetworkReply *qreply;

    std::vector<std::string> ipUrls;
    QString myIp;
    bool gotMyIp = false;
    int ipUrlstry = 0;
};

#endif // NODECLIENT_H
