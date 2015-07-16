#include "splashdialog.h"
#include "ui_splashdialog.h"

SplashDialog::SplashDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SplashDialog)
{
    ui->setupUi(this);
}

SplashDialog::~SplashDialog()
{
    delete ui;
}
