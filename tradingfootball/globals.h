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
#define APPLICATION_NAME "Trading.Football"
#endif


#ifndef BUILD_TYPE
  #ifndef BUILD_STABLE
    #define BUILD_TYPE "beta"
  #else
    #ifdef PRODFOOTBALL
    #define BUILD_TYPE "prod"
    #else
    #define BUILD_TYPE "stage"
    #endif
  #endif
#endif


#ifndef MAJOR_VERSION
  #define MAJOR_VERSION  1
#endif

#ifndef MINOR_VERSION
  #define MINOR_VERSION  2
#endif

#ifndef REVISION_NUMBER
  #define REVISION_NUMBER 3
#endif

#ifndef BUILD_NUMBER
  #define BUILD_NUMBER   0
#endif

#ifndef COPYRIGHT_DATE
    #define COPYRIGHT_DATE "2015"
#endif

#ifndef VERSION_STRING
    #define VERSION_STRING QString("%1.%2.%3").arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(REVISION_NUMBER)
#endif


#include <QDebug>
inline QDebug operator<<(QDebug  debug ,std::string msg) {
#ifndef ALLOW_DEBUG
    return debug;
#else
    static QMutex messageHandlerMutex;
    QMutexLocker locker(&messageHandlerMutex);
    debug << QString(msg.data());
    return debug;
#endif
}

#include <QDateTime>
#include <QTimeZone>
static QString fromTime_t_toFantasyString(uint dtt) {
    QByteArray zone = "America/New_York";
    QTimeZone tz(zone);//str.c_str();

    return QDateTime::fromTime_t(dtt,tz).toString("ddd h:mm a").remove(" pm");
}

static QDateTime currentNewYorkTime() {
    QByteArray zone = "America/New_York";
    QTimeZone tz(zone);//str.c_str();

    return QDateTime::currentDateTime().toTimeZone(tz);
}

#endif
