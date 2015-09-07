#ifndef SCILYXPLUGIN_H
#define SCILYXPLUGIN_H

#include <QString>
#include <QtWidgets/QDialog>

class SciLyx;

class SciLyxPlugin : public QDialog
{
    Q_OBJECT
	
public:
    virtual ~SciLyxPlugin();
    virtual QString name() = 0;
    virtual bool init() = 0;
    SciLyx *scilyx()
    {
        return mSciLyx;
    }
private:
    friend class SciLyx;
    void setEnviroment(SciLyx *scilyx);
	SciLyx *mSciLyx;
};

QT_BEGIN_NAMESPACE
    Q_DECLARE_INTERFACE(SciLyxPlugin, "org.scilyx.SciLyxPlugin")
QT_END_NAMESPACE
#endif // SCILYXPLUGIN_H
