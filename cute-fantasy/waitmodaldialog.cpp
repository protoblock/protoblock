#include <QDesktopWidget>
#include "waitmodaldialog.h"
#include "ui_waitmodaldialog.h"



WaitModalDialog::WaitModalDialog():QDialog(NULL),
  ui(new Ui::WaitModalDialog){
  ui->setupUi(this);

  move(QApplication::desktop()->screen()->rect().center() - rect().center());
  setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed));
  setAttribute(Qt::WA_SetWindowModality,true);
  setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint| Qt::WindowStaysOnTopHint);
  setWindowTitle("");
  setWindowModality(Qt::ApplicationModal);  
}

WaitModalDialog::~WaitModalDialog() {
  myAnimationMovie.stop();
  if (myTimer.isActive()) myTimer.stop();
  delete ui;
}

void WaitModalDialog::init(const QString & fileName,int timeout,const QString & initialMessage,
                             bool showProgressMessage,
                             bool showLogMessage){
  ui->myAnimationLabel->setMovie(&myAnimationMovie);
  setMessage(initialMessage);
  myShowProgressMessage = showProgressMessage;
  myShowLogMessage = showLogMessage;
  myTimeout = timeout;
  myAnimationMovie.setFileName(fileName);
  if (myTimeout > 0)
      connect(&myTimer,SIGNAL(timeout()),this,SLOT(stopAndClose()));
}

void WaitModalDialog::setAnimationFileName(const QString & fileName){
  myAnimationMovie.setFileName(fileName);
}

void WaitModalDialog::setMessage(const QString & message){
  ui->myMessageLabel->setText(message);
}

void WaitModalDialog::startExec(){
  myAnimationMovie.start();
  if (myTimeout > 0)
    myTimer.start(myTimeout);
  exec();
}


void WaitModalDialog::stopAndClose(){
  myAnimationMovie.stop();
  if (myTimer.isActive()) myTimer.stop();
  accept(); 
}


bool WaitModalDialog::eventFilter(QObject *obj, QEvent *event){
//  switch (event->type()){
//    case Par3EventType::Log :
//      if (myShowLogMessage) {
//        LogEvent  * logEvent = dynamic_cast<LogEvent*>(event);
//        if (logEvent !=NULL)
//          setMessage(logEvent->message());
//      }
//      break;
//    case Par3EventType::ProgressUpdate :
//      if (myShowProgressMessage) {
//        ProgressNotifyEvent * progressNotifyEvent = dynamic_cast<ProgressNotifyEvent*>(event);
//        if (progressNotifyEvent!=NULL)
//          setMessage(progressNotifyEvent->message());
//      }
//      break;

//  }
  return QDialog::eventFilter(obj,event);
}


