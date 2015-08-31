#include "scilyx.h"

#include <QDebug>
#include <QVBoxLayout>
#include "gitbrowser/filesbrowser.h"
#include "lyxwidget/lyxwidget.h"
#include "scilyxplugin.h"
#include <QTimer>
#include <QCloseEvent>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QApplication>
#include <QQuickView>
#include <QDeclarativeView>
#include <QUrl>
#include <QQmlComponent>
#include <QQmlEngine>

SciLyx::SciLyx(QString localRepoFolder)
    : QWidget(0)/*, mEditor(0)*/
{
    QString sciLyxVar = sciLyxPath();
#ifdef _WIN32
    sciLyxVar = sciLyxVar.replace("/", "\\");
#endif
//    QString pathEnv = QString::fromLocal8Bit(qgetenv("Path"));
//    QString pathSuffix = QString(";%1\\Python27;%1\\miktex\\bin;%1\\ImageMagick-6.9.0-Q16;%1\\gs\\gs9.15\\bin").arg(sciLyxVar);
//    pathEnv += pathSuffix;
//    qputenv("Path", pathEnv.toLocal8Bit());
    qputenv("scilyx", sciLyxVar.toLocal8Bit());

    mGitBrowser = new GitBrowser(localRepoFolder);
    mEditor = new LyxWidget(this);
    mEditor->hide();
    connect(mGitBrowser, SIGNAL(openEditorTriggered(QString)), mEditor, SLOT(openDocumentInNewWindow(QString)));
    connect(mGitBrowser, SIGNAL(openOldVersionInEditorTriggered(QString,QString,QByteArray)), mEditor, SLOT(openOldDocumentInNewWindow(QString,QString,QByteArray)));
    connect(mEditor, SIGNAL(documentSaved()), mGitBrowser, SLOT(commitChanges()));
    connect(mEditor, SIGNAL(configureStarted()), mGitBrowser, SLOT(configureStarted()));
    connect(mEditor, SIGNAL(configureFinished()), mGitBrowser, SLOT(configureFinished()));
    QVBoxLayout *layout = new QVBoxLayout(this);
    this->setLayout(layout);
    layout->addWidget(mGitBrowser);
    layout->setMargin(0);
    setWindowTitle("SciLyx");
}

SciLyx::~SciLyx()
{
    mGitBrowser->deleteLater();
    mEditor->deleteLater();
    unloadPlugins();
}

QString SciLyx::currentPath()
{
    return mGitBrowser->currentAbsolutePath();
}

bool SciLyx::registerAction(QString name, QAction *action)
{
    return mGitBrowser->registerAction(name, action);
}

bool SciLyx::unregisterAction(QString name)
{
    return mGitBrowser->unregisterAction(name);
}

bool SciLyx::loadConfig()
{
    return mGitBrowser->readRepoConfig();
}

void SciLyx::loadConfigOrAskUser()
{
    mGitBrowser->readRepoConfigOrAskUser();
}

void SciLyx::clearToolBar()
{
    mGitBrowser->clearToolBar();
}

void SciLyx::addToolBarAction(QString name)
{
    mGitBrowser->addToolBarAction(name);
}

void SciLyx::addToolBarSeparator()
{
    mGitBrowser->addToolBarSeparator();
}

bool SciLyx::openEditor(QString fileName)
{
    return mGitBrowser->openEditor(fileName);
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

QString SciLyx::sciLyxPath()
{
    QString sciLyxPath = QCoreApplication::applicationDirPath();
    QFile pathes("pathes.conf");
    if (!pathes.exists())
    {
        return sciLyxPath;
    }
    if (!pathes.open(QFile::ReadOnly))
    {
        return sciLyxPath;
    }
    while(!pathes.atEnd())
    {
        QString line = QString::fromUtf8(pathes.readLine()).trimmed();
        if (line.length()==0)
            continue;
        QStringList ls = line.split("=");
        if (ls.length()!=2)
        {
            return sciLyxPath;
        }
        ls[0] = ls[0].trimmed();
        if (ls[0]=="scilyx")
        {
            sciLyxPath = ls[1].trimmed();
        }
    }
    return sciLyxPath;
}
