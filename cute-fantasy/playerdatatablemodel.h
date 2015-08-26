#ifndef PLAYERDATATABLEMODEL_H
#define PLAYERDATATABLEMODEL_H

#include "tkeyedlist.h"
#include "playerdataviewmodel.h"

class PlayerDataTableModel : public TKeyedListModel<QString,PlayerDataViewModel> {

public:
  PlayerDataTableModel();
  ~PlayerDataTableModel();

  void initialize();
protected:
  QVariant getColumnDisplayData(quint32 column,PlayerDataViewModel * data);
  int getColumnCount();
};

#endif // PLAYERDATATABLEMODEL_H
