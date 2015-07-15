//
//  sfgui.cpp
//  cute-fantasy
//
//  Created by Jay Berg on 4/8/14.
//
//
#include "sfgui.h"
#include "client.h"
#include <ProtoData.pb.h>
#include "ui_sfgui.h"
#include <QDateTime>
#include <QClipboard>

using namespace fantasybit;

sfGUI::sfGUI(QWidget *parent) : QWidget(parent), ui(new Ui::sfGUI)
{
    ui->setupUi(this);
    //QObject::connect(this, SIGNAL(on_flash()), this, SLOT(flashing()) );
    ui->textBrowser->append(QCoreApplication::applicationDirPath());
}

/*
void sfGUI::flashing()
{
    ui->progressBar->setVisible(true);
}
*/

void sfGUI::fromServer(const DeltaData &in)
{
    if ( in.type() == DeltaData_Type_HEARTBEAT )
    {
        ui->textBrowser->insertPlainText(".");
        deltaData.CopyFrom(in);
        updatedelta();
    }
    else if ( in.type() == DeltaData_Type_SNAPSHOT)
    {
        if ( !snapData.has_type() ) {
            ui->textBrowser->append(QDateTime::currentDateTime().toString() + " Connected");

            ui->message->setText("Live: ");
            ui->fantasyname->setEnabled(true);
            ui->generate->setEnabled(true);
        }

        snapData.CopyFrom(in);
        updatesnap();
    }

    ui->textBrowser->append(QString::fromStdString(in.DebugString()));
}
/**/

void sfGUI::updatesnap()
{
    if ( snapData.myfantasyname().status() != MyNameStatus::none ) {
        ui->fantasyname->setText(QString::fromStdString(snapData.myfantasyname().name()));
        ui->fantasynamestatus->setText("Fantasy Name Status: " +
            QString::fromStdString(MyNameStatus_Name(snapData.myfantasyname().status())));
    }


    ui->message->setText(QString::fromStdString(
               "Live: " +
                GlobalState_State_Name(snapData.globalstate().state()) +
                " " +
                std::to_string(snapData.globalstate().season())
                ));

}

void sfGUI::updatedelta()
{

}

sfGUI::~sfGUI()
{
    delete ui;
}


void fantasybit::sfGUI::on_generate_clicked()
{

    if ( ui->fantasyname->text() == "") return;

    ui->textBrowser->append(QDateTime::currentDateTime().toString() + " requesting: " + ui->fantasyname->text());

    indata.set_type(InData_Type_NEWNAME);
    indata.set_data(ui->fantasyname->text().toStdString());

    emit fromGUI(indata);
}


void fantasybit::sfGUI::on_copy_clicked()
{
    /*
    QClipboard *clipboard = QApplication::clipboard();
    if ( ui->twitter->isChecked() )
    {
        QString tweet("@satoshifantsy (");
        tweet += m_namestatus.name().c_str();
        tweet += ") (";
        tweet += QString::number(m_namestatus.nametransaction().hash()),
        tweet += ") (";
        tweet += m_namestatus.nametransaction().sigid().c_str();
        tweet += ")";
        ui->proofofwork->append(tweet);
        clipboard->setText(tweet);
        ui->message->setText("Proof copied to clipboard, please paste ad tweet");
    }
    else
    {
        clipboard->setText(ui->proofofwork->toPlainText());
        ui->message->setText("Proof copied to clipboard");
    }
    */
}

