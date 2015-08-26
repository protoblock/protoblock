#ifndef PLAYERDATAVIEWMODEL_H
#define PLAYERDATAVIEWMODEL_H

#include "viewmodels.h"
#include <QUrl>


class PlayerDataViewModel : public Descriptable, public Decorable {

public:

    enum PlayerPosition {
        UnknownPosition,
        QB,
        RB,
        WR,
        DEF,
        TE,
        K
    };

    PlayerDataViewModel();

    ~PlayerDataViewModel();

    PlayerDataViewModel(const PlayerDataViewModel & copy); 

    PlayerDataViewModel(const QString & playerId,const QString teamId);

    bool operator ==(const PlayerDataViewModel &other) const;

    QString  playerId() const;

    QString  teamId() const;

    void setPlayerInfo(PlayerPosition position, PlayerStatus_Status playerStatus,
                       const QString &name, uint fheight, uint iheight,
                       uint age, const QUrl &playerPictureLink);

    PlayerPosition playerPosition();

    QString playerPositionString() const;

    QString playerName() const;

    QString playerHeight() const;

    // for sorting purpose
    uint playerHeightInInch() const;

    QString playerWeight() const;

    PlayerStatus_Status playerStatus();

    QString  PlayerDataViewModel::playerStatusString() const;

    uint playerAge();

    QPixmap playerPicture() const;
private:
    QString myPlayerId;
    QString myTeamId;
    QString myPlayerName;
    uint myPlayerFeetHeight;
    uint myPlayerInchHeight;
    uint myPlayerWeight;
    uint myPlayerAge;
    PlayerPosition myPlayerPosition;
    QPixmap myPlayerPicture;
    QUrl myPlayerPictureLink;
    PlayerStatus_Status myPlayerStatus;
};

Q_DECLARE_METATYPE(PlayerDataViewModel*)


#endif // PLAYERDATAVIEWMODEL_H
