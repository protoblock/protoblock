#ifndef BENCODEPARSER_H
#define BENCODEPARSER_H

#include <QByteArray>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QList>

typedef QMap<QByteArray,QVariant> Dictionary;
Q_DECLARE_METATYPE(Dictionary)

class BencodeParser
{
public:
    BencodeParser();

    bool parse(const QByteArray &sourse);
    QString errorString() const;

    QMap<QByteArray, QVariant> dictionary() const;
    QByteArray infoSection() const;

private:
    bool getByteString(QByteArray *byteString);
    bool getInteger(qint64 *integer);
    bool getList(QList<QVariant> *list);
    bool getDictionary(QMap<QByteArray, QVariant> *dictionary);

    QMap<QByteArray, QVariant>  m_dictionaryValue;

    QString                     m_errString;
    QByteArray                  m_sourse;
    int                         m_index;

    int                         m_infoStart;
    int                         m_infoLength;
};

#endif
