#ifndef TESTMNEMONICDIALOG_H
#define TESTMNEMONICDIALOG_H

#include <QDialog>

namespace Ui {
class TestMnemonicDialog;
}

class TestMnemonicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestMnemonicDialog(QWidget *parent = 0);
    ~TestMnemonicDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::TestMnemonicDialog *ui;
};

#endif // TESTMNEMONICDIALOG_H
