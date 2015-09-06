#ifndef DOCGENWINDOW_H
#define DOCGENWINDOW_H

#include "scilyx.h"
#include <QString>
#include <QDialog>
#include "lyxgen.h"

class DocGenWindow : public QDialog
{
    Q_OBJECT
	
public:
    DocGenWindow(QWidget *parent = 0);
    virtual ~DocGenWindow();
    virtual QString name() = 0;
    virtual QString buttonName() = 0;
    virtual QString description() = 0;
    virtual void execute() = 0;
    SciLyx *scilyx();
private:
    friend class SciLyx;
	SciLyx *mSciLyx;
};

#endif // DOCGENWINDOW_H
