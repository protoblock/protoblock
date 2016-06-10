#include "torrentfileinfo.h"
#include <QSharedPointer>
#include <QFile>
#include <QDebug>
#include <QHash>
#include <QVariant>
#include <QByteArray>
#include <QDir>
#include <QCryptographicHash>

void TorrentFileInfo::clear()
{
    m_Comment.clear();
    m_CreatedBy.clear();
    m_CreationDate.setTime_t(0);
    m_FilesInfo.clear();
    m_InfoHashSHA1.clear();
    m_IsPeersFromTrackersOnly = false;
    m_PieceLength = 0;
    m_Pieces.clear();
    m_RootDirName.clear();
    m_TrackerUrlList.clear();
}

const FileInfo &TorrentFileInfo::GetFileInfo(uint pos)
{
    if ( pos >= m_FilesInfo.size() ){
        qCritical() << "Index out of range" << Q_FUNC_INFO;
        return m_FilesInfo[ 0 ];
    }
    return m_FilesInfo[ pos ];
}

const FileInfo &TorrentFileInfo::GetFileInfo(uint pos) const
{
    if ( pos >= m_FilesInfo.size() ){
        qCritical() << "Index out of range" << Q_FUNC_INFO;
        return m_FilesInfo[ 0 ];
    }
    return m_FilesInfo[ pos ];
}

const FileInfo &TorrentFileInfo::GetFileInfo()
{
    Q_ASSERT( m_FilesInfo.size() > 0 );
    return m_FilesInfo[0];
}

const FileInfo &TorrentFileInfo::GetFileInfo() const
{
    Q_ASSERT( m_FilesInfo.size() > 0 );
    return m_FilesInfo[0];
}

quint64 TorrentFileInfo::GetTotalFilesSize() const
{
    quint64 totalSize = 0;
    for ( auto &file : m_FilesInfo ){
        totalSize += file.FileSize;
    }
    return totalSize;
}


//// Static
TorrentFileInfo TorrentFileInfo::parse(const QByteArray &fileContent )
{
    const int SHA1_HASH_LENGTH = 20;

    TorrentFileParser parser;
    QObject::connect(&parser, &TorrentFileParser::error, [](const QString &err){
        qWarning() << "Parser error : " << err;
    } );

    if ( !parser.parse( fileContent ) ){
        return TorrentFileInfo();
    }

    auto getIfContains = []( BencodeHash &hash, const QByteArray &fild ) -> QVariant
    {
        if ( hash.contains( fild ) ){
            return hash[ fild ];
        }
        else{
            qDebug() << "Fild " << fild << " is missing";
            return QVariant();
        }
    };

    auto hash = parser.GetBencodeHash();

    TorrentFileInfo torrentFileInfo;
    torrentFileInfo.m_Comment      = getIfContains( hash, "comment" ).toString();
    torrentFileInfo.m_CreatedBy    = getIfContains( hash, "created by" ).toString();
    auto dateTime                   = getIfContains( hash, "creation date" ).toLongLong();
    torrentFileInfo.m_CreationDate.setTime_t( dateTime );

    auto announce = getIfContains( hash, "announce" ).toString();
    if ( !announce.isEmpty() ){
        torrentFileInfo.m_TrackerUrlList.append( QUrl( announce ) );
    }

    auto annonceList = getIfContains( hash, "announce-list" ).toStringList();
    for ( auto curTracker : annonceList ){
        torrentFileInfo.m_TrackerUrlList.append( QUrl(curTracker) );
    }

    if ( hash.contains( "info" ) ){
        torrentFileInfo.m_InfoHashSHA1 = QCryptographicHash::hash( parser.GetInfoArray(), QCryptographicHash::Sha1 );

        auto info = hash[ "info" ].value< BencodeHash >();
        torrentFileInfo.m_PieceLength = getIfContains( info, "piece length" ).toLongLong();

        QByteArray piecesString = getIfContains( info, "pieces" ).toByteArray();
        qint64 pos = 0;
        while ( pos < piecesString.size() ){
            torrentFileInfo.m_Pieces.append( piecesString.mid(pos, SHA1_HASH_LENGTH) );
            pos += SHA1_HASH_LENGTH;
        }

        torrentFileInfo.m_IsPeersFromTrackersOnly = info.contains("private") ?
                                                     ( hash["private"].toInt() == 1 ? true : false ) : false;

        if ( info.contains("files") ){      // MultiFile
            torrentFileInfo.m_RootDirName = getIfContains( info, "name" ).toString();
            QList< QVariant > files = getIfContains( info, "files" ).toList();
            for ( auto curFileVariant : files ){
                BencodeHash curFile = curFileVariant.value< BencodeHash >();

                FileInfo curFileInfo;
                QStringList directoryList = getIfContains( curFile, "path" ).toStringList();
                if ( directoryList.size() > 0 ){
                    curFileInfo.FilePath = directoryList.join('\\');
                }
                curFileInfo.FileSize = getIfContains( curFile, "length" ).toLongLong();
                curFileInfo.MD5Sum   = getIfContains( curFile, "md5sum" ).toByteArray();
                torrentFileInfo.m_FilesInfo.append( curFileInfo );
            }
        }
        else{                               // SingleFile
            FileInfo fileInfo;
                     fileInfo.FileSize = getIfContains( info, "length" ).toLongLong();
                     fileInfo.FilePath = getIfContains( info, "name" ).toString();
                     fileInfo.MD5Sum   = getIfContains( info, "md5sum" ).toByteArray();
            torrentFileInfo.m_FilesInfo.append( fileInfo );
        }
    }
    else{
        qCritical() << "No \'info\' in .torrent file";
        return TorrentFileInfo();
    }

    return torrentFileInfo;
}
