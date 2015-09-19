#include "matplugin.h"
#include "matwindow.h"

MatPlugin::~MatPlugin()
{

}

QString MatPlugin::name()
{
    return "material-chooser-plugin";
}

bool MatPlugin::init()
{
    MatWindow *win = new MatWindow(this);
    scilyx()->addDocGenWindow(win);

    return true;
}
