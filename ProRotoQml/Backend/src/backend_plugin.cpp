#include "backend_plugin.h"
#include <qqml.h>
#include <QDebug>
//api
#include "blockmeta.h"
#include "ldbhashreadertool.h"
#include "tempapi.h"
#include "getuserinfo.h"

//maps
#include "maps/ldbhashreadermap.h"
#include "maps/teammap.h"
#include "maps/playermap.h"

// models
#include "models/scheduledgamesmodel.h"
#include "models/awardsmodels.h"
#include "models/projectionsmodel.h"
#include "models/playermodel.h"
#include "models/leaderboardmodel.h"
#include "models/statsmodel.h"
#include "models/pnlmodel.h"
//TEST MODEL
#include "models/pnltestmodel.h"
//#include "models/playernewmodel.h"
#include "../QmlModels/QQmlObjectListModel.h"

using namespace fantasybit;
void ProRotoQmlBackendPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<QBlockMeta>(uri,1,0,"BlockMeta");

    // @uri Backend

    //api
    qmlRegisterType<GetUserInfo>(uri,1,0,"UserInfo");
//    qmlRegisterType<TempApi>(uri,1,0,"Backend");
    qmlRegisterSingletonType <LdbHashReaderMap>(uri,1,0,"LdbHashReaderMap",pLdbHashReaderMap);
    qmlRegisterSingletonType <TempApi>(uri,1,0,"Backend",pTempApi);
    qmlRegisterSingletonType <LdbHashReaderTool>(uri,1,0,"LdbHashReaderTool", pLdbHashReaderTool );
//    qmlRegisterSingletonType <QQmlObjectListModel<PlayerNewModelItem>>(uri,1,0,"PlayerNewModel", playerNewModel);

    //models
    qmlRegisterType<LeaderBoardModel>(uri,1,0,"LeaderBoardModel");
    qmlRegisterType<PlayerModel>(uri,1,0,"PlayersModel");
    qmlRegisterType<ProjectionsModel>(uri,1,0,"ProjectionsModel");
    qmlRegisterType<ScheduledGamesModel>(uri,1,0,"ScheduledGamesModel");
    qmlRegisterType<AwardsModel>(uri,1,0,"AwardsModel");
    qmlRegisterType<StatsModel>(uri,1,0,"PlayerStatsModel");
    qmlRegisterType<PNLModel>(uri,1,0,"PnlModel");
    //qmlRegisterType <PlayerNewModel*>(uri,1,0,"pPlayerNewModel");
//    qRegisterMetaType<PlayerNewModel*>("PlayerNewModel*");

    //maps
    qmlRegisterSingletonType<PlayerMap>(uri,1,0,"PlayerMap", playerMap);
    qmlRegisterSingletonType <TeamMap>(uri,1,0,"TeamMap", teamMap );

}

void ProRotoQmlBackendPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{

    Q_UNUSED (engine)
    Q_UNUSED (uri)
//    TempApi t;
}




QObject *playerMap(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    PlayerMap *playerMap = new PlayerMap();
    return playerMap;
}

QObject *teamMap(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    TeamMap *teamNameMap = new TeamMap();
    return teamNameMap;
}



QObject *pLdbHashReaderTool(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    LdbHashReaderTool *pLdbHashReaderTool = LdbHashReaderTool::instance();
    return pLdbHashReaderTool;
}



QObject *pLdbHashReaderMap(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    LdbHashReaderMap *pLdbHashReaderMap = new LdbHashReaderMap();
    return pLdbHashReaderMap;
}


QObject *pTempApi(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    TempApi *ptempApi = new TempApi ;
    return ptempApi;
}


QObject *playerNewModel(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

//    QQmlObjectListModel<PlayerNewModelItem> *playerNewModel =
//            new QQmlObjectListModel<PlayerNewModelItem>{};

//    for (auto &iter : TempApi::mPlayers) {
//        playerNewModel->append (
//                    new PlayerNewModelItem (
//                        iter.firstName,
//                        iter.lastName,
//                        iter.position,
//                        iter.team,
//                        iter.playerStatus,
//                        iter.playerId
//                        )
//                    ) ;
//    }

//    return playerNewModel;
    return nullptr;
}
