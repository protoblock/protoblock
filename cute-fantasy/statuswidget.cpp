#include "statuswidget.h"
#include "ui_statuswidget.h"

StatusWidget::StatusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusWidget)
{
    ui->setupUi(this);
}

StatusWidget::~StatusWidget()
{
    delete ui;
}

void StatusWidget::setStatusMessage(const QString & statusMessage){
  ui->myStatusMessageLabel->setText(statusMessage);
}

void StatusWidget::setSyncStatus(const QString & syncStatus){
 ui->mySyncStatusLabel->setText(syncStatus);
}

QString StatusWidget::getStatusMessage() const {
    return ui->myStatusMessageLabel->text();
}
QString StatusWidget::getSyncMessage() const {
    return ui->mySyncStatusLabel->text();
}

