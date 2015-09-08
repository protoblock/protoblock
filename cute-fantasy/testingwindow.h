#ifndef TESTINGWINDOW_H
#define TESTINGWINDOW_H

#include <QMainWindow>
#include "ProtoData.pb.h"
#include <unordered_map>
#include "Data.h"

namespace Ui {
class TestingWindow;
}
using fantasybit::GlobalState;
using namespace fantasybit;
using namespace std;

class MainLAPIWorker;
class TestingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestingWindow(QWidget *parent = 0);
    ~TestingWindow();

    void initialize();

    QTimer * timer;

signals:
    void UseMyFantasyName(QString);
    void BeOracle();
    void ClaimFantasyName(QString);

public slots:
    void GoLive(fantasybit::GlobalState);
    void OnNameStatus(fantasybit::MyFantasyName);
    //void OnMyFantasyNames(vector<fantasybit::MyFantasyName> );
    void OnNewWeek(int);
    void Timer();
private slots:
    void on_beoracle_clicked();

    void on_weeks_activated(int index);

    void on_SendBlock_clicked();

    void on_claimname_clicked();

    void on_GetTx_clicked();

    //void on_game_currentIndexChanged(int index);

    void on_game_activated(const QString &arg1);

    //void on_game_activated(const QString &arg1);

    void on_player_activated(const QString &arg1);

    void on_team_activated(const QString &arg1);

    void on_GetGameResult_clicked();

    void on_updatelb_clicked();

    void on_rundataagent_toggled(bool checked);

    void on_StageBlock_clicked();

    void on_nmeonic_clicked();

    void on_MsgButton_clicked();

    void on_Update_PLayers_clicked();

private:
    MainLAPIWorker *  myCoreInstance;
    std::unordered_map<string,GameInfo> mGames;
    std::unordered_map<string,GameResult> mGameResult;
    std::unordered_map<string,GameRoster> mGameRoster;
    std::unordered_map<string,PlayerDetail> mPlayerDetail;
    std::vector<PlayerData> myPlayerData;
    std::unordered_map<string,GameResult> mStagedGameResult;
    int TestingWindow::randomNum(int num) ;
        GameResult TestingWindow::fakeit(GameInfo &g);

    MessageData myMessageData;
    std::unordered_map<string,::google::protobuf::RepeatedPtrField< ::fantasybit::FantasyBitAward>>
    mRewards;

    std::unordered_map<string,PlayerResult>
    mResult;

    std::string mStagedBlock;
    int mStagedBlockNum;
    int realweek() ;

    bool sendStageBlock() ;
    bool makeStageBlock(DataTransition &dt);


    Ui::TestingWindow *ui;
};

#endif // TESTINGWINDOW_H
