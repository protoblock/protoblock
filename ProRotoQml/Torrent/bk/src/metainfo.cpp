#include "bencodeparser.h"
#include "metainfo.h"

#include <QDateTime>
#include <QMetaType>
#include <QString>

MetaInfo::MetaInfo()
{
    clear();
}

MetaInfo::MetaInfo(const MetaInfo &other)
{
    copy( other );
}

MetaInfo& MetaInfo::operator =(const MetaInfo &other)
{
    copy( other );
    return *this;
}

MetaInfo::MetaInfo(MetaInfo &&other)
{
    errString           = std::move( other.errString );
    content             = std::move( other.content );
    infoData            = std::move( other.infoData );
    metaInfoMultiFiles  = std::move( other.metaInfoMultiFiles);
    metaInfoAnnounceList= std::move( other.metaInfoAnnounceList);
    metaInfoCreationDate= std::move( other.metaInfoCreationDate );
    metaInfoComment     = std::move( other.metaInfoComment );
    metaInfoCreatedBy   = std::move( other.metaInfoCreatedBy );
    metaInfoName        = std::move( other.metaInfoName );
    metaInfoPieceLength = std::move( other.metaInfoPieceLength );
    metaInfoSha1Sums    = std::move( other.metaInfoSha1Sums );
}

MetaInfo& MetaInfo::operator =(MetaInfo &&other)
{
    errString           = std::move( other.errString );
    content             = std::move( other.content );
    infoData            = std::move( other.infoData );
    metaInfoMultiFiles  = std::move( other.metaInfoMultiFiles);
    metaInfoAnnounceList= std::move( other.metaInfoAnnounceList);
    metaInfoCreationDate= std::move( other.metaInfoCreationDate );
    metaInfoComment     = std::move( other.metaInfoComment );
    metaInfoCreatedBy   = std::move( other.metaInfoCreatedBy );
    metaInfoName        = std::move( other.metaInfoName );
    metaInfoPieceLength = std::move( other.metaInfoPieceLength );
    metaInfoSha1Sums    = std::move( other.metaInfoSha1Sums );
    return *this;
}

void MetaInfo::clear()
{
    errString = "Unknown error";
    content.clear();
    infoData.clear();
    metaInfoMultiFiles.clear();
    metaInfoAnnounceList.clear();
    metaInfoCreationDate = QDateTime();
    metaInfoComment.clear();
    metaInfoCreatedBy.clear();
    metaInfoName.clear();
    metaInfoPieceLength = 0;
    metaInfoSha1Sums.clear();
}

void MetaInfo::copy(const MetaInfo &src)
{
    errString           = src.errString;
    content             = src.content;
    infoData            = src.infoData;
    metaInfoMultiFiles  = src.metaInfoMultiFiles;
    metaInfoAnnounceList= src.metaInfoAnnounceList;
    metaInfoCreationDate= src.metaInfoCreationDate;
    metaInfoComment     = src.metaInfoComment;
    metaInfoCreatedBy   = src.metaInfoCreatedBy;
    metaInfoName        = src.metaInfoName;
    metaInfoPieceLength = src.metaInfoPieceLength;
    metaInfoSha1Sums    = src.metaInfoSha1Sums;
}

bool MetaInfo::parse(const QByteArray &data)
{
    clear();
    content = data;

    BencodeParser parser;
    if (!parser.parse(content)) {
        errString = parser.errorString();
        return false;
    }

    infoData = parser.infoSection();

    QMap<QByteArray, QVariant> dict = parser.dictionary();
    if (!dict.contains("info"))
        return false;

    QMap<QByteArray, QVariant> info = qvariant_cast<Dictionary>(dict.value("info"));

    if (info.contains("files")) {
        metaInfoFileForm = MultiFileForm;

        QList<QVariant> files = info.value("files").toList();

        for (int i = 0; i < files.size(); ++i) {
            QMap<QByteArray, QVariant> file = qvariant_cast<Dictionary>(files.at(i));
            QList<QVariant> pathElements = file.value("path").toList();
            QByteArray path;
            for (QVariant &p : pathElements) {
                if (!path.isEmpty())
                    path += "/";
                path += p.toByteArray();
            }

            MetaInfoMultiFile multiFile;
            multiFile.Length = file.value("length").toLongLong();
            multiFile.Path = QString::fromUtf8(path);
            multiFile.MD5sum = file.value("md5sum").toByteArray();
            metaInfoMultiFiles << multiFile;
        }

        metaInfoName = QString::fromUtf8(info.value("name").toByteArray());
        metaInfoPieceLength = info.value("piece length").toInt();
        QByteArray pieces = info.value("pieces").toByteArray();
        for (int i = 0; i < pieces.size(); i += 20)
            metaInfoSha1Sums << pieces.mid(i, 20);
    } else if (info.contains("length")) {
        metaInfoFileForm = SingleFileForm;
        metaInfoSingleFile.Length = info.value("length").toLongLong();
        metaInfoSingleFile.MD5sum = info.value("md5sum").toByteArray();
        metaInfoSingleFile.Name = QString::fromUtf8(info.value("name").toByteArray());
        metaInfoSingleFile.PieceLength = info.value("piece length").toInt();

        QByteArray pieces = info.value("pieces").toByteArray();
        for (int i = 0; i < pieces.size(); i += 20)
            metaInfoSha1Sums << pieces.mid(i, 20);
    }

    if (dict.contains("announce-list")) {
        metaInfoAnnounceList = dict[ "announce-list" ].toStringList();
    }

    metaInfoAnnounceList << QString::fromUtf8(dict.value("announce").toByteArray());

    if (dict.contains("creation date"))
        metaInfoCreationDate.setTime_t(dict.value("creation date").toInt());
    if (dict.contains("comment"))
        metaInfoComment = QString::fromUtf8(dict.value("comment").toByteArray());
    if (dict.contains("created by"))
        metaInfoCreatedBy = QString::fromUtf8(dict.value("created by").toByteArray());

    return true;
}

QByteArray MetaInfo::infoValue() const
{
    return infoData;
}

QString MetaInfo::errorString() const
{
    return errString;
}

MetaInfo::FileForm MetaInfo::fileForm() const
{
    return metaInfoFileForm;
}

const QStringList &MetaInfo::announceList() const
{
    return metaInfoAnnounceList;
}

const QDateTime &MetaInfo::creationDate() const
{
    return metaInfoCreationDate;
}

const QString &MetaInfo::comment() const
{
    return metaInfoComment;
}

const QString &MetaInfo::createdBy() const
{
    return metaInfoCreatedBy;
}

const MetaInfoSingleFile &MetaInfo::singleFile() const
{
    return metaInfoSingleFile;
}

const QList<MetaInfoMultiFile> &MetaInfo::multiFiles() const
{
    return metaInfoMultiFiles;
}

const QString &MetaInfo::name() const
{
    return metaInfoName;
}

int MetaInfo::pieceLength() const
{
    return metaInfoPieceLength;
}

const QList<QByteArray> &MetaInfo::sha1Sums() const
{
    return metaInfoSha1Sums;
}

qint64 MetaInfo::totalSize() const
{
    if (fileForm() == SingleFileForm)
        return singleFile().Length;

    qint64 size = 0;
    for (auto &file : multiFiles())
        size += file.Length;
    return size;
}
