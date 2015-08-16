#ifndef GLOBALSTATEVIEWMODEL_H
#define GLOBALSTATEVIEWMODEL_H

#include "viewmodels.h"

class GlobalStateViewModel {

    GlobalState_State myState;
    quint32 mySeason;

public:

    GlobalStateViewModel();

    GlobalStateViewModel(const GlobalStateViewModel & copy);

    GlobalStateViewModel(const ::fantasybit::GlobalState & copy);

    void copyFrom(const ::fantasybit::GlobalState & copy);

    bool operator ==(const GlobalStateViewModel &other) const ;

    GlobalState_State state();

    quint32 season();

    QString seasonString() const;

    QString stateString() const ;

    void setState(GlobalState_State state);

    void setSeason(quint32 season);
};



#endif // GLOBALSTATEVIEWMODEL_H
