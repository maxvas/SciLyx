#include <QApplication>
#include <QRect>
#include <QDir>
#include <QDesktopWidget>
#include <scilyx.h>
#include <gitbrowser/filesbrowser.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString localRepo = QDir::homePath()+"/lims/documents";
    SciLyx sciLyx(localRepo);
    sciLyx.loadPlugins("scilyx_plugins");
    sciLyx.loadConfigOrAskUser();
    sciLyx.addToolBarAction("settings");
    sciLyx.addToolBarAction("new-document");
    sciLyx.setMinimumSize(600, 400);
    sciLyx.show();
    return a.exec();
}
