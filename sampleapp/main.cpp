#include <QApplication>
#include <QDir>
#include <scilyx.h>
#include <gitbrowser/filesbrowser.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QString remoteRepo = "max@192.168.2.10:/home/max/lims/documents.git";
//    QString pass = "kalamantin";
//    QString author = "Max";
//    QString email = "max-vas@bk.ru";
//    QString remoteRepo = "limsadmin@192.168.0.14:/home/limsadmin/lims/documents.git";
//    QString pass = "Qweasd789";
//    QString author = "limsadmin";
//    QString email = "carpovpv@mail.ru";
    QString localRepo = QDir::homePath()+"/lims/documents";
    SciLyx f4(localRepo);
    f4.show();
    f4.loadConfigOrAskUser();
    return a.exec();
//    FilesBrowser fb("", "", "", "", "");
//    fb.show();
//    return a.exec();

}
