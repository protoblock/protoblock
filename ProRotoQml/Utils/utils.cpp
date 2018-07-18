#include "utils.h"





Utils::Utils(QObject *parent) :
    QObject(parent)
{

}



QString Utils::formatSize(const int bytes)
{
    float num = bytes;
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while(num >= 1024.0 && i.hasNext())
     {
        unit = i.next();
        num /= 1024.0;
    }
    return QString().setNum(num,'f',2)+" "+unit;
}


