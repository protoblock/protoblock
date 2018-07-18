#ifndef ADDTORRENT_H
#define ADDTORRENT_H

#include <QObject>
#include "QQmlConstRefPropertyHelpers.h"
#include "QQmlListPropertyHelper.h"

class AddTorrent : public QObject
{
    Q_OBJECT
    QML_READONLY_CSTREF_PROPERTY (QString,deafultTorrentPath)
    QML_READONLY_CSTREF_PROPERTY (QString,metaFileName)
    QML_READONLY_CSTREF_PROPERTY (QString,metaAnnounceUrl)
    QML_READONLY_CSTREF_PROPERTY (QString,metaComment)
    QML_READONLY_CSTREF_PROPERTY (QString,metaCreatedBy)
    QML_READONLY_CSTREF_PROPERTY (QString,metaSize)
    QML_READONLY_CSTREF_PROPERTY (QString,metaFileForm)
    QML_READONLY_CSTREF_PROPERTY (QStringList, metaFiles)
    QML_READONLY_CSTREF_PROPERTY (QString, lastTorrentPath)


    QML_WRITABLE_CSTREF_PROPERTY (QString ,torrentFile)
    QML_WRITABLE_CSTREF_PROPERTY (QString ,torrentDestinationFolder)

public:
    AddTorrent(QObject *parent = 0);
    QString torrentFileName() const;
    QString destinationFolder() const;
    Q_INVOKABLE void setTorrent(const QString &torrentFile);

private:
    QString destinationDirectory;
    QString lastDirectory;
    QString lastDestinationDirectory;
};

#endif
