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
#include <sstream>
#include "fbutils.h"

/**
 * @brief The RestfullClient class : This is NOT a thread safe class.
 */
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
            Q_ASSERT(manager != NULL && request != NULL);
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
            if (myNam->networkAccessible()==QNetworkAccessManager::Accessible){
#ifdef TRACE
                qDebug() << "Post : " << myRequest->url().toDisplayString();
#endif
                return myNam->post(*myRequest,postData);
            }
            else {
                qDebug()<< "No network connection !";
                return NULL;
            }

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
        //make sure we move the network access manager and the event loop
        // the the thread we're going to operate within.
        myNetworkManager.setParent(this);
        myEventLoop.setParent(this);
        if (ownerThread != QThread::currentThread())
            this->moveToThread(ownerThread);

        myBaseUrl = argBaseUrl;
        myCurrentNetworkReply = NULL;

        //init waiting signals connection
        //when the network manager is finished recieving a net reply read it all
        connect(&myNetworkManager,SIGNAL(finished(QNetworkReply*)),this, SLOT(finishedSlot(QNetworkReply *)));

        //when done reading quit the waiting loop if it's on exec state.
        connect(this, SIGNAL(doneReading()),&myEventLoop, SLOT(quit()));

        //when ssl errors, report them.
        connect(&myNetworkManager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                this, SLOT(sslErrorsSlot(QNetworkReply*,QList<QSslError>)));

        //when disconnected exit a probable current waiting loop
        connect(&myNetworkManager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
                this, SLOT(networkAccessibleChangedSlot(QNetworkAccessManager::NetworkAccessibility)));
    }

    bool postProtoMessageData(const QString & route,const QString & contentType,const google::protobuf::Message & protoMessage){
        std::string data = protoMessage.SerializeAsString().data();
        return postRawData(route,contentType,data.data(),data.size());
    }

    bool postRawData(const QString & route,const QString & contentType,const QByteArray & postData){
        QNetworkRequest request;
        restNetworkStatus();
        request.setUrl(QUrl(myBaseUrl.toString()+"/"+route));
        PostDataDecorator decorator(&myNetworkManager,&request);        
        myCurrentNetworkReply = decorator.postBinaryData(contentType,postData);
        if (myCurrentNetworkReply == NULL) return false;
        waitForReply();
        return true;
    }

    bool postRawData(const QString & route,const QString & contentType,const char * data,int size){
        QNetworkRequest request;
        restNetworkStatus();
        request.setUrl(QUrl(myBaseUrl.toString()+"/"+route));
        PostDataDecorator decorator(&myNetworkManager,&request);
        myCurrentNetworkReply = decorator.postBinaryData(contentType,data,size);
        if (myCurrentNetworkReply == NULL) return false;
        waitForReply();
        return true;
    }

    bool postJasonData(const QString & route,const QString & contentType,const QJsonDocument & jsonDoc){
        QNetworkRequest request;
        restNetworkStatus();
        request.setUrl(QUrl(myBaseUrl.toString()+"/"+route));
        PostDataDecorator decorator(&myNetworkManager,&request);        
        myCurrentNetworkReply = decorator.postJsonData(contentType,jsonDoc);
        if (myCurrentNetworkReply == NULL) return false;
        waitForReply();
        return true;
    }

    bool postStringData(const QString & route,const QString & contentType,const QString & text,
                        RestfullCallTextEncoding encoding){
        QNetworkRequest request;
        restNetworkStatus();
        request.setUrl(QUrl(myBaseUrl.toString()+"/"+route));
        PostDataDecorator decorator(&myNetworkManager,&request);
        myCurrentNetworkReply = decorator.postStringData(contentType,text,encoding);
        if (myCurrentNetworkReply == NULL) return false;
        waitForReply();
        return true;
    }

    bool getData(const QString & route,
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

        //qDebug() << "Get : " << request.url().toDisplayString();
        if (myNetworkManager.networkAccessible()==QNetworkAccessManager::Accessible){
            myCurrentNetworkReply = myNetworkManager.get(request);
            //qDebug() << "waitForReply : " << request.url().toDisplayString();

            waitForReply();
            return true;
        }
        else {
            qDebug()<< "No network connection !";
            return false;
        }
    }

    bool getData(const QString & route) {
        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;
        return getData(route,params,headers);
    }

    bool getData(const QString & route,const QMap<QString,QVariant> parameters) {
        QMap<QString,QString>  headers;
        return getData(route,parameters,headers);
    }

    bool getData(const QString & route, const QMap<QString,QString> headersMap) {
        QMap<QString,QVariant> parameters;
        return getData(route,parameters,headersMap);
    }

    QByteArray lastReply() {
#ifdef TRACE
        if ( myLastRepliedData.size() > 100 )
            qDebug() << myLastRepliedData.size();
        else
            qDebug() << myLastRepliedData;
#endif

        return myLastRepliedData;
    }


    bool postTData(const QString & route,
                 const QMap<QString,QString> parameters,
                 const QMap<QString,QString> headersMap)  {
        QNetworkRequest request;
        restNetworkStatus();
        //construct url with parameters
        QString url = myBaseUrl.toString();
        //if (parameters.count()> 0) url+="?";

        QByteArray postBodyContent;
        foreach (QString paramName, parameters.keys()) {
            //url+=QString("%1=%2").arg(paramName) + QUrl::toPercentEncoding(parameters.value(paramName).toString());
            postBodyContent.append(QUrl::toPercentEncoding(paramName) + QString("=").toUtf8() +
                                   QUrl::toPercentEncoding(parameters.value(paramName)));

        }
        request.setUrl(QUrl(url));

        //add headers
        foreach (QString headerKey, headersMap.keys()) {
            request.setRawHeader(headerKey.toUtf8(),headersMap.value(headerKey).toUtf8());
        }

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
#ifdef TRACE
        qDebug() << "Post : " << request.url().toDisplayString();
#endif

        if (myNetworkManager.networkAccessible()==QNetworkAccessManager::Accessible){
            myCurrentNetworkReply = myNetworkManager.post(request,postBodyContent);
            //qDebug() << "waitForReply : " << request.url().toDisplayString();

            waitForReply();
            return true;
        }
        else {
            qDebug()<< "No network connection !";
            return false;
        }
    }
    /**/


