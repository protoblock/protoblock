#ifndef GAMEPROJECTIONMODELVIEW_H
#define GAMEPROJECTIONMODELVIEW_H

#include "viewmodels.h"
#include <QFlags>
#include <QMetaType>

class GameProjectionModelView :public Descriptable, public Decorable{


public:

    /**
     * @brief The ScoreState enum : reflect the score state in the ui.
     */
    enum ScoreState {
        NonScored, //score not set by the user (initial state).
        Scored, //user sets a score but didn't marked for sending
        ReadyToSend,// user set the score and marked the score to be sent for
        // next send projection action. User can revert the score state
        //
        Sent //score sent previously during the user session.

    };


    /**
     * @brief The ProjectionFlag enum :
     */
    enum ProjectionFlag {
        ProjectionNormalState = 0x0,
        PlayerJustSwitchedTeam = 0x1,
        PlayerInjured = 0x2,
        PlayerAbsent = 0x4,
        GameStarted = 0x8
    };

    Q_DECLARE_FLAGS(ProjectionFlags, ProjectionFlag)

    void setProjectionFlag( ProjectionFlag flag, bool on );

    bool testProjectionFlag(ProjectionFlag flag) const;



    ScoreState myScoreState = NonScored;
    QString myTeamId;
    QString myPlayerId;
    QString myPlayerName;
    QString myPos;
    int myScore;


    GameProjectionModelView();
    GameProjectionModelView(const QString & teamId,
                            const QString & playerName,
                            const QString & pos,
                            const QString & playerId,
                            int score=0);
    GameProjectionModelView(GameProjectionModelView & copy);

    void loadPlayerInfo();

private:
    ProjectionFlags myProjectionFlag;
    bool myPlayerInfoLoaded = false;
};

Q_DECLARE_METATYPE(GameProjectionModelView*)
Q_DECLARE_OPERATORS_FOR_FLAGS(GameProjectionModelView::ProjectionFlags)

#endif // GAMEPROJECTIONMODELVIEW_H
