#include "fantasyplayertablemodel.h"

FantasyPlayerTableModel::FantasyPlayerTableModel(): TKeyedListModel<QString,FantasyPlayerViewModel>(NULL) {
    initialize();
}

FantasyPlayerTableModel::~FantasyPlayerTableModel() {}

void FantasyPlayerTableModel::initialize(){
    QStringList  headers;
    headers << "FantasyName" << "FantsyBits";
    setHorizontalHeaders(headers);
}

QVariant FantasyPlayerTableModel::getColumnDisplayData(quint32 column,FantasyPlayerViewModel * data){
    if (data==NULL) return QVariant();
    if( column ==0)
        return data->playerName();
    if( column ==1)
        return data->bits();
    return QVariant();

}

int FantasyPlayerTableModel::getColumnCount(){
    return 2;
}
