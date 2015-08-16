#ifndef TEAMDATAVIEWMODEL_H
#define TEAMDATAVIEWMODEL_H

#include "viewmodels.h"
#include "playerdataviewmodel.h"
#include "teamstateviewmodel.h"
#include <QMap>

class TeamDataViewModel : public Descriptable, public Decorable{

    QMap<QString,PlayerDataViewModel> myPlayers;
    QMap<quint32,TeamStateViewModel> myTeamStates;
    QString myTeamId;

public:

    TeamDataViewModel();

    ~TeamDataViewModel();

    TeamDataViewModel(const TeamDataViewModel& copy);

    TeamDataViewModel(const  ::fantasybit::Data & copy);

    bool operator ==(const TeamDataViewModel &other) const;

    QString  teamId() const;

    bool addPlayer(const QString & teamPlayerId);

    bool removePlayer(const QString & teamPlayerId);

    QList<PlayerDataViewModel> players() const;

    quint32 playersCount();

    bool hasPlayer(const QString & playerId);

    void setWeekState(quint32 week,TeamState_State state);

    quint32 weeksCount();

    QList<TeamStateViewModel>  weekStates() const;

};

Q_DECLARE_METATYPE(TeamDataViewModel*)


#endif // TEAMDATAVIEWMODEL_H
