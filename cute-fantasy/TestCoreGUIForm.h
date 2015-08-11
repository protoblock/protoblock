#ifndef TESTCOREGUIFORM_H
#define TESTCOREGUIFORM_H

#include <QObject>
#include <QWidget>
#include "LAPIWorker.h"

namespace Ui {
class TestCoreGUIForm;
}

class QWaitCondition;
class TestCoreGUIForm : public QWidget
{
    Q_OBJECT

public:
    explicit TestCoreGUIForm(MainLAPIWorker * coreInstance,QWaitCondition * wait,QWidget *parent = 0);
    ~TestCoreGUIForm();
signals:
    void requestPong(const QVariant & data);
    void requestPlayersForWeek(int week);
public slots:
    void handleNotificationOrResponse(const QVariant & data);
    void GoLive();
    void NewData(const fantasybit::DeltaData &);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::TestCoreGUIForm *ui;
    MainLAPIWorker * myCoreInstance;
};

#endif // TESTCOREGUIFORM_H
