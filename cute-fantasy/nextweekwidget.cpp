#include "nextweekwidget.h"
#include "ui_nextweekwidget.h"

NextWeekWidget::NextWeekWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NextWeekWidget)
{
    ui->setupUi(this);
}

NextWeekWidget::~NextWeekWidget()
{
    delete ui;
}
