#include "scilyxplugin.h"
#include "scilyx.h"

void SciLyxPlugin::setEnviroment(SciLyx *scilyx)
{
    mSciLyx = scilyx;
    init();
}
