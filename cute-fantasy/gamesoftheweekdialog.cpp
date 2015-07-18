#include "gamesoftheweekdialog.h"
#include "ui_gamesoftheweekdialog.h"
#include <QTabBar>

GamesOfTheWeekDialog::GamesOfTheWeekDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GamesOfTheWeekDialog)
{
    ui->setupUi(this);    
    QTabBar * tabBar = ui->myTeamTab->tabBar();
    tabBar->setStyle(new CustomTabStyle);
}

GamesOfTheWeekDialog::~GamesOfTheWeekDialog()
{
    delete ui;
}
