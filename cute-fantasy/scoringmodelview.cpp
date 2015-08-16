#include "scoringmodelview.h"

ScoringModelView::ScoringModelView(){}
ScoringModelView::~ScoringModelView(){}
ScoringModelView::ScoringModelView(const QString & teamId,
                 const QString & teamName,
                 const QString & playerId,int score){
    myScore = score;
    myTeamId= teamId;
    myTeamName= teamName;
    myPlayerId= playerId;
}
ScoringModelView::ScoringModelView(ScoringModelView & copy){
    myScore = copy.myScore;
    myTeamId= copy.myTeamId;
    myTeamName= copy.myTeamName;
    myPlayerId= copy.myPlayerId;

}
