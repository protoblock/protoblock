#ifndef DATAAGENTUI_H
#define DATAAGENTUI_H

#include <QWidget>
#include <QThread>
#include <QString>

#include <ProtoData.pb.h>

#include "teamsloader.h"
#include "playerloader.h"
#include "spinboxdelegate.h"
#include "snapshotviewmodel.h"
#include "teamstatetablemodel.h"
#include "playerdatatablemodel.h"
#include "teamdatatablemodel.h"
#include "fantasyplayertablemodel.h"
#include "scoringtablemodel.h"


namespace Ui {
class DataAgentUI;
}



class DataAgentUI : public QWidget
{
    Q_OBJECT
    enum State { CONNECTING, LIVE };
    InData indata{};
    bool first = true;

public:
    explicit DataAgentUI(QWidget *parent = 0);
    ~DataAgentUI();

public slots:    
    void fromServer(const DeltaData &in);    
    void refreshViews(const DeltaData &in);

signals:       
    void fromGUI(const InData &);
    void onClose();

protected:
    virtual void closeEvent(QCloseEvent *){ emit onClose();}

private slots:
    void on_myAddTeam_clicked();
    void on_generate_clicked();
    void on_myAddScoringLineButton_clicked();
    void on_myDeleteAllRowsButton_clicked();
    void on_mySendResultsButton_clicked();
    void on_myTeamsCmb_currentIndexChanged(int index);
    void on_copy_clicked();

    void on_mySendResultsHack_clicked();

private:
    Ui::DataAgentUI *ui;
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
};

#endif // DATAAGENTUI_H
