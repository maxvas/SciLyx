#ifndef COMMITINFO_H
#define COMMITINFO_H

#include <QString>
#include <QDateTime>

//Класс, предназначенный для хранения данных о коммите
class CommitInfo
{
public:
    CommitInfo(QString data);
    QString fileName;
    QString commit;
    QDateTime dt;
    QString author;
    QString comment;

};

#endif // COMMITINFO_H
