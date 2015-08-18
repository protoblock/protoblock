#include "snapshotviewmodel.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

SnapShotViewModel::SnapShotViewModel(){}

SnapShotViewModel::SnapShotViewModel(const DeltaData & data){ fromDeltaData(data);}

SnapShotViewModel::~SnapShotViewModel(){}


bool SnapShotViewModel::updateDB(FantasyPlayerViewModel * viewModel){
    if (viewModel==NULL) return false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName("162.254.27.178");
    db.setPort(3306);
    db.setDatabaseName("satoshifantasy");
    db.setUserName("datafeed");
    db.setPassword("s@tof@nt@sy6#1");
    bool success = true;
    if (!db.open()) {
         qWarning() << "Database error occurred :" << db.lastError().databaseText();
         //LOG(lg,error) << "db errror :"<< db.lastError().databaseText().toStdString();
        return false;
    }


    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO fantasyteam_copy (fantasyteam, fantasybits, stake) VALUES(:player_name,:bits,:bits)");
    insertQuery.bindValue(":player_name",viewModel->playerName());
    insertQuery.bindValue(":bits",viewModel->bits());

    if ( !insertQuery.exec() )
    {
        QSqlQuery updateQuery(db);
        updateQuery.prepare("UPDATE fantasyteam_copy set fantasybits= :bits, stake= :bits where fantasyteam= :player_name");
        updateQuery.bindValue(":player_name",viewModel->playerName());
        updateQuery.bindValue(":bits",viewModel->bits());
        if (!updateQuery.exec()) {
            //LOG(lg,info) << " exec ret " << updateQuery.lastError().databaseText();
            success= false;
        }
    }
    db.close();
    return success;
}

void SnapShotViewModel::fromDeltaData(const DeltaData & data) {
//       if (data.type() == DeltaData_Type_SNAPSHOT) {
         //clear snapshot data
//            fantasyName = "";
//            fantasyNameBalance = 0;
//            globalStateModel.setSeason(0);
//            //globalStateModel.setState(STATE_UNKNOWN);
//            teams.clear();
//            teamStates.clear();
//            players.clear();
//            fantasyPlayers.clear();
//        }

    if (data.myfantasyname_size() > 0 ) {
        fantasyName = QString::fromStdString(data.myfantasyname(0).name());
        myNameStatus = data.myfantasyname(0).status();
    }

    if (data.has_globalstate())
        globalStateModel.copyFrom(data.globalstate());

    for (auto t : data.datas()) {
        if (true){//TODO t.type() == ::fantasybit::Data_Type_TEAM) {
            TeamDataViewModel viewModel(t);
            teams.insert(viewModel.teamId(),viewModel);
        }
        else
        if (t.type() == ::fantasybit::Data_Type_PLAYER){
            PlayerDataViewModel viewModel(t);
            players.insert(viewModel.playerId(),viewModel);
        }
    }

    for (const auto &t : data.teamstates()){
        TeamStateViewModel viewModel(t);
        teamStates.insert(viewModel.teamId(),viewModel);
        if ( t.week() > week) week = t.week();
    }

    for (const auto &t : data.players())
    {
//            fantasyPlayers[QString::fromStdString(t.name())] =
//                  FantasyPlayerViewModel(t);
        auto it = fantasyPlayers.find(QString::fromStdString(t.name()));
        quint64 curr = 0;
        if ( it != fantasyPlayers.end() )
            curr = it->bits();

        FantasyPlayerViewModel viewModel(t);
        viewModel.setBits(viewModel.bits() + curr);

#ifdef DATAAGENTGUI
#ifdef DATAAGENT_UPDATEDB
        updateDB(&viewModel);
#endif
#endif
        fantasyPlayers.insert(viewModel.playerName(),viewModel);
        if ( viewModel.playerName() == fantasyName ) {
            myNameStatus = MyNameStatus::confirmed;
            fantasyNameBalance = viewModel.bits();
        }


    }
}

