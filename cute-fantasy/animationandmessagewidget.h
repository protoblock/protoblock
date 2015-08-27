#ifndef ANIMATIONANDMESSAGEWIDGET_H
#define ANIMATIONANDMESSAGEWIDGET_H

#include <QMovie>
#include <QWidget>

namespace Ui {
class AnimationAndMessageWidget;
}

class AnimationAndMessageWidget : public QWidget
{
  Q_OBJECT
  
public:
  explicit AnimationAndMessageWidget(QWidget *parent = 0);
  ~AnimationAndMessageWidget();
  void setAnimationFileName(const QString & fileName);
  void setMessage(const QString & message);

public slots:
  void hideAnimation();
  void showAndStartAnimation();
  void start();
  void stop();
  
private:
  Ui::AnimationAndMessageWidget *ui;
  QMovie  myAnimationMovie;
};

#endif // ANIMATIONANDMESSAGEWIDGET_H
