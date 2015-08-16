#ifndef PLAYERDATAVIEWMODEL_H
#define PLAYERDATAVIEWMODEL_H

#include "viewmodels.h"
class PlayerDataViewModel : public Descriptable, public Decorable {

    QString myPlayerId;
    QString myTeamId;

public:

    PlayerDataViewModel();
    ~PlayerDataViewModel();

    PlayerDataViewModel(const PlayerDataViewModel & copy);

    PlayerDataViewModel(const  ::fantasybit::Data & copy);

    PlayerDataViewModel(const QString & playerId,const QString teamId);

    bool operator ==(const PlayerDataViewModel &other) const;

    QString  playerId() const;
    QString  teamId() const;

};

Q_DECLARE_METATYPE(PlayerDataViewModel*)


#endif // PLAYERDATAVIEWMODEL_H
