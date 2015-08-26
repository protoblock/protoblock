#include "gameprojectiontablemodel.h"

GameProjectionTableModel::GameProjectionTableModel(): TKeyedListModel<QString,GameProjectionModelView>(NULL) {
    initialize();
}

GameProjectionTableModel::~GameProjectionTableModel() {}

void GameProjectionTableModel::initialize(){
    QStringList  headers;
    headers << "Team" << "Player" << "Pos" << "Projection";
    setHorizontalHeaders(headers);
}

QVariant GameProjectionTableModel::getColumnDisplayData(quint32 column,GameProjectionModelView * data){
    if (data==NULL) return QVariant();
    //load player info once
    data->loadPlayerInfo();
    if( column ==0)
        return data->myTeamId;
    if( column ==1)
        return data->myPlayerFirstName + " "+ data->myPlayerLastName;
    if( column ==2)
        return data->myPos;
    if ( column == 3)
        return data->myScore;
    return QVariant();

}

QVariant GameProjectionTableModel::getColumnDecorateData(uint column, GameProjectionModelView *value){
    if (column !=3) return QVariant();
    if (value->myScore !=0 && value->myScoreState != GameProjectionModelView::Sent)
        return QPixmap(":/icons/scored.png");
    if (value->myScore !=0 && value->myScoreState == GameProjectionModelView::Sent)
        return QPixmap(":/icons/scoresent.png");
    return QVariant();
}

int GameProjectionTableModel::getColumnCount(){
    return 4;
}

void GameProjectionTableModel::setDataFromColumn(GameProjectionModelView * data, const QModelIndex & index, const QVariant &vvalue, int role){
    if (index.column() >= getColumnCount()) return;
    //only allow edition of the score
    if (index.column()==3)
        data->myScore = vvalue.toInt();
}
