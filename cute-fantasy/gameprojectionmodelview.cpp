#include "gameprojectionmodelview.h"
#include "playerloader.h"

void GameProjectionModelView::setProjectionFlag( ProjectionFlag flag, bool on ) {
    if ( bool( myProjectionFlag & flag ) == on )
        return;

    if ( on )
        myProjectionFlag |= flag;
    else
        myProjectionFlag &= ~flag;
}

bool GameProjectionModelView::testProjectionFlag(ProjectionFlag flag) const{
    return myProjectionFlag & flag;
}

GameProjectionModelView::GameProjectionModelView(){}

GameProjectionModelView::GameProjectionModelView(const QString & teamId,
                                                 const QString & playerName,
                                                 const QString & pos,
                                                 const QString & playerId,
                                                 int score){
    myScore = score;
    myTeamId= teamId;
    myPlayerId= playerId;
    myPlayerName = playerName;
    myPos = pos;
}

GameProjectionModelView::GameProjectionModelView(GameProjectionModelView & copy){
    myScore = copy.myScore;
    myTeamId= copy.myTeamId;
    myPlayerId= copy.myPlayerId;
    myPlayerName = copy.myPlayerName;

}

void GameProjectionModelView::loadPlayerInfo(){
    if (myPlayerInfoLoaded) return;
    QPair<QString,QString> playerInfo = PlayerLoader::getPlayerInfo(myPlayerId);
    myPlayerName = playerInfo.first;
    myPos = playerInfo.second;
}
