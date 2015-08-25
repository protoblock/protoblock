#ifndef GAMEPROJECTIONMODELVIEW_H
#define GAMEPROJECTIONMODELVIEW_H

#include "viewmodels.h"

class GameProjectionModelView :public Descriptable, public Decorable {

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

    ScoreState myScoreState = NonScored;
    PlayerStatus::Status myPlayerTeamStatus;
    PlayerGameStatus myPlayerGameStatus;
    QString myTeamId;
    QString myPlayerId;
    QString myPlayerFirstName;
    QString myPlayerLastName;
    QString myPos;
    int myScore;

    GameProjectionModelView();
    ~GameProjectionModelView();
    GameProjectionModelView(const QString & teamId,
                            const QString & playerFirstName,
                            const QString & playerLastName,
                            const QString & pos,
                            const QString & playerId,
                            PlayerStatus::Status playerTeamStatus,
                            PlayerGameStatus playerGameStatus,
                            int score=0);
    GameProjectionModelView(const GameProjectionModelView & copy);

    void loadPlayerInfo();

    void setProjection(int projection){
        myScore = projection;
        myScoreState = Scored;
    }

private:   
    bool myPlayerInfoLoaded = false;
};

Q_DECLARE_METATYPE(GameProjectionModelView*)

#endif // GAMEPROJECTIONMODELVIEW_H
