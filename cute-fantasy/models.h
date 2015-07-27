#ifndef MODELS_H
#define MODELS_H

#include <QStandardItemModel>
#include <ProtoData.pb.h>
#include "tlistmodel.h"
#include "tkeyedlist.h"

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

using namespace fantasybit;

class GlobalStateViewModel {

    GlobalState_State myState;
    quint32 mySeason;

public:

    GlobalStateViewModel(){}

    GlobalStateViewModel(const GlobalStateViewModel & copy){
        mySeason = copy.mySeason;
        myState = copy.myState;
    }

    GlobalStateViewModel(const ::fantasybit::GlobalState & copy){
        mySeason = copy.season();
        myState = copy.state();
    }

    void copyFrom(const ::fantasybit::GlobalState & copy){
        mySeason = copy.season();
        myState = copy.state();
    }

    bool operator ==(const GlobalStateViewModel &other) const {
        return other.mySeason == mySeason && other.myState == myState;
    }

    GlobalState_State state() { return myState; }

    quint32 season() { return mySeason; }

    QString seasonString() const { return QString("%1").arg(mySeason) ;}

    QString stateString() const {
        switch (myState) {
        case GlobalState_State_PREDRAFT: return QString("PREDRAFT");
        case GlobalState_State_PRESEASON: return QString("PRESEASON");
        case GlobalState_State_ROSTER53MAN: return QString("ROSTER53MAN");
        case GlobalState_State_INSEASON: return QString("INSEASON");
        default:
            return "Season Unknown";
        }
    }

    void setState(GlobalState_State state){ myState = state; }

    void setSeason(quint32 season){ mySeason = season; }
};

class TeamStateViewModel : public Descriptable, public Decorable {

    TeamState_State myState;
    quint32 myWeek;
    QString myTeamId;

public:

    TeamStateViewModel() {}

    TeamStateViewModel(const TeamStateViewModel & copy){
        myState = copy.myState;
        myWeek = copy.myWeek;
        myTeamId = copy.myTeamId;
    }

    TeamStateViewModel(const ::fantasybit::TeamState & teamstate){
        myState = teamstate.state();
        myWeek = teamstate.week();
        myTeamId = QString::fromStdString(teamstate.teamid());
    }

    TeamStateViewModel(TeamState_State state,quint32 week,const QString & teamId){
        myState = state;
        myWeek = week;
        myTeamId = teamId;
    }

    void copyFrom(const ::fantasybit::TeamState & teamstate){
        myState = teamstate.state();
        myWeek = teamstate.week();
        myTeamId = QString::fromStdString(teamstate.teamid());
    }

    bool operator ==(const TeamStateViewModel &other) const {
        return other.myState == myState &&
                other.myWeek == myWeek &&
                other.myTeamId.toLower() == myTeamId.toLower();
    }

    QString teamStateString() const {
        switch (myState) {
        case TeamState_State_PREGAME: return "PREGAME";
        case TeamState_State_INGAME : return "INGAME";
        default:
            return "Team Satet Unknown";
        }
    }

    QString weekString() const {
        return QString("%1").arg(myWeek);
    }

    TeamState_State teamState() { return myState; }

    quint32 week() const { return myWeek; }

    QString teamId()const { return myTeamId; }

    void setState(TeamState_State state){
        myState = state;
    }
};

Q_DECLARE_METATYPE(TeamStateViewModel*)

class TeamStateTableModel : public TKeyedListModel<QString,TeamStateViewModel> {

public:
  TeamStateTableModel(): TKeyedListModel<QString,TeamStateViewModel>(NULL) {
    initialize();
  }
  ~TeamStateTableModel() {}

  void initialize(){
    QStringList  headers;
    headers << "NFL Team"<< "Status"<<"Week";
    setHorizontalHeaders(headers);
  }
protected:
  QVariant getColumnDisplayData(quint32 column,TeamStateViewModel * data){
    if (data==NULL) return QVariant();
    if( column ==2)
      return data->week();
    if( column ==0)
      return data->teamId();
    if( column ==1)
      return data->teamStateString();
    return QVariant();

  }
  int getColumnCount(){
    return 3;
  }
};

class PlayerDataViewModel : public Descriptable, public Decorable {

    QString myPlayerId;
    QString myTeamId;

public:

    PlayerDataViewModel(){}
    ~PlayerDataViewModel(){}

    PlayerDataViewModel(const PlayerDataViewModel & copy){
        myPlayerId = copy.myPlayerId;
        myTeamId = copy.myTeamId;
    }

    PlayerDataViewModel(const  ::fantasybit::Data & copy) {
        if (copy.type() != Data_Type_PLAYER) return;
        PlayerData playerData = copy.GetExtension(PlayerData::player_data);
        myPlayerId = QString::fromStdString(playerData.playerid());
        myTeamId = QString::fromStdString(playerData.teamid());
    }

