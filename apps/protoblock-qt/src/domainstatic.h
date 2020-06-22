#ifndef DOMAINSTATIC_H
#define DOMAINSTATIC_H

#include <QCoreApplication>
#include "core.h"
#include "PeerNode.h"
#include "FantasyAgent.h"

using namespace fantasybit;

class DoMainStatic
{
public:
//    DoMainStatic() : app(iapp) {} ;

    template<typename QT>
    static int domain(int argc, char *argv[], QT*app) {
            Core::instance()->bootstrap();

//            Core::instance()->guiIsAwake();

            QThread::currentThread()->msleep(10);
            Core::resolveByName<MainLAPIWorker>("coreapi")->NFLState().seasonFreeze(2015);
            Core::resolveByName<MainLAPIWorker>("coreapi")->NameData().seasonFreeze(2015);

            return app->exec();
    }
};

#endif // DOMAINSTATIC_H
