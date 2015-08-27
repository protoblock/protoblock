#include "animationandmessagewidget.h"
#include "ui_animationandmessagewidget.h"

AnimationAndMessageWidget::AnimationAndMessageWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::AnimationAndMessageWidget)
{
  ui->setupUi(this);
  ui->myAnimationLabel->setMovie(&myAnimationMovie);
}

AnimationAndMessageWidget::~AnimationAndMessageWidget()
{
  delete ui;
}


void AnimationAndMessageWidget::setAnimationFileName(const QString & fileName){
  myAnimationMovie.setFileName(fileName);
}

void AnimationAndMessageWidget::setMessage(const QString & message){
  ui->myMessageLabel->setText(message);
}

void AnimationAndMessageWidget::start(){
  myAnimationMovie.start();
}

void AnimationAndMessageWidget::stop(){
 myAnimationMovie.stop();
}

void AnimationAndMessageWidget::hideAnimation(){
  myAnimationMovie.stop();
  ui->myAnimationLabel->hide();
}

void AnimationAndMessageWidget::showAndStartAnimation(){
  ui->myAnimationLabel->show();
  myAnimationMovie.start();
}