signals:

    void doneReading();

private slots:

    void finishedSlot(QNetworkReply* reply){
        //qDebug() << "finishedSlot";
        if (reply != NULL){
            myLastRepliedData = reply->readAll();
            reply->deleteLater();
        }
        else
            qDebug() << "null reply";
        emit doneReading();
    }

    void sslErrorsSlot(QNetworkReply * reply, const QList<QSslError> & errors) {
        if (reply !=NULL)
            myCurentSSLErrors.clear();
        if (errors.count()>1)
            qDebug() << "ssl error for: ";
        myCurentSSLErrors.append(errors);
    }

    void networkErrorSlot(QNetworkReply::NetworkError error ) {
        if (myCurrentNetworkReply != NULL){
            QUrl redirection =
                    myCurrentNetworkReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

            if (redirection.isValid()) //no action is taken we're always allowing redirection
                qDebug() << "server redirection : " << redirection.toString() ;
            else {
                qDebug() << "net error : " << myCurrentNetworkReply->errorString();
                //the finsihed signal will follow
            }
        }
        myCurrentNetworkError = error;
    }

    void networkAccessibleChangedSlot(QNetworkAccessManager::NetworkAccessibility accessible){
        if (accessible != QNetworkAccessManager::Accessible)
            if (myEventLoop.isRunning()) myEventLoop.quit();
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
        myLastRepliedData.clear();
        //when network error, report it and exit wait loop.
        if (myCurrentNetworkReply != NULL) {
            connect(myCurrentNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)),
                    this, SLOT(networkErrorSlot(QNetworkReply::NetworkError)));
            myEventLoop.exec();
        }
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

    /**
     * @brief myEventLoop : waiting loop.
     */
    QEventLoop myEventLoop;

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

#ifdef TRACE
        qDebug() << ret.isNull() << ret.isEmpty() << ret.isArray() << ret.isObject();
