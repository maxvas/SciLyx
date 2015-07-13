#ifndef CSVFILE_H
#define CSVFILE_H

#include "ui_CsvFile.h"
#include <QObject>
#include "AbstractDataSelector.h"

class DataSourceCsvFile : public AbstractDataSelector, public Ui::DataSourceCsvFile
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.programstyle.eln.AbstractDataSelector")
    Q_INTERFACES(AbstractDataSelector)

public:
    DataSourceCsvFile();
    QString name();
    void closeEvent(QCloseEvent *e);

public slots:
private slots:
    void on_selectFileB_clicked();
    void on_okB_clicked();
    void on_cancelB_clicked();
};

#endif // CSVFILE_H
