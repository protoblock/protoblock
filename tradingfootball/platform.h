#ifndef PLATFORM_H
#define PLATFORM_H

#include "genericsingleton.h"

class AppSettings;
class Platform : public GenericSingleton<Platform>
{
    friend class GenericSingleton<Platform>;
    Platform();

public:
    ~Platform();
    AppSettings * settings();
    std::string getRootDir();

};
#endif // PLATFORM_H
