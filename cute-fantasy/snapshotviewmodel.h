#ifndef SNAPSHOTVIEWMODEL_H
#define SNAPSHOTVIEWMODEL_H

#include "viewmodels.h"
#include "globalstateviewmodel.h"
#include "teamstateviewmodel.h"
#include "teamdataviewmodel.h"
#include "playerdataviewmodel.h"
#include "fantasyplayerviewmodel.h"

class SnapShotViewModel {
public:
    QString fantasyName;
    quint64 fantasyNameBalance = 0;
    int     week = 0;
    MyNameStatus myNameStatus = MyNameStatus::none;

    GlobalStateViewModel globalStateModel;
    QMap<QString,TeamDataViewModel> teams;
    QMap<QString,TeamStateViewModel> teamStates;
    QMap<QString,PlayerDataViewModel> players;
    QMap<QString,FantasyPlayerViewModel> fantasyPlayers;

    SnapShotViewModel();

    SnapShotViewModel(const DeltaData & data);

    ~SnapShotViewModel();


    bool updateDB(FantasyPlayerViewModel * viewModel);

    void fromDeltaData(const DeltaData & data);
};

#endif // SNAPSHOTVIEWMODEL_H
