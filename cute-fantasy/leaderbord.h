#ifndef LEADERBORD_H
#define LEADERBORD_H

#include "viewmodels.h"

class LeaderBaordViewModel : public Descriptable, public Decorable {

public:

    LeaderBaordViewModel() {

    }
    LeaderBaordViewModel(LeaderBaordViewModel const & src) {
        *this = src;
    }
    ~LeaderBaordViewModel() {

    }

    LeaderBaordViewModel & operator=(const LeaderBaordViewModel & src) {
        if( this != &src){
            myFantasyName = src.myFantasyName;
            myBalance = src.myBalance;
        }
        return *this;
    }

    bool operator==(LeaderBaordViewModel const & rhs) {
        return rhs.FantasyName()==this->FantasyName();
    }

    //setters
    void setFantasyName(const QString & argFantasyNameValue) { myFantasyName = argFantasyNameValue;}

    void setBalance(int  argBalanceValue) { myBalance = argBalanceValue;}

    //getters
    QString FantasyName() const { return myFantasyName;}
    int Balance(){ return myBalance;}

private :

    QString myFantasyName;
    int myBalance;

};

Q_DECLARE_METATYPE(LeaderBaordViewModel*)

#include "tkeyedlist.h"

class   LeaderBaordViewModelTableModel : public TKeyedListModel<QString,LeaderBaordViewModel> {

public:

    LeaderBaordViewModelTableModel() : TKeyedListModel<QString,LeaderBaordViewModel>(NULL) {
        initialize();
    }

    ~LeaderBaordViewModelTableModel() {}

protected:

    QVariant getColumnDisplayData(quint32 column,LeaderBaordViewModel * data) {
        if (data==NULL) return QVariant();
        if( column ==0)
            return data->FantasyName();
        if( column ==1)
            return QString("%1").arg(data->Balance());
        return QVariant();

    }

    int getColumnCount() {
        return 2;
    }

private:

    void initialize() {
        QStringList  headers;
        headers << "FantasyName";
        headers << "Balance";
        setHorizontalHeaders(headers);
    }
};

#endif // LEADERBORD_H
