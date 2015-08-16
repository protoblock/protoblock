#ifndef TEAMSTATETABLEMODEL_H
#define TEAMSTATETABLEMODEL_H


#include "tkeyedlist.h"
#include "teamstateviewmodel.h"

class TeamStateTableModel : public TKeyedListModel<QString,TeamStateViewModel> {

public:
    TeamStateTableModel();
    ~TeamStateTableModel();

    /**
   * @brief getTeamState : returns -1 for unknown team state, the team state enum integer value otherwise.
   * @param teamId : the team we're looking for its state.
   * @return returns -1 for unknown team state, the team state enum integer value otherwise.
   */
    int getTeamState(const QString & teamId) const;

    void initialize();

protected:

    QVariant getColumnDisplayData(quint32 column,TeamStateViewModel * data);
    int getColumnCount();
};

#endif // TEAMSTATETABLEMODEL_H
