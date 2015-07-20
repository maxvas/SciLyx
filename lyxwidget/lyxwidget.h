#ifndef LYXWIDGET_H
#define LYXWIDGET_H

#include <QWidget>
#include <QModelIndex>
#include <QMainWindow>
#include <QThread>

namespace lyx{
    namespace frontend{
        class GuiView;
    }
}

namespace Ui
{
    class Lyx;
}

class LyxGlobal{
public:
    static QWidget *mainWidget();
    static void setMainWidget(QWidget* widget);
    static void setLyxWindow(lyx::frontend::GuiView* mainWindow);
    static lyx::frontend::GuiView *lyxWindow();

private:
    QWidget* p_mainWidget;
    lyx::frontend::GuiView *p_lyxWindow;
};

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

class LyxWidget : public QWidget
{
    Q_OBJECT
public:
    LyxWidget(QWidget *parent = 0);
    ~LyxWidget();
    static void insertLyxInMainWidget(lyx::frontend::GuiView* mainWindow);
    static void callDocumentSavedHandler();
    static void callConfigureStartedSignal();
    static void callConfigureFinishedSignal();
    int getPrefferedWidth() const;

private:
    Ui::Lyx * ui;
    void closeEvent(QCloseEvent *event);

signals:
    void lyxClosed();
    void documentSaved();
    void configureStarted();
    void configureFinished();
private slots:
    void exec();
    void startLyxGUI();
    void updateBuffer();

public slots:
    void openDocumentInNewWindow(QString fileName);
    void openOldDocumentInNewWindow(QString fileName, QString commit, QByteArray data);
};

#endif // LYXWIDGET_H
