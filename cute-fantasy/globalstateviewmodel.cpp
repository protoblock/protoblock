#include "globalstateviewmodel.h"


GlobalStateViewModel::GlobalStateViewModel(){}

GlobalStateViewModel::GlobalStateViewModel(const GlobalStateViewModel & copy){
    mySeason = copy.mySeason;
    myState = copy.myState;
}

GlobalStateViewModel::GlobalStateViewModel(const ::fantasybit::GlobalState & copy){
    mySeason = copy.season();
    myState = copy.state();
}

void GlobalStateViewModel::copyFrom(const ::fantasybit::GlobalState & copy){
    mySeason = copy.season();
    myState = copy.state();
}

bool GlobalStateViewModel::operator ==(const GlobalStateViewModel &other) const {
    return other.mySeason == mySeason && other.myState == myState;
}

GlobalState_State GlobalStateViewModel::state() { return myState; }

quint32 GlobalStateViewModel::season() { return mySeason; }

QString GlobalStateViewModel::seasonString() const { return QString("%1").arg(mySeason) ;}

QString GlobalStateViewModel::stateString() const {
    switch (myState) {
    case GlobalState_State_PREDRAFT: return QString("PREDRAFT");
    case GlobalState_State_PRESEASON: return QString("PRESEASON");
    case GlobalState_State_ROSTER53MAN: return QString("ROSTER53MAN");
    case GlobalState_State_INSEASON: return QString("INSEASON");
    default:
        return "Season Unknown";
    }
}

void GlobalStateViewModel::setState(GlobalState_State state){ myState = state; }

void GlobalStateViewModel::setSeason(quint32 season){ mySeason = season; }
