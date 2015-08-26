#ifndef GLOBALS_H
#define GLOBALS_H

#ifndef DELETE_AND_NULLIFY
#define DELETE_AND_NULLIFY(X)   if (X) { delete X; X = NULL; }
#endif

#ifndef APP_SETTINGS
#define APP_SETTINGS
#define ORGANIZATION_NAME "Satoshi Fantasy LLC"
#define ORGANIZATION_DOMAIN "satoshifantasy.com"
#define APPLICATION_NAME "Cute Fantasy"
#endif

#define QT_LOGGING_TO_CONSOLE



#include <QDebug>
inline QDebug operator<<(QDebug  debug ,std::string msg) {
    static QMutex messageHandlerMutex;
    QMutexLocker locker(&messageHandlerMutex);
    debug << QString(msg.data());
    return debug;
}

#endif



