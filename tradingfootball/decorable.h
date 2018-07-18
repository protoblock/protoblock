// decorable.h
// Written by: Moez Bouselmi
//
// This file contains a definition for the class Decorable which can be used to overload
// existants classes in order to be used with Qt Model View framework.

#ifndef DECORABLE_H
#define DECORABLE_H

#include <QPixmap>
#include <QPainter>
#include <QIcon>
#include <QStyle>


class Decorable {

public:
    explicit Decorable();
    Decorable(const Decorable & copy);
    Decorable& operator=(const Decorable & right);
    virtual QPixmap decoration() const {return myPixmap; }

    void setDecoration(const QPixmap & pxm){
        myPixmap = pxm;
    }
    void setPixmapFromResource(const QString & resourcePath){
      myPixmap = QPixmap(resourcePath);
    }

    void setDecoration(const QIcon & icon,int width,int height){
        myPixmap = icon.pixmap(QSize(width,height));
    }
    void setDecoration(QStyle::StandardPixmap styleIcon);
    void setDecoration(QRgb color );
    void setDecoration( const QString & name );
    void setDecoration(const char * name);
    void setDecoration(const QColor & color);
    void setDecoration(Qt::GlobalColor color);
    void setDecoration(QBrush brush);
    QBrush brush() const { return myBrush;}

    virtual ~Decorable() { }

protected :
    QPixmap myPixmap;
    QBrush myBrush;
};

#endif // DECORABLE_H
