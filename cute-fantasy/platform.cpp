#include "platform.h"
#include "appsettings.h"

Platform::Platform(){}

Platform::~Platform(){}

AppSettings * Platform::settings() {
    return AppSettings::instance();
}

std::string Platform::getRootDir() {
    std::string path=  Platform::settings()->getSetting(AppSettings::ApplicationStorageDir).toString().toStdString();
    return path;
}

