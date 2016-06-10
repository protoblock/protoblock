#include "addtorrent.h"
#include "metainfo.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QLineEdit>
#include <QMetaObject>

static QString stringNumber(qint64 number)
{
    if (number > (1024 * 1024 * 1024))
        return QString::asprintf("%.2fGB", number / (1024.0 * 1024.0 * 1024.0));
    else if (number > (1024 * 1024))
        return QString::asprintf("%.2fMB", number / (1024.0 * 1024.0));
    else if (number > (1024))
        return QString::asprintf("%.2fKB", number / (1024.0));
    else
        return QString::asprintf("%d bytes", int(number));
}

AddTorrent::AddTorrent(QObject *parent)
  : QObject(parent)
{
    setdeafultTorrentPath (QDir::current().path());
}

void AddTorrent::setTorrent(const QString &torrentFile)
{
    if (torrentFile.isEmpty()) {
        return;
    }

    set_torrentFile(torrentFile);
    if (!torrentFile.isEmpty())
        lastDirectory = QFileInfo(torrentFile).absolutePath();

    if (lastDestinationDirectory.isEmpty())
        lastDestinationDirectory = lastDirectory;

    MetaInfo metaInfo;
    QFile torrent(torrentFile);
    if (!torrent.open(QFile::ReadOnly) || !metaInfo.parse(torrent.readAll())) {
        return;
    }
    setmetaFileName (torrentFile);
    setmetaAnnounceUrl (metaInfo.announceUrl());
    if (metaInfo.comment().isEmpty())
        setmetaComment ("<unknown>");
    else
        setmetaComment (metaInfo.comment());
    if (metaInfo.createdBy().isEmpty())
        setmetaCreatedBy ("<unknown>");
    else
        setmetaCreatedBy (metaInfo.createdBy());
    setmetaSize(stringNumber(metaInfo.totalSize()));
    if (metaInfo.fileForm() == MetaInfo::SingleFileForm) {
        setmetaFileForm (metaInfo.singleFile().name);
    } else {
        QStringList html;

        foreach (MetaInfoMultiFile file, metaInfo.multiFiles()) {
            QString name = metaInfo.name();
            if (!name.isEmpty()) {
                html.append(name);
//                if (!name.endsWith('/'))
//                    html.append ('/');
            }
            html.append (file.path);

        }
        setmetaFiles (html);
    }

    QFileInfo info(torrentFile);
    set_torrentDestinationFolder (info.absoluteFilePath ());
}

QString AddTorrent::torrentFileName() const
{
    return m_torrentFile;
}

QString AddTorrent::destinationFolder() const
{
    return m_torrentDestinationFolder ;
}
