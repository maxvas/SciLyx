#ifndef SCILYX_H
#define SCILYX_H

#include <QWidget>

class GitBrowser;
class LyxWidget;
class QVBoxLayout;
class SciLyxPlugin;
class DocGenWindow;

class SciLyx : public QWidget
{
    Q_OBJECT
public:
    SciLyx(QString localRepoFolder);
    ~SciLyx();
    QString currentPath();
    bool registerAction(QString name, QAction *action);
    bool unregisterAction(QString name);
    bool loadConfig();
    void loadConfigOrAskUser();
    void clearToolBar();
    void addToolBarAction(QString name);
    void addToolBarSeparator();
    void loadPlugins(QString pluginsPath = "");
    void unloadPlugins();
    void addDocGenWindow(DocGenWindow* win);
    void insertDocumentFragment(QByteArray lyx);

public Q_SLOTS:
    bool openEditor(QString fileName);
    void showDocGenWindow(QString name);

private:
    GitBrowser *mGitBrowser;
    LyxWidget *mEditor;
    void closeEvent(QCloseEvent *event);
    QHash<QString, SciLyxPlugin* > mPlugins;
    QString sciLyxPath();

};

#endif // SCILYX_H
