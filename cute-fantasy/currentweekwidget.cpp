#include "currentweekwidget.h"
#include "ui_currentweekwidget.h"

CurrentWeekWidget::CurrentWeekWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurrentWeekWidget)
{
    ui->setupUi(this);
}

CurrentWeekWidget::~CurrentWeekWidget()
{
    delete ui;
}
