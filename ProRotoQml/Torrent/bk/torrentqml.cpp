#include "torrentqml.h"
#include "torrentmodel.h"

TorrentQml::TorrentQml(QObject *parent) : QObject(parent)
{

    // Set up connections
//    connect(this, SIGNAL(itemSelectionChanged()),
//            this, SLOT(setActionsEnabled()));
//    connect(this, SIGNAL(fileDropped(QString)),
//            this, SLOT(acceptFileDrop(QString)));

    connect(this, SIGNAL(uploadValueChanged(int)),
            this, SLOT(setUploadLimit(int)));

    connect(this, SIGNAL(downLoadValueChanged(int)),
            this, SLOT(setDownloadLimit(int)));

    connect(this, SIGNAL(newTorrentTriggered()),
            this, SLOT(addTorrent()));

    connect(this, SIGNAL(pauseTriggered()),
            this, SLOT(pauseTorrent()));

    connect(this, SIGNAL(removeTorrent()),
            this, SLOT(removeTorrent()));

    connect(this, SIGNAL(upTriggered(bool)),
            this, SLOT(moveTorrentUp()));

    connect(this, SIGNAL(downTriggered(bool)),
            this, SLOT(moveTorrentDown()));

    QMetaObject::invokeMethod(this, "loadSettings", Qt::QueuedConnection);
    loadSettings();
}

TorrentQml::~TorrentQml()
{
    closeEvent ();
}


void TorrentQml::loadSettings()
{
    // Load base settings (last working directory, upload/download limits).
    QSettings settings("ProtoblockTorrent", "Torrent");
    lastDirectory = settings.value("LastDirectory").toString();
    if (lastDirectory.isEmpty())
        lastDirectory = QDir::currentPath();
    int up = settings.value("UploadLimit").toInt();
    int down = settings.value("DownloadLimit").toInt();
    uploadLimitSlider->setValue(up ? up : 170);
    downloadLimitSlider->setValue(down ? down : 550);

    // Resume all previous downloads.
    int size = settings.beginReadArray("Torrents");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QByteArray resumeState = settings.value("resumeState").toByteArray();
        QString fileName = settings.value("sourceFileName").toString();
        QString dest = settings.value("destinationFolder").toString();

        if (addTorrent(fileName, dest, resumeState)) {
            TorrentClient *client = jobs.last().client;
            client->setDownloadedBytes(settings.value("downloadedBytes").toLongLong());
            client->setUploadedBytes(settings.value("uploadedBytes").toLongLong());
        }
    }
}

//add a invokable
bool TorrentQml::addTorrent()
{
    // Show the file dialog, let the user select what torrent to start downloading.
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a torrent file"),
//                                                    lastDirectory,
//                                                    tr("Torrents (*.torrent);;"
//                                                       " All files (*.*)"));
    if (fileName.isEmpty())
        return false;
    lastDirectory = QFileInfo(fileName).absolutePath();

    // Show the "Add Torrent" dialog.
    AddTorrent *addTorrent = new AddTorrent(this);
    addTorrent->setTorrent(fileName);
    addTorrent->deleteLater();
//    if (!addTorrent->exec())
//        return false;

    // Add the torrent to our list of downloads
    addTorrent(fileName, addTorrent->destinationFolder());
    if (!saveChanges) {
        saveChanges = true;
        QTimer::singleShot(1000, this, SLOT(saveSettings()));
    }
    return true;
}






void MainWindow::removeTorrent()
{
    // Find the row of the current item, and find the torrent client
    // for that row.
    int row = torrentView->indexOfTopLevelItem(torrentView->currentItem());
    TorrentClient *client = jobs.at(row).client;

    // Stop the client.
    client->disconnect();
    connect(client, SIGNAL(stopped()), this, SLOT(torrentStopped()));
    client->stop();

    // Remove the row from the view.
//    delete torrentView->takeTopLevelItem(row);
    jobs.removeAt(row);
    setActionsEnabled();

    saveChanges = true;
    saveSettings();
}




void MainWindow::torrentStopped()
{
    // Schedule the client for deletion.
    TorrentClient *client = qobject_cast<TorrentClient *>(sender());
    client->deleteLater();
}



void TorrentQml::torrentError(TorrentClient::Error)
{
    // Delete the client.
    TorrentClient *client = qobject_cast<TorrentClient *>(sender());
    int row = rowOfClient(client);
    QString fileName = jobs.at(row).torrentFileName;
    jobs.removeAt(row);

    QString er = QString("An error occurred while downloading %0: %1").arg(fileName)
                         .arg(client->errorString());
    seterrorString (er);
    // Display the warning.
//    delete torrentView->takeTopLevelItem(row);
    client->deleteLater();
}







