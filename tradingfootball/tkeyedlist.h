// tkeyedlistmodel.h
// Written by: Moez Bouselmi
//
// This file contains a definition for the template class TKeyedList which can be used
// to quickly obtain an editable Model from a simple QList.
// The model can be also edited by using a simple calls as :
// void addItem(X item);
//   bool removeItem(X item);
// and get the model synchronized.
// The TKeyedList check wether the X is Descriptable (see descriptable.h)or Decorable
// (see decorable.h) instance to provide the views using the model information about
// these roles :
//
//   Qt::ToolTipRole
//   Qt::StatusTipRole
//   Qt::WhatsThisRole
//   Qt::DisplayRole

// usage constraint X must have a default constructor and a copy contructor and  assignement operator

#ifndef TKEYEDLISTMODEL_H
#define TKEYEDLISTMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QPixmap>
#include <QPoint>
#include "descriptable.h"
#include "decorable.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include "globals.h"



class TableColumnsDefinition {

public:

    TableColumnsDefinition(){}

    TableColumnsDefinition(const QStringList & colmunHeaders){
        for(int i=0;i<colmunHeaders.count();i++)
            myColumnsState.append(QPair<QString,bool>(colmunHeaders.at(i),true));
    }

    TableColumnsDefinition &operator=(const TableColumnsDefinition & other){
        myColumnsState.clear();
        myColumnsState = other.myColumnsState;
    }

    void resetToVisibleHeaders(const QStringList & colmunHeaders){
        myColumnsState.clear();
        for(int i=0;i<colmunHeaders.count();i++)
            myColumnsState.append(QPair<QString,bool>(colmunHeaders.at(i),true));
    }

    QStringList visibleHeaders()const {
        QStringList result;
        for(int i=0;i<myColumnsState.size();i++)
            if (myColumnsState[i].second)
                result << myColumnsState[i].first;
        return result;
    }

    int visibleColumnCount() const {
        uint i=0;
        for(int j=0;j<myColumnsState.size();j++)
            if (myColumnsState[j].second) i++;
        return i;
    }

    int columnCount(){
        return myColumnsState.size();
    }

    bool isColumnVisible(const QString & columnHeader){
		for (int j = 0; j < myColumnsState.size(); j++) {
			if (myColumnsState[j].first == columnHeader)
				return myColumnsState[j].second;
		}
		return false;
    }

    void setColumnVisible(const QString & columnHeader,bool on){
        for(int j=0;j<myColumnsState.size();j++)
            if (myColumnsState[j].first == columnHeader){
                myColumnsState[j].second = on;
            }
    }

    int columnVisibleOrder(const QString & columnHeader) const {
        int countVisibleColumns=-1;
        for(int j=0;j<myColumnsState.size();j++){
            if (myColumnsState[j].second) countVisibleColumns++;
			if (myColumnsState[j].first == columnHeader){
				if (myColumnsState[j].second)
					return countVisibleColumns;
				else
					return -1;
			}
        }
        return -1;
    }

    int columnOrder(const QString & columnHeader){
        int j;
        for(j=0;j<myColumnsState.size();j++){
            if (myColumnsState[j].first == columnHeader)
                return j;
        }
        return -1;
    }

    bool appendColumn(const QString & columnHeader,bool isVisible = true){
        //check if a column exist with the same header
        for(int j=0;j<myColumnsState.size();j++)
            if (myColumnsState[j].first == columnHeader)
                return false;

        QPair<QString,bool> column(columnHeader,isVisible);
        myColumnsState.append(column);
        return true;
    }

    bool insertColumn(const QString & columnHeader,uint position,bool isVisible = true){
        if (position> myColumnsState.size()) return false;
        //check if a column exist with the same header
        for(int j=0;j<myColumnsState.size();j++)
            if (myColumnsState[j].first == columnHeader)
                return false;

        QPair<QString,bool> column(columnHeader,isVisible);
        myColumnsState.insert(position,column);
        return true;
    }

    void removeColumn(const QString & columnHeader){
        uint i=0;
        while (i < myColumnsState.size()){
            if (myColumnsState[i].first == columnHeader) break;
            i++;
        }
        if (i < myColumnsState.size())
            myColumnsState.remove(i);
    }

    QString getVisibleColumnHeader(int column) const{
        int i = 0;
        while (i < myColumnsState.size()){
            QString header = myColumnsState[i].first;
            if (columnVisibleOrder(header) == column)
              return header;
            i++;
        }
        return "";
    }

    void clear(){
        myColumnsState.clear();
    }

private:
    QVector<QPair<QString,bool>> myColumnsState;
};


using namespace std;
//X must have a default constructor
template <typename TKey,class X>
class TKeyedListModel : public QAbstractTableModel {

public:
    explicit TKeyedListModel(QObject *parent =0, bool autoDelete = false)
        : QAbstractTableModel(parent)
    {
        myIsAutoDeleteModelElements = autoDelete;

    }

