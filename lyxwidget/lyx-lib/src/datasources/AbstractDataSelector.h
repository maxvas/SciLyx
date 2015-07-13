#ifndef ABSTRACTDATASELECTOR_H
#define ABSTRACTDATASELECTOR_H

#include <QString>
#include <QtWidgets/QDialog>
#include "LapTable.h"

class AbstractDataSelector : public QDialog
{
    Q_OBJECT
public:
    virtual QString name() = 0;

Q_SIGNALS:
    void finished(LapTable *table);
};

QT_BEGIN_NAMESPACE
    Q_DECLARE_INTERFACE(AbstractDataSelector, "org.programstyle.eln.AbstractDataSelector")
QT_END_NAMESPACE
#endif // ABSTRACTDATASELECTOR_H
