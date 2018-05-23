#ifndef THREADEDQOBJECT_H
#define THREADEDQOBJECT_H

#include <QThread>

/**
 * @brief The ThreadedObject class
 */
template <class T> class ThreadedQObject {
public:
    ThreadedQObject(){
        myObjectInstance.moveToThread(&myWorkerThread);
    }

    ~ThreadedQObject(){
        if (myWorkerThread.isRunning())
            myWorkerThread.quit();
        myWorkerThread.wait();//AKA join
    }

    T * object() { return & myObjectInstance; }
    QThread * thread()  { return & myWorkerThread; }

protected :
    QThread myWorkerThread;
    T myObjectInstance;
};


//Object::deleteLater   - QThread::finished





#endif // THREADEDQOBJECT_H

