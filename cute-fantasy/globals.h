#ifndef GLOBALS_H
#define GLOBALS_H

#include <QThread>

#ifndef DELETE_AND_NULLIFY
#define DELETE_AND_NULLIFY(X)   if (X) { delete X; X = NULL; }
#endif

template <class T>
class ThreadedObject {
public:
    ThreadedObject(){
        myObjectInstance.moveToThread(&myWorkerThread);
    }
    ~ThreadedObject(){
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


#endif // GLOBALS_H

