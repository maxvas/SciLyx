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

#endif // QFILEREMOVER_H
