#ifndef FANTASYNAMESETUPDIALOG_H
#define FANTASYNAMESETUPDIALOG_H

#include <QDialog>

namespace Ui {
class FantasyNameSetupDialog;
}

class FantasyNameSetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FantasyNameSetupDialog(QWidget *parent = 0);
    ~FantasyNameSetupDialog();

private:
    Ui::FantasyNameSetupDialog *ui;
};

#endif // FANTASYNAMESETUPDIALOG_H
