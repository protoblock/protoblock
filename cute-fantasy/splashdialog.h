#ifndef SPLASHDIALOG_H
#define SPLASHDIALOG_H

#include <QDialog>

namespace Ui {
class SplashDialog;
}

class SplashDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SplashDialog(QWidget *parent = 0);
    ~SplashDialog();

private:
    Ui::SplashDialog *ui;
};

#endif // SPLASHDIALOG_H
