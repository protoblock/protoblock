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
    //m_status = MyNameStatus::none;
    m_namestatus.set_name("");
    m_namestatus.set_status(none);
}

void sfGUI::fromServer(const OutData &in)
{
    int i = 0;
//    if ( in.has_type() )
        i++;
    if ( in.type() == OutData_Type_SNAPSHOT)
    {
        //ui->textBrowser->clear();
        if ( m_state == CONNECTING )
        {
            ui->textBrowser->append(QDateTime::currentDateTime().toString() + " Connected");
            m_state = SNAPSHOT;
            if ( in.has_myfantasyname() )
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

            updatestatic();
        }
    }
    else if ( m_state == REQUESTED || m_state == MINING)
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
                ui->textBrowser->append(QDateTime::currentDateTime().toString() + "mining for: " + QString::fromStdString(in.myfantasyname().name()));
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
    emit fromGUI(indata);
    m_state = REQUESTED;
    updatestatic();
}

void fantasybit::sfGUI::on_copy_clicked()
{
    ui->message->setText("Proof copied. Please paste to twitter or email");

    ui->textBrowser->append("copied proof-of-work to clipboard");
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->proofofwork->toPlainText());
}
