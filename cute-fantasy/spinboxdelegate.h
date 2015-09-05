#ifndef SPINBOXDELEGATE_H
#define SPINBOXDELEGATE_H


#include <QStyledItemDelegate>

class SpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    SpinBoxDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const Q_DECL_OVERRIDE;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEnabled(bool  on) {
        myEnabled = on;
    }
    bool isEnabled(){
        return myEnabled;
    }

private :
    bool myEnabled;
};

//#include <QPushButton>

//class SendPlayerProjectionButton : public QPushButton {
//    Q_OBJECT
//public:
//    SendPlayerProjectionButton(QWidget * parent=0):QPushButton(parent){
//        QObject::connect(this,SIGNAL(clicked(bool)),this,SLOT(onClicked()));
//    }

//    void setFantasyName(const QString & argFantasyNameValue) {
//        myFantasyName = argFantasyNameValue;
//    }
//    QString FantasyName() const {
//        return myFantasyName;
//    }
//signals:
//    void sendProjection(QString fantasyName);
//public slots:
//  void onClicked(){
//      emit sendProjection(myFantasyName);
//  }

//private:
//    QString myFantasyName;

//};

#include <QItemDelegate>
class SendFantasyPlayerButtonDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    SendFantasyPlayerButtonDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
private:
    QString getCurrentFanatsyName(QAbstractItemModel *model,const QModelIndex &index) const;

signals:
    void sendProjection(QString fantasyName);

};


#endif // SPINBOXDELEGATE_H
