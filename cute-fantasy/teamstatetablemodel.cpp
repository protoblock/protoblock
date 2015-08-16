#include "teamstatetablemodel.h"


TeamStateTableModel::TeamStateTableModel(): TKeyedListModel<QString,TeamStateViewModel>(NULL) {
    initialize();
}

TeamStateTableModel::~TeamStateTableModel() {}

int TeamStateTableModel::getTeamState(const QString & teamId) const {
    foreach(TeamStateViewModel * item,myList){
        if (item->teamId().trimmed().toUpper()== teamId.trimmed().toUpper()) {
            if (item->teamStateString() == "Team State Unknown") return -1;
            return (int) item->teamState();
        }
        continue;
    }
    return -1;
}

void TeamStateTableModel::initialize(){
    QStringList  headers;
    headers << "NFL Team"<< "Status"<<"Week";
    setHorizontalHeaders(headers);
}

QVariant TeamStateTableModel::getColumnDisplayData(quint32 column,TeamStateViewModel * data){
    if (data==NULL) return QVariant();
    if( column ==2)
        return data->week();
    if( column ==0)
        return data->teamId();
    if( column ==1)
        return data->teamStateString();
    return QVariant();

}

int TeamStateTableModel::getColumnCount(){
    return 3;
}