    PlayerDataViewModel(const QString & playerId,const QString teamId) {
        myPlayerId = playerId;
        myTeamId = teamId;
    }

    bool operator ==(const PlayerDataViewModel &other) const{
        return (other.myPlayerId.toLower() == myPlayerId.toLower() &&
                other.myTeamId.toLower() == myTeamId.toLower());
    }

    QString  playerId() const { return myPlayerId; }
    QString  teamId() const { return myTeamId; }

};

Q_DECLARE_METATYPE(PlayerDataViewModel*)

class PlayerDataTableModel : public TKeyedListModel<QString,PlayerDataViewModel> {

public:
  PlayerDataTableModel(): TKeyedListModel<QString,PlayerDataViewModel>(NULL) {
    initialize();
  }
  ~PlayerDataTableModel() {}

  void initialize(){
    QStringList  headers;
    headers << "Team Id"<< "Player Id";
    setHorizontalHeaders(headers);
  }
protected:
  QVariant getColumnDisplayData(quint32 column,PlayerDataViewModel * data){
    if (data==NULL) return QVariant();
    if( column ==0)
      return data->teamId();
    if( column ==1)
      return data->playerId();
    return QVariant();

  }
  int getColumnCount(){
    return 2;
  }
};

class TeamDataViewModel : public Descriptable, public Decorable{

    QMap<QString,PlayerDataViewModel> myPlayers;
    QMap<quint32,TeamStateViewModel> myTeamStates;
    QString myTeamId;

public:

    TeamDataViewModel(){}

    ~TeamDataViewModel(){}

    TeamDataViewModel(const TeamDataViewModel& copy){
        myTeamId = copy.myTeamId;
    }

    TeamDataViewModel(const  ::fantasybit::Data & copy) {
        if (copy.type() != Data_Type_TEAM) return;
        TeamData teamData = copy.GetExtension(TeamData::team_data);
        myTeamId = QString::fromStdString(teamData.teamid());
    }

    bool operator ==(const TeamDataViewModel &other) const{
        return (other.myTeamId.toLower() == myTeamId.toLower());
    }

    QString  teamId() const { return myTeamId; }

    bool addPlayer(const QString & teamPlayerId){
        if (teamPlayerId.trimmed()=="") return false;
        if (myPlayers.keys().contains(teamPlayerId)) return false;
        myPlayers.insert(teamPlayerId,PlayerDataViewModel(teamPlayerId,myTeamId));
    }

    bool removePlayer(const QString & teamPlayerId){
        if (teamPlayerId.trimmed()=="") return false;
        if (!myPlayers.keys().contains(teamPlayerId)) return false;
        int removed = myPlayers.remove(teamPlayerId);
        return removed > 0;
    }

    QList<PlayerDataViewModel> players() const {
        return myPlayers.values();
    }

    quint32 playersCount() {return myPlayers.size();}

    bool hasPlayer(const QString & playerId){
        return myPlayers.contains(playerId);
    }

    void setWeekState(quint32 week,TeamState_State state){
        if (myTeamStates.contains(week))
            myTeamStates[week].setState(state);
        else
            myTeamStates.insert(week,TeamStateViewModel(state,week,myTeamId));
    }

    quint32 weeksCount() { return myTeamStates.size(); }

    QList<TeamStateViewModel>  weekStates() const { return myTeamStates.values();}

};

Q_DECLARE_METATYPE(TeamDataViewModel*)


class TeamDataTableModel : public TKeyedListModel<QString,TeamDataViewModel> {

public:
  TeamDataTableModel(): TKeyedListModel<QString,TeamDataViewModel>(NULL) {
    initialize();
  }
  ~TeamDataTableModel() {}

  void initialize(){
    QStringList  headers;
    headers << "Team Id";
    setHorizontalHeaders(headers);
  }
protected:
  QVariant getColumnDisplayData(quint32 column,TeamDataViewModel * data){
    if (data==NULL) return QVariant();
    if( column ==0)
      return data->teamId();
    return QVariant();

  }
  int getColumnCount(){
    return 1;
  }
};

class FantasyPlayerViewModel : public Descriptable, public Decorable {
    QString myPlayerName;
    quint64 myBits;

public:

    FantasyPlayerViewModel(){}
    ~FantasyPlayerViewModel(){}

    FantasyPlayerViewModel(const FantasyPlayerViewModel & copy){
        myPlayerName = copy.myPlayerName;
        myBits = copy.myBits;
    }

    FantasyPlayerViewModel(const ::fantasybit::FantasyPlayer & copy) {
        myPlayerName = QString::fromStdString(copy.name());
        myBits = copy.bits();
    }

    FantasyPlayerViewModel(const QString & playerName,quint64 bits) {
        myPlayerName = playerName;
        myBits = bits;
    }

