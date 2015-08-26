#include "fantasyplayerviewmodel.h"


FantasyPlayerViewModel::FantasyPlayerViewModel(){}
FantasyPlayerViewModel::~FantasyPlayerViewModel(){}

FantasyPlayerViewModel::FantasyPlayerViewModel(const FantasyPlayerViewModel & copy){
    myPlayerName = copy.myPlayerName;
    myBits = copy.myBits;
}

FantasyPlayerViewModel::FantasyPlayerViewModel(const ::fantasybit::FantasyPlayer & copy) {
    myPlayerName = QString::fromStdString(copy.name());
    myBits = copy.bits();
}

FantasyPlayerViewModel::FantasyPlayerViewModel(const QString & playerName,quint64 bits) {
    myPlayerName = playerName;
    myBits = bits;
}

bool FantasyPlayerViewModel::operator ==(const FantasyPlayerViewModel &other) const{
    return (other.myPlayerName.toLower() == myPlayerName.toLower() &&
            other.myBits == myBits);
}

QString  FantasyPlayerViewModel::playerName() const { return myPlayerName; }
quint64  FantasyPlayerViewModel::bits() const { return myBits; }

void FantasyPlayerViewModel::setBits(quint64 bits){ myBits = bits;}
void FantasyPlayerViewModel::setFantasyName(const QString & name){ myPlayerName = name; }



