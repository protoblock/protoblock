#ifndef TORRENTFILEINFO_H
#define TORRENTFILEINFO_H

#include "macro.h"
#include "torrentfileparser.h"

#include <QObject>
#include <QUrl>
#include <QList>
#include <QHash>
#include <QDateTime>
#include <QString>
#include <QByteArray>

struct FileInfo{
    FileInfo(){
        FileSize = 0;
    }
    //name ( для многофайловых торрентфайлов path )
    QString             FilePath;
    //length размер файла в байтах
    quint64             FileSize;
    //md5sum 32-символьная шестнадцатеричная строка соответствующая MD5-сумме файла
    QByteArray          MD5Sum;
};

class TorrentFileInfo
{
public:
    TorrentFileInfo(){ clear(); }
    void clear();

//// Shared Properties
    PROP_GET( QList< QUrl >,    TrackerUrlList) //announce + announce-list
    PROP_GET( QDateTime,        CreationDate)   //creation date Дата создания торрента
    PROP_GET( QString,          CreatedBy )     //created by    Имя и версия программы, которая использовалась для создания torrent-файла
    PROP_GET( QString,          Comment )       //comment       Текстовый комментарий в свободной форме от автора
    PROP_GET( quint64,          PieceLength )   //piece length  Размер каждого куска в байтах
    PROP_GET( QList< QByteArray >, Pieces )     //pieces Строка, составленная объединением 20-байтовых значений SHA1-хэшей каждого куска (один кусок — один хэш)
    PROP_GET( bool,             IsPeersFromTrackersOnly )//private
    PROP_GET( QByteArray,       InfoHashSHA1 )  //SHA1 хэшсумма блока info

    //name Имя корневой директории, которую содержит торрент. Носит рекомендательный характер.
    //Для однофайловых не используется
    PROP_GET( QString,          RootDirName )

public:
    const QUrl&             GetTrackerUrl() const{ return m_TrackerUrlList[0]; }
    bool                    IsSingleFile() const { return m_FilesInfo.size() == 1; }
    int                     GetFilesCount() const{ return m_FilesInfo.size(); }
    const FileInfo&         GetFileInfo( uint pos );
    const FileInfo&         GetFileInfo( uint pos ) const;

    const FileInfo&         GetFileInfo();
    const FileInfo&         GetFileInfo() const;

    quint64                 GetTotalFilesSize() const;

//// Static
public:
    static TorrentFileInfo parse(const QByteArray &fileContent);

private:

    QList< FileInfo >       m_FilesInfo;
};

#endif // TORRENTFILEINFO_H
