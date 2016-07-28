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
    bool createDefaultIOSDirs();
    void createIosDir(const QString &dir);
    AppSettings * settings();
    std::string getRootDir();
    QSysInfo systemInfo;
    void writeToLog(QString &msg);
};
#endif // PLATFORM_H
