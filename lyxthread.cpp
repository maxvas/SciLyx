#include "lyxthread.h"
#include "lyxwidget/lyxwidget.h"

LyxThread::LyxThread(QWidget *parent)
    :QThread(parent), parent(parent)
{

}

void LyxThread::run()
{
    LyxWidget *lyxWidget = new LyxWidget(parent);
    lyxWidgetCreated(lyxWidget);
    lyxWidget->exec();
    exec();
}
