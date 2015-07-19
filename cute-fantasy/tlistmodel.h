// tlistmodel.h
// Written by: Moez Bouselmi
//
// This file contains a definition for the template class TListModel which can be used
// to quickly obtain an editable Model from a simple QList.
// The model can be also edited by using a simple calls as :
// void addItem(X item);
//   bool removeItem(X item);
// and get the model synchronized.
// The TListModel check wether the X is Descriptable (see descriptable.h)or Decorable
// (see decorable.h) instance to provide the views using the model information about
// these roles :
//
//   Qt::ToolTipRole
//   Qt::StatusTipRole
//   Qt::WhatsThisRole
//   Qt::DisplayRole

// usage constraint X must have a default constructor

#ifndef TLISTMODEL_H
#define TLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QPixmap>
#include <QPoint>
#include "descriptable.h"
#include "decorable.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include "globals.h"

using namespace std;
//X must have a default constructor
template <class X>
class TListModel : public QAbstractTableModel {

public:
  explicit TListModel(QObject *parent =0, bool autoDelete = false)
    : QAbstractTableModel(parent)
  { myIsAutoDeleteModelElements = autoDelete; }

  ~TListModel(){
    if (myIsAutoDeleteModelElements){
      qDeleteAll(myList.begin(),myList.end());
      qDebug("TList items deleted");
    }
  }

  TListModel (const TListModel& copy,QObject *parent =0): QAbstractTableModel(parent){
    myList.append(copy.list());
    myIsAutoDeleteModelElements = copy.isAutoDelete();
  }

  TListModel<X> &operator=(const TListModel<X> & other){
    TListModel<X> * newModel = new TListModel<X>(other.list());
    myIsAutoDeleteModelElements = other.isAutoDelete();
    return *newModel ;
  }


  TListModel(QList<X*> data, QObject *parent =0)
    : QAbstractTableModel(parent)
  {
    myList.append(data);    
  }

  bool isAutoDelete() { return myIsAutoDeleteModelElements; }
  void setAutoDelete(bool on) { myIsAutoDeleteModelElements = on;}


  void setEditable(bool on) { myEditable = on;}


  int   rowCount(const QModelIndex & parent = QModelIndex() ) const
  {
    Q_UNUSED(parent);
    return myList.size();
  }

  int   columnCount(const QModelIndex & parent = QModelIndex() ) const
  {
    Q_UNUSED(parent);
    TListModel<X> * me = (TListModel<X> *) this;
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
    TListModel<X> * me = (TListModel<X> *) this;

    if (role == Qt::DisplayRole) {
        X * value = myList.at(index.row());
        return me->getColumnDisplayData(index.column(),value);
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

    return QVariant();
  }

  bool setData(const QModelIndex &index, const QVariant &vvalue, int role)
  {
    if (index.row() >= 0 && index.row() < myList.size()
        && (role == Qt::EditRole || role == Qt::DisplayRole)) {
      if (!index.isValid()) return false;
      TListModel<X> * me = (TListModel<X> *) this;
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

    if (myEditable)
      return QAbstractItemModel::flags(index)| Qt::ItemIsEditable;
    else
      return QAbstractItemModel::flags(index);// TODO implement Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
  }

  bool insertRows(int row, int count, const QModelIndex &parent)
  {
    if (count < 1 || row < 0 || row > rowCount(parent))
      return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int r = 0; r < count; ++r)
      myList.insert(row, NULL);

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

  void addItem(X * item) {
    int count = myList.count();    
    insertRows(count,1,QModelIndex());
    myList.replace(count,item);
  }
  
  bool removeItem(X * item){
    if (!myList.contains(item)) return false;
    int row = myList.indexOf(item);    
    removeRows(row,1,QModelIndex());
    return true;
  }

  void removeAll(){    

    QList<X*> list;
    list.append(myList);
    foreach(X* item,list) {
      removeItem(item);
      if (myIsAutoDeleteModelElements)
        DELETE_AND_NULLIFY(item);
    }
  }

  QList<X*>  list() const {
    return myList;
  }

  QVariant headerData(int section, Qt::Orientation orientation, int role) const {
    TListModel<X> * me = (TListModel<X> *) this;
    if (section >= me->getColumnCount()) return QVariant();
    if (orientation != Qt::Horizontal) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();
    if (section >= myHorizontalHeaders.count()) return QVariant();    
    return myHorizontalHeaders.at(section);
  }

  bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role){
    TListModel<X> * me = (TListModel<X> *) this;
    if (section >= me->getColumnCount()) return false;
    if (orientation != Qt::Horizontal) return false;
    if (role != Qt::DisplayRole) return false;
    if (section >= myHorizontalHeaders.count()) return false;
    myHorizontalHeaders.replace(section,value.toString());
    emit headerDataChanged(Qt::Horizontal,section,section);
    return true;
  }

protected :

  void setHorizontalHeaders(const QStringList  &  headers){
    myHorizontalHeaders.clear();    
    int i=0;
    foreach (QString header, headers){
      myHorizontalHeaders.append("");
      setHeaderData(i++,Qt::Horizontal,header,Qt::DisplayRole);
    }
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
    return 1;
  }

  QList<X*> myList;
private:
  QStringList myHorizontalHeaders;
  bool myIsAutoDeleteModelElements;
  bool myEditable;
};

#endif // TLISTMODEL_H
