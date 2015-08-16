#include "teamdatatablemodel.h"

TeamDataTableModel::TeamDataTableModel(): TKeyedListModel<QString,TeamDataViewModel>(NULL) {
  initialize();
}
TeamDataTableModel::~TeamDataTableModel() {}

void TeamDataTableModel::initialize(){
  QStringList  headers;
  headers << "Team Id";
  setHorizontalHeaders(headers);
}

QVariant TeamDataTableModel::getColumnDisplayData(quint32 column,TeamDataViewModel * data){
  if (data==NULL) return QVariant();
  if( column ==0)
    return data->teamId();
  return QVariant();

}
int TeamDataTableModel::getColumnCount(){
  return 1;
}

