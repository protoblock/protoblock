#include "spinboxdelegate.h"
#include <QSpinBox>
#include <QDebug>

SpinBoxDelegate::SpinBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent){
    myEnabled = true;


}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem & /*option*/ ,
    const QModelIndex &/* index */) const {
    QSpinBox *editor = new QSpinBox(parent);
    //PlayerProjectionWidget *editor = new PlayerProjectionWidget(parent);
    editor->setFrame(false);
    editor->setMinimum(0);
    editor->setMaximum(100);   
    editor->setEnabled(myEnabled);
    return editor;
}

void SpinBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const {
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();
    model->setData(index, value, Qt::EditRole);
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const {
    editor->setGeometry(option.rect);
}


#include "tablemodels.h"

SendFantasyPlayerButtonDelegate::SendFantasyPlayerButtonDelegate(QObject *parent) : QItemDelegate(parent){}



#include <QApplication>
#include  <QEvent>
#include <QBitmap>
void SendFantasyPlayerButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
 {
     QStyleOptionButton button;
     QRect r = option.rect;
     r.setWidth(r.width()-30);

     QString fantasyName = getCurrentFanatsyName(const_cast<QAbstractItemModel *>(index.model()),index);

     if (option.state & QStyle::State_Selected)
             painter->fillRect(option.rect, option.palette.highlight());

         int size = qMin(option.rect.width(), option.rect.height());
         int brightness = index.model()->data(index, Qt::DisplayRole).toInt();
         double radius = (size / 2.0) - (brightness / 255.0 * size / 2.0);
         if (radius == 0.0)
             return;

         painter->save();
         QPen pen;


         painter->setRenderHint(QPainter::Antialiasing, true);
         if (option.state & QStyle::State_Selected)
             pen.setColor(option.palette.highlightedText().color());
         else
             pen.setColor(option.palette.text().color());

         painter->setPen(pen);


     painter->drawText(r,fantasyName);
     painter->restore();

     r = option.rect;
     int x,y,w,h;
     x = r.left() + r.width() - 30;//the X coordinate
     y = r.top();//the Y coordinate
     w = 30;//button width
     h = 30;//button height

     button.rect = QRect(x,y,w,h);
     button.text = "+";
     //button.state = QStyle::State_Enabled;

     QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
     if (QApplication::mouseButtons().testFlag(Qt::LeftButton))
         button.state = QStyle::State_Sunken;
     else
         button.state = QStyle::State_Raised;
     QApplication::style()->drawControl(QStyle::CE_PushButton,&button,painter);
 }
#include <QMouseEvent>
 bool SendFantasyPlayerButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
 {
     if( event->type() == QEvent::MouseButtonRelease )
     {
         QMouseEvent * e = (QMouseEvent *)event;
         int clickX = e->x();
         int clickY = e->y();

         QRect r = option.rect;//getting the rect of the cell
         int x,y,w,h;
         x = r.left() + r.width() - 30;//the X coordinate
         y = r.top();//the Y coordinate
         w = 30;//button width
         h = 30;//button height

         if( clickX > x && clickX < x + w )
             if( clickY > y && clickY < y + h )
             {
                     QString fantasyName = getCurrentFanatsyName(model,index);
                     if (!fantasyName.trimmed().isEmpty()) emit sendProjection(fantasyName);
                     return true;
             }
     }
     return false;
 }

 QString SendFantasyPlayerButtonDelegate::getCurrentFanatsyName(QAbstractItemModel *model,const QModelIndex &index) const{
     LeaderBoardTableModel * leaderboardModel = dynamic_cast<LeaderBoardTableModel *>(model);
     if (leaderboardModel!=NULL){
         ViewModel * data =leaderboardModel->getItemByIndex(index);
         if (data ==NULL) return "";
         QString fantasyName = data->propertyValue<QString,PropertyNames::Fantasy_Name>();
         return fantasyName;
     }
     return "";
 }


