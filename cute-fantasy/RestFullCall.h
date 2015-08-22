#ifndef RESTFULLCALL_H
#define RESTFULLCALL_H

#include <QObject>
#include <QSharedDataPointer>
#include <QNetworkAccessManager>
#include <QThread>
#include <QNetworkReply>
#include <QEventLoop>
#include "globals.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <google/protobuf/message.h>
#include <QJsonArray>
#include "ProtoData.pb.h"
#include <QDebug>


class RestfullClient : public QObject{

    Q_OBJECT
public:
    enum RestfullCallPostDataType {
        Binary,
        Json,
        String
    };

    enum RestfullCallTextEncoding {
        UTF8,
        Latin1,//ISO 8859-1, use it for ascii
        Local8Bits // convert to 8 bits according to the os local
    };

private:

    class PostDataDecorator   {
    public:
        PostDataDecorator(QNetworkAccessManager * manager, QNetworkRequest * request){
            myRequest = request;
            myNam = manager;
        }

        QNetworkReply * postBinaryData(const QString & contentType,const QByteArray & buffer){
            return postRawData(contentType,RestfullCallPostDataType::Binary,buffer);
        }

        QNetworkReply * postBinaryData(const QString & contentType,const char * data,int size){
            return postRawData(contentType,RestfullCallPostDataType::Binary,QByteArray(data,size));
        }

        QNetworkReply * postJsonData(const QString & contentType,const QJsonDocument & jsonDocString){
           return postGenericStringData(contentType,RestfullCallPostDataType::Json,QString(jsonDocString.toJson(QJsonDocument::Compact)),UTF8);
        }

        QNetworkReply * postStringData(const QString & contentType,const QString & str,RestfullCallTextEncoding encoding){
           return postGenericStringData(contentType,RestfullCallPostDataType::String,str,encoding);
        }

    private:
        QNetworkRequest * myRequest;
        QNetworkAccessManager * myNam;

        QNetworkReply * postRawData(const QString & contentType,
                          RestfullCallPostDataType pType,
                          const QByteArray & postData) {

            myRequest->setHeader(QNetworkRequest::ContentTypeHeader,contentType);
            myRequest->setHeader(QNetworkRequest::ContentLengthHeader,postData.length());
            return myNam->post(*myRequest,postData);
        }

        QNetworkReply * postGenericStringData(const QString & contentType,RestfullCallPostDataType pType,
                                const QString & theString,
                                RestfullCallTextEncoding encoding){
            QByteArray buffer;
            switch (encoding) {
            case RestfullCallTextEncoding::UTF8:  buffer = theString.toUtf8(); break;
            case RestfullCallTextEncoding::Latin1: buffer = theString.toLatin1(); break;
            case RestfullCallTextEncoding::Local8Bits: buffer = theString.toLocal8Bit(); break;
            default:
                break;
            }
            return postRawData(contentType,pType,buffer);
        }
    };

public:

    RestfullClient(const QUrl & argBaseUrl,QThread * ownerThread = QThread::currentThread()){
        if (ownerThread != QThread::currentThread())
            this->moveToThread(ownerThread);
        myCurrentNetworkReply = NULL;
        myBaseUrl = argBaseUrl;
    }

    void postProtoMessageData(const QString & route,const QString & contentType,const google::protobuf::Message & protoMessage){
        std::string data = protoMessage.SerializeAsString().data();
        return postRawData(route,contentType,data.data(),data.size());
    }

    void postRawData(const QString & route,const QString & contentType,const QByteArray & postData){
        QNetworkRequest request;
        restNetworkStatus();
        request.setUrl(QUrl(myBaseUrl.toString()+"/"+route));
        PostDataDecorator decorator(&myNetworkManager,&request);
        myCurrentNetworkReply = decorator.postBinaryData(contentType,postData);
        waitForReply();
        myCurrentNetworkReply->deleteLater();
        processReplyData();
    }

