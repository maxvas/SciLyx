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

void NewDocument::on_okPB_clicked()
{
    close();
}

void NewDocument::on_cancelPB_clicked()
{
    close();
}
