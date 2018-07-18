// descriptable.h
// Written by: Moez Bouselmi
//
// This file contains a definition for the class Descriptable which can be used to overload
// existants classes that can have a name and a description, this usefull also in some other classes
// related to Qt Model View frameowke as tlist.h.

#ifndef DESCRIPTABLE_H
#define DESCRIPTABLE_H

#include <QString>


class Descriptable {

public:
    explicit Descriptable(const QString & name = QString(""));
    Descriptable(const Descriptable & copy);
    Descriptable(const QString & name,const QString & description);

    Descriptable &operator=(const Descriptable & right);

    void setDescription(const QString & description);
    virtual QString description() const;

    virtual void setNameAndDescription(const QString  & value);
    virtual void setEntityName(const QString & name);
    virtual QString entityName() const;
    virtual ~Descriptable() {}

protected :
        QString  myDescription;
        QString myEntityName;
};

#endif // DESCRIPTABLE_H
