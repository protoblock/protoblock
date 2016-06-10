#include "filemanager.h"
#include "metainfo.h"

#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QTimerEvent>
#include <QCryptographicHash>

FileManager::FileManager(QObject *parent)
    : QThread(parent)
{
    m_quit = false;
    m_totalLength = 0;
    m_readId = 0;
    m_startVerification = false;
    m_wokeUp = false;
    m_newFile = false;
    m_numPieces = 0;
    m_verifiedPieces.fill(false);
}

FileManager::~FileManager()
{
    m_quit = true;
    m_cond.wakeOne();
    wait();

    for (QFile *file : m_files) {
        file->close();
        delete file;
    }
}

int FileManager::read(int pieceIndex, int offset, int length)
{
    ReadRequest request;
    request.PieceIndex = pieceIndex;
    request.Offset = offset;
    request.Length = length;

    QMutexLocker locker(&m_mutex);
    request.Id = m_readId++;
    m_readRequests << request;

    if (!m_wokeUp) {
        m_wokeUp = true;
        QMetaObject::invokeMethod(this, "wakeUp", Qt::QueuedConnection);
    }

    return request.Id;
}

void FileManager::write(int pieceIndex, int offset, const QByteArray &data)
{
    WriteRequest request;
    request.PieceIndex = pieceIndex;
    request.Offset = offset;
    request.Data = data;

    QMutexLocker locker(&m_mutex);
    m_writeRequests << request;

    if (!m_wokeUp) {
        m_wokeUp = true;
        QMetaObject::invokeMethod(this, "wakeUp", Qt::QueuedConnection);
    }
}

void FileManager::verifyPiece(int pieceIndex)
{
    QMutexLocker locker(&m_mutex);
    m_pendingVerificationRequests << pieceIndex;
    m_startVerification = true;

    if (!m_wokeUp) {
        m_wokeUp = true;
        QMetaObject::invokeMethod(this, "wakeUp", Qt::QueuedConnection);
    }
}

int FileManager::pieceLengthAt(int pieceIndex) const
{
    QMutexLocker locker(&m_mutex);
    return (m_sha1Sum.size() == pieceIndex + 1)
        ? (m_totalLength % m_pieceLength) : m_pieceLength;
}

QBitArray FileManager::completedPieces() const
{
    QMutexLocker locker(&m_mutex);
    return m_verifiedPieces;
}

void FileManager::setCompletedPieces(const QBitArray &pieces)
{
    QMutexLocker locker(&m_mutex);
    m_verifiedPieces = pieces;
}

QString FileManager::errorString() const
{
    return m_errString;
}

void FileManager::run()
{
    if (!generateFiles())
        return;

    do {
        {
            // Go to sleep if there's nothing to do.
            QMutexLocker locker(&m_mutex);
            if (!m_quit && m_readRequests.isEmpty() && m_writeRequests.isEmpty() && !m_startVerification)
                m_cond.wait(&m_mutex);
        }

        // Read pending read requests
        m_mutex.lock();
        QList<ReadRequest> newReadRequests = m_readRequests;
        m_readRequests.clear();
        m_mutex.unlock();
        while (!newReadRequests.isEmpty()) {
            ReadRequest request = newReadRequests.takeFirst();
            QByteArray block = readBlock(request.PieceIndex, request.Offset, request.Length);
            emit dataRead(request.Id, request.PieceIndex, request.Offset, block);
        }

        // Write pending write requests
        m_mutex.lock();
        QList<WriteRequest> newWriteRequests = m_writeRequests;
        m_writeRequests.clear();
        while (!m_quit && !newWriteRequests.isEmpty()) {
            WriteRequest request = newWriteRequests.takeFirst();
            writeBlock(request.PieceIndex, request.Offset, request.Data);
        }

        // Process pending verification requests
        if (m_startVerification) {
            m_newPendingVerificationRequests = m_pendingVerificationRequests;
            m_pendingVerificationRequests.clear();
            verifyFileContents();
            m_startVerification = false;
        }
        m_mutex.unlock();
        m_newPendingVerificationRequests.clear();

    } while (!m_quit);

    // Write pending write requests
    m_mutex.lock();
    QList<WriteRequest> newWriteRequests = m_writeRequests;
    m_writeRequests.clear();
    m_mutex.unlock();
    while (!newWriteRequests.isEmpty()) {
        WriteRequest request = newWriteRequests.takeFirst();
        writeBlock(request.PieceIndex, request.Offset, request.Data);
    }
}

