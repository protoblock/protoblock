#ifndef GAMEPROJECTIONTABLEMODEL_H
#define GAMEPROJECTIONTABLEMODEL_H

#include "tkeyedlist.h"
#include "gameprojectionmodelview.h"

class GameProjectionTableModel : public TKeyedListModel<QString,GameProjectionModelView> {

public:

    GameProjectionTableModel();
    ~GameProjectionTableModel() ;
    void initialize();

protected:

    QVariant getColumnDisplayData(quint32 column,GameProjectionModelView * data);
    QVariant getColumnDecorateData(uint column, GameProjectionModelView *value);
    int getColumnCount();
    void setDataFromColumn(GameProjectionModelView * data, const QModelIndex & index, const QVariant &vvalue, int role);
};

#endif // GAMEPROJECTIONTABLEMODEL_H
