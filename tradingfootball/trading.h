#ifndef TRADING_H
#define TRADING_H

#include "ProtoData.pb.h"
#include <QWidget>
#include "LAPIWorker.h"
#include "tablemodels.h"
#include "dataservice.h"

class DepthTablesModel : public QAbstractTableModel {
    struct BookItem {
        int bs = 0;
        int b = 0;
        int a = 0;
        int as = 0;
    };

public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const {
        if (orientation != Qt::Horizontal) return QVariant();
        if (role != Qt::DisplayRole) return QVariant();
        switch (section) {
        case 0:
            return "Bid Size";
        case 1:
            return "Bid";
        case 2:
            return "Ask";
        case 3:
            return "Ask Size";
        }
        return QVariant();
    }

    int   rowCount(const QModelIndex & parent = QModelIndex() ) const {
        if ( !current ) return 0;
        return current->size();
    }
    int   columnCount(const QModelIndex & parent = QModelIndex() ) const {
        Q_UNUSED(parent);
        //if ( !current ) return 0;
        return 4;
    }

    QVariant   data(const QModelIndex & index, int role = Qt::DisplayRole ) const {

        if ( current == nullptr)
            return QVariant();

        if ( index.row() > current->size())
            return QVariant();

        int ret = 0;
        switch ( index.column() ) {
            case 0:
                ret = current->at(index.row()).bs;
            break;
            case 1:
                ret = current->at(index.row()).b;
            break;
            case 2:
                ret = current->at(index.row()).a;
            break;
            case 3:
                ret = current->at(index.row()).as;
            break;
            default:
            break;
        }

        if ( ret == 0 )
            return QVariant();

        return ret;
    }

    std::vector<BookItem> *current = nullptr;
    std::unordered_map<string,std::vector<BookItem>> symbolDepth;

    void addSnap(MarketSnapshot *ms) {
       if ( ms->depth_size() <= 0) return;

       int datachangeddbot = 0;
       int prevrow = rowCount();
       auto it = symbolDepth.find(ms->symbol());
       if ( it == end(symbolDepth)) {
           auto it2 = symbolDepth.insert(make_pair(ms->symbol(),std::vector<BookItem>()));
           if ( !it2.second )
               return;

           it = it2.first;
       }
       else {
           if ( current == &it->second)
                datachangeddbot = rowCount();

           it->second.clear();
       }

       for ( int i = 0; i < ms->depth_size(); i++) {
           auto d = ms->depth(i);
           BookItem bi;
           bi.a = d.a();
           bi.b = d.b();
           bi.as = d.as();
           bi.bs = d.bs();
           it->second.push_back(bi);
       }

       if ( !current ) {
           //current = &it->second;
           //datachangeddbot = rowCount();
       }
       else {
           if ( datachangeddbot > 0 && rowCount() > datachangeddbot)
               datachangeddbot = rowCount();
       }

       int deltarow = rowCount() - prevrow;
       if ( deltarow > 0) {
            insertRows(0,deltarow);
       }

       if ( datachangeddbot > 0 )
            allDataChanged(datachangeddbot);
    }


    void changeSymbol(const string &syb) {
        int prevrows = rowCount();

        auto it = symbolDepth.find(syb);
        if ( it == end(symbolDepth)) {
            auto it2 = symbolDepth.insert(make_pair(syb,std::vector<BookItem>()));
            if ( !it2.second )
                return;

            it = it2.first;
        }

        if ( !current ) {
            current = &it->second;
            //insertColumns(0,4);
        }
        else if ( current != &it->second) {
            current = &it->second;
        }

        int deltarow = rowCount() - prevrows;
        if ( deltarow > 0) {
            insertRows(0,deltarow);
        }



        allDataChanged(max(rowCount()-1,prevrows-1));
    }

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) {
      if (count < 1 || row < 0 || row > rowCount(parent))
        return false;

      beginInsertRows(QModelIndex(), row, row + count - 1);

      endInsertRows();

      return true;
    }

    bool setData(const QModelIndex & inindex, const QVariant & value, int role = Qt::EditRole) {
        emit dataChanged(index(0,0),index(1,1));
        return true;
    }

    void allDataChanged(int botright) {
        emit dataChanged(index(0,0),index(botright,3));
    }
};

namespace Ui {
class Trading;
}

class Trading : public QWidget
{
    Q_OBJECT

public:
    explicit Trading(QWidget *parent = 0);
    ~Trading();

    void NewOrder(bool);
    MainLAPIWorker * theLAPIWorker;
    ExchangeOrder eo;
    void Init();

    static map<string,string> icons;
    void SetCurrentWeekData(int week);
    string myPlayerid;
    int mCurrentWeek = 0;
private slots:
    void on_buyit_clicked();

    void on_sellit_clicked();

public slots:
    void OnMarketTicker(fantasybit::MarketTicker *);
    void OnMarketSnapShot(fantasybit::MarketSnapshot*);
    void OnLive(bool subscribe) {
        auto st = DataService::instance()->GetGlobalState();

        SetCurrentWeekData(st.week());
    }
    void playerListCliked(const QModelIndex &index);


signals:
    void SendOrder(fantasybit::ExchangeOrder);
    //void SendOrder();

private:
    Ui::Trading *ui;
    DepthTablesModel mDepthTableModel;
    PlayerListModal mPlayerListModel;
};




#endif // TRADING_H
