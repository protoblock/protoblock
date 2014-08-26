//
//  sfgui.cpp
//  cute-fantasy
//
//  Created by Jay Berg on 4/8/14.
//
//
#include "sfgui.h"
#include "client.h"
#include <fb/ProtoData.pb.h>
#include "ui_sfgui.h"
#include <QDateTime>
#include <QClipboard>

using namespace fantasybit;

sfGUI::sfGUI(QWidget *parent) : QWidget(parent), ui(new Ui::sfGUI)
{
    ui->setupUi(this);
    m_state = CONNECTING;
    m_namestatus.set_name("");
    m_namestatus.set_status(none);
    //QObject::connect(this, SIGNAL(on_flash()), this, SLOT(flashing()) );
    ui->textBrowser->append(QCoreApplication::applicationDirPath());
}

/*
void sfGUI::flashing()
{
    ui->progressBar->setVisible(true);
}
*/

void sfGUI::fromServer(const OutData &in)
{
    if ( in.type() == OutData_Type_HEARTBEAT )
    {
        ui->textBrowser->insertPlainText(".");
    }
    else if ( in.type() == OutData_Type_SNAPSHOT && m_state == CONNECTING )
    {
        ui->textBrowser->append(QDateTime::currentDateTime().toString() + " Connected");
        m_state = SNAPSHOT;
        if ( in.has_myfantasyname() )
        {
            switch ( in.myfantasyname().status())
            {
                case none:
                    break;
                case notavil:
                    ui->textBrowser->append(QString::fromStdString(in.myfantasyname().name()) + "not available.");
                    break;
                case requested:
                    ui->textBrowser->append("previous try for: " + QString::fromStdString(in.myfantasyname().name()) + ", was aborted. please retry");
                    ui->fantasyname->setText(in.myfantasyname().name().c_str());
                    break;
                case found:
                    ui->textBrowser->append(QString::fromStdString(in.myfantasyname().name()) + ", already found." );
                    //ui->textBrowser->append(QString::fromStdString(in.myfantasyname().nametransaction().DebugString()));
                    m_state = FOUND;
                    m_namestatus.CopyFrom(in.myfantasyname());
                    ui->fantasyname->setText(in.myfantasyname().name().c_str());
                case transaction_sent:
                    ui->textBrowser->append(QString::fromStdString(in.myfantasyname().name()) + ", found, pending confirmation." );
                    //ui->textBrowser->append(QString::fromStdString(in.myfantasyname().nametransaction().DebugString()));
                    m_state = FOUND;
                    m_namestatus.CopyFrom(in.myfantasyname());
                    ui->fantasyname->setText(in.myfantasyname().name().c_str());
                    break;
                case confirmed:
                    ui->textBrowser->append(QString::fromStdString(in.myfantasyname().name()) + ", confirmed!" );
                    //ui->textBrowser->append(QString::fromStdString(in.myfantasyname().nametransaction().DebugString()));
                    m_state = CONFIRMED;
                    m_namestatus.CopyFrom(in.myfantasyname());
                    ui->fantasyname->setText(in.myfantasyname().name().c_str());
                    break;
                default:
                    break;
            }
        }
        updatestatic();
    }
    else if ( in.type() != OutData_Type_SNAPSHOT  || (m_state == REQUESTED || m_state == MINING))
    {
        m_namestatus.CopyFrom(in.myfantasyname());
        switch ( in.myfantasyname().status())
        {
            case notavil:
                m_state = SNAPSHOT;
                ui->textBrowser->append(QDateTime::currentDateTime().toString() + QString::fromStdString(in.myfantasyname().name()) + "not available.");
                break;
            case requested:
                m_state = MINING;
                ui->textBrowser->append(QDateTime::currentDateTime().toString() + " mining for: " + QString::fromStdString(in.myfantasyname().name()));
                ui->textBrowser->append("please be patient. process can take 1 to 6 hours.");
                break;
            case found:
                ui->textBrowser->append(QDateTime::currentDateTime().toString() + QString::fromStdString(in.myfantasyname().name()) + ", found!" );
                m_state = FOUND;
                break;
            default:
                break;
        }
        updatestatic();
    }

    //ui->textBrowser->append(QString::fromStdString(in.DebugString()));
}
/**/

void sfGUI::updatestatic()
{
    if ( m_state == SNAPSHOT )
    {
        ui->message->setText("Generate your own fantasy name: ");
        ui->fantasyname->setEnabled(true);
        ui->generate->setEnabled(true);
        ui->progressBar->setValue(10);
    }
    else if ( m_state == REQUESTED )
    {
        ui->message->setText("Requested " + ui->fantasyname->text());
        ui->fantasyname->setEnabled(false);
        ui->generate->setEnabled(false);
        ui->progressBar->setValue(20);
    }
    else if ( m_state == MINING )
    {
        ui->message->setText("Generating " + ui->fantasyname->text() + "...");
        ui->fantasyname->setEnabled(false);
        ui->generate->setEnabled(false);
        ui->progressBar->setValue(40);
    }
    else if ( m_state == FOUND )
    {
        ui->message->setText(ui->fantasyname->text() + " found! Please share your proof-of-work ");
        ui->fantasyname->setEnabled(false);
        ui->generate->setEnabled(false);
        ui->progressBar->setValue(80);
        ui->copy->setEnabled(true);
        ui->proofofwork->setEnabled(true);
        ui->proofofwork->setText(m_namestatus.DebugString().c_str());
    }
    else if ( m_state == CONFIRMED )
    {
        ui->message->setText(ui->fantasyname->text() + "confirmed.");
        ui->fantasyname->setEnabled(false);
        ui->generate->setEnabled(false);
        ui->progressBar->setValue(100);
        ui->copy->setEnabled(true);
        ui->proofofwork->setEnabled(true);
        ui->proofofwork->setText(m_namestatus.DebugString().c_str());
    }
}

sfGUI::~sfGUI()
{
    delete ui;
}


void fantasybit::sfGUI::on_generate_clicked()
{
    if ( ui->fantasyname->text() == "") return;

    ui->textBrowser->append(QDateTime::currentDateTime().toString() + " requesting: " + ui->fantasyname->text());

    indata.set_type(InData_Type_MINENAME);
    indata.set_data(ui->fantasyname->text().toStdString());
    m_state = REQUESTED;
    updatestatic();

    emit fromGUI(indata);
}

void fantasybit::sfGUI::on_copy_clicked()
{
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
}