// Q_INVOKABLE
// The Big one !!

bool TorrentQml::addTorrent(const QString &fileName, const QString &destinationFolder,
                            const QByteArray &resumeState)
{

    // Check if the torrent is already being downloaded.
    foreach (Job job, jobs) {
        if (job.torrentFileName == fileName && job.destinationDirectory == destinationFolder) {
            QString wr = QString("The torrent file %1 is "
                                 "already being downloaded.").arg(fileName);
            setwarningString (wr);
            return false;
        }
    }

    // Create a new torrent client and attempt to parse the torrent data.
    TorrentClient *client = new TorrentClient(this);
    if (!client->setTorrent(fileName)) {
        QString er = QString("The torrent file %1 cannot not be opened/resumed.").arg(fileName);
        seterrorString (er);
        delete client;
        return false;
    }
    client->setDestinationFolder(destinationFolder);
    client->setDumpedState(resumeState);

    // Setup the client connections.
    connect(client, SIGNAL(stateChanged(TorrentClient::State)), this, SLOT(updateState(TorrentClient::State)));
    connect(client, SIGNAL(peerInfoUpdated()), this, SLOT(updatePeerInfo()));
    connect(client, SIGNAL(progressUpdated(int)), this, SLOT(updateProgress(int)));
    connect(client, SIGNAL(downloadRateUpdated(int)), this, SLOT(updateDownloadRate(int)));
    connect(client, SIGNAL(uploadRateUpdated(int)), this, SLOT(updateUploadRate(int)));
    connect(client, SIGNAL(stopped()), this, SLOT(torrentStopped()));
    connect(client, SIGNAL(error(TorrentClient::Error)), this, SLOT(torrentError(TorrentClient::Error)));

    // Add the client to the list of downloading jobs.
    Job job;
    job.client = client;
    job.torrentFileName = fileName;
    job.destinationDirectory = destinationFolder;
    jobs << job;

    // Create and add a row in the torrent view for this download.
    QString baseFileName = QFileInfo(fileName).fileName();
    if (baseFileName.toLower().endsWith(".torrent"))
        baseFileName.remove(baseFileName.size() - 8);

    QString toolTip = QString("Torrent: %1<br>Destination: %2").arg(baseFileName).arg(destinationFolder);
    TorrentModel *item = new TorrentModel(
                TorrentModelItem( baseFileName
                                               , toolTip
                                               ,"0/0"
                                               ,"0"
                                               ,"0.0 KB/s"
                                               ,"0.0 KB/s"
                                               ,"Idle"
                                               )
                                           );
    if (!saveChanges) {
        saveChanges = true;
        QTimer::singleShot(5000, this, SLOT(saveSettings()));
    }
    client->start();
    return true;
}




void MainWindow::saveSettings()
{
    if (!saveChanges)
      return;
    saveChanges = false;

    // Prepare and reset the settings
    QSettings settings("ProtoblockTorrent", "Torrent");
    settings.clear();

    settings.setValue("LastDirectory", lastDirectory);
    settings.setValue("UploadLimit",    m_uploadLimit);
    settings.setValue("DownloadLimit", m_downloadLimit);

    // Store data on all known torrents
    settings.beginWriteArray("Torrents");
    for (int i = 0; i < jobs.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("sourceFileName", jobs.at(i).torrentFileName);
        settings.setValue("destinationFolder", jobs.at(i).destinationDirectory);
        settings.setValue("uploadedBytes", jobs.at(i).client->uploadedBytes());
        settings.setValue("downloadedBytes", jobs.at(i).client->downloadedBytes());
        settings.setValue("resumeState", jobs.at(i).client->dumpedState());
    }
    settings.endArray();
    settings.sync();
}




void TorrentQml::updateState(TorrentClient::State)
{
    // Update the state string whenever the client's state changes.
    TorrentClient *client = qobject_cast<TorrentClient *>(sender());
    setstateString (client->stateString ());
}




void TorrentQml::updatePeerInfo()
{
    // Update the number of connected, visited, seed and leecher peers.
    TorrentClient *client = qobject_cast<TorrentClient *>(sender());

    setconnectedPeers (client->connectedPeerCount());
    setseedCount (client->seedCount ());
//    client->leechers();
    //    int row = rowOfClient(client);

//    QTreeWidgetItem *item = torrentView->topLevelItem(row);
//    item->setText(1, tr("%1/%2").arg(client->connectedPeerCount())
//                  .arg(client->seedCount()));
}



