#ifndef SAMPLEPLUGIN_H
#define SAMPLEPLUGIN_H

#include <scilyxplugin.h>
#include <QString>
#include <QtWidgets/QDialog>

class SciLyx;

class SamplePlugin : public SciLyxPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.scilyx.SciLyxPlugin")
    Q_INTERFACES(SciLyxPlugin)
	
public:
    ~SamplePlugin();
    QString name();
    bool init();
};

#endif // SAMPLEPLUGIN_H
