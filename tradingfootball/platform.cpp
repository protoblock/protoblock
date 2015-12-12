#include "platform.h"
#include "appsettings.h"
#include <QDir>
Platform::Platform(){}

Platform::~Platform(){}

AppSettings * Platform::settings() {
    return AppSettings::instance();
}

std::string Platform::getRootDir() {    
    QString qstrPath = AppSettings::instance()->
            getSetting(AppSettings::ApplicationStorageDir).toString();
            //+QString(QDir::separator());
    qDebug() <<  qstrPath.toStdString();
    return qstrPath.toStdString();
}

