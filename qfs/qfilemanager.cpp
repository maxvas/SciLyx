#include "qfilemanager.h"
#include "qfileremover.h"
#include <QStringList>

QFileManager::QFileManager(QObject *parent)
    :QObject(parent), isBusy(false)
{
    thread.start();
    connect(&thread, SIGNAL(started()), this, SLOT(onThreadStarted()));
}

void QFileManager::newTask(QString task, QStringList args)
{
    QFileTask *t = new QFileTask;
    t->task = task;
    t->args = args;
    queue.enqueue(t);
    tryNextTask();
}

void QFileManager::remove(QString path)
{
    newTask("remove", QStringList()<<path);
}

void QFileManager::onThreadStarted()
{
    remover = new QFileRemover;
    remover->moveToThread(&thread);
    connect(remover, SIGNAL(removeError(QString,QString)), this, SIGNAL(removeError(QString,QString)), Qt::QueuedConnection);
    connect(remover, SIGNAL(removeSuccess()), this, SIGNAL(removeSuccess()), Qt::QueuedConnection);
}

void QFileManager::tryNextTask()
{
    if (isBusy)
        return;
    if (queue.isEmpty())
        return;
    QFileTask *t = queue.dequeue();
    if (t->task=="remove")
    {
        isBusy = true;
        connect(remover, SIGNAL(progressChanged(int)), this, SIGNAL(progressChanged(int)), Qt::QueuedConnection);
        connect(remover, SIGNAL(removeError(QString,QString)), this, SLOT(removerFinished()), Qt::QueuedConnection);
        connect(remover, SIGNAL(removeSuccess()), this, SLOT(removerFinished()), Qt::QueuedConnection);
        QMetaObject::invokeMethod(remover, "remove", Qt::QueuedConnection, Q_ARG(QString, t->args[0]));
    }
}

void QFileManager::removerFinished()
{
    disconnect(remover, SIGNAL(progressChanged(int)), this, SIGNAL(progressChanged(int)));
    isBusy = false;
    tryNextTask();
}
