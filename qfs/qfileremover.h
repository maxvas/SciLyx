#ifndef QFILEREMOVER_H
#define QFILEREMOVER_H

#include <QObject>
#include <QFileInfo>
#include <QVector>
#include <QThread>

class QFSThread: public QThread
{
public:
    void run();
};

class QFileRemover: public QObject
{
    Q_OBJECT
public:
    QFileRemover(QObject *parent = 0);
private:
    void counter(QString path);
    void counter(QFileInfo entry);
    QVector<QString> removeList;
    int oldPercent;

public slots:
    void remove(QString path);

signals:
    void progressChanged(int value);
    void removeError(QString error, QString details);
    void removeSuccess();
    void finished();
};

#endif // QFILEREMOVER_H
