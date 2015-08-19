#include "previousweekwidget.h"
#include "ui_previousweekwidget.h"

PreviousWeekWidget::PreviousWeekWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviousWeekWidget)
{
    ui->setupUi(this);
}

PreviousWeekWidget::~PreviousWeekWidget()
{
    delete ui;
}
