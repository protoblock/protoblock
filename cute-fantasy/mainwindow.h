#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "FantasyName.h"

namespace Ui {
class MainWindow;
}

class MainLAPIWorker;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    enum WeekViewMode {
        PreviousWeek,
        CurrentWeek,
        NextWeek
    };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


signals:
    void UseMyFantasyName(QString);

public slots:

    void on_myNextWeek_clicked();
    void on_myPreviousWeek_clicked();
    void GoLive(fantasybit::GlobalState);
    void OnMyFantasyNames(std::vector<fantasybit::MyFantasyName> names);
    void OnNameStatus(fantasybit::MyFantasyName name);

private:
    void initialize();

    void nextWeek();
    void previousWeek();
    void currentWeek();

    void setWeekViewMode(WeekViewMode  viewMode);
    MainLAPIWorker *  myCoreInstance;
    Ui::MainWindow *ui;    
    int myCurrentWeek;
    WeekViewMode myCurrentWeekViewMode;
    fantasybit::MyFantasyName myCurrentFantasyName;
};

#endif // MAINWINDOW_H
