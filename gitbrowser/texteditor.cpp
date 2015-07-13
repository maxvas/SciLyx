#include "texteditor.h"
#include "ui_texteditor.h"
#include <QFile>

#define UNUSED(expr) do { (void)(expr); } while (0)

TextEditor::TextEditor(QString fileName) :
    QMainWindow(0), ui(new Ui::TextEditor), fileName(fileName)
{
    ui->setupUi(this);
    QFile file(fileName);
    if (!file.exists())
    {
        return;
    }
    if (file.open(QFile::ReadOnly))
    {
        ui->textEdit->setText(file.readAll());
        file.close();
    }
}

TextEditor::~TextEditor()
{
    delete ui;
}

void TextEditor::on_actSave_triggered()
{
    QFile file(fileName);
    if (!file.exists())
        return;
    if (file.open(QFile::ReadWrite))
    {
        file.write(ui->textEdit->toPlainText().toUtf8());
        file.close();
    }
    emit saved(fileName);
}

void TextEditor::closeEvent(QCloseEvent *event)
{
    UNUSED(event);
    this->deleteLater();
}
