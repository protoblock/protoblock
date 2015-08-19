#ifndef PREVIOUSWEEKWIDGET_H
#define PREVIOUSWEEKWIDGET_H

#include <QWidget>

namespace Ui {
class PreviousWeekWidget;
}

class PreviousWeekWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PreviousWeekWidget(QWidget *parent = 0);
    ~PreviousWeekWidget();

private:
    Ui::PreviousWeekWidget *ui;
};

#endif // PREVIOUSWEEKWIDGET_H
