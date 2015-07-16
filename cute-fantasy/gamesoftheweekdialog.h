#ifndef GAMESOFTHEWEEKDIALOG_H
#define GAMESOFTHEWEEKDIALOG_H

#include <QDialog>

namespace Ui {
class GamesOfTheWeekDialog;
}

class GamesOfTheWeekDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GamesOfTheWeekDialog(QWidget *parent = 0);
    ~GamesOfTheWeekDialog();

private:
    Ui::GamesOfTheWeekDialog *ui;
};

#endif // GAMESOFTHEWEEKDIALOG_H
