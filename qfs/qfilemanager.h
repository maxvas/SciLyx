#ifndef QFILEMANAGER_H
#define QFILEMANAGER_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QStringList>

class QFileRemover;

class QFileTask
{
public:
    QString task;
    QStringList args;
};

class QFileManager: public QObject
{
    Q_OBJECT
public:
    QFileManager(QObject *parent = 0);
private:
    QQueue< QFileTask* > queue;
    QFileRemover *remover;
    QThread thread;
    bool isBusy;

public slots:
    void remove(QString path);

private slots:
    void onThreadStarted();
    void newTask(QString task, QStringList args);
    void tryNextTask();
    void removerFinished();

signals:
    void removeError(QString error, QString details);
    void removeSuccess();
    void progressChanged(int value);
    void startSignal();

};

#endif // QFILEMANAGER_H
