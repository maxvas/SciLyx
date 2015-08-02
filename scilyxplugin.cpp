#include "scilyxplugin.h"
#include "scilyx.h"

SciLyxPlugin::~SciLyxPlugin()
{

}

void SciLyxPlugin::setEnviroment(SciLyx *scilyx)
{
    mSciLyx = scilyx;
    init();
}
