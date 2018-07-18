#ifndef BLOCKMETA_H
#define BLOCKMETA_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "StateData.pb.h"
#include "ldbhashreadertool.h"
//#include "pbjson.hpp"
#include "protoblockapi.h"

class QBlockMeta : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString prev READ prev )
    Q_PROPERTY(QString next READ next  )
    Q_PROPERTY(qint32 blocknum READ blocknum)
    Q_PROPERTY(QString pbState READ pbState )
    Q_PROPERTY(QStringList  all READ  all)
public:
    explicit QBlockMeta(QObject *parent = 0);
    LdbHashReaderTool *pLdbHashReaderTool;

    QString next();
    QString prev();
    qint32 blocknum();
    QString pbState();

    // This is like getting children
    Q_INVOKABLE QString getJson();
    // add this to a utils file to use later on
    QStringList all();

    //    QString blockmetastr() {
    //        return bmstr();
    //    }
signals:

public slots:


private:
    //    std::string bmstr;
    fantasybit::BlockMeta bm;
    QStringList aList;


};

#endif // BLOCKMETA_H
