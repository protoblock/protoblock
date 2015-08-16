#ifndef CORE_H
#define CORE_H

#include <QWaitCondition>
#include <fstream>
#include  "genericsingleton.h"
#include "platform.h"
#include "appsettings.h"
#include <QDebug>

class Core : public GenericSingleton<Core>
{
    friend class GenericSingleton<Core>;
    Core();
public:
    ~Core();
    static QWaitCondition waitForGUI;
    static QMutex messageHandlerMutex;
    static std::string getRootDir();
    void bootstrap();
};

void messageHandler(QtMsgType type,
                    const QMessageLogContext &context,
                    const QString &message);

inline QDebug operator<<(QDebug  debug ,std::string msg) {
    QMutexLocker locker(&Core::messageHandlerMutex);
    debug << QString(msg.data());
    return debug;
}
#endif // CORE_H