    void postRawData(const QString & route,const QString & contentType,const char * data,int size){
        QNetworkRequest request;
        restNetworkStatus();
        request.setUrl(QUrl(myBaseUrl.toString()+"/"+route));
        PostDataDecorator decorator(&myNetworkManager,&request);
        myCurrentNetworkReply = decorator.postBinaryData(contentType,data,size);
        waitForReply();
        myCurrentNetworkReply->deleteLater();
        processReplyData();
    }

    void postJasonData(const QString & route,const QString & contentType,const QJsonDocument & jsonDoc){
        QNetworkRequest request;
        restNetworkStatus();
        request.setUrl(QUrl(myBaseUrl.toString()+"/"+route));
        PostDataDecorator decorator(&myNetworkManager,&request);
        myCurrentNetworkReply = decorator.postJsonData(contentType,jsonDoc);
        waitForReply();
        myCurrentNetworkReply->deleteLater();
        processReplyData();
    }

    void postStringData(const QString & route,const QString & contentType,const QString & text,
                        RestfullCallTextEncoding encoding){
        QNetworkRequest request;
        restNetworkStatus();
        request.setUrl(QUrl(myBaseUrl.toString()+"/"+route));
        PostDataDecorator decorator(&myNetworkManager,&request);
        myCurrentNetworkReply = decorator.postStringData(contentType,text,encoding);
        waitForReply();
        myCurrentNetworkReply->deleteLater();
        processReplyData();
    }

    void getData(const QString & route,
                          const QMap<QString,QVariant> parameters,
                          const QMap<QString,QString> headersMap)  {
        QNetworkRequest request;
        restNetworkStatus();
        //construct url with parameters
        QString url = myBaseUrl.toString()+"/"+route;
        if (parameters.count()> 1) url+="/";

        foreach (QString paramName, parameters.keys()) {
            url+=QString("%1?%2").arg(paramName).arg(parameters.value(paramName).toString());
        }
        request.setUrl(QUrl(url));

        //add headers
        foreach (QString headerKey, headersMap.keys()) {
            request.setRawHeader(headerKey.toUtf8(),headersMap.value(headerKey).toUtf8());
        }

        myCurrentNetworkReply = myNetworkManager.get(request);
        waitForReply();
        myCurrentNetworkReply->deleteLater();
        processReplyData();
    }

    QByteArray lastReply() { return myLastRepliedData; }


protected:
    virtual void processReplyData(){
        qDebug("response : %s",myLastRepliedData.data());
    }

private slots:

    void finishedSlot(){}

    void sslErrorsSlot(QList<QSslError> errors) {
        myCurentSSLErrors.clear();
        myCurentSSLErrors.append(errors);
    }

    void networkErrorSlot(QNetworkReply::NetworkError error ) {
        myCurrentNetworkError = error;
    }

    void slotReadyRead(){
        myLastRepliedData.clear();
        myLastRepliedData = myCurrentNetworkReply->readAll();
    }

private:
    /**
      * @brief restNetworkStatus : reset network errors and status.
      */
    void restNetworkStatus(){
        myCurentSSLErrors.clear();
        myCurrentNetworkError = QNetworkReply::NoError;
        if (myCurrentNetworkReply != NULL)
            myCurrentNetworkReply->close();
        DELETE_AND_NULLIFY(myCurrentNetworkReply);
    }

    /**
       * @brief waitForReply : loop while waiting for asynchronus network reply to be available.
       * or a network error to happen.
       */
    void waitForReply(){
        QEventLoop loop;

        connect(myCurrentNetworkReply,SIGNAL(finished()),
                this, SLOT(finishedSlot()));
        connect(myCurrentNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(networkErrorSlot(QNetworkReply::NetworkError)));
        connect(myCurrentNetworkReply, SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(sslErrorsSlot(QList<QSslError>)));
        connect(myCurrentNetworkReply, SIGNAL(readyRead()),
                this, SLOT(slotReadyRead()));

        connect(myCurrentNetworkReply,SIGNAL(finished()),
                &loop, SLOT(quit()));
        connect(myCurrentNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)),
                &loop, SLOT(quit()));
