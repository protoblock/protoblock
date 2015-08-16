#include "appsettings.h"

QMutex AppSettings::myMutex;
AppSettings::AppSettings(){
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat,
                       QSettings::SystemScope,
                       QCoreApplication::applicationDirPath());
    //TODO sync
}

AppSettings::~AppSettings(){}

QVariant AppSettings::getSetting(SettingsKeys settingKey) {
    QSettings settings;
    QString settingName = AppSettings::instance()->translateSettingEnum(settingKey);
    QVariant value = settings.value(settingName);
    if (!value.isValid()) {
        QMutexLocker locker(&myMutex);
        if (!value.isValid())
            return DefaultAppSettings::getDefaultSetting(settingKey);
    }
    return settings.value(settingName);
}

