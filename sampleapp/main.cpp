#include <QApplication>
#include <QDir>
#include <scilyx.h>
#include <gitbrowser/filesbrowser.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString localRepo = QDir::homePath()+"/lims/documents";
    SciLyx sciLyx(localRepo);
    sciLyx.loadPlugins();
    sciLyx.loadConfigOrAskUser();
    sciLyx.addToolBarAction("settings");
    sciLyx.addToolBarAction("new-document");
    sciLyx.show();
    return a.exec();

}