void MainWindow::updateProgress(int percent)
{
    TorrentClient *client = qobject_cast<TorrentClient *>(sender());
    int row = rowOfClient(client);
    // Update the progressbar.
//item- // = torrentView->topLevelItem(row);
//    if (item)
//        item->setText(2, QString::number(percent));
}




void MainWindow::updateDownloadRate(int bytesPerSecond)
{
    // Update the download rate.
    TorrentClient *client = qobject_cast<TorrentClient *>(sender());
    int row = rowOfClient(client);
    const QString num = QString::asprintf("%.1f KB/s", bytesPerSecond / 1024.0);
    item->topLevelItem(row)->setText(3, num);

    if (!saveChanges) {
        saveChanges = true;
        QTimer::singleShot(5000, this, SLOT(saveSettings()));
    }
}




void TorrentQml::updateUploadRate(int bytesPerSecond)
{
    // Update the upload rate.
    TorrentClient *client = qobject_cast<TorrentClient *>(sender());
    int row = rowOfClient(client);
    const QString num = QString::asprintf("%.1f KB/s", bytesPerSecond / 1024.0);
    item ->topLevelItem(row)->setText(4, num);

    if (!saveChanges) {
        saveChanges = true;
        QTimer::singleShot(5000, this, SLOT(saveSettings()));
    }
}




void TorrentQml::pauseTorrent()
{
    // Pause or unpause the current torrent.
    int row = torrentView->indexOfTopLevelItem(torrentView->currentItem());
    TorrentClient *client = jobs.at(row).client;
    client->setPaused(client->state() != TorrentClient::Paused);
}




void MainWindow::moveTorrentUp()
{
    QTreeWidgetItem *item = torrentView->currentItem();
    int row = torrentView->indexOfTopLevelItem(item);
    if (row == 0)
        return;

    Job tmp = jobs.at(row - 1);
    jobs[row - 1] = jobs[row];
    jobs[row] = tmp;

//    QTreeWidgetItem *itemAbove = torrentView->takeTopLevelItem(row - 1);
//    torrentView->insertTopLevelItem(row, itemAbove);
//    setActionsEnabled();
}




void TorrentQml::moveTorrentDown()
{
//    QTreeWidgetItem *item = torrentView->currentItem();
//    int row = torrentView->indexOfTopLevelItem(item);
    if (row == jobs.size() - 1)
        return;

    Job tmp = jobs.at(row + 1);
    jobs[row + 1] = jobs[row];
    jobs[row] = tmp;

//    QTreeWidgetItem *itemAbove = torrentView->takeTopLevelItem(row + 1);
//    torrentView->insertTopLevelItem(row, itemAbove);
//    setActionsEnabled();
}




static int rateFromValue(int value)
{
    int rate = 0;
    if (value >= 0 && value < 250) {
        rate = 1 + int(value * 0.124);
    } else if (value < 500) {
        rate = 32 + int((value - 250) * 0.384);
    } else if (value < 750) {
        rate = 128 + int((value - 500) * 1.536);
    } else {
        rate = 512 + int((value - 750) * 6.1445);
    }
    return rate;
}



void TorrentQml::setUploadLimit(int value)
{
    int rate = rateFromValue(value);
    QString tempUL = QString("%1 KB/s").arg(QString::asprintf("%4d", rate));
    setuploadLimitLabel (tempUL);
    RateController::instance()->setUploadLimit(rate * 1024);
}

void TorrentQml::setDownloadLimit(int value)
{
    int rate = rateFromValue(value);
    QString tempDL =  QString("%1 KB/s").arg(QString::asprintf("%4d", rate));
    setdownloadLimitLabel (tempDL);
    RateController::instance()->setDownloadLimit(rate * 1024);
}

void TorrentQml::closeEvent()
{
    if (jobs.isEmpty())
        return;

    // Save upload / download numbers.
    saveSettings();
    saveChanges = false;

    // Stop all clients, remove the rows from the view and wait for
    // them to signal that they have stopped.
    jobsToStop = 0;
    jobsStopped = 0;
    foreach (Job job, jobs) {
        ++jobsToStop;
        TorrentClient *client = job.client;
        client->disconnect();
        connect(client, SIGNAL(stopped()), this, SLOT(torrentStopped()));
        client->stop();
    }

    if (jobsToStop > jobsStopped)
//        quitDialog->exec();
    this->deleteLater();
}
