#ifndef FANTASYPLAYERUI_H
#define FANTASYPLAYERUI_H

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
#include "scheduleloader.h"

namespace Ui {
class FantasyPlayerUI;
}

class FantasyPlayerUI : public QWidget
{
    Q_OBJECT
    enum State { CONNECTING, LIVE };
    InData indata{};
    bool first = true;
public:
    explicit FantasyPlayerUI(QWidget *parent = 0);
    ~FantasyPlayerUI();
public slots:
    void fromServer(const DeltaData &in);
    void refreshViews(const DeltaData &in);
signals:
    void onClose();
    void fromGUI(const InData &);

protected:
    virtual void closeEvent(QCloseEvent *){ emit onClose();}

private slots:
    void on_mySendProjectionsButton_clicked();

    void on_myDeleteAllRowsButton_clicked();

    void on_myAddScoringLineButton_clicked();

    void on_generate_clicked();

    void on_copy_clicked();

    void on_myTeamsCmb_currentIndexChanged(int index);

private:
    Ui::FantasyPlayerUI *ui;
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
    QMultiMap<int,ScheduleLoader::JsonSchedule> myPreloadedSchedule;
    QList<QString> myTeamTransitions;
    SpinBoxDelegate myDelegate;
};

#endif // FANTASYPLAYERUI_H
