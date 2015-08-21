#include "currentweekwidget.h"
#include "ui_currentweekwidget.h"

CurrentWeekWidget::CurrentWeekWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurrentWeekWidget)
{
    ui->setupUi(this);    
    ui->myGamesTableView->setModel(&myGameTableModel);
    myGameModelFilter.setSourceModel(&myGameTableModel);
    ui->myLeaderBaordTableView->setModel(&myLeaderBoardModel);
    ui->myProjectionTableView->setModel(&myProjectionsModel);
    //start with upcoming games filter
    ui->myUpcomingGamesRb->setChecked(true);
}

CurrentWeekWidget::~CurrentWeekWidget()
{
    delete ui;
}

void CurrentWeekWidget::on_myCompletedGamesRb_toggled(bool checked)
{
    if (checked)
        if (myGameModelFilter.filter()== GameViewFilterProxyModel::Completed)
            return;
        else
            myGameModelFilter.setGameStatusFilter(GameViewFilterProxyModel::Completed);
}

void CurrentWeekWidget::on_myInGamesRb_toggled(bool checked)
{
    if (checked)
        if (myGameModelFilter.filter()== GameViewFilterProxyModel::InGame)
            return;
        else
            myGameModelFilter.setGameStatusFilter(GameViewFilterProxyModel::InGame);
}

void CurrentWeekWidget::on_myUpcomingGamesRb_toggled(bool checked)
{
    if (checked)
        if (myGameModelFilter.filter()== GameViewFilterProxyModel::Upcoming)
            return;
        else
            myGameModelFilter.setGameStatusFilter(GameViewFilterProxyModel::Upcoming);
}
