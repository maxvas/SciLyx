#include "lyxwidget.h"
#include "ui_lyxwidget.h"

#include <QDebug>
#include <QVBoxLayout>
#include <LyX.h>
#include <frontends/qt4/GuiView.h>
#include <support/debug.h>
#include <support/os.h>
#include <iostream>
#include <QCloseEvent>
#include <QTimer>
#include <QThread>

using namespace std;

#define lyxInstance (&lyx::Singleton<lyx::LyX>::Instance())
#define widgetInstance LyxGlobal::mainWidget()

int LyxWidget::getPrefferedWidth() const
{
    return 900;
}

void LyxWidget::openDocumentInNewWindow(QString fileName)
{
    lyx::LyX *lyx = lyxInstance;
    lyx->openInNewWindow(fileName.toStdString());
}

void LyxWidget::openOldDocumentInNewWindow(QString fileName, QString commit, QByteArray data)
{
    lyx::LyX *lyx = lyxInstance;
    lyx->openOldFileInNewWindow(fileName.toStdString(), commit.toStdString(), data.data());
}

void LyxWidget::closeEvent(QCloseEvent * event)
{
    lyx::LyX *lyx = lyxInstance;
    if (!lyx->tryCloseAll())
    {
        event->ignore();
    }
    else
    {
        QWidget::closeEvent(event);
    }
}

void LyxWidget::startLyxGUI()
{
    lyx::LyX *lyx = lyxInstance;
    lyx->startGUI();
}

void LyxWidget::updateBuffer()
{
    lyx::LyX *lyx = lyxInstance;
    lyx->updateMainBuffer();
}

LyxWidget::LyxWidget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::Lyx)
{
    ui->setupUi(this);

    QStringList args;
    args<<QApplication::arguments()[0];
    int argc = args.length();
    char** data = new char*[argc];
    for (int i=0; i<argc; i++)
    {
        QByteArray arr = args[i].toLocal8Bit().data();
        data[i] = new char[arr.length()+1];
        strcpy(data[i], arr);
        //data[i+arr.length()] = 0;
        cout<<data[i]<<endl;
    }
    lyx::lyxerr.setStream(cerr);
    lyx::support::os::init(argc, data);

    LyxGlobal::setMainWidget(this);
    lyx::LyX *lyx = lyxInstance;
    lyx->addMainWindowCreatedHandler(LyxWidget::insertLyxInMainWidget);
    lyx->setDocumentSavedHandlerHandler(LyxWidget::callDocumentSavedHandler);
    lyx->exec(argc, data);
    //startLyxGUI();
    QTimer::singleShot(0, this, SLOT(startLyxGUI()));
}

LyxWidget::~LyxWidget()
{
    delete ui;
}

void LyxWidget::insertLyxInMainWidget(lyx::frontend::GuiView* mainWindow)
{
    LyxGlobal::setLyxWindow(mainWindow);
    QWidget *lyxWindow = (QWidget*)mainWindow;//Окно LyX
    QWidget *mainWidget = widgetInstance;//Окно системы
    QLayout *l = new QVBoxLayout(mainWidget);
    l->addWidget(lyxWindow);
    mainWidget->setLayout(l);
    lyxWindow->connect(mainWindow, SIGNAL(closed()), widgetInstance, SIGNAL(lyxClosed()));
}

void LyxWidget::callDocumentSavedHandler()
{
    LyxWidget *lyxWidget = (LyxWidget*)widgetInstance;
    lyxWidget->documentSaved();
}

QWidget *LyxGlobal::mainWidget()
{
    LyxGlobal *lg = &lyx::Singleton<LyxGlobal>::Instance();
    return lg->p_mainWidget;
}

void LyxGlobal::setMainWidget(QWidget *widget)
{
    LyxGlobal *lg = &lyx::Singleton<LyxGlobal>::Instance();
    lg->p_mainWidget = widget;
}

void LyxGlobal::setLyxWindow(lyx::frontend::GuiView *mainWindow)
{
    LyxGlobal *lg = &lyx::Singleton<LyxGlobal>::Instance();
    lg->p_lyxWindow = mainWindow;
}

lyx::frontend::GuiView *LyxGlobal::lyxWindow()
{
    LyxGlobal *lg = &lyx::Singleton<LyxGlobal>::Instance();
    return lg->p_lyxWindow;
}
