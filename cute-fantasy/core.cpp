#include "core.h"
#include <iostream>

QWaitCondition  Core::waitForGUI;
QMutex Core::messageHandlerMutex;
Core::Core(){}
void Core::bootstrap(){     
    qSetMessagePattern(Platform::settings()->getSetting(AppSettings::LogMessagePattern).toString());
    qInstallMessageHandler(messageHandler);
}

Core::~Core(){}
std::string Core::getRootDir() {
    std::string path=  Platform::settings()->getSetting(AppSettings::ApplicationStorageDir).toString().toStdString();
    return path;
}

void messageHandler(QtMsgType type,
                    const QMessageLogContext &context,
                    const QString &message)
{

    QMutexLocker lock(&Core::messageHandlerMutex);
    QString logFileName = Platform::settings()->getSetting(AppSettings::LogFilePath).toString();
    static std::ofstream  logFile(logFileName.toStdString());
    if (logFile)
        logFile << qPrintable(qFormatLogMessage(type,context, message))<< std::endl;
#ifdef QT_DEBUG    
    std::cerr  << qPrintable(qFormatLogMessage(type,context, message))<< std::endl;

#endif

}

