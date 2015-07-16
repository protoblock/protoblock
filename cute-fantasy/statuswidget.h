#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QWidget>

namespace Ui {
class StatusWidget;
}

class StatusWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString StatusMessage READ getStatusMessage WRITE setStatusMessage USER true)
    Q_PROPERTY(QString SyncMessage READ getSyncMessage WRITE setSyncStatus USER true)

public:
    explicit StatusWidget(QWidget *parent = 0);
    ~StatusWidget();    

public slots:
    void setStatusMessage(const QString & statusMessage);
    void setSyncStatus(const QString & syncStatus);
    QString getStatusMessage() const;
    QString getSyncMessage() const;

private:
    Ui::StatusWidget *ui;

};

#endif // STATUSWIDGET_H
