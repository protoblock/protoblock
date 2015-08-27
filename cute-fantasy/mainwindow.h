#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "FantasyName.h"
#include <vector>
#include "waitmodaldialog.h"

namespace Ui {
class MainWindow;
}

using fantasybit::GlobalState;
class MainLAPIWorker;
class CurrentWeekWidget;
class NextWeekWidget;
class PreviousWeekWidget;

using namespace std;
using namespace fantasybit;


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:    

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
    void NewProjection(fantasybit::FantasyBitProj &);
    void ClaimFantasyName(QString);

public slots:

    void on_myNextWeek_clicked(); 
    void on_myPreviousWeek_clicked();
    void GoLive(fantasybit::GlobalState state);
    void OnMyFantasyNames(vector<fantasybit::MyFantasyName> names);
    void OnNameStatus(fantasybit::MyFantasyName name);
    void OnProjAck(fantasybit::FantasyBitProj);
    void OnAward(QVariant);
    void OnNewWeek(int);
    void OnGameOver(QString);
    void OnGameStart(QString);


private slots:
    void on_myFantasyNamesCombo_currentIndexChanged(int index);
    void on_myFantasyNamesCombo_currentTextChanged(const QString &name);

    void on_myFantasyNamesCombo_editTextChanged(const QString &arg1);

private:
    void initialize();
    void initDefaultGuiDisplay();

    void nextWeek();
    void previousWeek();
    void currentWeek();
    void navigateToWeek(int week);


    MainLAPIWorker *  myLAPIWorker;
    Ui::MainWindow *ui;    
    int myCurrentWeek;    
    fantasybit::MyFantasyName myCurrentFantasyName;
    GlobalState myGlobalState;
    WaitModalDialog myWaitDialog;
    bool myAddNamesPending= false;

};

#endif // MAINWINDOW_H
