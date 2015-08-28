#ifndef CURRENTWEEKWIDGET_H
#define CURRENTWEEKWIDGET_H

#include <QWidget>
#include "tablemodels.h"
#include "FantasyName.h"

namespace Ui {
class CurrentWeekWidget;
}

class CurrentWeekWidget : public QWidget
{
    friend class MainWindow;
    Q_OBJECT

public:

    explicit CurrentWeekWidget(QWidget *parent = 0);
    ~CurrentWeekWidget();
    void setCurrentWeekData(GlobalState state);

    void onGameOver(string gameId);
    void onGameStart(string gameId);


signals:
    void NewProjection(fantasybit::FantasyBitProj);

public slots:
    void onUserSwitchFantasyName(const std::string fantasyPlayerId);

private slots:

    void on_myCompletedGamesRb_toggled(bool checked);
    void on_myInGamesRb_toggled(bool checked);
    void on_myUpcomingGamesRb_toggled(bool checked);

    void on_mySendProjectionButton_clicked();

private:

    void updateCurrentFantasyPlayerProjections();
    Ui::CurrentWeekWidget *ui;
    GameTableModel  myGameTableModel{WeekDisplayType::CurrentWeek};
    GameViewFilterProxyModel myGameModelFilter;    
    ProjectionSheetTableModel myProjectionsModel {WeekDisplayType::CurrentWeek};

    int myCurrentWeek;
    std::string myFantasyName;
    GlobalState myGlobalState;
    std::vector<fantasybit::GameRoster> myGameRosters;    
    SpinBoxDelegate myProjectionDelegate;





};

#endif // CURRENTWEEKWIDGET_H
