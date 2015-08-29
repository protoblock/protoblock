#ifndef GLOBALS_H
#define GLOBALS_H
#include <QString>

#ifndef DELETE_AND_NULLIFY
#define DELETE_AND_NULLIFY(X)   if (X) { delete X; X = NULL; }
#endif

#ifndef APP_SETTINGS
#define APP_SETTINGS
#define ORGANIZATION_NAME "Satoshi Fantasy LLC"
#define ORGANIZATION_DOMAIN "satoshifantasy.com"
#define APPLICATION_NAME "trading football"
#endif


#include <QDebug>
inline QDebug operator<<(QDebug  debug ,std::string msg) {
    static QMutex messageHandlerMutex;
    QMutexLocker locker(&messageHandlerMutex);
    debug << QString(msg.data());
    return debug;
}

#include <QDateTime>
#include <QTimeZone>
static QString fromTime_t_toFantasyString(uint dtt) {
    QByteArray zone = "America/New_York";
    QTimeZone tz(zone);//str.c_str();

    return QDateTime::fromTime_t(dtt,tz).toString("ddd h:mm a").remove(" pm");
}

#endif



