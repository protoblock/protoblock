#include "fnametool.h"
#include "ui_fnametool.h"
#include "dataservice.h"
#include "qmessagebox.h"

fnametool::fnametool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fnametool)
{
    ui->setupUi(this);
}

void fnametool::initialize() {
    useNewName = "";
    myLAPIWorker = Core::resolveByName<MainLAPIWorker>("coreapi");

    QObject::connect(this,SIGNAL(ImportSucess(QString)),myLAPIWorker,SLOT(OnUseName(QString)));
}

fnametool::~fnametool()
{
    delete ui;
}

void fnametool::UseName(QString name) {
    if ( name == "") {
        ui->exportButton->setEnabled(false);
        ui->usefname->setEnabled(false);
        ui->exportButton->setHidden(true);
        ui->usefname->setHidden(true);
    }
    else
        ui->usefname->setText(name);
}


void fnametool::on_exportButton_clicked()
{
    auto mn = DataService::instance()->exportMnemonic(ui->usefname->text().toStdString());

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(APPLICATION_NAME);

    QString text;
    if ( mn == "" ) {
        text = "cant find secret";
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setText(text);
        QFont font = msgBox.font();
        font.setBold(true);
        msgBox.setFont(font);
        msgBox.exec();
    }
    else {
        text = "<P><FONT COLOR='#FF0000' style='font-size:14px'>" \
        "Display Secret on Screen? <br><br>" \
        "A 12 word secret mnemonic is the master private key that controls your Fantasy Name. <br><br>" \
        "This is your \"brain wallet\" Write it down. Keep it safe. Memorize it. Dont share it. <br><br>" \
        "Anyone with this key will have control of any Fantasy " \
        "Bits associated with your FantasyName. " \
        "</FONT></P>";

        int result = QMessageBox::Cancel;
        msgBox.setText(text);
        QFont font = msgBox.font();
        font.setBold(true);
        //font.setPointSize(10);
        //font.setPointSize(font.pointSize()*1.2);
        msgBox.setFont(font);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        msgBox.setButtonText(QMessageBox::Yes,"Yes, I understand. Display Secret Key.");
        msgBox.setDefaultButton(QMessageBox::Yes);
        result = msgBox.exec();

        if (result== QMessageBox::Yes)
            QMessageBox::information(this,ui->usefname->text() + " - secret",mn.data());
    }
    this->close();
}

void fnametool::on_importButton_clicked()
{
   auto mn =  ui->importMnemonic->toPlainText();
   auto mfn = DataService::instance()->importMnemonic(mn.toStdString());

   if ( mfn.status() == MyNameStatus::confirmed ) {
       emit ImportSucess(QString(mfn.name().data()));

       useNewName = QString(mfn.name().data());
       QMessageBox::information(this,APPLICATION_NAME,
                                QString("\"%1\" imported!").arg(mfn.name().data()));

       this->close();
   }
   else        
       QMessageBox::information(this,APPLICATION_NAME,
                       QString("import failed. please input valid secret"));


}

QString fnametool::newName() {
    return useNewName;
}

void fnametool::on_cancelButton_clicked()
{
    this->close();
}

void fnametool::on_clearButton_clicked()
{
   ui->importMnemonic->clear();
}
