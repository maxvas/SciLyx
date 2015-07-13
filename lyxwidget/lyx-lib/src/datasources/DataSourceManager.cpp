#include "DataSourceManager.h"

#include <QApplication>
#include <QDir>
#include <QPluginLoader>
#include <QDebug>
#include "AbstractDataSelector.h"

DataSourceManager::DataSourceManager(QObject *parent)
    :QObject(parent)
{
    QDir pluginsDir;
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        if (!fileName.startsWith("datasource"))
        {
            continue;
        }
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName), this);
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            AbstractDataSelector *dataSelector = qobject_cast<AbstractDataSelector *>(plugin);
            if (dataSelector)
            {
                dataSources[dataSelector->name()] = dataSelector;
                connect(dataSelector, SIGNAL(finished(LapTable*)), this, SIGNAL(dataSelected(LapTable*)));
            }
        } else {
            qDebug()<<"Error on loading data selector "<<fileName<<pluginLoader.errorString();
        }
    }
}

DataSourceManager::~DataSourceManager()
{
    foreach (AbstractDataSelector *selector, dataSources) {
        delete selector;
    }
}

QStringList DataSourceManager::list()
{
    return dataSources.keys();
}

void DataSourceManager::execute(QString name)
{
    if (!dataSources.contains(name)){
        return;
    }
    dataSources[name]->show();
}
