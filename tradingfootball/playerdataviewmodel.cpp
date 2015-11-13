#include "playerdataviewmodel.h"

PlayerDataViewModel::PlayerDataViewModel(){
    myPlayerPosition = UnknownPosition;
    myPlayerStatus = PlayerStatus_Status_OTHER;//unknown
}
PlayerDataViewModel::~PlayerDataViewModel(){}

PlayerDataViewModel::PlayerDataViewModel(const PlayerDataViewModel & copy){
    myPlayerId = copy.myPlayerId;
    myTeamId = copy.myTeamId;
    myPlayerPosition = copy.myPlayerPosition;
    myPlayerStatus = copy.myPlayerStatus;
    myPlayerName = copy.myPlayerName;
    myPlayerFeetHeight = copy.myPlayerFeetHeight;
    myPlayerInchHeight = copy.myPlayerInchHeight;
    myPlayerAge = copy.myPlayerAge;
    myPlayerPictureLink = copy.myPlayerPictureLink;
    myPlayerPicture = copy.myPlayerPicture;
}

PlayerDataViewModel::PlayerDataViewModel(const QString & playerId,const QString teamId) {
    myPlayerId = playerId;
    myTeamId = teamId;
}

bool PlayerDataViewModel::operator ==(const PlayerDataViewModel &other) const{
    return (other.myPlayerId.toLower() == myPlayerId.toLower() &&
            other.myTeamId.toLower() == myTeamId.toLower());
}

QString  PlayerDataViewModel::playerId() const {
    return myPlayerId;
}
QString  PlayerDataViewModel::teamId() const { return myTeamId; }

void PlayerDataViewModel::setPlayerInfo(PlayerPosition position, fantasybit::PlayerStatus_Status playerStatus,  const QString &name,
                                        uint fheight, uint iheight, uint age,
                                        const QUrl &playerPictureLink){
    myPlayerPosition = position;
    myPlayerStatus = playerStatus;
    myPlayerName = name;
    myPlayerFeetHeight = fheight;
    myPlayerInchHeight = iheight;
    myPlayerAge = age;
    myPlayerPictureLink = playerPictureLink;
}


PlayerDataViewModel::PlayerPosition PlayerDataViewModel::playerPosition(){
    return myPlayerPosition;
}

QString PlayerDataViewModel::playerName() const{
 return myPlayerName;
}

QString PlayerDataViewModel::playerHeight() const {
    return QString("%1ft-%2in").arg(myPlayerFeetHeight).arg(myPlayerInchHeight);
}

QString PlayerDataViewModel::playerWeight() const {
 return QString("%1P").arg(myPlayerWeight);
}

uint PlayerDataViewModel::playerAge() {
  return myPlayerAge;
}

QString PlayerDataViewModel::playerPositionString() const{
    switch (myPlayerPosition) {
    case QB: return "QB";
    case RB: return "RB";
    case WR: return "WR";
    case DEF: return "DEF";
    case TE: return "TE";
    case K: return "K";
    default :return "?";
    }
}

uint PlayerDataViewModel::playerHeightInInch() const{
    return (myPlayerFeetHeight*12)+  myPlayerInchHeight;
}

PlayerStatus_Status PlayerDataViewModel::playerStatus() {
    return myPlayerStatus;
}

QString  PlayerDataViewModel::playerStatusString() const{
    switch (myPlayerStatus) {
    case PlayerStatus_Status_ACTIVE : return "Active";
    case PlayerStatus_Status_INACTIVE: return "Inactive";
    case PlayerStatus_Status_OTHER :return "Other";
    case PlayerStatus_Status_FA :return "FA";
    default: // default
      return "?";
    }
}

QPixmap PlayerDataViewModel::playerPicture() const {
if (myPlayerPicture.isNull()) {
//TODO get the picture from the web
}
return myPlayerPicture;
}