    bool operator ==(const FantasyPlayerViewModel &other) const{
        return (other.myPlayerName.toLower() == myPlayerName.toLower() &&
                other.myBits == myBits);
    }

    QString  playerName() const { return myPlayerName; }
    quint64  bits() const { return myBits; }

    void setBits(quint64 bits){ myBits = bits;}
    void setFantasyName(const QString & name){ myPlayerName = name; }

};

Q_DECLARE_METATYPE(FantasyPlayerViewModel*)

class FantasyPlayerTableModel : public TKeyedListModel<QString,FantasyPlayerViewModel> {

public:
  FantasyPlayerTableModel(): TKeyedListModel<QString,FantasyPlayerViewModel>(NULL) {
    initialize();
  }
  ~FantasyPlayerTableModel() {}

  void initialize(){
    QStringList  headers;
    headers << "FantasyName" << "FantsyBits";
    setHorizontalHeaders(headers);
  }
protected:
  QVariant getColumnDisplayData(quint32 column,FantasyPlayerViewModel * data){
    if (data==NULL) return QVariant();
    if( column ==0)
      return data->playerName();
    if( column ==1)
      return data->bits();
    return QVariant();

  }
  int getColumnCount(){
    return 2;
  }
};

class SnapShotViewModel {
public:
    QString fantasyName;
    quint64 fantasyNameBalance = 0;
    int     week = 1;
    MyNameStatus myNameStatus = MyNameStatus::none;

    GlobalStateViewModel globalStateModel;
    QMap<QString,TeamDataViewModel> teams;
    QMap<QString,TeamStateViewModel> teamStates;
    QMap<QString,PlayerDataViewModel> players;
    QMap<QString,FantasyPlayerViewModel> fantasyPlayers;

    SnapShotViewModel(){}

    SnapShotViewModel(const DeltaData & data){ fromDeltaData(data);}

    ~SnapShotViewModel(){}


    bool updateDB(FantasyPlayerViewModel * viewModel){
        if (viewModel==NULL) return false;
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

        db.setHostName("162.254.27.178");
        db.setPort(3306);
        db.setDatabaseName("satoshifantasy");
        db.setUserName("datafeed");
        db.setPassword("s@tof@nt@sy6#1");
        bool success = true;
        if (!db.open()) {
             qDebug() << "Database error occurred :" << db.lastError().databaseText();
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

    void fromDeltaData(const DeltaData & data) {
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

        if (data.has_myfantasyname()) {
            fantasyName = QString::fromStdString(data.myfantasyname().name());
            myNameStatus = data.myfantasyname().status();
        }

        if (data.has_globalstate())
            globalStateModel.copyFrom(data.globalstate());

        for (auto t : data.datas()) {
            if (t.type() == ::fantasybit::Data_Type_TEAM) {
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
            updateDB(&viewModel);
            fantasyPlayers.insert(viewModel.playerName(),viewModel);
            if ( viewModel.playerName() == fantasyName ) {
                myNameStatus = MyNameStatus::confirmed;
                fantasyNameBalance = viewModel.bits();
            }


        }
    }
};



class ScoringModelView :public Descriptable, public Decorable{
public:
    int myScore;
    QString myTeamId;
    QString myTeamName;
    QString myPlayerId;    
    ScoringModelView(){}
    ScoringModelView(const QString & teamId,
                     const QString & teamName,
                     const QString & playerId,int score=0){
        myScore = score;
        myTeamId= teamId;
        myTeamName= teamName;
        myPlayerId= playerId;
    }
    ScoringModelView(ScoringModelView & copy){
        myScore = copy.myScore;
        myTeamId= copy.myTeamId;
        myTeamName= copy.myTeamName;
        myPlayerId= copy.myPlayerId;

    }
};

Q_DECLARE_METATYPE(ScoringModelView*)

class ScoringTableModel : public TListModel<ScoringModelView> {

public:
  ScoringTableModel(): TListModel<ScoringModelView>(NULL) {
    initialize();
  }
  ~ScoringTableModel() {}

  void initialize(){
    QStringList  headers;
    headers << "Team" << "PlayerId" << "Proj/Result";
    setHorizontalHeaders(headers);
  }

protected:
  QVariant getColumnDisplayData(quint32 column,ScoringModelView * data){
    if (data==NULL) return QVariant();
    if( column ==0)
      return data->myTeamName;
    if( column ==1)
      return data->myPlayerId;
    if( column ==2)
      return data->myScore;
    return QVariant();

  }
  int getColumnCount(){
    return 3;
  }
  void setDataFromColumn(ScoringModelView* data, const QModelIndex &index,const QVariant &vvalue,int role){
    if (index.column() >= getColumnCount()) return;
    //only allow edition of the score
    if (index.column()==2)
        data->myScore = vvalue.toInt();
  }
};


#endif // MODELS_H