    ~TKeyedListModel(){
        if (myIsAutoDeleteModelElements){
            qDeleteAll(myList.begin(),myList.end());
            qDebug("TList items deleted");
        }
    }

    TKeyedListModel (const TKeyedListModel& copy,QObject *parent =0): QAbstractTableModel(parent){
        myList.append(copy.list());
        myKeyMap = QMap<TKey,X*>(copy.myKeyMap);
        myIsAutoDeleteModelElements = copy.myIsAutoDeleteModelElements;
        myEditableColumnsState = copy.myEditableColumnsState;
        myColumns = copy.myColumns;
    }

    TKeyedListModel<TKey,X> &operator=(const TKeyedListModel<TKey,X> & other) // copy assignment
    {
        if (this != &other) {
            //clear old elements
            myKeyMap = QMap<TKey,X*>(other.myKeyMap);
            removeAll();
            myList.append(other.list());
            myIsAutoDeleteModelElements = other.isAutoDelete();
            myEditableColumnsState = other.myEditableColumnsState;
            myColumns = other.myColumns;
        }
        return *this;
    }

    TKeyedListModel(QMap<TKey,X*> data, QObject *parent =0)
        : QAbstractTableModel(parent)
    {
        foreach(TKey key,data.keys()){
            myKeyMap.insert(key,data.value(key));
            myList.append(data.value(key));
        }

    }

    bool isAutoDelete() { return myIsAutoDeleteModelElements; }

    void setAutoDelete(bool on) { myIsAutoDeleteModelElements = on;}

    QStringList horizontalHeaders() const { return myColumns.visibleHeaders(); }

    void setEditable(int column,bool on) {
        myEditableColumnsState[column] = on;
    }

    void setBold(int column,bool on) {
        myBoldColumnsState[column] = on;
    }

    void setAlign(int column,Qt::AlignmentFlag a) {
        myAlignmentFlag[column] = a;
    }

    int   rowCount(const QModelIndex & parent = QModelIndex() ) const {
        Q_UNUSED(parent);
        return myList.size();
    }

    int   columnCount(const QModelIndex & parent = QModelIndex() ) const
    {
        Q_UNUSED(parent);
        TKeyedListModel<TKey,X> * me = (TKeyedListModel<TKey,X> *) this;
        return me->getColumnCount();
    }

    X *   getItemByIndex(const QModelIndex & index){
        if (!index.isValid())
            return NULL;

        if (index.row() >= myList.size() || index.row() < 0)
            return NULL;
        //TListModel<X> * me = (TListModel<X> *) this;
        return myList.at(index.row());
    }

    QVariant   data(const QModelIndex & index, int role = Qt::DisplayRole ) const {
        if (!index.isValid())
            return QVariant();

        if (index.row() >= myList.size() || index.row() < 0)
            return QVariant();
        TKeyedListModel<TKey,X> * me = (TKeyedListModel<TKey,X> *) this;

        if (role == Qt::DisplayRole) {
            X * value = myList.at(index.row());
            return me->getColumnDisplayData(index.column(),value);
        }

        if (role == Qt::UserRole) {
            X * value = myList.at(index.row());
            return me->getColumnSortData(index.column(),value);
        }

        if (role == Qt::ToolTipRole || role == Qt::StatusTipRole || role == Qt::WhatsThisRole) {
            X * value = myList.at(index.row());
            Descriptable * descriptable  = dynamic_cast<Descriptable *> (value);
            if (descriptable == 0)
                return QVariant();
            else
                return descriptable->description();
        }
        if (role == Qt::DecorationRole) {
            X  * value = myList.at(index.row());
            return me->getColumnDecorateData(index.column(),value);
        }

        if (role == Qt::EditRole) {
            X  * value = myList.at(index.row());
            return me->getColumnDisplayData(index.column(),value);
        }

        if ( role == Qt::FontRole ) {
            if ( myBoldColumnsState.value(index.column(),false) )
            {
                QFont boldFont;
                boldFont.setBold(true);
                return boldFont;
            }
        }

        if (role == Qt::TextAlignmentRole) {
            if ( myAlignmentFlag.contains(index.column()))
                return myAlignmentFlag.value(index.column());
        }
        return QVariant();
    }

    bool setData(const QModelIndex &index, const QVariant &vvalue, int role)
    {
        if (index.row() >= 0 && index.row() < myList.size()
                && (role == Qt::EditRole || role == Qt::DisplayRole)) {
            if (!index.isValid()) return false;
            //TKeyedListModel<TKey,X> * me = (TKeyedListModel<TKey,X> *) this;
            X * value = myList.at(index.row());
            if (value==NULL) return false;
            setDataFromColumn(value,index,vvalue,role);
            emit dataChanged(index, index);
            return true;
        }
        return false;
    }

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        if (!index.isValid())
            return QAbstractItemModel::flags(index);//TODO | Qt::ItemIsDropEnabled;

