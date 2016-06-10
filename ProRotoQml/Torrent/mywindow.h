#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QWidget>
#include <QSet>
#include <QDebug>
#include <QFile>
#include <QLayout>
#include <QPushButton>
#include <QSharedPointer>
#include <QSpinBox>

#include "torrentfileinfo.h"
#include "requesttoservermanager.h"
#include "torrentclient.h"
#include "torrent.h"
#include "peerconnection.h"
#include "downloader.h"

class MyWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MyWindow( QWidget *parent = nullptr );
    ~MyWindow();

private:
    QList< PeerInfo > m_peers;
    QSharedPointer< Torrent > m_torrent;
    TorrentClient *m_client;
    QSpinBox      *m_spinBox;
    Downloader    *m_downloader;
};

#endif // MYWINDOW_H
