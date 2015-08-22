#include "core.h"
#include <iostream>
#include "LAPIWorker.h"
#include "threadedqobject.h"

QWaitCondition  Core::waitForGUI;
Core::Core(){}
void Core::bootstrap(){     
    qSetMessagePattern(Platform::settings()->getSetting(AppSettings::LogMessagePattern).toString());
    qInstallMessageHandler(messageHandler);
    ThreadedQObject<MainLAPIWorker> coreApi;
    coreApi.thread()->connect(coreApi.thread(),
                              SIGNAL(started()),
                              coreApi.object(),
                              SLOT(startPoint()));

    coreApi.thread()->start();
    registerNamedInstance("coreapi",coreApi.object());
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

