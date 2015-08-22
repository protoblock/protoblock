#ifndef CORE_H
#define CORE_H

#include <QWaitCondition>
#include <fstream>
#include  "genericsingleton.h"
#include "platform.h"
#include "appsettings.h"
#include <QDebug>
#include "threadedqobject.h"
#include "LAPIWorker.h"
class IResolvable;
class Core : public GenericSingleton<Core>
{
    friend class GenericSingleton<Core>;
    Core();
public:
    ~Core();
    static QWaitCondition waitForGUI;
    ThreadedQObject<MainLAPIWorker> myCoreApi;
    void bootstrap();
    template <class T>
    static bool registerNamedInstance(const QString & name ,T * theInstance) {
        QMutexLocker locker(&Core::instance()->myMutex);
        QString theName = name.toLower();
        if (Core::instance()->myNamedResolvableInstance.contains(theName))
            return true;
        Core::instance()->myNamedResolvableInstance.insert(theName,theInstance);
        return true;
    }

    static bool unregisterNamedInstance(const QString & name) {
        QMutexLocker locker(&Core::instance()->myMutex);
        QString theName = name.toLower();
        if (!Core::instance()->myNamedResolvableInstance.contains(theName)) return false;
        Core::instance()->myNamedResolvableInstance.remove(theName);
        return true;
    }

    static bool unregisterInstance(IResolvable * instance) {
        QMutexLocker locker(&Core::instance()->myMutex);
        QString name = Core::instance()->myNamedResolvableInstance.key(instance,"");
        if (name=="") return false;
        Core::instance()->myNamedResolvableInstance.remove(name);
        return true;
    }

    template <class T>
    static T * resolveByName(const QString & name) {
        QMutexLocker locker(&instance()->myMutex);
        IResolvable * theInstance = instance()->myNamedResolvableInstance.value(name.toLatin1(),NULL);
        return static_cast<T*>(theInstance);
        }
private:
    QMap<QString,IResolvable *> myNamedResolvableInstance;
    QMutex myMutex;

};

void messageHandler(QtMsgType type,
                    const QMessageLogContext &context,
                    const QString &message);


#endif // CORE_H
