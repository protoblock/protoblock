#include "scoringtablemodel.h"

ScoringTableModel::ScoringTableModel(): TListModel<ScoringModelView>(NULL) {
    initialize();
}
ScoringTableModel::~ScoringTableModel() {}

void ScoringTableModel::initialize(){
    QStringList  headers;
    headers << "Team" << "PlayerId" << "Proj/Result";
    setHorizontalHeaders(headers);
}


QVariant ScoringTableModel::getColumnDisplayData(quint32 column,ScoringModelView * data){
    if (data==NULL) return QVariant();
    if( column ==0)
        return data->myTeamName;
    if( column ==1)
        return data->myPlayerId;
    if( column ==2)
        return data->myScore;
    return QVariant();

}
int ScoringTableModel::getColumnCount(){
    return 3;
}
void ScoringTableModel::setDataFromColumn(ScoringModelView* data, const QModelIndex &index,const QVariant &vvalue,int role){
    if (index.column() >= getColumnCount()) return;
    //only allow edition of the score
    if (index.column()==2)
        data->myScore = vvalue.toInt();
}
