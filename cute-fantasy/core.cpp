#include "core.h"
#include <iostream>


Core::Core(){}
void Core::bootstrap(){

    qRegisterMetaType<fantasybit::GlobalState>("fantasybit::GlobalState");
    qRegisterMetaType<fantasybit::MyFantasyName>("fantasybit::MyFantasyName");
    //qRegisterMetaType<fantasybit::FantasyBitProj>("fantasybit::FantasyBitProj");
    qRegisterMetaType<vector<fantasybit::FantasyBitProj>>("vector<fantasybit::FantasyBitProj>");
    qRegisterMetaType<vector<fantasybit::MyFantasyName>>("vector<fantasybit::MyFantasyName>");
    qRegisterMetaType<pair<string,fantasybit::PlayerStatus>>("pair<string,fantasybit::PlayerStatus>");
    qRegisterMetaType<fantasybit::PlayerStatus>("fantasybit::PlayerStatus");
    qRegisterMetaType<pair<string,fantasybit::FantasyNameBal>>("pair<string,fantasybit::FantasyNameBal>");
    qRegisterMetaType<fantasybit::GameStatus_Status>("fantasybit::GameStatus_Status");

    qRegisterMetaType<string>("string");
    qSetMessagePattern(Platform::settings()->getSetting(AppSettings::LogMessagePattern).toString());
    qInstallMessageHandler(messageHandler);
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

    static QMutex messageHandlerMutex ;
    QMutexLocker lock(&messageHandlerMutex);
    QString logFileName = Platform::settings()->getSetting(AppSettings::LogFilePath).toString();
    static std::ofstream  logFile(logFileName.toStdString(),std::ofstream::app);
    if (logFile)
        logFile << qPrintable(qFormatLogMessage(type,context, message))<< std::endl;

#ifdef QT_DEBUG    
 //   std::cerr  << qPrintable(qFormatLogMessage(type,context, message))<< std::endl;

#endif

    /**/
}

