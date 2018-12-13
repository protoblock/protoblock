#ifndef CORE_H
#define CORE_H
//#if __has_feature(cxx_rvalue_references)
// This code will only be compiled with the -std=c++11 and -std=gnu++11
// options, because rvalue references are only standardized in C++11.
//#endif
#include <QWaitCondition>
#include <fstream>
#include  "genericsingleton.h"
#include "platform.h"
#include "appsettings.h"
#include <QDebug>
#include "threadedqobject.h"
#include "LAPIWorker.h"
#include "ProtoData.pb.h"
#include "ApiData.pb.h"
#include "ExchangeData.h"
#include "Processor.h"
#include "ExData.pb.h"

#include <vector>

using std::vector;

class IResolvable;
class Core : public GenericSingleton<Core>
{
    friend class GenericSingleton<Core>;

    Core();

    bool oldstuff;
public:

    ~Core();   

    void bootstrap();


    void waitForGui();    

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
public slots:
    void guiIsAwake();
private:

    QMap<QString,IResolvable *> myNamedResolvableInstance;
    QWaitCondition myWaitForGUI;
    QMutex myMutex;
    QMutex myWaitForGuiMutex;
    ThreadedQObject<MainLAPIWorker> myCoreApi;

};

//void messageHandler(QtMsgType type,
//                    const QMessageLogContext &context,
//                    const QString &message);

Q_DECLARE_METATYPE(fantasybit::GlobalState)
Q_DECLARE_METATYPE(fantasybit::MyFantasyName)
Q_DECLARE_METATYPE(fantasybit::FantasyBitProj)
Q_DECLARE_METATYPE(vector<fantasybit::MyFantasyName>)
Q_DECLARE_METATYPE(fantasybit::PlayerStatus)
Q_DECLARE_METATYPE(vector<fantasybit::FantasyBitProj>)
Q_DECLARE_METATYPE(fantasybit::Distribution)
Q_DECLARE_METATYPE(fantasybit::FantasyNameHash)
Q_DECLARE_METATYPE(fantasybit::FantasyNameBal)
Q_DECLARE_METATYPE(fantasybit::ExchangeOrder)
Q_DECLARE_METATYPE(fantasybit::MarketTicker)
Q_DECLARE_METATYPE(fantasybit::MarketSnapshot*)
Q_DECLARE_METATYPE(fantasybit::DepthFeedDelta*)
Q_DECLARE_METATYPE(fantasybit::TradeTic*)
Q_DECLARE_METATYPE(fantasybit::FullPosition)
Q_DECLARE_METATYPE(fantasybit::FullOrderDelta)
Q_DECLARE_METATYPE(fantasybit::TransferTrans)
Q_DECLARE_METATYPE(fantasybit::SwapAsk)
Q_DECLARE_METATYPE(fantasybit::SwapBid)
Q_DECLARE_METATYPE(fantasybit::SwapOrder)
Q_DECLARE_METATYPE(fantasybit::SwapFill)
Q_DECLARE_METATYPE(fantasybit::SwapSent)





#endif // CORE_H
