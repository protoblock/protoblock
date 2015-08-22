#include "core.h"
#include <iostream>


QWaitCondition  Core::waitForGUI;
Core::Core(){}
void Core::bootstrap(){     
    qSetMessagePattern(Platform::settings()->getSetting(AppSettings::LogMessagePattern).toString());
    qInstallMessageHandler(messageHandler);    
    registerNamedInstance("coreapi",myCoreApi.object());
    QObject::connect(myCoreApi.thread(),SIGNAL(started()),myCoreApi.object(),SLOT(startPoint()));
    myCoreApi.thread()->start();
}

Core::~Core(){}


void messageHandler(QtMsgType type,
                    const QMessageLogContext &context,
                    const QString &message)
{
    static QMutex messageHandlerMutex;
    QMutexLocker lock(&messageHandlerMutex);
    QString logFileName = Platform::settings()->getSetting(AppSettings::LogFilePath).toString();
    static std::ofstream  logFile(logFileName.toStdString());
    if (logFile)
        logFile << qPrintable(qFormatLogMessage(type,context, message))<< std::endl;
#ifdef QT_DEBUG    
    std::cerr  << qPrintable(qFormatLogMessage(type,context, message))<< std::endl;

#endif

}