//        connect(myCurrentNetworkReply, SIGNAL(readyRead()),
//                &loop, SLOT(quit()));
        loop.exec();
    }




    QNetworkAccessManager  myNetworkManager;
    /**
     * @brief myCurrentNetworkReply : current network reply getting processed
     */
    QNetworkReply * myCurrentNetworkReply;

    /**
     * @brief myLastRepliedData : last network captured reply.
     */
    QByteArray myLastRepliedData;

    /**
     * @brief myCurrentNetworkError : last network error
     */
    QNetworkReply::NetworkError  myCurrentNetworkError;

    /**
     * @brief myCurentSSLErrors : last ssl error
     */
    QList<QSslError> myCurentSSLErrors;

    /**
     * @brief myBaseUrl : base url
     */
    QUrl myBaseUrl;
};


class RestfullService {
public:
    static fantasybit::Block retrieveBlock(const QString & baseUrl,const QString route,int blockNum,QThread * ownerThread = QThread::currentThread()){
        RestfullClient client(QUrl(baseUrl),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;
        //hard coded url
        //TODO move to settings
        QString customRoute("%1/block/%3");
        customRoute = customRoute.arg(route).arg(blockNum);
        client.getData(customRoute,params,headers);

        fantasybit::Block blk;
        auto arrby = client.lastReply();
        QJsonDocument ret = QJsonDocument::fromJson(arrby);


        qDebug() << ret.isNull() << ret.isEmpty() << ret.isArray() << ret.isObject();

        QJsonArray qa = ret.array();
        QJsonValueRef json = qa[0];

        QString str(json.toObject().value("data").toString());

        qDebug() << str;

        auto arrayblock =
                QByteArray::fromBase64(str.toUtf8());

        blk.ParseFromArray(arrayblock,arrayblock.size());

        return blk;
    }

    static int getHeight(const QString & baseUrl, QThread * ownerThread = QThread::currentThread()) {
        RestfullClient client(QUrl(baseUrl),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;
        //hard coded url
        //TODO move to settings
        QString customRoute("block-height");
        client.getData(customRoute,params,headers);

        auto arrby = client.lastReply();
        qDebug() << " retunr " << arrby <<"!!!";
        QJsonDocument ret = QJsonDocument::fromJson(arrby);

        qDebug() << ret.isNull() << ret.isEmpty() << ret.isArray() << ret.isObject();


        QJsonArray qa = ret.array();
        QJsonValueRef json = qa[0];


        //qDebug() <<
        int hi = json.toObject().value("height").toInt();
        //int hi = val.toInt();

        return hi;
    }

    static QByteArray sendBlock(const QString & baseUrl,const QString route,
                                int height,const QString& blockID,std::string block /* you can use Message*/,
                                QThread * ownerThread = QThread::currentThread()){
        QJsonDocument doc;
        QJsonObject obj;
        obj.insert("height",QJsonValue(height));
        obj.insert("blockID",QJsonValue(blockID));
        QByteArray blob(block.data(),block.size()) ;
        obj.insert("data",QJsonValue(QString(blob.toBase64())));
        doc.setObject(obj);
        RestfullClient client(QUrl(baseUrl),ownerThread);
        client.postJasonData(route,"application/json",doc);
        return client.lastReply();
    }

    static std::string getTx(const QString & baseUrl,
                                   QThread * ownerThread = QThread::currentThread()){
        RestfullClient client(QUrl(baseUrl),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;
        //hard coded url
        //TODO move to settings
        QString customRoute("/tx");
        //customRoute = customRoute.arg(route).arg(blockNum);
        client.getData(customRoute,params,headers);

        return client.lastReply().toStdString();
    }


};

#endif // RESTFULLCALL_H
