#include "gamesoftheweekdialog.h"
#include "ui_gamesoftheweekdialog.h"

GamesOfTheWeekDialog::GamesOfTheWeekDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GamesOfTheWeekDialog)
{
    ui->setupUi(this);
}

GamesOfTheWeekDialog::~GamesOfTheWeekDialog()
{
    delete ui;
}
