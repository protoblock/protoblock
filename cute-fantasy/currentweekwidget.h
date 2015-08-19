#ifndef CURRENTWEEKWIDGET_H
#define CURRENTWEEKWIDGET_H

#include <QWidget>

namespace Ui {
class CurrentWeekWidget;
}

class CurrentWeekWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CurrentWeekWidget(QWidget *parent = 0);
    ~CurrentWeekWidget();

private:
    Ui::CurrentWeekWidget *ui;
};

#endif // CURRENTWEEKWIDGET_H
