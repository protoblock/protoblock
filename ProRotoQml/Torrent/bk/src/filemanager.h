#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QBitArray>
#include <QList>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include "metainfo.h"

QT_BEGIN_NAMESPACE
class QByteArray;
class QFile;
class QTimerEvent;
QT_END_NAMESPACE

class FileManager : public QThread
{
    Q_OBJECT

public:
    FileManager(QObject *parent = 0);
    virtual ~FileManager();

//// setters
    inline void setMetaInfo(const MetaInfo &info) { m_metaInfo = info; }
    inline void setDestinationFolder(const QString &directory) { m_destinationPath = directory; }
//// end setters

    int     read(int pieceIndex, int offset, int length);
    void    write(int pieceIndex, int offset, const QByteArray &data);
    void    verifyPiece(int pieceIndex);

//// const
    int             pieceLengthAt(int pieceIndex) const;
    inline qint64   totalSize() const { return m_totalLength; }
    inline int      pieceCount() const { return m_numPieces; }
    QBitArray       completedPieces() const;
    void            setCompletedPieces(const QBitArray &pieces);
    QString         errorString() const;
//// end const

public slots:
    void startDataVerification();

signals:
    void dataRead(int id, int pieceIndex, int offset, const QByteArray &data);
    void error();
    void verificationProgress(int percent);
    void verificationDone();
    void pieceVerified(int pieceIndex, bool verified);

protected:
    void run() override;

private slots:
    bool verifySinglePiece(int pieceIndex);
    void wakeUp();

private:
    bool        generateFiles();
    QByteArray  readBlock(int pieceIndex, int offset, int length);
    bool        writeBlock(int pieceIndex, int offset, const QByteArray &data);
    void        verifyFileContents();

    struct WriteRequest {
        int         PieceIndex;
        int         Offset;
        QByteArray  Data;
    };
    struct ReadRequest {
        int     PieceIndex;
        int     Offset;
        int     Length;
        int     Id;
    };

    QString             m_errString;
    QString             m_destinationPath;
    MetaInfo            m_metaInfo;
    QList<QFile *>      m_files;
    QList<QByteArray>   m_sha1Sum;
    QBitArray           m_verifiedPieces;

    bool                m_newFile;
    int                 m_pieceLength;
    qint64              m_totalLength;
    int                 m_numPieces;
    int                 m_readId;
    bool                m_startVerification;
    bool                m_quit;
    bool                m_wokeUp;

    QList<WriteRequest> m_writeRequests;
    QList<ReadRequest>  m_readRequests;
    QList<int>          m_pendingVerificationRequests;
    QList<int>          m_newPendingVerificationRequests;
    QList<qint64>       m_fileSizes;

    mutable QMutex          m_mutex;
    mutable QWaitCondition  m_cond;
};

#endif
