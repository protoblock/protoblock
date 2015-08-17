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

#define qInfo QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).noDebug


//#include "boostLog.h"

//#ifndef BOOST_LOG
//#define BOOST_LOG
//#define LOG(logger, severity) LOGIT(logger, severity,  FILE, LINE, FUNCTION)
//#endif
#endif