void FileManager::startDataVerification()
{
    QMutexLocker locker(&m_mutex);
    m_startVerification = true;
    m_cond.wakeOne();
}

bool FileManager::generateFiles()
{
    m_numPieces = -1;

    // Set up the thread local data
    if (m_metaInfo.fileForm() == MetaInfo::SingleFileForm) {
        QMutexLocker locker(&m_mutex);
        MetaInfoSingleFile singleFile = m_metaInfo.singleFile();

        QString prefix;
        if (!m_destinationPath.isEmpty()) {
            prefix = m_destinationPath;
            if (!prefix.endsWith("/"))
                prefix += "/";
            QDir dir;
            if (!dir.mkpath(prefix)) {
                m_errString = tr("Failed to create directory %1").arg(prefix);
                emit error();
                return false;
            }
        }
        QFile *file = new QFile(prefix + singleFile.Name);
        if (!file->open(QFile::ReadWrite)) {
            m_errString = tr("Failed to open/create file %1: %2")
                        .arg(file->fileName()).arg(file->errorString());
            emit error();
            delete file;
            return false;
        }

        if (file->size() != singleFile.Length) {
            m_newFile = true;
            if (!file->resize(singleFile.Length)) {
                m_errString = tr("Failed to resize file %1: %2")
                            .arg(file->fileName()).arg(file->errorString());
                delete file;
                emit error();
                return false;
            }
        }
        m_fileSizes << file->size();
        m_files << file;
        file->close();

        m_pieceLength = singleFile.PieceLength;
        m_totalLength = singleFile.Length;
        m_sha1Sum = m_metaInfo.sha1Sums();
    } else {
        QMutexLocker locker(&m_mutex);
        QDir dir;
        QString prefix;

        if (!m_destinationPath.isEmpty()) {
            prefix = m_destinationPath;
            if (!prefix.endsWith("/"))
                prefix += "/";
        }
        if (!m_metaInfo.name().isEmpty()) {
            prefix += m_metaInfo.name();
            if (!prefix.endsWith("/"))
                prefix += "/";
        }
        if (!dir.mkpath(prefix)) {
            m_errString = tr("Failed to create directory %1").arg(prefix);
            emit error();
            return false;
        }

        for (const auto &entry : m_metaInfo.multiFiles()) {
            QString filePath = QFileInfo(prefix + entry.Path).path();
            if (!QFile::exists(filePath)) {
                if (!dir.mkpath(filePath)) {
                    m_errString = tr("Failed to create directory %1").arg(filePath);
                    emit error();
                    return false;
                }
            }

            QFile *file = new QFile(prefix + entry.Path);
            if (!file->open(QFile::ReadWrite)) {
                m_errString = tr("Failed to open/create file %1: %2")
                            .arg(file->fileName()).arg(file->errorString());
                emit error();
                delete file;
                return false;
            }

            if (file->size() != entry.Length) {
                m_newFile = true;
                if (!file->resize(entry.Length)) {
                    m_errString = tr("Failed to resize file %1: %2")
                                .arg(file->fileName()).arg(file->errorString());
                    emit error();
                    delete file;
                    return false;
                }
            }
            m_fileSizes << file->size();
            m_files << file;
            file->close();

            m_totalLength += entry.Length;
        }

        m_sha1Sum = m_metaInfo.sha1Sums();
        m_pieceLength = m_metaInfo.pieceLength();
    }
    m_numPieces = m_sha1Sum.size();
    return true;
}

