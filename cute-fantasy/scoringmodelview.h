#ifndef SCORINGMODELVIEW_H
#define SCORINGMODELVIEW_H

#include "viewmodels.h"

class ScoringModelView :public Descriptable, public Decorable{
public:
    int myScore;
    QString myTeamId;
    QString myTeamName;
    QString myPlayerId;
    ScoringModelView();
    ~ScoringModelView();
    ScoringModelView(const QString & teamId,
                     const QString & teamName,
                     const QString & playerId,int score=0);
    ScoringModelView(ScoringModelView & copy);
};

Q_DECLARE_METATYPE(ScoringModelView*)


#endif // SCORINGMODELVIEW_H
