#ifndef GAMEVIEWMODEL_H
#define GAMEVIEWMODEL_H

#include "viewmodels.h"
#include <QMetaType>
#include <QDateTime>

class GameViewModel : public Descriptable, public Decorable {

public:

    GameViewModel(void) {

    }
    GameViewModel(GameViewModel const & src) {
          *this = src;
    }
    ~GameViewModel(void) {

    }

    GameViewModel & operator=(GameViewModel const & src) {
        myGameId = src.myGameId;
        myHomeTeam = src.myHomeTeam;
        myAwayTeam = src.myAwayTeam;
        myGameStatus = src.myGameStatus;
        myGameTime = src.myGameTime;
    }

    bool  operator==(GameViewModel const & rhs) {
        return (rhs.GameId() == myGameId);
    }

    //setters
    void setGameId(const QString & argGameIdValue) { myGameId = argGameIdValue;}
    void setHomeTeam(const QString & argHomeTeamValue) { myHomeTeam = argHomeTeamValue;}
    void setAwayTeam(const QString & argAwayTeamValue) { myAwayTeam = argAwayTeamValue;}
    void setGameStatus(GameStatus_Status  argGameStatusValue) { myGameStatus = argGameStatusValue;}
    void setGameTime(const QDateTime & argGameTimeValue) { myGameTime = argGameTimeValue;}

    //getters
    QString GameId() const { return myGameId;}
    QString HomeTeam() const { return myHomeTeam;}
    QString AwayTeam() const { return myAwayTeam;}
    GameStatus_Status GameStatus(){ return myGameStatus;}
    QDateTime gameTime() const { return myGameTime;}


private :

    QString myGameId;
    QString myHomeTeam;
    QString myAwayTeam;
    GameStatus_Status myGameStatus;
    QDateTime myGameTime;
};

Q_DECLARE_METATYPE(GameViewModel*)

#include "tkeyedlist.h"

class  GameTableModel : public TKeyedListModel<QString,GameViewModel> {

public:

    GameTableModel(): TKeyedListModel<QString,GameViewModel>(NULL) {
        initialize();
    }
    ~GameTableModel() {}

protected:

    QVariant getColumnDisplayData(quint32 column,GameViewModel * data) {
        if (data==NULL) return QVariant();
        if( column ==0)
            return data->GameId();
        if( column ==1)
            return data->HomeTeam();
        if( column ==2)
            return data->AwayTeam();
        if( column ==3)
            return QString("%1").arg(data->GameStatus());
        if( column ==4)
            return data->gameTime();
        return QVariant();
    }
    int getColumnCount() {
        return 5;
    }
private:
    void initialize() {
        QStringList  headers;

        headers << "GameId";
        headers << "HomeTeam";
        headers << "AwayTeam";
        headers << "GameStatus";
        headers << "GameTime";
        setHorizontalHeaders(headers);
    }
};

#include <QSortFilterProxyModel>

class GameViewFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:

    enum GamesFilter {
        Completed ,
        InGame,
        Upcoming,
        All
    };

    GameViewFilterProxyModel(QObject *parent = 0)
        : QSortFilterProxyModel(parent)
    {
        myGamesFilter = GamesFilter::All;
    }
    void setGameStatusFilter(GamesFilter gamesFilter){
        myGamesFilter = gamesFilter;
        invalidateFilter();
    }

    GamesFilter  filter(){
        return myGamesFilter;
    }

protected:
    //filtering
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {        
        if (myGamesFilter == GamesFilter::All)
           return true;

         GameTableModel * model = dynamic_cast<GameTableModel *>(sourceModel());
         if (model==NULL) return true;
         QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
         GameViewModel * item = model->getItemByIndex(index);
         if (item == NULL) return true;

         if (myGamesFilter == GamesFilter::Completed)
            return item->GameStatus()== GameStatus_Status_CLOSED;

         if (myGamesFilter == GamesFilter::Upcoming)
            return (item->GameStatus()== GameStatus_Status_SCHEDULED) ||
                    (item->GameStatus()== GameStatus_Status_PREGAME);

         if (myGamesFilter == GamesFilter::InGame)
            return item->GameStatus()== GameStatus_Status_INGAME;
    }
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const {
        GameTableModel * model = dynamic_cast<GameTableModel *>(sourceModel());
        if (model==NULL) return true;
        GameViewModel * leftItem = model->getItemByIndex(left);
        if (leftItem == NULL)  return true;
        GameViewModel * rightItem = model->getItemByIndex(right);
        if (rightItem == NULL)  return true;
        return leftItem->gameTime() < rightItem->gameTime();
    }

private:
    GamesFilter myGamesFilter;

};


#endif // GAMEVIEWMODEL_H

