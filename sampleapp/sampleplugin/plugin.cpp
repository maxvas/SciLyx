#include "plugin.h"
#include "samplewindow.h"

SamplePlugin::~SamplePlugin()
{

}

QString SamplePlugin::name()
{
    return "sample-plugin";
}

bool SamplePlugin::init()
{
	SampleWindow *win = new SampleWindow;
    scilyx()->addDocGenWindow(win);
    return true;
}
