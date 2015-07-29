#include "models.h"
#include "playerloader.h"


void GameProjectionModelView::loadPlayerInfo(){
        if (myPlayerInfoLoaded) return;
        QPair<QString,QString> playerInfo = PlayerLoader::getPlayerInfo(myPlayerId);
        myPlayerName = playerInfo.first;
        myPos = playerInfo.second;
}

