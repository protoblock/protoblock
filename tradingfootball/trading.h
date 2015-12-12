#ifndef TRADING_H
#define TRADING_H

#include "ProtoData.pb.h"
#include <QWidget>
#include "LAPIWorker.h"
#include "tablemodels.h"
#include "dataservice.h"
#include "julylightchanges.h"

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

        if (role != Qt::DisplayRole)
            return QVariant();

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

       if ( current && current == &it->second) {
           if ( datachangeddbot > 0 && rowCount() > datachangeddbot)
               datachangeddbot = rowCount();


           int deltarow = rowCount() - prevrow;
           if ( deltarow > 0) {
                insertRows(0,deltarow);
           }
           else if ( deltarow < 0)
               removeRows(rowCount(),-deltarow);


           if ( datachangeddbot > 0 )
                allDataChanged(datachangeddbot);
       }

    }

    void onDelta(DepthFeedDelta *dfd) {
        auto it = symbolDepth.find(dfd->symbol());
        if ( it == end(symbolDepth)) {
            auto it2 = symbolDepth.insert(make_pair(dfd->symbol(),std::vector<BookItem>()));
            if ( !it2.second )
                return;

            it = it2.first;
        }

        std::vector<BookItem> &book = it->second;
        int prevrows = 0;
        if ( current == &it->second) {
            prevrows = rowCount();
        }
        if ( dfd->isbid() )
        {
            bool nopush = false;
            if ( book.size() > 0 ) {
                if ( book[book.size()-1].b == 0)
                    nopush = true;;
            }

            for ( int i =0; i<book.size(); i++) {
                if ( book[i].b == 0 ) {
                    if ( dfd->size() > 0 ) {
                        book[i].b = dfd->price();
                        book[i].bs = dfd->size();
                    }
                    break;
                }
                else if ( dfd->price() < book[i].b)  {
                    if ( i < book.size()-1)
                        continue;

                    if ( dfd->size() > 0) {
                        BookItem bi;
                        bi.a = bi.as = 0;
                        bi.b = dfd->price();
                        bi.bs = dfd->size();
                        book.push_back(bi);
                    }
                    break;
                }
                else if ( dfd->price() > book[i].b ) {
                    if ( dfd->size() > 0) {
                        int end = book.size()-1;
                        if ( nopush )
                            ;//end = book.size()-1;
                        else {
                            //end = book.size()-2;
                            BookItem bi;
                            bi.a = bi.as = 0;
                            bi.b = book[book.size()-1].b;
                            bi.bs = book[book.size()-1].bs;
                            book.push_back(bi);
                        }
                        for (int j=end;j > i;--j) {
                            if ( nopush && book[j-1].b != 0 )
                                nopush = false;

                            if ( !nopush ) {
                                book[j].b = book[j-1].b;
                                book[j].bs = book[j-1].bs;
                            }
                        }

                        book[i].b = dfd->price();
                        book[i].bs = dfd->size();
                    }

                    break;
                }
                else {
                    if ( dfd->size() > 0 )
                        book[i].bs = dfd->size();
                    else {
                        int j=i;
                        for (;
                              j<book.size()-1 && book[j].b > 0;
                              ++j) {
                            book[j].b = book[j+1].b;
                            book[j].bs = book[j+1].bs;
                        }
                        book[j].b = book[j].bs = 0;
                    }

                    break;
                }
            }
        }
        else //!isbid
        {
            bool nopush = false;
            if ( book.size() > 0 ) {
                if ( book[book.size()-1].a == 0)
                    nopush = true;
            }

            for ( int i =0; i<book.size(); i++) {
                if ( book[i].a == 0 ) {
                    if ( dfd->size() > 0 ) {
                        book[i].a = dfd->price();
                        book[i].as = dfd->size();
                    }
                    break;
                }
                else if ( dfd->price() > book[i].a)  {
                    if ( i < book.size()-1)
                        continue;

                    if ( dfd->size() > 0) {
                        BookItem bi;
                        bi.b = bi.bs = 0;
                        bi.a = dfd->price();
                        bi.as = dfd->size();
                        book.push_back(bi);
                    }
                    break;
                }
                else if ( dfd->price() < book[i].a ) {
                    if ( dfd->size() > 0) {
                        int end = book.size()-1;
                        if ( nopush )
                            ;//end = dfd->size()-1;
                        else {
                            //end = dfd->size()-2;
                            BookItem bi;
                            bi.b = bi.bs = 0;
                            bi.a = book[book.size()-1].a;
                            bi.as = book[book.size()-1].as;
                            book.push_back(bi);
                        }
                        for (int j=end;j > i;--j) {
                            if ( nopush && book[j-1].a != 0 )
                                nopush = false;

                            if ( !nopush ) {
                                book[j].a = book[j-1].a;
                                book[j].as = book[j-1].as;
                            }
                        }

                        book[i].a = dfd->price();
                        book[i].as = dfd->size();
                    }
                    break;
                }
                else {
                    if ( dfd->size() > 0 )
                        book[i].as = dfd->size();
                    else {
                        int j=i;
                        for (;
                              j<book.size()-1 && book[j].a > 0;
                              ++j) {
                           book[j].a = book[j+1].a;
                           book[j].as = book[j+1].as;
                        }
                        book[j].a =
                        book[j].as = 0;
                    }
                    break;
                }
            }
        }

        if (book.size() == 0 && dfd->size() >0 && dfd->price() > 0) {
            BookItem bi;
            if ( dfd->isbid() ) {
                bi.a = bi.as = 0;
                bi.b = dfd->price();
                bi.bs = dfd->size();
            }
            else {
                bi.b = bi.bs = 0;
                bi.a = dfd->price();
                bi.as = dfd->size();
            }
            book.push_back(bi);
        }

        if ( current == &it->second) {
            int deltarow = rowCount() - prevrows;
            if ( deltarow > 0) {
                insertRows(0,deltarow);
            }
            else if ( deltarow < 0)
                removeRows(rowCount(),-deltarow);

            allDataChanged(max(rowCount()-1,prevrows-1));
        }

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
        else if ( deltarow < 0)
            removeRows(rowCount(),-deltarow);

        allDataChanged(max(rowCount()-1,prevrows-1));
    }

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) {
     // if (count < 1 || row < 0 || row > rowCount(parent))
     //   return false;

      beginInsertRows(QModelIndex(), row, row + count - 1);
      endInsertRows();

      return true;
    }

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) {
     // if (count <= 0 || row < 0 || (row + count) > rowCount(parent))
     //   return false;

      beginRemoveRows(QModelIndex(), row, row + count - 1);
      endRemoveRows();

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
    string myFantasyName;
    string myPositionsName;
    void SetFantasyName(std::string name);

    void SetMyPositions();
private slots:
    void on_buyit_clicked();

    void on_sellit_clicked();

    void UpdateBuys(int arg1);

    void UpdateSells(int arg1);

    void on_buyPriceAsMarketAsk_clicked();

    void on_buyPriceAsMarketBid_clicked();

    void on_buyPriceAsMarketLastPrice_clicked();

    void on_sellPriceAsMarketLastPrice_clicked();

    void on_sellPriceAsMarketAsk_clicked();

    void on_sellPriceAsMarketBid_clicked();

    void on_pushButton_clicked();

public slots:
    void OnMarketTicker(fantasybit::MarketTicker *);
    void OnMarketSnapShot(fantasybit::MarketSnapshot*);
    void OnDepthDelta(fantasybit::DepthFeedDelta*);
    void OnTradeTick(fantasybit::TradeTic*);
    void OnMyNewOrder(fantasybit::Order& ord) {
        qDebug() << "level2 OnMyNewOrder MyOrder New" << ord.DebugString();
    }


    void OnLive(bool subscribe) {
#ifdef TRACE
    qDebug() << "level2 Trading OnLive";
#endif

        auto st = DataService::instance()->GetGlobalState();
        SetCurrentWeekData(st.week());
        if ( myFantasyName != "" && myPositionsName != myFantasyName)
            SetMyPositions();
    }
    void playerListCliked(const QModelIndex &index);


signals:
    void SendOrder(fantasybit::ExchangeOrder);
    //void SendOrder();

private:
    Ui::Trading *ui;
    DepthTablesModel mDepthTableModel;
    PlayerListModal mPlayerListModel;
    OrderTablesModel mOrderTableModel;

    std::vector<JulyLightChanges *> mJLC;
    bool mUpdatingS = false;
    bool mUpdatingB = false;

};




#endif // TRADING_H
