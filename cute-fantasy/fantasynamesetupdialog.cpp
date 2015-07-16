#include "fantasynamesetupdialog.h"
#include "ui_fantasynamesetupdialog.h"

FantasyNameSetupDialog::FantasyNameSetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FantasyNameSetupDialog)
{
    ui->setupUi(this);
}

FantasyNameSetupDialog::~FantasyNameSetupDialog()
{
    delete ui;
}
