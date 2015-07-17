#ifndef SCILYX_H
#define SCILYX_H

#include <QWidget>

class FilesBrowser;
class LyxWidget;
class QVBoxLayout;
class SciLyxPlugin;

class SciLyx : public QWidget
{
    Q_OBJECT
public:
    SciLyx(QString remote, QString pass, QString localRepoFolder, QString author, QString email);
    ~SciLyx();
    QString currentPath();
    bool registerAction(QString name, QAction *action);
    bool unregisterAction(QString name);

public slots:
    bool openEditor(QString fileName);

private:
    FilesBrowser *mFileManager;
    LyxWidget *mEditor;
    void closeEvent(QCloseEvent *event);
    void loadPlugins(QString pluginsPath);
    void unloadPlugins();
    QHash<QString, SciLyxPlugin* > mPlugins;
    QString sciLyxPath();

};

#endif // SCILYX_H
