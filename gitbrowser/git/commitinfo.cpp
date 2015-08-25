#include "commitinfo.h"
#include <QStringList>
#include <QDebug>

CommitInfo::CommitInfo(QString data)
{
    QStringList ls = data.split("\n");
    commit = ls[0].trimmed();
    author = ls[1].split(": ")[1].trimmed();
    QString dateString = ls[2].split(": ")[1].trimmed();
    dateString = dateString.mid(0, dateString.length()-6);
    comment = ls[4].trimmed();
    //dt = QDateTime::fromString(dateString, "ddd MMM d HH:mm:ss yyyy");
    dt = QDateTime::fromString(dateString);
}
