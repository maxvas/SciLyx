#ifndef SCILYX_H
#define SCILYX_H

#include <QWidget>

class GitBrowser;
class LyxWidget;
class QVBoxLayout;
class SciLyxPlugin;

class SciLyx : public QWidget
{
    Q_OBJECT
public:
    SciLyx(QString localRepoFolder);
    ~SciLyx();
    QString currentPath();
    bool registerAction(QString name, QAction *action);
    bool unregisterAction(QString name);
    void loadConfigOrAskUser();

public slots:
    bool openEditor(QString fileName);

private:
    GitBrowser *mGitBrowser;
    LyxWidget *mEditor;
    void closeEvent(QCloseEvent *event);
    void loadPlugins(QString pluginsPath);
    void unloadPlugins();
    QHash<QString, SciLyxPlugin* > mPlugins;
    QString sciLyxPath();

};

#endif // SCILYX_H