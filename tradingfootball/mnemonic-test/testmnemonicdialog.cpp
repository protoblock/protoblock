#include "testmnemonicdialog.h"
#include "ui_testmnemonicdialog.h"
#include "mnemonic.h"
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

void TestMnemonicDialog::on_pushButton_clicked()
{
    QString text = "hammer common grid quality inform away nest mandate minute case scrap amateur" ;
    QByteArray  buffer =Mnemonic::test(text);
    ui->textEdit->append(QString("mnemonic test : %1").arg(QString(buffer.toHex())));
}
