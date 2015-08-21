#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


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

private slots:

    void on_myNextWeek_clicked();
    void on_myPreviousWeek_clicked();

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
};

#endif // MAINWINDOW_H
