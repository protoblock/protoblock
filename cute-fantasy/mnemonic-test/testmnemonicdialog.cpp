#include "testmnemonicdialog.h"
#include "ui_testmnemonicdialog.h"

TestMnemonicDialog::TestMnemonicDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestMnemonicDialog)
{
    ui->setupUi(this);
}

TestMnemonicDialog::~TestMnemonicDialog()
{
    delete ui;
}
