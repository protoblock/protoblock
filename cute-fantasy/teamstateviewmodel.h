#ifndef TEAMSTATEVIEWMODEL_H
#define TEAMSTATEVIEWMODEL_H

#include "viewmodels.h"

class TeamStateViewModel : public Descriptable, public Decorable {

    TeamState_State myState;
    quint32 myWeek;
    QString myTeamId;

public:

    TeamStateViewModel();

    TeamStateViewModel(const TeamStateViewModel & copy);

    TeamStateViewModel(const ::fantasybit::TeamState & teamstate);

    TeamStateViewModel(TeamState_State state,quint32 week,const QString & teamId);

    void copyFrom(const ::fantasybit::TeamState & teamstate);

    bool operator ==(const TeamStateViewModel &other) const;

    QString teamStateString() const;

    QString weekString() const;

    TeamState_State teamState();

    quint32 week() const;

    QString teamId()const;

    void setState(TeamState_State state);
};

Q_DECLARE_METATYPE(TeamStateViewModel*)

#endif // TEAMSTATEVIEWMODEL_H
