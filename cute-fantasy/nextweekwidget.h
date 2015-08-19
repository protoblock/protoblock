#ifndef NEXTWEEKWIDGET_H
#define NEXTWEEKWIDGET_H

#include <QWidget>

namespace Ui {
class NextWeekWidget;
}

class NextWeekWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NextWeekWidget(QWidget *parent = 0);
    ~NextWeekWidget();

private:
    Ui::NextWeekWidget *ui;
};

#endif // NEXTWEEKWIDGET_H
