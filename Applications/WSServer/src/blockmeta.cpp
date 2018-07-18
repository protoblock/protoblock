#include "blockmeta.h"

QBlockMeta::QBlockMeta(QObject *parent) :
    QObject(parent)
{
    pLdbHashReaderTool = LdbHashReaderTool::instance();

    pLdbHashReaderTool->readData(pLdbHashReaderTool->readstr("blockhead"),bm);

//    pLdbHashReaderTool->readstr(pLdbHashReaderTool->readstr("blockhead"),bmstr);

}

QString QBlockMeta::next(){
    QString nx = QString::fromStdString (bm.next ());
    return nx;
}

QString QBlockMeta::prev(){
    QString pre = QString::fromStdString (bm.prev ());
    return pre;
}

qint32 QBlockMeta::blocknum()
{
    return bm.blocknum();
}

QString QBlockMeta::pbState()
{
    QString pbstate = QString::fromStdString ( bm.pbstateid() );
    return pbstate;
}

QString QBlockMeta::getJson()
{
//    std::string str;
//    pbjson::pb2json(&bm,str);
//    QString json = str.data();
    return "json";
}

QStringList QBlockMeta::all(){
    QString a = bm.DebugString ().data ();
    QStringList sL = a.split (QRegExp("\n"));
    foreach (QString i , sL) {
        aList << i;
    }
    return aList;
}

