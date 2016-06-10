#ifndef METAINFO_H
#define METAINFO_H

#include <QByteArray>
#include <QDateTime>
#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QVariant>

struct MetaInfoSingleFile
{
    qint64              Length;
    QByteArray          MD5sum;
    QString             Name;
    int                 PieceLength;
    //QList<QByteArray>   SHA1SumList;
};

struct MetaInfoMultiFile
{
    qint64      Length;
    QByteArray  MD5sum;
    QString     Path;
};

class MetaInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString metaInfoName READ name )
public:

    enum FileForm {
        SingleFileForm,
        MultiFileForm
    };

    MetaInfo();
    MetaInfo( const MetaInfo &other );
    MetaInfo(       MetaInfo &&other );
    MetaInfo& operator =( const MetaInfo &other );
    MetaInfo& operator =(       MetaInfo &&other );

    void clear();
    void copy( const MetaInfo &src );

    bool parse(const QByteArray &data);
    QString errorString() const;

    QByteArray infoValue() const;

    FileForm fileForm() const;
    const QStringList &announceList() const;
    const QDateTime &creationDate() const;
    const QString &comment() const;
    const QString &createdBy() const;

    // For single file form
    const MetaInfoSingleFile &singleFile() const;

    // For multifile form
    const QList<MetaInfoMultiFile> &multiFiles() const;
    const QString &name() const;
    int pieceLength() const;
    const QList<QByteArray> &sha1Sums() const;

    // Total size
    qint64 totalSize() const;

private:
    QString errString;
    QByteArray content;
    QByteArray infoData;

    FileForm metaInfoFileForm;
    MetaInfoSingleFile metaInfoSingleFile;
    QList<MetaInfoMultiFile> metaInfoMultiFiles;
    QStringList metaInfoAnnounceList;
    QDateTime metaInfoCreationDate;
    QString metaInfoComment;
    QString metaInfoCreatedBy;
    QString metaInfoName;
    int metaInfoPieceLength;
    QList<QByteArray> metaInfoSha1Sums;
};

Q_DECLARE_METATYPE( MetaInfo )

#endif
