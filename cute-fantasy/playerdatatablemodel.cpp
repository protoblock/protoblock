#include "playerdatatablemodel.h"

PlayerDataTableModel::PlayerDataTableModel(): TKeyedListModel<QString,PlayerDataViewModel>(NULL) {
  initialize();
}
PlayerDataTableModel::~PlayerDataTableModel() {}

void PlayerDataTableModel::initialize(){
  QStringList  headers;
  headers << "Team Id"<< "Player Id";
  setHorizontalHeaders(headers);
}

QVariant PlayerDataTableModel::getColumnDisplayData(quint32 column,PlayerDataViewModel * data){
  if (data==NULL) return QVariant();
  if( column ==0)
    return data->teamId();
  if( column ==1)
    return data->playerId();
  return QVariant();

}
int PlayerDataTableModel::getColumnCount(){
  return 2;
}
