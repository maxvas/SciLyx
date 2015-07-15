#include "scilyx.h"

#include <QDebug>
#include <QVBoxLayout>
#include "gitbrowser/filesbrowser.h"
#include "lyxwidget/lyxwidget.h"
#include "scilyxplugin.h"
#include <QTimer>
#include <QCloseEvent>
#include <QPluginLoader>

SciLyx::SciLyx(QString remote, QString pass, QString localRepoFolder, QString author, QString email)
    : QWidget(0), mEditor(0)
{
    mFileManager = new FilesBrowser(remote, pass, localRepoFolder, author, email, this);
    mEditor = new LyxWidget(this);
    mEditor->hide();
    connect(mFileManager, SIGNAL(openEditorTriggered(QString)), mEditor, SLOT(openDocumentInNewWindow(QString)));
    connect(mFileManager, SIGNAL(openOldVersionInEditorTriggered(QString,QString,QByteArray)), mEditor, SLOT(openOldDocumentInNewWindow(QString,QString,QByteArray)));
    connect(mEditor, SIGNAL(documentSaved()), mFileManager, SLOT(commitChanges()));
    connect(mEditor, SIGNAL(configureStarted()), mFileManager, SLOT(configureStarted()));
    connect(mEditor, SIGNAL(configureFinished()), mFileManager, SLOT(configureFinished()));
    QVBoxLayout *layout = new QVBoxLayout(this);
    this->setLayout(layout);
    layout->addWidget(mFileManager);
    layout->setMargin(0);
    loadPlugins("");
}

SciLyx::~SciLyx()
{
    mFileManager->deleteLater();
    mEditor->deleteLater();
    unloadPlugins();
}

QString SciLyx::currentPath()
{
    return mFileManager->currentAbsolutePath();
}

bool SciLyx::registerAction(QString name, QAction *action)
{
    return mFileManager->registerAction(name, action);
}

bool SciLyx::unregisterAction(QString name)
{
    return mFileManager->unregisterAction(name);
}

bool SciLyx::openEditor(QString fileName)
{
    return mFileManager->openEditor(fileName);
}

void SciLyx::
closeEvent(QCloseEvent *event)
{
    if (!mEditor->close())
    {
        event->ignore();
    }
    else
    {
        QWidget::closeEvent(event);
    }
}

void SciLyx::loadPlugins(QString pluginsPath)
{
    QDir pluginsDir(pluginsPath);
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        if (!fileName.startsWith("sl_"))
        {
            continue;
        }
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName), this);
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            SciLyxPlugin *slPlugin = qobject_cast<SciLyxPlugin *>(plugin);
            if (slPlugin)
            {
                if (mPlugins.contains(slPlugin->name()))
                {
                    qDebug()<<"Error on loading SciLyxPlugin "<<fileName<<". Plugin name "<<slPlugin->name()<<"has already registered.";
                    continue;
                }else
                {
                    mPlugins[slPlugin->name()] = slPlugin;
                    slPlugin->setEnviroment(this);
                }
            }
        } else {
            qDebug()<<"Error on loading SciLyxPlugin "<<fileName<<". "<<pluginLoader.errorString();
        }
    }
}

void SciLyx::unloadPlugins()
{
    foreach (SciLyxPlugin *plugin, mPlugins.values()) {
        mPlugins.remove(plugin->name());
        delete plugin;
    }
}
