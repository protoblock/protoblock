#ifndef TEAMDATATABLEMODEL_H
#define TEAMDATATABLEMODEL_H

#include "tkeyedlist.h"
#include "teamdataviewmodel.h"

class TeamDataTableModel : public TKeyedListModel<QString,TeamDataViewModel> {

public:
  TeamDataTableModel();
  ~TeamDataTableModel();

  void initialize();
protected:
  QVariant getColumnDisplayData(quint32 column,TeamDataViewModel * data);
  int getColumnCount();
};

#endif // TEAMDATATABLEMODEL_H
