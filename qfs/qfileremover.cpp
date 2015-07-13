#include "qfileremover.h"
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <iostream>

QFileRemover::QFileRemover(QObject *parent)
    :QObject(parent)
{

}

void QFileRemover::counter(QString path)
{
    counter(QFileInfo(path));
}

void QFileRemover::counter(QFileInfo entry)
{
    removeList.prepend(entry.absoluteFilePath());
    if (entry.isDir())
    {
        QString fileName = entry.absoluteFilePath();
        QDir dir(fileName);
        dir.setFilter(QDir::Files | QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        foreach (QFileInfo subEntry, dir.entryInfoList()) {
            counter(subEntry);
        }
        return;
    }
}

void QFileRemover::remove(QString path)
{
    counter(path);
    QDir dir;
    int i=0;
    oldPercent = 0;
    emit progressChanged(0);
    foreach (QString str, removeList) {
        float v = i*100.0/(float)removeList.count();
        int percent = (int)v;
        if (percent!=oldPercent)
        {
            oldPercent = percent;
            emit progressChanged(percent);
        }
        QFileInfo info(str);
        if (!info.exists())
        {
            i++;
            continue;
        }
        if (!info.isDir())
        {
            QFile file(str);
            if (!file.remove())
            {
                emit removeError("Ошибка при удалении файла", "Ошибка при удалении файла "+str);
            }
        } else {
            if (!dir.rmdir(str))
            {
                emit removeError("Ошибка при удалении каталога", "Ошибка при удалении файла "+str);
            }
        }
        i++;
    }
    emit progressChanged(100);
    emit removeSuccess();
}
