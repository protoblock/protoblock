#ifndef LDBHASHREADERMAP_H
#define LDBHASHREADERMAP_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QDebug>

class LdbHashReaderMap : public QObject
{
    Q_OBJECT
public:
    explicit LdbHashReaderMap(QObject *parent = 0);
    Q_INVOKABLE QString getKey(const QString key);
    void setMap();
private:
    QMap<QString , QString> m_map;
};

#endif // LDBHASHREADERMAP_H
