#include "platform.h"
#include "appsettings.h"

Platform::Platform(){}

Platform::~Platform(){}

AppSettings * Platform::settings() {
    return AppSettings::instance();
}

