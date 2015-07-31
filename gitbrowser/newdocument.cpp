#include "newdocument.h"
#include "ui_newdocument.h"

NewDocument::NewDocument(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDocument)
{
    ui->setupUi(this);
}

NewDocument::~NewDocument()
{
    delete ui;
}

void NewDocument::generateFileName()
{
    ui->fileNameLE->setText("Новый документ");
    ui->fileNameLE->selectAll();
}

void NewDocument::setFolder(QString path)
{
    folder.setPath(path);
}

void NewDocument::on_okPB_clicked()
{
    close();
    emit submit(ui->fileNameLE->text()+".lyx");
}

void NewDocument::on_cancelPB_clicked()
{
    close();
}

void NewDocument::on_fileNameLE_textChanged(const QString &arg1)
{
    QString fileName = ui->fileNameLE->text()+".lyx";
    QFile file(folder.path()+"/"+fileName);
    if (file.exists())
    {
        ui->fileNameLE->setStyleSheet("color: #FF0000");
    } else
    {
        ui->fileNameLE->setStyleSheet("color: #000000");
    }
}
