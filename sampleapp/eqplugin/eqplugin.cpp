#include "eqplugin.h"
#include "eqwindow.h"

EqPlugin::~EqPlugin()
{

}

QString EqPlugin::name()
{
    return "equipment-chooser-plugin";
}

bool EqPlugin::init()
{
    EqWindow *win = new EqWindow(this);
    scilyx()->addDocGenWindow(win);
    return true;
}
