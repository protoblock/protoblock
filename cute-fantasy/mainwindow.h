#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "FantasyName.h"


namespace Ui {
class MainWindow;
}

using fantasybit::GlobalState;
class MainLAPIWorker;
class CurrentWeekWidget;
class NextWeekWidget;
class PreviousWeekWidget;


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

    void SubscribeGameState();
    void UseMyFantasyName(QString);        
    void SubscribeMyNameTx(QString);
    void SubscribeMyProjTx(QString);
    void SubscribeAwards(QString);
    void SubscribePlayerData();
    void SubscribeScheduleData();
    void SubscribeTeamRoster();
    void SubscribePlayerGameStatus();
    void NewProjection(fantasybit::FantasyBitProj);
    void ClaimFantasyName(QString);

public slots:

    void on_myNextWeek_clicked(); 
    void on_myPreviousWeek_clicked();
    void GoLive(GlobalState state);
    void OnMyFantasyNames(std::vector<fantasybit::MyFantasyName> names);
    void OnNameStatus(fantasybit::MyFantasyName name);
    void OnProjAck(fantasybit::FantasyBitProj);
    void OnAward(QVariant);

    void OnNewWeek(int);
    void OnGameOver(QString);
    void OnGameStart(QString);


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
    GlobalState myGlobalState;    

};

#endif // MAINWINDOW_H
