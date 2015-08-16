//
//  sfgui.h
//  cute-fantasy
//
//  Created by Jay Berg on 4/8/14.
//
//
#ifndef SFGUI_H
#define SFGUI_H

#include <QWidget>
#include <ProtoData.pb.h>
#include <QThread>
#include <QString>
#include "snapshotviewmodel.h"
#include "teamstatetablemodel.h"
#include "playerdatatablemodel.h"
#include "teamdatatablemodel.h"
#include "fantasyplayertablemodel.h"
#include "scoringtablemodel.h"
#include "playerloader.h"
#include "spinboxdelegate.h"
#include "teamsloader.h"

namespace Ui {
class sfGUI;
}


class sfGUI : public QWidget
{
    Q_OBJECT

    enum State { CONNECTING, LIVE };
    //MyNameStatus m_status;
    //State m_state;
    //MyFantasyName m_namestatus{};
    InData indata{};
    //DeltaData snapData{}, deltaData{};
    bool first = true;
public:
    explicit sfGUI(QWidget *parent = 0);
    ~sfGUI();

public slots:
    void fromServer(const DeltaData &in);// {}
    void refreshViews(const DeltaData &in);

    //void fromServer(const fantasybit::OutData &in);
private slots:


    void on_generate_clicked();

    void on_copy_clicked();
    //void flashing();


    void on_myTeamsCmb_currentIndexChanged(int index);

    void on_myAddScoringLineButton_clicked();

    void on_mySendProjectionsButton_clicked();

    void on_mySendResultsButton_clicked();

    void on_myDeleteAllRowsButton_clicked();

    void on_myAddTeam_clicked();

protected:
    virtual void closeEvent(QCloseEvent *)
    {
        emit onClose();
    }

signals:
    void onClose();
    void fromGUI(const InData &);
    //void on_flash();

private:
    Ui::sfGUI *ui;

    void updatesnap();
    void updatedelta();
    /**
     * @brief myCurrentSnapShot : contains last snapshot data
     */
    SnapShotViewModel myCurrentSnapShot;

    //the following models will hold data shown by the table views.
    TeamStateTableModel myTeamsStateTableModel;
    PlayerDataTableModel myPlayerDataTableModel;
    TeamDataTableModel myTeamDataTableModel;
    FantasyPlayerTableModel myFantasyPlayerTableModel;
    ScoringTableModel myScoringTableModel;
    QList<TeamLoader::JsonTeam> myPreloadedTeams;
    QList<PlayerLoader::JsonPlayer> myPreloadedPlayers;
    QList<QString> myTeamTransitions;
    SpinBoxDelegate myDelegate;
    QMutex myMutex;


};

#endif // SFGUI_H
