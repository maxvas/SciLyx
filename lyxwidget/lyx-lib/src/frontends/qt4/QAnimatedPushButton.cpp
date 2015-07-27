#include "QAnimatedPushButton.h"
#include <QMovie>
#include <QIcon>

QAnimatedPushButton::QAnimatedPushButton(QWidget *parent)
    :QPushButton(parent)
{
    myMovie = new QMovie(this);
    connect(myMovie, SIGNAL(frameChanged(int)), this, SLOT(setButtonIcon()));
}

void QAnimatedPushButton::setAnimatedIcon(QString fileName)
{
    myMovie->setFileName(fileName);
    myMovie->stop();
}

void QAnimatedPushButton::showAnimation()
{
    myMovie->start();
}

void QAnimatedPushButton::hideAnimation()
{
    myMovie->stop();
    this->setIcon(QIcon());
}

void QAnimatedPushButton::setButtonIcon()
{
    this->setIcon(QIcon(myMovie->currentPixmap()));
}

#include "moc_QAnimatedPushButton.cpp"
