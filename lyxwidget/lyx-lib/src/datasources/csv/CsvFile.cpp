#include "CsvFile.h"

#include "QtWidgets/QDialog"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <sstream>

DataSourceCsvFile::DataSourceCsvFile()
    :AbstractDataSelector()
{
    setupUi(this);
    columnSeparatorCB->addItem("Табуляция", "\t");
    columnSeparatorCB->addItem("Пробел", " ");
    decimalSeparatorCB->addItem("Точка", ".");
    decimalSeparatorCB->addItem("Запятая", ",");
}

QString DataSourceCsvFile::name()
{
    return "Из *.csv файла";
}

void DataSourceCsvFile::closeEvent(QCloseEvent *e)
{
    (void)e;
    textBrowser->setPlainText("");
}

void DataSourceCsvFile::on_selectFileB_clicked()
{
    QLocale::setDefault(QLocale::Russian);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть CSV файл"), "", tr("CSV файлы (*.csv *.txt)"));
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::warning(this,
                             "Ошибка чтения файла",
                             "Невозможно открыть файл на чтение. Возможно, у Вас нет соответствующих прав, или файл не существует",
                             "OK");
        return;
    }
    textBrowser->setPlainText(file.readAll());
}

void DataSourceCsvFile::on_okB_clicked()
{
    QString columnSeparator = columnSeparatorCB->currentData().toString();
    QString decimalSeparator = decimalSeparatorCB->currentData().toString();
    QLocale locale = QLocale("English");
    if (decimalSeparator==",")
    {
        locale = QLocale("Russian");
    }
    QStringList ls = textBrowser->toPlainText().split("\n");
    int column = 0;
    int maxColumn = 0;
    int row = 0;
    foreach(QString line, ls)
    {
        row++;
        QStringList values = line.split(columnSeparator);
        column = 0;
        foreach (QString value, values) {
            column++;
            if (column>maxColumn)
            {
                maxColumn = column;
            }
        }
    }
    LapTable *table = new LapTable(maxColumn, row);
    row = 0;
    foreach(QString line, ls)
    {
        QStringList values = line.split(columnSeparator);
        column = 0;
        foreach (QString value, values) {
            table->data[row][column] = QVariant(locale.toDouble(value));
            column++;
        }
        row++;
    }
    close();
    emit finished(table);
}

void DataSourceCsvFile::on_cancelB_clicked()
{
    close();
}
