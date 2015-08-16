#ifndef SCORINGTABLEMODEL_H
#define SCORINGTABLEMODEL_H

#include "tlistmodel.h"
#include "scoringmodelview.h"

class ScoringTableModel : public TListModel<ScoringModelView> {

public:
  ScoringTableModel();
  ~ScoringTableModel();

  void initialize();

protected:
  QVariant getColumnDisplayData(quint32 column,ScoringModelView * data);
  int getColumnCount();
  void setDataFromColumn(ScoringModelView* data, const QModelIndex &index,const QVariant &vvalue,int role);
};


#endif // SCORINGTABLEMODEL_H
