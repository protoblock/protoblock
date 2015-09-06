#ifndef SPINBOXDELEGATE_H
#define SPINBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QSpinBox>
#include <QKeyEvent>
#include <QTableView>

class SpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    SpinBoxDelegate(QObject *parent = 0);

    void setTableView(QTableView * argTableViewValue) {
        myTableView = argTableViewValue;
    }

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const Q_DECL_OVERRIDE;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEnabled(bool  on) { myEnabled = on;}

    bool isEnabled(){ return myEnabled; }

protected:
    bool eventFilter(QObject *obj, QEvent *evt)
    {
        if (evt->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
            {
                QSpinBox * editor = dynamic_cast<QSpinBox*>(obj);
                if (editor==NULL) return QStyledItemDelegate::eventFilter(obj,evt);
                QVariant vIndex =  editor->property("EditedIndex");
                QModelIndex index =  vIndex.value<QModelIndex>();
                if (!index.isValid()) return QStyledItemDelegate::eventFilter(obj,evt);
                QAbstractItemModel * model = const_cast<QAbstractItemModel *>(index.model());
                if (model==NULL) return QStyledItemDelegate::eventFilter(obj,evt);
                setModelData(editor,model,index);
                closeEditor(editor);
                QModelIndex  nextIndex = model->index(index.row()+1,index.column());
                if (!nextIndex.isValid()) return true;
                if (myTableView==NULL) return true;
                myTableView->setCurrentIndex(nextIndex);
                myTableView->edit(nextIndex);
                return true;
            }
        }
        if (evt->type() == QEvent::FocusOut) {
                QSpinBox * editor = dynamic_cast<QSpinBox*>(obj);
                if (editor==NULL) return QStyledItemDelegate::eventFilter(obj,evt);
                QVariant vIndex =  editor->property("EditedIndex");
                QModelIndex index =  vIndex.value<QModelIndex>();
                if (!index.isValid()) return QStyledItemDelegate::eventFilter(obj,evt);
                QAbstractItemModel * model = const_cast<QAbstractItemModel *>(index.model());
                if (model==NULL) return QStyledItemDelegate::eventFilter(obj,evt);
                setModelData(editor,model,index);
                closeEditor(editor);
                return true;
        }

        return QStyledItemDelegate::eventFilter(obj, evt);
    }

private :
    bool myEnabled;
    QTableView * myTableView;
};

#endif // SPINBOXDELEGATE_H
