/*
 *  Credits to  : https://github.com/itay-grudev/SingleApplication
* The MIT License (MIT)
*
* Copyright (c) 2015 Itay Grudev
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
#include "singleapplication.h"
#include <QSharedMemory>
#include <QLocalSocket>
#include <QLocalServer>
#include <QMutex>
#include <cstdlib>

#ifdef Q_OS_UNIX
    #include <signal.h>
    #include <unistd.h>
#endif

class SingleApplicationPrivate {
public:
    SingleApplicationPrivate(SingleApplication *q_ptr) : q_ptr(q_ptr) { }

    void startServer(QString &serverName)
    {
        // Start a QLocalServer to listen for connections
        server = new QLocalServer();
        server->removeServer(serverName);
        server->listen(serverName);
        QObject::connect(server, SIGNAL(newConnection()), q_ptr, SLOT(slotConnectionEstablished()));
    }

#ifdef Q_OS_UNIX
    void crashHandler()
    {
        // This guarantees the program will work even with multiple
        // instances of SingleApplication in different threads
        // Which in my opinion is idiotic, but lets handle that too
        {
            sharedMemMutex.lock();
            sharedMem.append(memory);
            sharedMemMutex.unlock();
        }
        // Handle any further termination signals to ensure the
        // QSharedMemory block is deleted even if the process crashes
        signal(SIGSEGV, SingleApplicationPrivate::terminate);
        signal(SIGABRT, SingleApplicationPrivate::terminate);
        signal(SIGFPE, SingleApplicationPrivate::terminate);
        signal(SIGILL, SingleApplicationPrivate::terminate);
        signal(SIGINT, SingleApplicationPrivate::terminate);
        signal(SIGTERM, SingleApplicationPrivate::terminate);
    }

    static void terminate(int signum)
    {
        while( ! sharedMem.empty() ) {
            delete sharedMem.back();
            sharedMem.pop_back();
        }
        ::exit(128 + signum);
    }

    static QList<QSharedMemory*> sharedMem;
    static QMutex sharedMemMutex;
#endif

    QSharedMemory *memory;
    SingleApplication *q_ptr;
    QLocalServer  *server;
    QLocalSocket  *socket;
};

#ifdef Q_OS_UNIX
    QList<QSharedMemory*> SingleApplicationPrivate::sharedMem;
    QMutex SingleApplicationPrivate::sharedMemMutex;
#endif

/**
 * @brief Constructor. Checks and fires up LocalServer or closes the program
 * if another instance already exists
 * @param argc
 * @param argv
 */
SingleApplication::SingleApplication(int &argc, char *argv[])
    : QAPPLICATION_CLASS(argc, argv), d_ptr(new SingleApplicationPrivate(this))
{
    QString serverName = QAPPLICATION_CLASS::organizationName() + QAPPLICATION_CLASS::applicationName();
    serverName.replace(QRegExp("[^\\w\\-. ]"), "");

    // Garantee thread safe behaviour with a shared memory block
    d_ptr->memory = new QSharedMemory(serverName);

    // Create a shared memory block with a minimum size of 1 byte
    if( d_ptr->memory->create(1, QSharedMemory::ReadOnly) )
    {
#ifdef Q_OS_UNIX
        // Handle any further termination signals to ensure the
        // QSharedMemory block is deleted even if the process crashes
        d_ptr->crashHandler();
#endif
        // Successful creation means that no main process exists
        // So we start a Local Server to listen for connections
        d_ptr->startServer(serverName);
    } else {
        // Connect to the Local Server of the main process to notify it
        // that a new process had been started
        d_ptr->socket = new QLocalSocket();
        d_ptr->socket->connectToServer(serverName);

        // Even though a shared memory block exists, the original application might have crashed
        // So only after a successful connection is the second instance terminated
        if( d_ptr->socket->waitForConnected(100) )
        {
            ::exit(EXIT_SUCCESS); // Terminate the program using STDLib's exit function
        } else {
            delete d_ptr->memory;
            ::exit(EXIT_SUCCESS);
        }
    }
}

/**
 * @brief Destructor
 */
SingleApplication::~SingleApplication()
{
    delete d_ptr->memory;
    d_ptr->server->close();
}

/**
 * @brief Executed when the showUp command is sent to LocalServer
 */
void SingleApplication::slotConnectionEstablished()
{
    QLocalSocket *socket = d_ptr->server->nextPendingConnection();
    socket->close();
    delete socket;
    emit showUp();
}
