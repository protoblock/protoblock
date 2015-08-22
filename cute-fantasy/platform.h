#ifndef PLATFORM_H
#define PLATFORM_H

#include "genericsingleton.h"

class AppSettings;
class Platform : GenericSingleton<Platform>
{
    friend class GenericSingleton<Platform>;
    Platform();

public:
    ~Platform();
    static AppSettings * settings();
    static std::string getRootDir();
};

#endif // PLATFORM_H
