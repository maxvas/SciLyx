#ifndef MATPLUGIN_H
#define MATPLUGIN_H

#include <scilyxplugin.h>
#include <QString>
#include <QtWidgets/QDialog>

class SciLyx;

class MatPlugin : public SciLyxPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.scilyx.SciLyxPlugin")
    Q_INTERFACES(SciLyxPlugin)
	
public:
    ~MatPlugin();
    QString name();
    bool init();
};

#endif // MATPLUGIN
