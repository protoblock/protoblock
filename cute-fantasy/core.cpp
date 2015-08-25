#include "core.h"
#include <iostream>


Core::Core(){}
void Core::bootstrap(){     
    qSetMessagePattern(Platform::settings()->getSetting(AppSettings::LogMessagePattern).toString());
    qInstallMessageHandler(messageHandler);    
    qRegisterMetaType<fantasybit::GlobalState>("GlobalState");
    qRegisterMetaType<fantasybit::MyFantasyName>("MyFantasyName");
    qRegisterMetaType<fantasybit::FantasyBitProj>("FantasyBitProj");
    qRegisterMetaType<std::vector<fantasybit::MyFantasyName>>("Vector_MyFantasyName");
    registerNamedInstance("coreapi",myCoreApi.object());
    QObject::connect(myCoreApi.thread(),SIGNAL(started()),myCoreApi.object(),SLOT(startPoint()));
    myCoreApi.thread()->start();
}

void Core::waitForGui(){
    myWaitForGuiMutex.lock();
    myWaitForGUI.wait(&myWaitForGuiMutex);
}

void Core::guiIsAwake(){
    myWaitForGUI.wakeAll();
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

