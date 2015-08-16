#include "teamstateviewmodel.h"



TeamStateViewModel::TeamStateViewModel() {}

TeamStateViewModel::TeamStateViewModel(const TeamStateViewModel & copy){
    myState = copy.myState;
    myWeek = copy.myWeek;
    myTeamId = copy.myTeamId;
}

TeamStateViewModel::TeamStateViewModel(const ::fantasybit::TeamState & teamstate){
    myState = teamstate.state();
    myWeek = teamstate.week();
    myTeamId = QString::fromStdString(teamstate.teamid());
}

TeamStateViewModel::TeamStateViewModel(TeamState_State state,quint32 week,const QString & teamId){
    myState = state;
    myWeek = week;
    myTeamId = teamId;
}

void TeamStateViewModel::copyFrom(const ::fantasybit::TeamState & teamstate){
    myState = teamstate.state();
    myWeek = teamstate.week();
    myTeamId = QString::fromStdString(teamstate.teamid());
}

bool TeamStateViewModel::operator ==(const TeamStateViewModel &other) const {
    return other.myState == myState &&
            other.myWeek == myWeek &&
            other.myTeamId.toLower() == myTeamId.toLower();
}

QString TeamStateViewModel::teamStateString() const {
    switch (myState) {
    case TeamState_State_PREGAME: return "PREGAME";
    case TeamState_State_INGAME : return "INGAME";
    default:
        return "Team State Unknown";
    }
}

QString TeamStateViewModel::weekString() const {
    return QString("%1").arg(myWeek);
}

TeamState_State TeamStateViewModel::teamState() { return myState; }

quint32 TeamStateViewModel::week() const { return myWeek; }

QString TeamStateViewModel::teamId()const { return myTeamId; }

void TeamStateViewModel::setState(TeamState_State state){
    myState = state;
}
