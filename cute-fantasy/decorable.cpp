#include "decorable.h"
#include <QApplication>


Decorable::Decorable(){
    myBrush = QBrush(Qt::lightGray);
}

Decorable::Decorable(const Decorable & copy){
  myBrush = copy.brush();
  myPixmap = copy.decoration();
}

Decorable& Decorable::operator=(const Decorable & right){
  myBrush = right.brush();
  myPixmap = right.decoration();
  return *this;
}

void Decorable::setDecoration(QRgb color ){
    myPixmap = QPixmap(QSize(32,32));
    QPainter p(&myPixmap);
    myBrush = QBrush(QColor(color));
    p.setBrush(myBrush);
    p.fillRect(myPixmap.rect(),Qt::white);
    p.setPen(Qt::black);
    p.drawRoundedRect(QRectF(8,8,16,16),2,2);

}

void Decorable::setDecoration( const QString & name ){
    myPixmap = QPixmap(QSize(32,32));
    QPainter p(&myPixmap);
    myBrush = QBrush(QColor(name));
    p.setBrush(myBrush);
    p.fillRect(myPixmap.rect(),Qt::white);
    p.setPen(Qt::black);
    p.drawRoundedRect(QRectF(8,8,16,16),2,2);

}

void Decorable::setDecoration(const char * name){
    myPixmap = QPixmap(QSize(32,32));
    QPainter p(&myPixmap);
    myBrush = QBrush(QColor(name));
    p.setBrush(myBrush);
    p.fillRect(myPixmap.rect(),Qt::white);
    p.setPen(Qt::black);
    p.drawRoundedRect(QRectF(8,8,16,16),2,2);
}

void Decorable::setDecoration(const QColor & color){
    myPixmap = QPixmap(QSize(32,32));
    QPainter p(&myPixmap);
    p.setBrush(color);
    myBrush = p.brush();
    p.fillRect(myPixmap.rect(),Qt::white);
    p.setPen(Qt::black);
    p.drawRoundedRect(QRectF(8,8,16,16),2,2);

}

void Decorable::setDecoration(Qt::GlobalColor color){
    myPixmap = QPixmap(QSize(32,32));
    QPainter p(&myPixmap);
    p.setBrush(color);
    myBrush = p.brush();
    p.fillRect(myPixmap.rect(),Qt::white);
    p.setPen(Qt::black);
    p.drawRoundedRect(QRectF(8,8,16,16),2,2);

}

void Decorable::setDecoration(QStyle::StandardPixmap styleIcon){
    QStyle * currentStyle = QApplication::style();
    setDecoration(currentStyle->standardIcon (styleIcon),16,16);
}

void Decorable::setDecoration(QBrush brush){
    myPixmap = QPixmap(QSize(32,32));
    QPainter p(&myPixmap);
    p.fillRect(myPixmap.rect(),Qt::white);
    myBrush = brush;
    p.setPen(Qt::black);
    p.setBrush(myBrush);
    p.drawRoundedRect(QRectF(8,8,16,16),2,2);
}
