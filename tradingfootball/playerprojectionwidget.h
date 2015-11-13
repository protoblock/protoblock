#ifndef PLAYERPROJECTIONWIDGET_H
#define PLAYERPROJECTIONWIDGET_H

#include <QWidget>

namespace Ui {
class PlayerProjectionWidget;
}

class PlayerProjectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerProjectionWidget(QWidget *parent = 0);
    ~PlayerProjectionWidget();

private:
    Ui::PlayerProjectionWidget *ui;
};

#endif // PLAYERPROJECTIONWIDGET_H
