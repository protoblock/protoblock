#ifndef NEXTWEEKWIDGET_H
#define NEXTWEEKWIDGET_H

#include <QWidget>
#include "tablemodels.h"

namespace Ui {
class NextWeekWidget;
}

class NextWeekWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NextWeekWidget(QWidget *parent = 0);
    ~NextWeekWidget();
    void setWeekData(int week);

private:
    Ui::NextWeekWidget *ui;
    int myWeekNumber = -1;
    GameTableModel  myGameTableModel{WeekDisplayType::UpcomingWeek};
    ProjectionSheetTableModel myProjectionsModel {WeekDisplayType::UpcomingWeek};
    int myCurrentWeek;
    fantasybit::WeeklySchedule myWeeklySchedule;
};

#endif // NEXTWEEKWIDGET_H
