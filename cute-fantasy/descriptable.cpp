#include "descriptable.h"

Descriptable::Descriptable(const Descriptable & copy){
  myEntityName = copy.entityName();
  myDescription= copy.description();
}

Descriptable & Descriptable::operator=(const Descriptable & right){
  myEntityName = right.entityName();
  myDescription= right.description();
  return * this;
}

Descriptable::Descriptable(const QString & name){
    myDescription = "";
    myEntityName = name;
}
Descriptable::Descriptable(const QString & name,const QString &  description){
    myDescription = description;
    myEntityName = name;
}
void Descriptable::setDescription(const QString &  description){
    myDescription = description;
}
QString Descriptable::description() const {
    return myDescription;
}
void Descriptable::setNameAndDescription(const QString &  value){
    myEntityName = value;
    myDescription = value;
}
void Descriptable::setEntityName( const QString &  name)
{
    myEntityName = name;
}
QString Descriptable::entityName() const {
    return myEntityName;
}
