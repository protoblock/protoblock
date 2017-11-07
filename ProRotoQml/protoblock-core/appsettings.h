#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QSharedDataPointer>
#include <QMap>
#include <QVariant>
#include <QSettings>
#include <QCoreApplication>
#include <QMutex>
#include <QCoreApplication>
#include "genericsingleton.h"
#include <QStandardPaths>
#include <QDir>
#include <stdlib.h>

class AppSettings : public GenericSingleton<AppSettings> {

    friend class GenericSingleton<AppSettings>;

public:
    enum SettingsKeys {
        LastFantasyName,
        ServerIp,
        ServerPort,
        LogFilePath,
        ApplicationStorageDir,
        PortHand,
        PortStncServ,
        PortLiveBlock,
        PortLiveTx,
        PortLiveTxNat,
        LogMessagePattern,
        LeaderBoardRefreshInterval,
        ConfirmSendAndCopyProjections,
        GenesisTranactionLocation,
        GenesisTransition2014Location,
        GenesisBootLocation2016
    };

private:

    static QMutex myMutex;

    AppSettings();

    QString translateSettingEnum(SettingsKeys settings) const {
        switch (settings) {
        case LastFantasyName:  return "lastfantasyname";
        case ServerIp:  return "serverip";
        case ServerPort:  return "serverport";
        case LogFilePath:  return "logfilepath";
        case ApplicationStorageDir: return "applicationstoragedir";
        case PortHand  :return  "porthand";
        case PortStncServ: return "portstncserv";
        case PortLiveBlock: return "portliveblock";
        case PortLiveTx: return "portlivetx";
        case PortLiveTxNat: return "portlivetxnat";
        case LogMessagePattern: return "logmessagepattern";
        case LeaderBoardRefreshInterval : return "leaderboardrefreshinterval";
        case ConfirmSendAndCopyProjections : return "confirmsendandcopyprojections";
        case GenesisTranactionLocation : return "genesistranactionlocation";
        case GenesisBootLocation2016 : return "GenesisBootLocation2016";
        case GenesisTransition2014Location : return "GenesisTransition2014Location";
        default:
            return "";
        }
    }

    /**
     * @brief The DefaultAppSettings class : a container class to hold hard coded default settings.
     */
    class DefaultAppSettings {
    public:
        DefaultAppSettings(){}
        ~DefaultAppSettings() {}
        static QVariant getDefaultSetting(SettingsKeys settingKey){
            QString storageDirName =QString("storage");
            QString logFileName = QString("protoblock-2016.log");

            switch (settingKey) {
            case LastFantasyName:  return "";
            case ServerIp:  return "127.0.0.1";
            case ServerPort:  return 80;
            case LogFilePath:  return logPath(logFileName);
            case ApplicationStorageDir: return storagePath(storageDirName);
            case PortHand  : return  8130;
            case PortStncServ: return 8125;
            case PortLiveBlock: return 8126;
            case PortLiveTx: return 8127;
            case PortLiveTxNat: return 8128;
            case LogMessagePattern : return
                        "[%{time yyyyMMdd h:mm:ss.zzz} "
                        "%{if-debug}Debug%{endif}"
                        "%{if-info}Information%{endif}"
                        "%{if-warning}Warning%{endif}%"
                        "{if-critical}Critical%{endif}"
                        "%{if-fatal}Fatal%{endif}] "
                        "<%{file}:%{line}::%{function}>"
                        "- %{message}";
            case LeaderBoardRefreshInterval: return 5;
            case ConfirmSendAndCopyProjections: return true;
            case GenesisTranactionLocation :
            #ifdef Q_OS_WIN
                return storagePath(storageDirName)+"/"+"GenesisTransition-Tr-Transaction.txt";
            #endif
            #ifdef Q_OS_MAC
            {
                QDir dir(QCoreApplication::applicationDirPath());
                dir.cdUp();
                dir.cd("Resources");
                return dir.absolutePath()+QString("/GenesisTransition-Tr-Transaction.txt");
            }
            #endif
            case GenesisTransition2014Location :
            #ifdef Q_OS_WIN
                return storagePath(storageDirName)+"/"+"Transition2014.out";
            #endif
            #ifdef Q_OS_MAC
            {
                QDir dir(QCoreApplication::applicationDirPath());
                dir.cdUp();
                dir.cd("Resources");
                return dir.absolutePath()+QString("/Transition2014.out");
            }
            #endif

            case GenesisBootLocation2016 :
            #ifdef Q_OS_WIN
                return storagePath(storageDirName);//+"bootstraptest201601.out";
            #endif
            #ifdef Q_OS_MAC
            {
                QDir dir(QCoreApplication::applicationDirPath());
                dir.cdUp();
                dir.cd("Resources");
                return dir.absolutePath()+QString("/");//bootstraptest201601.out");
            }
            #endif


            }
        }
    };


public:

    ~AppSettings();

    QVariant getSetting(SettingsKeys settingKey);
    QVariant getSetting(QString settingName);
    void setSetting(SettingsKeys settingKey,QVariant & value);
private:
    static QString storagePath(const QString & dirName){
        #ifdef Q_OS_WIN
#ifdef CUSTOM_TESTING_PATH
        return "D:/data/stageprotoblock/storage/";
//        return "C:\\work\\build-ProRoto2016-Release-57\\Applications\\ProtoBlock2016\\release/storage/";
#else
            return QCoreApplication::applicationDirPath()+"/storage/";
#endif
        #endif
        #ifdef Q_OS_MAC
            return makePath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/tradingfootball/"+dirName);
        #endif
    }

    static QString logPath(const QString & fileName){
        #ifdef Q_OS_WIN
            return "./cutefantasy-2016.log";
        #endif
        #ifdef Q_OS_MAC
           return makePath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation))+"/tradingfootball/"+fileName;
        #endif
    }
    static QString makePath(const QString & path){
       QDir dir(path);
       QString absPath = dir.absolutePath();
       if (!dir.exists())
           dir.mkpath(absPath);
       Q_ASSERT(QDir(absPath).exists());
       return absPath;
    }
};

#endif // APPSETTINGS_H
