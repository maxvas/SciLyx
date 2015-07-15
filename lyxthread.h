#ifndef LYXTHREAD_H
#define LYXTHREAD_H

#include <QThread>

class LyxWidget;

class LyxThread : public QThread
{
    Q_OBJECT
    QWidget *parent;

public:
    LyxThread(QWidget *parent);
    void run();

signals:
    void lyxWidgetCreated(LyxWidget *widget);
};

#endif // SCILYX_H
