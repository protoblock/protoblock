#ifndef TESTCOREGUIFORM_H
#define TESTCOREGUIFORM_H

#include <QObject>
#include <QWidget>
#include "LAPIWorker.h"

namespace Ui {
class TestCoreGUIForm;
}


class TestCoreGUIForm : public QWidget
{
    Q_OBJECT

public:
    explicit TestCoreGUIForm(MainLAPIWorker * coreInstance,QWidget *parent = 0);
    ~TestCoreGUIForm();
signals:
    void requestPong(const QVariant & data);
    void requestPlayersForWeek(int week);
public slots:
    void handleNotificationOrResponse(const QVariant & data);
    void GoLive();
    void NewData(const fantasybit::DeltaData &);
    void OnMyFantasyNames(std::vector<fantasybit::MyFantasyName> &);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::TestCoreGUIForm *ui;
    MainLAPIWorker * myCoreInstance;
};

#endif // TESTCOREGUIFORM_H
