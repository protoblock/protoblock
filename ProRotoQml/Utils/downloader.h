#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QSsl>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkAccessManager>


class Downloader: public QObject
{
    Q_OBJECT
public:
    Downloader(QObject *parent = 0);
    Q_PROPERTY(QString file READ file WRITE setFile )
    Q_PROPERTY(QString savePath READ savePath WRITE setSavePath)
    Q_PROPERTY(QString savedFile READ savedFile NOTIFY savedFileChanged)
    Q_PROPERTY(quint64 bytesReceived READ bytesReceived NOTIFY bytesReceivedChanged)
    Q_PROPERTY(quint64 bytesTotal READ bytesTotal NOTIFY bytesTotalChanged)
    Q_PROPERTY(QString units READ units NOTIFY unitsChanged )
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(double incommingSpeed READ incommingSpeed NOTIFY incommingSpeedChanged)

    Q_INVOKABLE void doDownload();

    QString file();
    void setFile(const QString &file);
    QString savePath();
    void setSavePath(const QString &savePath);
    QString savedFile();
    void setSavedFileName(const QString savedFile);

    void append(const QUrl &url);
    void append(const QStringList &urlList);
    QString saveFileName(const QString &downloadFile);

    quint64 bytesReceived();
    void setBytesRecived(const quint64  &bytesReceived);

    QString units();
    void setUnits(const QString &units);

    double incommingSpeed();
    void setincommingSpeed(const double &incommingSpeed);

    quint64 bytesTotal();
    void setBytesTotal(const quint64 &bytesTotal);

    QString errorString();
    void setErrorString(const QString &errorString);

signals:
    void finished();
    void started(bool);
    void incommingSpeedChanged();
    void bytesReceivedChanged();
    void unitsChanged();
    void bytesTotalChanged();
    void errorStringChanged();
    void fileChanged();
    void savePathChanged();
    void savedFileChanged();

private slots:
    void startNextDownload();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    void downloadReadyRead();

private:
    QNetworkAccessManager manager;
    QQueue<QUrl> downloadQueue;
    QNetworkReply *currentDownload;
    QFile output;
    QString mFile;
    QString mPath;
    QTime downloadTime;
    QString m_savedFile;

    int downloadedCount;
    int totalCount;

    // for progression
    qint64 m_bytesReceived;
    qint64 m_bytesTotal;
    QString m_units;
    double m_speed;
    //error checking
    QString m_error;
};

#endif
