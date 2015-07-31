#ifndef NEWDOCUMENT_H
#define NEWDOCUMENT_H

#include <QDialog>
#include <QDir>

namespace Ui {
class NewDocument;
}

class NewDocument : public QDialog
{
    Q_OBJECT

public:
    explicit NewDocument(QWidget *parent = 0);
    ~NewDocument();
    void generateFileName();
    void setFolder(QString path);


private slots:
    void on_okPB_clicked();

    void on_cancelPB_clicked();

    void on_fileNameLE_textChanged(const QString &arg1);

private:
    Ui::NewDocument *ui;
    QDir folder;

signals:
    void submit(QString fileName);
};

#endif // NEWDOCUMENT_H
