#ifndef TORRENTFILEPARSER_H
#define TORRENTFILEPARSER_H

#include <QObject>
#include <QByteArray>
#include <QList>
#include <QVariant>
#include <QHash>
#include <QSharedPointer>

typedef QHash<QByteArray,QVariant> BencodeHash;
Q_DECLARE_METATYPE(BencodeHash)

class TorrentFileParser : public QObject
{
    Q_OBJECT
public:
    TorrentFileParser(QObject *parent = nullptr);
    ~TorrentFileParser(){}

    bool parse(const QByteArray &data );
    void clear();

    QByteArray GetInfoArray() const;
    BencodeHash GetBencodeHash() const;

private:
    bool    parseString( QByteArray &byteString );
    bool    parseNumber( qint64 &number );
    bool    parseList(QList< QVariant > &resList );
    bool    parseHash(QHash< QByteArray, QVariant > &resHash );

    bool    parseStringLength( int &length );
    QChar   currentCharacter() const;
    QChar   nextCharacter() const;
    bool    endOfData() const;

protected:
    void emitError( const QString &errorString );

signals:
    void error( const QString &errorString );

//// Data
private:
    BencodeHash         m_hashRes;

    QByteArray          m_bencodeData;
    qint64              m_nextPos;

    qint64              m_infoStartPos;
    qint64              m_lengthOfInfo;

    QString             m_lastErrorString;
};

#endif // TORRENTFILEPARSER_H
