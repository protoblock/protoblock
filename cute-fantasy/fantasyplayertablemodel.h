#ifndef FANTASYPLAYERTABLEMODEL_H
#define FANTASYPLAYERTABLEMODEL_H

#include "tkeyedlist.h"
#include "fantasyplayerviewmodel.h"
#include <QString>

class FantasyPlayerTableModel : public TKeyedListModel<QString,FantasyPlayerViewModel> {

public:

    FantasyPlayerTableModel();
    ~FantasyPlayerTableModel();
    void initialize();

protected:

    QVariant getColumnDisplayData(quint32 column,FantasyPlayerViewModel * data);
    int getColumnCount();
};

#endif // FANTASYPLAYERTABLEMODEL_H
