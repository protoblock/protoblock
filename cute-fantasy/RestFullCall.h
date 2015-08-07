#ifndef RESTFULLCALL_H
#define RESTFULLCALL_H

#include <QObject>
#include <QSharedDataPointer>
#include <QNetworkAccessManager>
#include <QThread>
#include <QNetworkReply>
#include <QEventLoop>
#include "globals.h"

class RestfullCall : public QObject
{
    Q_OBJECT
public:
    RestfullCall(){}
    void restFullSynchrounousCallWithRawPostData(const QUrl & url,QByteArray & postData,const QString & contentType){
        QNetworkRequest request;
        restNetworkStatus();
        request.setUrl(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader,contentType);
        request.setHeader(QNetworkRequest::ContentLengthHeader,postData.length());
        myCurrentNetworkReply = myNetworkManager.post(request,postData);
        waitForReply();
        myCurrentNetworkReply->deleteLater();
        processReplyData();
    }


    void restFullSynchrounousCallWithUTF8TextPostData(const QUrl & url,QString & postData,const QString & contentType){
        QNetworkRequest request;
        restNetworkStatus();
        request.setUrl(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader,contentType);
        request.setHeader(QNetworkRequest::ContentLengthHeader,postData.toUtf8().length());
        myCurrentNetworkReply = myNetworkManager.post(request,postData.toUtf8());
        waitForReply();
        myCurrentNetworkReply->deleteLater();
        processReplyData();
    }

protected:
    virtual void processReplyData(){
        //do something with  myLastRepliedData

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
        connect(myCurrentNetworkReply, SIGNAL(readyRead()),
                &loop, SLOT(quit()));
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
};

#endif // RESTFULLCALL_H
