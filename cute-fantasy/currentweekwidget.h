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

public slots:
    void onUserSwitchFantasyName(const std::string fantasyPlayerId);

private slots:

    void on_myCompletedGamesRb_toggled(bool checked);
    void on_myInGamesRb_toggled(bool checked);
    void on_myUpcomingGamesRb_toggled(bool checked);

private:

    void updateCurrentFantasyPlayerProjections();
    Ui::CurrentWeekWidget *ui;
    GameTableModel  myGameTableModel{WeekDisplayType::CurrentWeek};
    GameViewFilterProxyModel myGameModelFilter;
    LeaderBoardTableModel myLeaderBoardModel{WeekDisplayType::CurrentWeek};
    ProjectionSheetTableModel myProjectionsModel {WeekDisplayType::CurrentWeek};
    bool myCurrentWeekDataLoaded;
    int myCurrentWeek;
    std::string myFantasyPlayerId;
    GlobalState myGlobalState;
    std::vector<fantasybit::GameRoster> myGameRosters;
    std::vector< std::shared_ptr<fantasybit::FantasyName> > myLeaderBoardData;
};

#endif // CURRENTWEEKWIDGET_H
