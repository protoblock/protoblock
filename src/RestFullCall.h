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

        //when ssl errors, report them.0
        connect(&myNetworkManager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                this, SLOT(sslErrorsSlot(QNetworkReply*,QList<QSslError>)));

        //when disconnected exit a probable current waiting loop
        connect(&myNetworkManager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
                this, SLOT(networkAccessibleChangedSlot(QNetworkAccessManager::NetworkAccessibility)));
    }

    bool postProtoMessageData(const QString & route,const QString & contentType,const GOOGLE_NAMESPACE::protobuf::Message & protoMessage){
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
                 const QMap<QString,QString> parameters,
                 const QMap<QString,QString> headersMap)  {
        QNetworkRequest request;
        restNetworkStatus();
        //construct url with parameters
        QString url;
        if ( route != "" )
            url = myBaseUrl.toString()+"/"+route;
        else
            url = myBaseUrl.toString();

        bool first = true;
        foreach (QString paramName, parameters.keys()) {
            if ( first ) {
                first = false;
                url+=QString("?%1=%2").arg(paramName).arg(parameters.value(paramName));
            }
            else {
                url+=QString("&%1=%2").arg(paramName).arg(parameters.value(paramName));
            }
        }

        qDebug() << " url " << url;
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
        QMap<QString,QString> params;
        return getData(route,params,headers);
    }

    bool getData(const QString & route,const QMap<QString,QString> parameters) {
        QMap<QString,QString>  headers;
        return getData(route,parameters,headers);
    }

