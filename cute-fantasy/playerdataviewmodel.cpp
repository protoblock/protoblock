#include "playerdataviewmodel.h"

PlayerDataViewModel::PlayerDataViewModel(){}
PlayerDataViewModel::~PlayerDataViewModel(){}

PlayerDataViewModel::PlayerDataViewModel(const PlayerDataViewModel & copy){
    myPlayerId = copy.myPlayerId;
    myTeamId = copy.myTeamId;
}

PlayerDataViewModel::PlayerDataViewModel(const  ::fantasybit::Data & copy) {
    if (copy.type() != Data_Type_PLAYER) return;
    PlayerData playerData = copy.GetExtension(PlayerData::player_data);
    myPlayerId = QString::fromStdString(playerData.playerid());
    myTeamId = QString::fromStdString(playerData.teamid());
}

PlayerDataViewModel::PlayerDataViewModel(const QString & playerId,const QString teamId) {
    myPlayerId = playerId;
    myTeamId = teamId;
}

bool PlayerDataViewModel::operator ==(const PlayerDataViewModel &other) const{
    return (other.myPlayerId.toLower() == myPlayerId.toLower() &&
            other.myTeamId.toLower() == myTeamId.toLower());
}

QString  PlayerDataViewModel::playerId() const { return myPlayerId; }
QString  PlayerDataViewModel::teamId() const { return myTeamId; }
