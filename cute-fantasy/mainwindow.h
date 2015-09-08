#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "FantasyName.h"
#include <vector>
#include "waitmodaldialog.h"
#include "spinboxdelegate.h"

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

    //void SubscribeGameState();
    void UseMyFantasyName(QString);        
    //void SubscribeMyNameTx(QString);
    //void SubscribeMyProjTx(QString);
    //void SubscribeAwards(QString);
    //void SubscribePlayerData();
    //void SubscribeScheduleData();
    //void SubscribeTeamRoster();
    //void SubscribePlayerGameStatus();
    //void NewProjection(vector<fantasybit::FantasyBitProj>);
    void ClaimFantasyName(QString);

public slots:

    void on_myNextWeek_clicked(); 
    void on_myPreviousWeek_clicked();
    void GoLive(fantasybit::GlobalState state);
    void GlobalStateChange(fantasybit::GlobalState state);
    //void OnMyFantasyNames(vector<fantasybit::MyFantasyName> names);
    void OnNameStatus(fantasybit::MyFantasyName name);
    void OnProjAck(fantasybit::FantasyBitProj projection);
    //void OnAward(QVariant);
    void OnNameBalance(fantasybit::FantasyNameBal balance);
    void OnNewWeek(int);
    void OnGameOver(string);
    void OnGameStart(string);
    void OnPlayerStatusChange(pair<string, fantasybit::PlayerStatus> in);
    void onSendFantasyNameProjection(QString fantasyName);
    void leaderboardCliked(const QModelIndex & index);

private slots:
    //void on_myFantasyNamesCombo_currentIndexChanged(int index);
    void on_myClaimFantasyNameButton_clicked();
    void refreshLeaderBoard();
    void showLeaderboardContextualMenu(const QPoint & point);

private:
    void initialize();
    void initDefaultGuiDisplay();
    QString appTitle() const {
        return QString("%1 %2.%3.%4 %5").arg(APPLICATION_NAME)
                .arg(MAJOR_VERSION)
                .arg(MINOR_VERSION)
                .arg(REVISION_NUMBER)
                .arg(BUILD_TYPE);
    }
    void nextWeek();
    void previousWeek();
    void currentWeek();
    void navigateToWeek(int week);

    MainLAPIWorker *  myLAPIWorker;
    Ui::MainWindow *ui;    
    uint myCurrentWeek;
    fantasybit::MyFantasyName myCurrentFantasyName;
    GlobalState myGlobalState;
    WaitModalDialog myWaitDialog;
    bool myAddNamesPending= false;
    QTimer myLeaderBoardTimer; 
    bool myIamLive = false;       
};

#endif // MAINWINDOW_H
