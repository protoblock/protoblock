//#ifndef GAMEVIEWMODEL_H
//#define GAMEVIEWMODEL_H

//#include "viewmodels.h"
//#include <QMetaType>
//#include <QDateTime>

////class CurrentWeekGameViewModel : public Descriptable, public Decorable {

////public:

////    CurrentWeekGameViewModel(void) {}

////    CurrentWeekGameViewModel(const GameInfo & game,GameStatus_Status status){
////        myGameId = QString(game.id().data());
////        myHomeTeam = QString(game.home().data());
////        myAwayTeam = QString(game.away().data());
////        myGameStatus = status;
////        myGameTime = QDateTime::fromMSecsSinceEpoch(game.time());
////    }

////    CurrentWeekGameViewModel(const CurrentWeekGameViewModel & src) { *this = src; }

////    ~CurrentWeekGameViewModel(){}

////    CurrentWeekGameViewModel & operator=(const CurrentWeekGameViewModel & src) {
////        myGameId = src.myGameId;
////        myHomeTeam = src.myHomeTeam;
////        myAwayTeam = src.myAwayTeam;
////        myGameStatus = src.myGameStatus;
////        myGameTime = src.myGameTime;
////        return *this;
////    }

////    bool  operator==(const CurrentWeekGameViewModel & rhs) {
////        return (rhs.GameId() == myGameId);
////    }

////    //setters
////    void setGameId(const QString & argGameIdValue) { myGameId = argGameIdValue;}
////    void setHomeTeam(const QString & argHomeTeamValue) { myHomeTeam = argHomeTeamValue;}
////    void setAwayTeam(const QString & argAwayTeamValue) { myAwayTeam = argAwayTeamValue;}
////    void setGameStatus(GameStatus_Status  argGameStatusValue) { myGameStatus = argGameStatusValue;}
////    void setGameTime(const QDateTime & argGameTimeValue) { myGameTime = argGameTimeValue;}

////    //getters
////    QString GameId() const { return myGameId;}
////    QString HomeTeam() const { return myHomeTeam;}
////    QString AwayTeam() const { return myAwayTeam;}
////    GameStatus_Status GameStatus(){ return myGameStatus;}
////    QDateTime gameTime() const { return myGameTime;}


////private :

////    QString myGameId;
////    QString myHomeTeam;
////    QString myAwayTeam;
////    GameStatus_Status myGameStatus;
////    QDateTime myGameTime;
////};

////Q_DECLARE_METATYPE(CurrentWeekGameViewModel*)


////class GameResultViewModel : public Descriptable, public Decorable {

////public:

////    GameResultViewModel() {}

////    GameResultViewModel(const GameResultViewModel & src) {
////        *this = src;
////    }
////    ~GameResultViewModel() {

////    }

////    GameResultViewModel & operator=(const GameResultViewModel & src) {
////        if (this!=&src){
////            myGameId = src.myGameId;

////            myKickOffTime = src.myKickOffTime;
////            myHomePlayerResults.clear();
////            myAwayPlayerResults.clear();
////            myHomePlayerResults.append(src.myHomePlayerResults);
////            myAwayPlayerResults.append(src.myAwayPlayerResults);
////        }
////        return *this;
////    }

////    bool operator==(GameResultViewModel const & rhs) {
////        return myGameId == rhs.myGameId && myKickOffTime == rhs.myKickOffTime;
////    }

////    //setters
////    void setGameId(const QString & argGameIdValue) { myGameId = argGameIdValue;}
////    void setKickOffTime(QDateTime  argKickOffTimeValue) { myKickOffTime = argKickOffTimeValue;}
////    void setHomePlayerResults(const QList<PlayerResult> & argHomePlayerResultsValue) { myHomePlayerResults = argHomePlayerResultsValue;}
////    void setAwayPlayerResults(const QList<PlayerResult> & argAwayPlayerResultsValue) { myAwayPlayerResults = argAwayPlayerResultsValue;}

////    //getters
////    QString GameId() const { return myGameId;}
////    QDateTime KickOffTime(){ return myKickOffTime;}
////    QList<PlayerResult> HomePlayerResults() const { return myHomePlayerResults;}
////    QList<PlayerResult> AwayPlayerResults() const { return myAwayPlayerResults;}


////private :
////    QString myGameId;
////    QDateTime myKickOffTime;
////    QList<PlayerResult> myHomePlayerResults;
////    QList<PlayerResult> myAwayPlayerResults;
////};

////Q_DECLARE_METATYPE(GameResultViewModel*)

////#include "tkeyedlist.h"

////class   GameResultViewModelTableModel : public TKeyedListModel<QString,GameResultViewModel> {

////public:

////    GameResultViewModelTableModel() : TKeyedListModel<QString,GameResultViewModel>(NULL) {
////        initialize();
////    }
////    ~GameResultViewModelTableModel() {}

////protected:
////    QVariant getColumnDisplayData(quint32 column,GameResultViewModel * data) {

////        if (data==NULL) return QVariant();
////        if( column ==0)
////            return data->GameId();
////        if( column ==1)
////            return data->KickOffTime();
////        return QVariant();
////    }
////    int getColumnCount() {
////        return 2;
////    }
////private:
////    void initialize() {
////        QStringList  headers;
////        headers << "GameId";
////        headers << "KickOffTime";
////        setHorizontalHeaders(headers);
////    }
////};



//#include "tkeyedlist.h"

//class  GameTableModel : public TKeyedListModel<QString,GameViewModel> {

//public:

//    GameTableModel(): TKeyedListModel<QString,GameViewModel>(NULL) {
//        initialize();
//    }
//    ~GameTableModel() {}

//protected:

//    QVariant getColumnDisplayData(quint32 column,GameViewModel * data) {
//        if (data==NULL) return QVariant();
//        if( column ==0)
//            return data->GameId();
//        if( column ==1)
//            return data->HomeTeam();
//        if( column ==2)
//            return data->AwayTeam();
//        if( column ==3)
//            return QString("%1").arg(data->GameStatus());
//        if( column ==4)
//            return data->gameTime();
//        return QVariant();
//    }
//    int getColumnCount() {
//        return 5;
//    }
//private:
//    void initialize() {
//        QStringList  headers;

//        headers << "GameId";
//        headers << "HomeTeam";
//        headers << "AwayTeam";
//        headers << "GameStatus";
//        headers << "GameTime";
//        setHorizontalHeaders(headers);
//    }
//};



//#endif // GAMEVIEWMODEL_H

