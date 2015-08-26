#include "gameprojectionmodelview.h"

GameProjectionModelView::GameProjectionModelView(){}

GameProjectionModelView::~GameProjectionModelView(){}

GameProjectionModelView::GameProjectionModelView(const QString & teamId,
                                                 const QString & playerFirstName,
                                                 const QString & playerLastName,
                                                 const QString & pos,
                                                 const QString & playerId,
                                                 PlayerStatus::Status playerTeamStatus,
                                                 PlayerGameStatus playerGameStatus,
                                                 int score){
    myScore = score;
    myTeamId= teamId;
    myPlayerId= playerId;
    myPlayerFirstName = playerFirstName;
    myPlayerLastName = playerLastName;
    myPos = pos;
    myPlayerGameStatus = playerGameStatus;
    myPlayerTeamStatus = playerTeamStatus;
}

GameProjectionModelView::GameProjectionModelView(const GameProjectionModelView & copy){
    myScore = copy.myScore;
    myTeamId= copy.myTeamId;
    myPlayerId= copy.myPlayerId;    
    myPlayerFirstName = copy.myPlayerFirstName;
    myPlayerLastName = copy.myPlayerLastName;
    myPos = copy.myPos;
    myPlayerGameStatus = copy.myPlayerGameStatus;
    myPlayerTeamStatus = copy.myPlayerTeamStatus;

}

void GameProjectionModelView::loadPlayerInfo(){
    if (myPlayerInfoLoaded) return;
//    QPair<QString,QString> playerInfo = PlayerLoader::getPlayerInfo(myPlayerId);
//    myPlayerName = playerInfo.first;
//    myPos = playerInfo.second;
}
