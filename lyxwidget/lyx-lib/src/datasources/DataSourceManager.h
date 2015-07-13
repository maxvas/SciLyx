#ifndef DATASOURCEMANAGER_H
#define DATASOURCEMANAGER_H

#include <QObject>
#include <QHash>
#include <QApplication>
#include <QDir>
#include <QPluginLoader>
#include "AbstractDataSelector.h"
#include "LapTable.h"

class DataSourceManager : public QObject
{
    Q_OBJECT

public:
    DataSourceManager(QObject *parent = 0);
    ~DataSourceManager();
    QStringList list();
    void execute(QString name);

Q_SIGNALS:
    void dataSelected(LapTable *table);

private:
    QHash<QString, AbstractDataSelector* > dataSources;
};

#endif // DATASOURCEMANAGER_H
