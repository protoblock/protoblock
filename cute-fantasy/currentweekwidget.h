#ifndef CURRENTWEEKWIDGET_H
#define CURRENTWEEKWIDGET_H

#include <QWidget>
#include "gamedataviewmodel.h"
#include "leaderbord.h"
#include "gameprojectiontablemodel.h"

namespace Ui {
class CurrentWeekWidget;
}

class CurrentWeekWidget : public QWidget
{
    Q_OBJECT

public:

    explicit CurrentWeekWidget(QWidget *parent = 0);
    ~CurrentWeekWidget();

private slots:
    void on_myCompletedGamesRb_toggled(bool checked);

    void on_myInGamesRb_toggled(bool checked);

    void on_myUpcomingGamesRb_toggled(bool checked);

private:
    Ui::CurrentWeekWidget *ui;    
    GameTableModel  myGameTableModel;
    GameViewFilterProxyModel myGameModelFilter;
    LeaderBaordViewModelTableModel myLeaderBoardModel;
    GameProjectionTableModel myProjectionsModel;

};

#endif // CURRENTWEEKWIDGET_H
