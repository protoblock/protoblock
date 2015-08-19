#include "teamdataviewmodel.h"

TeamDataViewModel::TeamDataViewModel(){}

TeamDataViewModel::~TeamDataViewModel(){}

TeamDataViewModel::TeamDataViewModel(const TeamDataViewModel& copy){
    myTeamId = copy.myTeamId;
}

TeamDataViewModel::TeamDataViewModel(const  ::fantasybit::Data & copy) {
    //if (copy.type() != Data_Type_TEAM) return;
    //TeamData teamData = copy.GetExtension(TeamData::team_data);
    //myTeamId = QString::fromStdString(teamData.teamid());
}

bool TeamDataViewModel::operator ==(const TeamDataViewModel &other) const{
    return (other.myTeamId.toLower() == myTeamId.toLower());
}

QString  TeamDataViewModel::teamId() const { return myTeamId; }

bool TeamDataViewModel::addPlayer(const QString & teamPlayerId){
    if (teamPlayerId.trimmed()=="") return false;
    if (myPlayers.keys().contains(teamPlayerId)) return false;
    myPlayers.insert(teamPlayerId,PlayerDataViewModel(teamPlayerId,myTeamId));
}

bool TeamDataViewModel::removePlayer(const QString & teamPlayerId){
    if (teamPlayerId.trimmed()=="") return false;
    if (!myPlayers.keys().contains(teamPlayerId)) return false;
    int removed = myPlayers.remove(teamPlayerId);
    return removed > 0;
}

QList<PlayerDataViewModel> TeamDataViewModel::players() const {
    return myPlayers.values();
}

quint32 TeamDataViewModel::playersCount() {return myPlayers.size();}

bool TeamDataViewModel::hasPlayer(const QString & playerId){
    return myPlayers.contains(playerId);
}

void TeamDataViewModel::setWeekState(quint32 week,TeamState_State state){
    if (myTeamStates.contains(week))
        myTeamStates[week].setState(state);
    else
        myTeamStates.insert(week,TeamStateViewModel(state,week,myTeamId));
}

quint32 TeamDataViewModel::weeksCount() { return myTeamStates.size(); }

QList<TeamStateViewModel>  TeamDataViewModel::weekStates() const { return myTeamStates.values();}

