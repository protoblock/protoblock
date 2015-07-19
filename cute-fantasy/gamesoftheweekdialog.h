#ifndef GAMESOFTHEWEEKDIALOG_H
#define GAMESOFTHEWEEKDIALOG_H

#include <QDialog>
#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QPainter>

namespace Ui {
class GamesOfTheWeekDialog;
}

class CustomTabStyle : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget=0) const
    {
        QSize rectSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab){
            rectSize = rectSize.transposed() + QSize(0,10);
        }
        return rectSize;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab *tabOption = qstyleoption_cast<const QStyleOptionTab *>(option))
            {
                QStyleOptionTab opt(*tabOption);
                opt.shape = QTabBar::RoundedWest;
                QProxyStyle::drawControl(element, &opt, painter, widget);
                return;
            }
        }
        QProxyStyle::drawControl(element, option, painter, widget);
    }

    void drawItemText(QPainter * painter, const QRect & rect,
                     int flags, const QPalette & pal, bool enabled,
                     const QString & text, QPalette::ColorRole textRole = QPalette::NoRole) const {
        //same rectanfle siz and position as sizeFromContents
        QRect  newRect(rect.topLeft(),rect.size().transposed());
        newRect.setHeight(newRect.height()+10);
        painter->save();
        painter->rotate(90);
        //hard code need to be adjusted.
        painter->translate(-15,-25);
        QCommonStyle::drawItemText(painter,newRect,flags,pal,enabled,text,textRole );
        painter->restore();
    }
};



class GamesOfTheWeekDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GamesOfTheWeekDialog(QWidget *parent = 0);
    ~GamesOfTheWeekDialog();

private:
    Ui::GamesOfTheWeekDialog *ui;
};

#endif // GAMESOFTHEWEEKDIALOG_H
