#include "bencodeparser.h"

#include <QList>
#include <QMetaType>

BencodeParser::BencodeParser()
{
}

bool BencodeParser::parse(const QByteArray &sourse)
{
    if (sourse.isEmpty()) {
        m_errString = QStringLiteral("No content");
        return false;
    }

    m_sourse = sourse;
    m_index = 0;
    m_infoStart = 0;
    m_infoLength = 0;
    return getDictionary( &m_dictionaryValue );
}

QString BencodeParser::errorString() const
{
    return m_errString;
}

QMap<QByteArray, QVariant> BencodeParser::dictionary() const
{
    return m_dictionaryValue;
}

QByteArray BencodeParser::infoSection() const
{
    return m_sourse.mid(m_infoStart, m_infoLength);
}

bool BencodeParser::getByteString(QByteArray *byteString)
{
    const int contentSize = m_sourse.size();
    int size = -1;
    do {
        char c = m_sourse.at(m_index);
        if (c < '0' || c > '9') {
            if (size == -1)
                return false;
            if (c != ':') {
                m_errString = QString("Unexpected character at pos %1: %2")
                        .arg(m_index).arg(c);
                return false;
            }
            ++m_index;
            break;
        }
        if (size == -1)
            size = 0;
        size *= 10;
        size += c - '0';
    } while (++m_index < contentSize);

    if (byteString)
        *byteString = m_sourse.mid(m_index, size);
    m_index += size;
    return true;
}

bool BencodeParser::getInteger(qint64 *integer)
{
    const int contentSize = m_sourse.size();
    if (m_sourse.at(m_index) != 'i')
        return false;

    ++m_index;
    qint64 num = -1;
    bool negative = false;

    do {
        char c = m_sourse.at(m_index);
        if (c < '0' || c > '9') {
            if (num == -1) {
                if (c != '-' || negative)
                    return false;
                negative = true;
                continue;
            } else {
                if (c != 'e') {
                    m_errString = QString("Unexpected character at pos %1: %2")
                            .arg(m_index).arg(c);
                    return false;
                }
                ++m_index;
                break;
            }
        }
        if (num == -1)
            num = 0;
        num *= 10;
        num += c - '0';
    } while (++m_index < contentSize);

    if (integer)
        *integer = negative ? -num : num;
    return true;
}

bool BencodeParser::getList(QList<QVariant> *list)
{
    const int contentSize = m_sourse.size();
    if (m_sourse.at(m_index) != 'l')
        return false;

    QList<QVariant> tmp;
    ++m_index;

    do {
        if (m_sourse.at(m_index) == 'e') {
            ++m_index;
            break;
        }

        qint64 number;
        QByteArray byteString;
        QList<QVariant> tmpList;
        QMap<QByteArray, QVariant> dictionary;

        if (getInteger(&number))
            tmp << number;
        else if (getByteString(&byteString))
            tmp << byteString;
        else if (getList(&tmpList))
            tmp << tmpList;
        else if (getDictionary(&dictionary))
            tmp << QVariant::fromValue<QMap<QByteArray, QVariant> >(dictionary);
        else {
            m_errString = QString("error at index %1").arg(m_index);
            return false;
        }
    } while (m_index < contentSize);

    if (list)
        *list = tmp;
    return true;
}

bool BencodeParser::getDictionary(QMap<QByteArray, QVariant> *dictionary)
{
    const int contentSize = m_sourse.size();
    if (m_sourse.at(m_index) != 'd')
        return false;

    QMap<QByteArray, QVariant> tmp;
    ++m_index;

    do {
        if (m_sourse.at(m_index) == 'e') {
            ++m_index;
            break;
        }

        QByteArray key;
        if (!getByteString(&key))
            break;

        if (key == "info")
            m_infoStart = m_index;

        qint64 number;
        QByteArray byteString;
        QList<QVariant> tmpList;
        QMap<QByteArray, QVariant> dictionary;

        if (getInteger(&number))
            tmp.insert(key, number);
        else if (getByteString(&byteString))
            tmp.insert(key, byteString);
        else if (getList(&tmpList))
            tmp.insert(key, tmpList);
        else if (getDictionary(&dictionary))
            tmp.insert(key, QVariant::fromValue<QMap<QByteArray, QVariant> >(dictionary));
        else {
            m_errString = QString("error at index %1").arg(m_index);
            return false;
        }

        if (key == "info")
            m_infoLength = m_index - m_infoStart;

    } while (m_index < contentSize);

    if (dictionary)
        *dictionary = tmp;
    return true;
}
