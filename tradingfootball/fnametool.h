#ifndef FNAMETOOL_H
#define FNAMETOOL_H

#include <QDialog>
#include <QString>

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

    QString newName();
signals:
    void ImportSucess(QString);
private slots:

    void on_exportButton_clicked();

    void on_importButton_clicked();

    void on_cancelButton_clicked();


    void on_clearButton_clicked();

private:
    MainLAPIWorker *  myLAPIWorker;
    QString useNewName;
    Ui::fnametool *ui;
};

#endif // FNAMETOOL_H
