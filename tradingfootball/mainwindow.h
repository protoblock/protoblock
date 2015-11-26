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
    void OnMyFantasyNames(vector<fantasybit::MyFantasyName> names);
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
    void on_myFantasyNamesCombo_currentIndexChanged(int index);
    void on_myClaimFantasyNameButton_clicked();
    void refreshLeaderBoard();
    void showLeaderboardContextualMenu(const QPoint & point);

    void myFantasyNamesImportExport(const QPoint &pos);

    void on_actionFantasyName_Import_Export_triggered();

    void on_toolButton_clicked();

    //void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_valueChanged(int value);

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
    void setCurrentFantasyName(fantasybit::MyFantasyName * fantasyName,bool useName);
    void MainWindow::doImportExport();

	QString translateNameStatus(fantasybit::MyNameStatus status) const {
		switch (status) {
		case fantasybit::confirmed:
			return QString("Confirmed");			
		case fantasybit::requested:
			return QString("Requested");			
		case fantasybit::notavil:
			return QString("Not Available");
		case fantasybit::transaction_sent:
			return QString("Transaction sent");			
		case fantasybit::none:
			return QString("none");			
		}
	}

    bool changingSlider = false;
    bool sliderPressed = false;
    bool sliderReleased = false;

    MainLAPIWorker *  myLAPIWorker;
    Ui::MainWindow *ui;    
    uint myCurrentWeek;
    fantasybit::MyFantasyName myCurrentFantasyName;
    GlobalState myGlobalState;
    WaitModalDialog myWaitDialog;
    bool myAddNamesPending= false;
    QTimer myLeaderBoardTimer; 
    bool myIamLive = false;    
	QMap<QString, fantasybit::MyFantasyName *> myFantasyNames;
    void setSlider(bool position);
    bool sliderright = true;
};

#endif // MAINWINDOW_H
