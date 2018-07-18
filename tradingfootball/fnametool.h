#ifndef FNAMETOOL_H
#define FNAMETOOL_H

#include <QDialog>
#include <QString>
#include "ProtoData.pb.h"

namespace Ui {
class fnametool;
}

class MainLAPIWorker;

class fnametool : public QDialog
{
    Q_OBJECT

public:
    explicit fnametool(QWidget *parent = 0);
    ~fnametool();

    void UseName(QString name);

    void initialize();

    fantasybit::MyFantasyName newName();
signals:
    void ImportSucess(QString);
private slots:

    void on_exportButton_clicked();

    void on_importButton_clicked();

    void on_cancelButton_clicked();


    void on_clearButton_clicked();

private:
    MainLAPIWorker *  myLAPIWorker;
    fantasybit::MyFantasyName useNewName;
    Ui::fnametool *ui;
};

#endif // FNAMETOOL_H
