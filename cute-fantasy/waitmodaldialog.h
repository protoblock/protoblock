#ifndef WaitModalDialog_H
#define WaitModalDialog_H

#include <QMovie>
#include <QTimer>
#include <QDialog>


namespace Ui {
class WaitModalDialog;
}

class  WaitModalDialog : public QDialog  {
  Q_OBJECT
public:  
  explicit WaitModalDialog();
  ~WaitModalDialog();
  void init(const QString & fileName,int timeout=-1,const QString & initialMessage="",
            bool showProgressMessage=true,
            bool showLogMessage=true);

public slots:
  void startExec();
  void stopAndClose();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
  void setAnimationFileName(const QString & fileName);
  void setMessage(const QString & message);

  qint64 myTimeout;
  Ui::WaitModalDialog *ui;
  QMovie  myAnimationMovie;
  QTimer myTimer;
  bool myShowProgressMessage;
  bool myShowLogMessage;
};

#endif // WaitModalDialog_H