//    bool getData(const QString & route, const QMap<QString,QString> headersMap) {
//        QMap<QString,QString> parameters;
//        return getData(route,parameters,headersMap);
//    }

    QByteArray lastReply() {
#ifdef TRACE3
        if ( myLastRepliedData.size() > 100 )
            qDebug() << myLastRepliedData.size();
        else
            qDebug() << myLastRepliedData;
#endif

        return myLastRepliedData;
    }

    int lastCode() {
        return statusCode;
    }

    int statusCode = 0;

    bool postTData(const QString & route,
                 const QMap<QString,QString> parameters,
                 const QMap<QString,QString> headersMap)  {
        QNetworkRequest request;
        restNetworkStatus();
        //construct url with parameters
        //if (parameters.count()> 0) url+="?";
        QString url;
        if ( route != "" )
            url = myBaseUrl.toString()+"/"+route;
        else
            url = myBaseUrl.toString();

        QByteArray postBodyContent;
        bool first = true;
        foreach (QString paramName, parameters.keys()) {
            if ( first ) {
                first = false;
                url+=QString("?%1=%2").arg(paramName).arg(parameters.value(paramName));
            }
            else {
                url+=QString("&%1=%2").arg(paramName).arg(parameters.value(paramName));
            }
//            postBodyContent += QString("&%1=%2").arg(paramName).arg(parameters.value(paramName).toString());
        }
        request.setUrl(url);
        //add headers
        foreach (QString headerKey, headersMap.keys()) {
            request.setRawHeader(headerKey.toUtf8(),headersMap.value(headerKey).toUtf8());
        }

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
#ifdef TRACE
        qDebug() << "Post: " << request.url().toDisplayString();
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
            statusCode =
                    reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

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
        reply->ignoreSslErrors();
    }

    void networkErrorSlot(QNetworkReply::NetworkError error ) {
        if (myCurrentNetworkReply == NULL)
            return;

        QUrl redirection =
                myCurrentNetworkReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

        if (redirection.isValid()) //no action is taken we're always allowing redirection
            qDebug() << "server redirection : " << redirection.toString() ;
        else {
            qDebug() << "net error : " << myCurrentNetworkReply->errorString();
            //the finsihed signal will follow
        }

        myCurrentNetworkError = error;
        myCurrentNetworkReply->ignoreSslErrors();

        statusCode =
                myCurrentNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
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
        QMap<QString,QString> params;
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
#ifdef TRACE3
        qDebug() << " get blk " << blockNum;
#endif

        RestfullClient client(QUrl(baseUrl),ownerThread);
#ifdef TRACE3
        qDebug() << " get blk 2 " << baseUrl;
#endif

        QMap<QString,QString>  headers;
        QMap<QString,QString> params;
        //hard coded url
        //TODO move to settings
        QString customRoute = "block/" + QString::number(blockNum);
        //customRoute = customRoute.arg(route).arg(blockNum);
        client.getData(customRoute,params,headers);
#ifdef TRACE3
        qDebug() << " get blk 3 " << baseUrl;
#endif



        return client.lastReply().toStdString();
    }

    static std::vector<std::string> getBlk(const QString & baseUrl,int32_t blockNum,
                                      int32_t blockEnd,
                              QThread * ownerThread = QThread::currentThread()){

        RestfullClient client(QUrl(baseUrl),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QString> params;

        std::vector<std::string> ret{};
        for ( int i = blockNum; i <= blockEnd; i++) {
#ifdef TRACE
//            qDebug() << "getBlk get blk: " << i;
#endif
            //hard coded url
            //TODO move to settings
            QString customRoute = "block/" + QString::number(i);
            //customRoute = customRoute.arg(route).arg(blockNum);
            client.getData(customRoute,params,headers);
#ifdef TRACE4
            qDebug() << " get blk 3 " << baseUrl;
#endif


            ret.push_back( client.lastReply().toStdString());
       }

        return ret;
    }

    static int32_t getHeight(const QString & baseUrl, QThread * ownerThread = QThread::currentThread()) {
//        qDebug() << "inside getHeight : cureent thread" << QThread::currentThread();

        RestfullClient client(QUrl(baseUrl),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QString> params;
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
        QMap<QString,QString> params;
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
        QMap<QString,QString> params;
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
        QMap<QString,QString> params;
        //hard coded url
        //TODO move to settings
        QString customRoute("tx/peek");
        //customRoute = customRoute.arg(route).arg(blockNum);
        client.getData(customRoute,params,headers);

        return client.lastReply().toStdString();
    }

};

class BitcoinRestfullService {

public:
    static uint64_t getBtcAddressBalance(const QString &addr,
                 QThread * ownerThread = QThread::currentThread()) {

        QString url = QString(fantasybit::BLOCKCHAINAPI.data());
        RestfullClient client(QUrl(url),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QString> params;// = {{ "api_key" , fantasybit::CHAINAPIKEY.data() }};
        QString customRoute("q/addressbalance/%1");
        //QString customRoute("q/addressbalance/%1?confirmations=1");
        //customRoute = customRoute.arg(route).arg(blockNum);
        client.getData(customRoute.arg(addr),params,headers);

        return client.lastReply().toULongLong();
        //toStdString();
    }



    static QByteArray getBtcAddressUnspent(const std::string &addr,
                 QThread * ownerThread = QThread::currentThread()) {
        return getBlockchainBtcAddressUnspent(addr,ownerThread);
    }

    static QByteArray getBlockchainBtcAddressUnspent(const std::string &addr,
                 QThread * ownerThread = QThread::currentThread()) {

        qDebug() << " getBlockchainBtcAddressUnspent";
        QString url = QString(fantasybit::BLOCKCHAINAPI.data());
        RestfullClient client(QUrl(url),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QString> params;// = {{ "api_key" , fantasybit::CHAINAPIKEY.data() }};
        params.insert ( QString("active"),QString(addr.data()));
        //params.insert ( QString("confirmations"),QString("1"));

        QString customRoute("unspent");
        //customRoute = customRoute.arg(route).arg(blockNum);
        client.getData(customRoute,params,headers);
        return client.lastReply();
        //toStdString();
    }

    static QByteArray getBlockchainBtcTX(const std::string &txid,
                 QThread * ownerThread = QThread::currentThread()) {

        qDebug() << " getBlockchainBtcTX";
        QString url = QString(fantasybit::BLOCKCHAINAPI.data());
        RestfullClient client(QUrl(url),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QString> params;
        params.insert ( QString("format"),QString("hex"));
        QString customRoute("rawtx/%1");
        client.getData(customRoute.arg(txid.data()),params,headers);
        return client.lastReply();
    }

    static QByteArray getChainsoBtcAddressUnspent(const std::string &addr,
                 QThread * ownerThread = QThread::currentThread()) {

        qDebug() << " getChainsoBtcAddressUnspent";

        QString url = QString(fantasybit::CHAINSOAPI.data()).arg("get_tx_unspent");
        RestfullClient client(QUrl(url),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QString> params;
        QString customRoute = addr.data();
        //customRoute = customRoute.arg(route).arg(blockNum);
        client.getData(customRoute,params,headers);

        return client.lastReply();
        //toStdString();
    }

    static QByteArray getBlockCypherTX(const std::string &txid,
                 QThread * ownerThread = QThread::currentThread()) {

        qDebug() << " getBlockCypherTX" << txid.data();

        QString url = QString(fantasybit::BLOCKCYPHERAPI.data()).append("/txs");
        RestfullClient client(QUrl(url),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QString> params;

        QString customRoute = txid.data();
        client.getData(customRoute,params,headers);

        return client.lastReply();
    }

    static QByteArray getChainsoIsTXSpent(const std::string &txid, uint numo,
                 QThread * ownerThread = QThread::currentThread()) {

        //GET /api/v2/is_tx_spent/{NETWORK}/{TXID}/{OUTPUT_NO}

        qDebug() << " getChainsoIsTXSpent" << txid.data() << numo;

        QString url = QString(fantasybit::CHAINSOAPI.data()).arg("is_tx_spent");
        RestfullClient client(QUrl(url),ownerThread);
        QMap<QString,QString>  headers;
//        QMap<QString,QString> params;
        QMap<QString,QString> params = {{ "api_key" , fantasybit::CHAINAPIKEY.data() }};

        QString customRoute = (txid + "/%1").data();
        customRoute = customRoute.arg(numo);
        client.getData(customRoute,params,headers);

        return client.lastReply();
        //toStdString();
    }

    static QByteArray getChainsoBtcAddress(const std::string &addr,
//                                           const QString &aftertx,
                 QThread * ownerThread = QThread::currentThread()) {
        qDebug() << " getChainsoBtcAddress";

        //https://chain.so/api/v2/address/BTCTEST/mnBZiPHayJMSCc5Vj32Tn65tn1CcuCPWkv
        QString url = QString(fantasybit::CHAINSOAPI.data()).arg("address");
        RestfullClient client(QUrl(url),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QString> params;
        QString customRoute = addr.data();
//        if ( !aftertx.isEmpty() )
//            customRoute = QString("%1/%2").arg(customRoute).arg(aftertx);
        client.getData(customRoute,params,headers);

        return client.lastReply();
        //toStdString();
    }

    static QByteArray pushTxXXX(const std::string &rawTx,
                             QThread * ownerThread = QThread::currentThread()) {
        QString url = QString(fantasybit::BLOCKCHAINAPI.data());
        RestfullClient client(QUrl(url),ownerThread);

        std::string sendit = "tx=" + rawTx;
        QString customRoute("pushtx");
//        client.postStringData(customRoute,"application/x-www-form-urlencoded",sendit.data(),RestfullClient::UTF8);
        QByteArray data(sendit.data(),sendit.size());
        client.postRawData(customRoute,"multipart/form-data;boundary=-------",data);
//        client.postRawData(customRoute,"",data);

        return client.lastReply();
        //toStdString();
    }

    static QByteArray pushChainsoBitcoinTx(const std::string &rawTx,
                             QThread * ownerThread = QThread::currentThread()) {
        QString url = QString(fantasybit::CHAINSOAPI.data()).arg("send_tx");
        RestfullClient client(QUrl(url),ownerThread);
//        QMap<QString,QString>  headers;
//        QMap<QString,QString> params;
//        params.insert ( QString("tx_hex"),QString(rawTx.data()));
       QString customRoute("");
//        client.postTData(customRoute,params,headers);

//        return client.lastReply();
        //toStdString();

        QJsonDocument doc;
        QJsonObject obj;
        obj.insert("tx_hex",QJsonValue(rawTx.data()));
        doc.setObject(obj);
        client.postJasonData(customRoute,"application/json",doc);
        return client.lastReply();

    }


    static QByteArray pushBitcoinTx(const std::string &rawTx,
                             QThread * ownerThread = QThread::currentThread()) {
        QString url = QString(fantasybit::BLOCKCHAINAPI.data());
        RestfullClient client(QUrl(url),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QString> params;
        params.insert ( QString("tx"),QString(rawTx.data()));
        QString customRoute("pushtx");
        client.postTData(customRoute,params,headers);

        return client.lastReply();
        //toStdString();
    }


};

#endif // RESTFULLCALL_H
