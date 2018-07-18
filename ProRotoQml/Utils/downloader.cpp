#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <stdio.h>
#include "downloader.h"


Downloader::Downloader(QObject *parent)
    : QObject(parent)

{
}

void Downloader::append(const QStringList &urlList)
{
    foreach (QString url, urlList)
        append(QUrl::fromEncoded(url.toLocal8Bit()));

    if (downloadQueue.isEmpty())
        QTimer::singleShot(0, this, SIGNAL(finished()));
}

void Downloader::append(const QUrl &url)
{
    if (downloadQueue.isEmpty())
        QTimer::singleShot(0, this, SLOT(startNextDownload()));

    downloadQueue.enqueue(url);
    ++totalCount;
}

/*!
 * \qmlsignal Downloader::doDownload()
 * start the download
 */
void Downloader::doDownload()
{
    QString fileToDownload = mFile ;
    append(fileToDownload);
    emit started(true);
}

/*!
 * \qmlproperty string Downloader::file
 *
 * sets the file name that one wants to use to download from the web
 */
QString Downloader::file()
{
    return mFile;
}

void Downloader::setFile(const QString &file)
{
    if(mFile == file)
        return;
    mFile = file ;
    emit fileChanged();
}
/*!
 * \qmlproperty string Downloader::savePath
 *
 * sets the path where one wants to save the file on the local machine
 */
QString Downloader::savePath()
{
    return mPath;
}

void Downloader::setSavePath(const QString &savePath)
{
    if(mPath == savePath)
        return;
    mPath = savePath;
    emit savePathChanged();
}

QString Downloader::savedFile()
{
    return m_savedFile;
}

void Downloader::setSavedFileName(const QString savedFile)
{
    if(m_savedFile ==savedFile)
        return;
    m_savedFile = savedFile;
    // emit ?  who cares  ?
}


QString Downloader::saveFileName(const QString &downloadFile)
{
    return  downloadFile;
}
/*!
 * \qmlsignal Downloader::bytesReceived()
 *
 * Returns the bytes that the download is getting from the internet speed
 */
quint64 Downloader::bytesReceived()
{
    return m_bytesReceived;
}

void Downloader::setBytesRecived(const quint64 &bytesReceived)
{

    m_bytesReceived = bytesReceived;
    emit  bytesReceivedChanged();
}
/*!
 * \qmlsignal Downloader::bytesTotal()
 *
 * the total ammount of bytes that it will take to download the file from the web
 */
quint64 Downloader::bytesTotal()
{
    return m_bytesTotal;
}

void Downloader::setBytesTotal(const quint64 &bytesTotal)
{
    m_bytesTotal = bytesTotal;
    emit bytesTotalChanged();
}

/*!
 * \qmlsignal Downloader::errorString()
 *
 * returns errors if there are any in the download.
 */

QString Downloader::errorString()
{
    return m_error;
}

void Downloader::setErrorString(const QString &errorString)
{
    m_error = errorString;
}
/*!
 * \qmlproperty string  Downloader::units
 *
 * set the units of the download of the file from the web
 */
QString Downloader::units()
{
    return m_units;
}

void Downloader::setUnits(const QString &units)
{
    if (m_units == units)
        return;
    m_units = units;
    emit unitsChanged();
}
/*!
 * \qmlsignal Downloader::incommingSpeed()
 *
 * used to get back the in-coming speed of the file that is getting downloaded
 */
double Downloader::incommingSpeed()
{
    return m_speed;
}

void Downloader::setincommingSpeed(const double &incommingSpeed)
{
    if(m_speed == incommingSpeed)
        return;
    m_speed = incommingSpeed;
    emit incommingSpeedChanged();
}

/*!
 * \qmlsignal Downloader::startNextDownload()
 * Used if you want to download multiple files
 */
void Downloader::startNextDownload()
{
    if (downloadQueue.isEmpty()) {
        qDebug() << downloadedCount << " " << totalCount << "files downloaded successfully\n";
        emit started(false);
        emit finished();
        return;
    }

    QUrl url = downloadQueue.dequeue();
    QFileInfo fileInf(url.toString());
    QString fileName = QString("%1%2").arg(mPath).arg(fileInf.fileName());


//    QString filename = saveFileName(fileName);

    setSavedFileName(fileName);



    qDebug() << fileName;

    output.setFileName(fileName);
    if (!output.open(QIODevice::WriteOnly)) {
        qDebug() <<  "Problem opening save file for download";
        startNextDownload();
        return;
    }

    QNetworkRequest request(url);
    currentDownload = manager.get(request);
    connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));
    connect(currentDownload, SIGNAL(finished()),
            SLOT(downloadFinished()));
    connect(currentDownload, SIGNAL(readyRead()),
            SLOT(downloadReadyRead()));

    // prepare the output
    qDebug () << "Downloading " <<  url.toEncoded() << "........";
    downloadTime.start();
}
/*!
 * \qmlsignal Downloader::downloadProgress()
 * returns to total download progress in MB kb or bytes
 */
void Downloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
    QString unit;
    if (speed < 1024) {
        unit = "bytes/sec";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "kB/s";
    } else {
        speed /= 1024*1024;
        unit = "MB/s";
    }


    setBytesRecived(bytesReceived);
    setincommingSpeed(speed);
    setUnits(unit);
    setBytesTotal(bytesTotal);

}
/*!
 * \qmlsignal Downloader::downloadFinished()
 * signal that is sent when the download is finished
 */
void Downloader::downloadFinished()
{
    output.close();

    if (currentDownload->error()) {
        // download failed
        setErrorString("Failed " + currentDownload->errorString());
    } else {
        qDebug() << "Succeeded. Downloading " << mPath;
        emit finished();
    }

    currentDownload->deleteLater();
}

void Downloader::downloadReadyRead()
{
    output.write(currentDownload->readAll());
}
