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
        LogMessagePattern
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
        case ApplicationStorageDir: "applicationstoragedir";
        case PortHand  :return  "porthand";
        case PortStncServ: return "portstncserv";
        case PortLiveBlock: return "portliveblock";
        case PortLiveTx: return "portlivetx";
        case PortLiveTxNat: return "portlivetxnat";
        case LogMessagePattern: return "logmessagepattern";
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
            switch (settingKey) {           
            case LastFantasyName:  return "";
            case ServerIp:  return "127.0.0.1";
            case ServerPort:  return 80;
            case LogFilePath:  return "./cutefantasy.log";
            case ApplicationStorageDir: return QCoreApplication::applicationDirPath()+"/storage/";
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
            default:               
                return QVariant();
            }
        }
    };


public:

    ~AppSettings();

    static QVariant getSetting(SettingsKeys settingKey);
};

#endif // APPSETTINGS_H