QByteArray FileManager::readBlock(int pieceIndex, int offset, int length)
{
    QByteArray block;
    qint64 startReadIndex = (quint64(pieceIndex) * m_pieceLength) + offset;
    qint64 currentIndex = 0;

    for (int i = 0; !m_quit && i < m_files.size() && length > 0; ++i) {
        QFile *file = m_files[i];
        qint64 currentFileSize = m_fileSizes.at(i);
        if ((currentIndex + currentFileSize) > startReadIndex) {
            if (!file->isOpen()) {
                if (!file->open(QFile::ReadWrite)) {
                    m_errString = tr("Failed to read from file %1: %2")
                        .arg(file->fileName()).arg(file->errorString());
                    emit error();
                    break;
                }
            }

            file->seek(startReadIndex - currentIndex);
            QByteArray chunk = file->read(qMin<qint64>(length, currentFileSize - file->pos()));
            file->close();

            block += chunk;
            length -= chunk.size();
            startReadIndex += chunk.size();
            if (length < 0) {
                m_errString = tr("Failed to read from file %1 (read %3 bytes): %2")
                            .arg(file->fileName()).arg(file->errorString()).arg(length);
                emit error();
                break;
            }
        }
        currentIndex += currentFileSize;
    }
    return block;
}

bool FileManager::writeBlock(int pieceIndex, int offset, const QByteArray &data)
{
    qint64 startWriteIndex = (qint64(pieceIndex) * m_pieceLength) + offset;
    qint64 currentIndex = 0;
    int bytesToWrite = data.size();
    int written = 0;

    for (int i = 0; !m_quit && i < m_files.size(); ++i) {
        QFile *file = m_files[i];
        qint64 currentFileSize = m_fileSizes.at(i);

        if ((currentIndex + currentFileSize) > startWriteIndex) {
            if (!file->isOpen()) {
                if (!file->open(QFile::ReadWrite)) {
                    m_errString = tr("Failed to write to file %1: %2")
                        .arg(file->fileName()).arg(file->errorString());
                    emit error();
                    break;
                }
            }

            file->seek(startWriteIndex - currentIndex);
            qint64 bytesWritten = file->write(data.constData() + written,
                                              qMin<qint64>(bytesToWrite, currentFileSize - file->pos()));
            file->close();

            if (bytesWritten <= 0) {
                m_errString = tr("Failed to write to file %1: %2")
                            .arg(file->fileName()).arg(file->errorString());
                emit error();
                return false;
            }

            written += bytesWritten;
            startWriteIndex += bytesWritten;
            bytesToWrite -= bytesWritten;
            if (bytesToWrite == 0)
                break;
        }
        currentIndex += currentFileSize;
    }
    return true;
}

void FileManager::verifyFileContents()
{
    // Verify all pieces the first time
    if (m_newPendingVerificationRequests.isEmpty()) {
        if (m_verifiedPieces.count(true) == 0) {
            m_verifiedPieces.resize(m_sha1Sum.size());

            int oldPercent = 0;
            if (!m_newFile) {
                int numPieces = m_sha1Sum.size();

                for (int index = 0; index < numPieces; ++index) {
                    verifySinglePiece(index);

                    int percent = ((index + 1) * 100) / numPieces;
                    if (oldPercent != percent) {
                        emit verificationProgress(percent);
                        oldPercent = percent;
                    }
                }
            }
        }
        emit verificationDone();
        return;
    }

    // Verify all pending pieces
    for (int index : m_newPendingVerificationRequests)
        emit pieceVerified(index, verifySinglePiece(index));
}

bool FileManager::verifySinglePiece(int pieceIndex)
{
    QByteArray block = readBlock(pieceIndex, 0, m_pieceLength);
    QByteArray sha1Sum = QCryptographicHash::hash(block, QCryptographicHash::Sha1);

    if (sha1Sum != m_sha1Sum.at(pieceIndex))
        return false;
    m_verifiedPieces.setBit(pieceIndex);
    return true;
}

void FileManager::wakeUp()
{
    QMutexLocker locker(&m_mutex);
    m_wokeUp = false;
    m_cond.wakeOne();
}
