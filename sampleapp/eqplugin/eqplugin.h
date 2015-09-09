#ifndef EQPLUGIN_H
#define EQPLUGIN_H

#include <scilyxplugin.h>
#include <QString>
#include <QtWidgets/QDialog>

class SciLyx;

class EqPlugin : public SciLyxPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.scilyx.SciLyxPlugin")
    Q_INTERFACES(SciLyxPlugin)
	
public:
    ~EqPlugin();
    QString name();
    bool init();
};

#endif // EQPLUGIN
