#ifndef FANTASYPLAYERVIEWMODEL_H
#define FANTASYPLAYERVIEWMODEL_H

#include "viewmodels.h"

class FantasyPlayerViewModel : public Descriptable, public Decorable {
    QString myPlayerName;
    quint64 myBits;

public:

    FantasyPlayerViewModel();
    ~FantasyPlayerViewModel();

    FantasyPlayerViewModel(const FantasyPlayerViewModel & copy);

    FantasyPlayerViewModel(const ::fantasybit::FantasyPlayer & copy);

    FantasyPlayerViewModel(const QString & playerName,quint64 bits);

    bool operator ==(const FantasyPlayerViewModel &other) const;

    QString  playerName() const;
    quint64  bits() const;

    void setBits(quint64 bits);
    void setFantasyName(const QString & name);
};

Q_DECLARE_METATYPE(FantasyPlayerViewModel*)


#endif // FANTASYPLAYERVIEWMODEL_H
