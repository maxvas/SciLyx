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
    sciLyx.addToolBarSeparator();
    sciLyx.addToolBarAction("settings");
    sciLyx.show();
    sciLyx.loadConfigOrAskUser();
    return a.exec();

}