        if (myEditableColumnsState.value(index.column(),false))
            return QAbstractItemModel::flags(index)| Qt::ItemIsEditable;
        else
            return QAbstractItemModel::flags(index);// TODO implement Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    }

    bool insertRows(int row, int count, const QModelIndex &parent)
    {
        if (count < 1 || row < 0 || row > rowCount(parent))
            return false;

        beginInsertRows(QModelIndex(), row, row + count - 1);

        //    for (int r = 0; r < count; ++r)
        //      myList.insert(row, NULL);

        endInsertRows();

        return true;
    }

    bool removeRows(int row, int count, const QModelIndex &parent)
    {

        if (count <= 0 || row < 0 || (row + count) > rowCount(parent))
            return false;

        beginRemoveRows(QModelIndex(), row, row + count - 1);
        for (int r = 0; r < count; ++r)
            myList.removeAt(row);

        endRemoveRows();

        return true;
    }

    bool setItemValue(const TKey & key,const X & value){
        X* oldItem = myKeyMap.value(key,NULL);
        if (oldItem == NULL){
            //we're going to add the item to the list
            addItem(key,new X(value));
        }
        else {
            *oldItem = value;
            int row = myList.indexOf(oldItem);
            emit dataChanged(index(row,1),index(row,0));
        }
        return true;
    }

    template <typename PROPNAME>
    bool updateItemProperty(const TKey & key,const QVariant & value){
        X* oldItem = myKeyMap.value(key,NULL);
        if (oldItem == NULL){
            //we're going to add the item to the list
            X * newItem = new X();
            newItem->template attachProperty<PROPNAME>(value);
            addItem(key,newItem);
        }
        else {
            oldItem->template attachProperty<PROPNAME>(value);
            int row = myList.indexOf(oldItem);
            emit dataChanged(index(row,1),index(row,0));
        }
        return true;
    }

    bool updateItemProperty(const QString & propertyName,const TKey & key,const QVariant & value){
        X* oldItem = myKeyMap.value(key,NULL);
        if (oldItem == NULL){
            //we're going to add the item to the list
            X * newItem = new X();
            newItem->attachProperty(propertyName,value);
            addItem(key,newItem);
        }
        else {
            oldItem->attachProperty(propertyName,value);
            int row = myList.indexOf(oldItem);
            emit dataChanged(index(row,1),index(row,0));
        }
        return true;
    }

    template <typename PROPNAME>
    bool itemPropertyValue(const TKey & key,QVariant & value){
        X* item = myKeyMap.value(key,NULL);
        if (item == NULL){
            return false;
        }
        else {
            value = item->template propertyValue<PROPNAME>();
            return true;
        }
    }

    X * takeItem(const TKey & key){
        X * item = myKeyMap.value(key,NULL);
        if (item == NULL) return NULL;
        myKeyMap.remove(key);
        int row = myList.indexOf(item);
        removeRows(row,1,QModelIndex());
        return item;
    }

    X* itemByKey(const TKey & key){
        X * item = myKeyMap.value(key,NULL);
        return item;
    }

    void emiDataChanged(const TKey & key){
        X* item = myKeyMap.value(key,NULL);
        if (item == NULL) return;
        int row = myList.indexOf(item);
        emit dataChanged(index(row,1),index(row,0));
    }

    void removeAll(){
        foreach(const TKey & key , myKeyMap.keys()){
            X* item = takeItem(key);
            if (myIsAutoDeleteModelElements) DELETE_AND_NULLIFY(item);
        }
    }

    QList<X*>  list() const {
        return myList;
    }