#endif


        QJsonArray qa = ret.array();
        QJsonValueRef json = qa[0];

        QString str(json.toObject().value("data").toString());

#ifdef TRACE
        qDebug() << str;
#endif


        auto arrayblock =
                QByteArray::fromBase64(str.toUtf8());

        blk.ParseFromArray(arrayblock,arrayblock.size());

        return blk;
    }

    static std::string getBlk(const QString & baseUrl,int32_t blockNum,
                              QThread * ownerThread = QThread::currentThread()){
#ifdef TRACE
        qDebug() << " get blk " << blockNum;
#endif

        RestfullClient client(QUrl(baseUrl),ownerThread);
#ifdef TRACE
        qDebug() << " get blk 2 " << baseUrl;
#endif

        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;
        //hard coded url
        //TODO move to settings
        QString customRoute = "block/" + QString::number(blockNum);
        //customRoute = customRoute.arg(route).arg(blockNum);
        client.getData(customRoute,params,headers);
#ifdef TRACE
        qDebug() << " get blk 3 " << baseUrl;
#endif



        return client.lastReply().toStdString();
    }

    static std::vector<std::string> getBlk(const QString & baseUrl,int32_t blockNum,
                                      int32_t blockEnd,
                              QThread * ownerThread = QThread::currentThread()){

        RestfullClient client(QUrl(baseUrl),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;

        std::vector<std::string> ret{};
        for ( int i = blockNum; i <= blockEnd; i++) {
#ifdef TRACE
            qDebug() << " get blk " << i;
#endif
            //hard coded url
            //TODO move to settings
            QString customRoute = "block/" + QString::number(i);
            //customRoute = customRoute.arg(route).arg(blockNum);
            client.getData(customRoute,params,headers);
#ifdef TRACE
            qDebug() << " get blk 3 " << baseUrl;
#endif


            ret.push_back( client.lastReply().toStdString());
       }

        return ret;
    }

    static int32_t getHeight(const QString & baseUrl, QThread * ownerThread = QThread::currentThread()) {
        //qDebug() << "inside getHeight : cureent thread" << QThread::currentThread();

        RestfullClient client(QUrl(baseUrl),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;
        //hard coded url
        //TODO move to settings
        QString customRoute("block-height");      
        client.getData(customRoute,params,headers);
        auto response = client.lastReply();
        QString str( response );
        str.remove("\"");
        //int i = to_string(i);
        int32_t i = str.toLong();
        return i;
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

    static std::string myGetTx(QThread * ownerThread = QThread::currentThread()){
        return getTx(fantasybit::LAPIURL.data(),ownerThread);
    }

    static std::string myGetTr(QThread * ownerThread = QThread::currentThread()){
        return getTr(fantasybit::LAPIURL.data(),ownerThread);
    }

    static std::string getTx(const QString & baseUrl,
                             QThread * ownerThread = QThread::currentThread()){
        RestfullClient client(QUrl(baseUrl),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;
        //hard coded url
        //TODO move to settings
        QString customRoute("tx");
        //customRoute = customRoute.arg(route).arg(blockNum);
        client.getData(customRoute,params,headers);

        return client.lastReply().toStdString();
    }

    static std::string getTr(const QString & baseUrl,
                             QThread * ownerThread = QThread::currentThread()){
        RestfullClient client(QUrl(baseUrl),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;
        //hard coded url
        //TODO move to settings
        QString customRoute("trade");
        //customRoute = customRoute.arg(route).arg(blockNum);
        client.getData(customRoute,params,headers);

        return client.lastReply().toStdString();
    }

    static std::string myPeekTx(QThread * ownerThread = QThread::currentThread()){
        return peekTx(fantasybit::LAPIURL.data(),ownerThread);
    }

    static std::string peekTx(const QString & baseUrl,
                             QThread * ownerThread = QThread::currentThread()){
        RestfullClient client(QUrl(baseUrl),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;
        //hard coded url
        //TODO move to settings
        QString customRoute("tx/peek");
        //customRoute = customRoute.arg(route).arg(blockNum);
        client.getData(customRoute,params,headers);

        return client.lastReply().toStdString();
    }

};

#endif // RESTFULLCALL_H
