#ifndef VIEW_MODELS_H
#define VIEW_MODELS_H

#include "Data.h"
#include "descriptable.h"
#include "decorable.h"
#include "tlistmodel.h"
#include "tkeyedlist.h"
//#include "propertynames.h"
#include <QMetaType>
#include <QVariant>


using namespace fantasybit;



class ViewModel : public Decorable , public Descriptable {

public:

    ViewModel() {}

    ~ViewModel() {}

    ViewModel(const ViewModel & src) {
       foreach(const QString propName,src.myProperties.keys())
           myProperties.insert(propName,src.myProperties.value(propName));
    }

    ViewModel & operator=(const ViewModel & other){
        if(this != &other) {
            myProperties.clear();
            foreach(const QString  propName,other.myProperties.keys())
                myProperties.insert(propName,other.myProperties.value(propName));
        }
        return *this;
    }

    template <typename PROPNAME>
    void attachProperty(const QVariant & value){
        myProperties.insert(PROPNAME::name(),value);
    }

    template <typename T,typename PROPNAME>
    void attachProperty(const T & value){
        myProperties.insert(PROPNAME::name(),qVariantFromValue<T>(value));
    }

    void attachProperty(const QString & propertyName,const QVariant & value){
        myProperties.insert(propertyName,value);
    }

    QVariant  propertyValue(const QString& propertyName){
        return myProperties.value(propertyName);
    }

    template <typename T>
    T propertyValue(const QString& propertyName){
        return qvariant_cast<T>(myProperties.value(propertyName));

    }


    template <typename T,typename PROPNAME>
    T propertyValue(){
        return qvariant_cast<T>(myProperties.value(PROPNAME::name()));
    }

    template <typename PROPNAME>
    QVariant propertyValue(){
        return myProperties.value(PROPNAME::name());
    }

    template <typename PROPNAME>
    bool hasProperty(){
        return hasProperty(PROPNAME::name());
    }

    bool hasProperty(const QString& propertyName){
        return myProperties.keys().contains(propertyName);
    }

    bool removePoperty(const QString& propertyName){
        return myProperties.remove(propertyName) ==1;
    }

private:
     QVariantMap myProperties;
};

Q_DECLARE_METATYPE(ViewModel *)

#endif // VIEW_MODELS_H