    QList<TKey> keys() const{
        return myKeyMap.keys();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const {
        TKeyedListModel<TKey,X> * me = (TKeyedListModel<TKey,X> *) this;
        if (section >= me->getColumnCount()) return QVariant();
        if (orientation != Qt::Horizontal) return QVariant();
        if (role != Qt::DisplayRole) return QVariant();
        if (section >= (int) myColumns.visibleColumnCount()) return QVariant();
        return myColumns.visibleHeaders().at(section);
    }

    //  bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role){
    //    TKeyedListModel<TKey,X> * me = (TKeyedListModel<TKey,X> *) this;
    //    if (section >= me->getColumnCount()) return false;
    //    if (orientation != Qt::Horizontal) return false;
    //    if (role != Qt::DisplayRole) return false;
    //    if (section >= myColumns.visibleColumnCount()) return false;
    //    myHorizontalHeaders.replace(section,value.toString());
    //    emit headerDataChanged(Qt::Horizontal,section,section);
    //    return true;
    //  }

    bool isColumnVisible(const QString & columnHeader){
        return myColumns.isColumnVisible(columnHeader);
    }

    void setColumnVisible(const QString & columnHeader,bool on){
        int columnOrder = myColumns.columnOrder(columnHeader);
        int  columnVisibleOrder = myColumns.columnVisibleOrder(columnHeader);
        if (columnOrder ==-1) return;
        if (myColumns.isColumnVisible(columnHeader) == on) return;
        if (on){

            beginInsertColumns(QModelIndex(), columnOrder, columnOrder);
            myColumns.setColumnVisible(columnHeader,true);
            endInsertColumns();
        }
        else {
            beginRemoveColumns(QModelIndex(), columnVisibleOrder, columnVisibleOrder);
            myColumns.setColumnVisible(columnHeader,false);
            endRemoveColumns();
        }
    }

    int columnVisibleOrder(const QString & columnHeader){
        return myColumns.columnVisibleOrder(columnHeader);
    }

    int columnOrder(const QString & columnHeader){
        return myColumns.columnOrder(columnHeader);
    }

    bool hasColumn(const QString & columnHeader){
        return myColumns.columnOrder(columnHeader) >= 0;
    }

    bool appendColumn(const QString & columnHeader,bool isVisible = true){
        int columnOrder = myColumns.columnOrder(columnHeader);
        if (columnOrder>=0) return false;
        int columnsVisibleCount = myColumns.visibleColumnCount();
        if (isVisible)
            beginInsertColumns(QModelIndex(),columnsVisibleCount, columnsVisibleCount);
        myColumns.appendColumn(columnHeader,isVisible);
        if (isVisible)
            endInsertColumns();

        return true;
    }

    bool insertColumn(const QString & columnHeader,uint position,bool isVisible = true){
        int columnOrder = myColumns.columnOrder(columnHeader);
        if (columnOrder>=0) return false;
        int columnsVisibleCount = myColumns.visibleColumnCount();
        if (position > columnsVisibleCount-1)  return false;
        if (isVisible)
            beginInsertColumns(QModelIndex(),position, position);
        myColumns.insertColumn(columnHeader,position,isVisible);
        if (isVisible)
            endInsertColumns();

        return true;
    }

    void removeColumn(const QString & columnHeader){
        int columnOrder = myColumns.columnOrder(columnHeader);
        if (columnOrder ==-1) return;
        int  columnVisibleOrder = myColumns.columnVisibleOrder(columnHeader);
        bool columnIsVisible = columnVisibleOrder >=0;
        if (columnIsVisible)
            beginRemoveColumns(QModelIndex(), columnVisibleOrder, columnVisibleOrder);
        myColumns.removeColumn(columnHeader);
        if (columnIsVisible)
            endRemoveColumns();
    }

    QString getVisibleColumnHeader(int column) const{
        return myColumns.getVisibleColumnHeader(column);
    }


protected :

    void setHorizontalHeaders(const QStringList  &  headers){
        myColumns.resetToVisibleHeaders(headers);
        int i=0;
        foreach (QString header, headers)
            setHeaderData(i++,Qt::Horizontal,header,Qt::DisplayRole);
    }

    virtual QVariant getColumnDisplayData(uint column,X * value){
        if (column == 0){
            Descriptable * descriptable  = dynamic_cast<Descriptable *> (value);
            if (descriptable == 0)
                return QVariant::fromValue<X*>(value);
            else
                return descriptable->entityName();
        }
        return QVariant();
    }

    virtual QVariant getColumnSortData(uint column, X * value) {
        return getColumnDisplayData(column,value);
    }

    virtual QVariant getColumnDecorateData(uint column,X * value){
        if (column == 0){
            Decorable * decoratable  = dynamic_cast<Decorable *> (value);
            if (decoratable == 0)
                return QVariant();
            else
                return decoratable->decoration();
        }
        return QVariant();
    }

    virtual void setDataFromColumn(X* /*data*/, const QModelIndex &/*index*/,const QVariant &/*vvalue*/,int /*role*/){}


    virtual int getColumnCount(){
        return myColumns.visibleColumnCount();
    }


    QList<X*> myList;
    QMap<TKey,X*> myKeyMap;
private:
    void addItem(const TKey & key,X * item) {
        if (item == NULL) return;
        int count = myList.count();
        myKeyMap.insert(key,item);
        myList.append(item);
        insertRows(count,1,QModelIndex());
        myList.replace(count,item);
    }

    bool myIsAutoDeleteModelElements;
    QMap<int,bool> myEditableColumnsState;
    QMap<int,bool> myBoldColumnsState;
    QMap<int,Qt::AlignmentFlag> myAlignmentFlag;
    TableColumnsDefinition myColumns;


}; 

#endif // TKEYEDLISTMODEL_H
