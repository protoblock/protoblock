#include "playerprojectionwidget.h"
#include "ui_playerprojectionwidget.h"

PlayerProjectionWidget::PlayerProjectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerProjectionWidget)
{
    ui->setupUi(this);
}

PlayerProjectionWidget::~PlayerProjectionWidget()
{
    delete ui;
}
