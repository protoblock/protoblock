#ifndef TORRENTQML_H
#define TORRENTQML_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QString>
#include <QSettings>
#include <QDir>
#include <QByteArray>
#include <QTimer>

#include "addtorrent.h"
#include "mainwindow.h"
#include "ratecontroller.h"
#include "torrentclient.h"
#include "torrentmodel.h"
#include "QQmlConstRefPropertyHelpers.h"


class TorrentQml : public QObject
{
    Q_OBJECT

    QML_READONLY_CSTREF_PROPERTY (QString, downloadLimitLabel)
    QML_READONLY_CSTREF_PROPERTY (QString ,uploadLimitLabel)
    QML_READONLY_CSTREF_PROPERTY (int ,seedCount)
    QML_READONLY_CSTREF_PROPERTY (int,connectedPeers)
    QML_READONLY_CSTREF_PROPERTY (QString ,stateString)
    QML_READONLY_CSTREF_PROPERTY (QString ,errorString)
    QML_READONLY_CSTREF_PROPERTY (QString ,warningString)

    QML_WRITABLE_CSTREF_PROPERTY (QString ,fileName)
    QML_WRITABLE_CSTREF_PROPERTY (int, downloadLimit)
    QML_WRITABLE_CSTREF_PROPERTY (int, uploadLimit)
    QML_WRITABLE_CSTREF_PROPERTY (QString, destinationFolder)


public:
    explicit TorrentQml(QObject *parent = 0);
    ~TorrentQml();


    Q_INVOKABLE void pauseTorrent(const int &torrentNumber);
    Q_INVOKABLE void moveTorrentUp(const int &torrentNumber);
    Q_INVOKABLE void moveTorrentDown(const int &torrentNumber);
    Q_INVOKABLE void removeTorrent(const int &torrentNumber);
//    void loadSettings();

protected:
    void closeEvent();

private slots:
    void loadSettings();
    void saveSettings();

    bool addTorrent();
    void removeTorrent();
    void pauseTorrent();
    void moveTorrentUp();
    void moveTorrentDown();

    void torrentStopped();
    void torrentError(TorrentClient::Error error);

    void updateState(TorrentClient::State state);
    void updatePeerInfo();
    void updateProgress(int percent);
    void updateDownloadRate(int bytesPerSecond);
    void updateUploadRate(int bytesPerSecond);

    void setUploadLimit(int bytes);
    void setDownloadLimit(int bytes);

    void about();
    void setActionsEnabled();
    void acceptFileDrop(const QString &fileName);


private:
    TorrentModel *item;
    int rowOfClient(TorrentClient *client) const;
    bool addTorrent(const QString &fileName, const QString &destinationFolder,
                    const QByteArray &resumeState = QByteArray());

    int uploadLimit;
    int downloadLimit;

    struct Job {
        TorrentClient *client;
        QString torrentFileName;
        QString destinationDirectory;
    };
    QList<Job> jobs;
    int jobsStopped;
    int jobsToStop;
    QString lastDirectory;
    bool saveChanges;
};

#endif // TORRENTQML_H
