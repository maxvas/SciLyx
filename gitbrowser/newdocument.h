#ifndef NEWDOCUMENT_H
#define NEWDOCUMENT_H

#include <QDialog>

namespace Ui {
class NewDocument;
}

class NewDocument : public QDialog
{
    Q_OBJECT

public:
    explicit NewDocument(QWidget *parent = 0);
    ~NewDocument();


private slots:
    void on_okPB_clicked();

    void on_cancelPB_clicked();

private:
    Ui::NewDocument *ui;
};

#endif // NEWDOCUMENT_H
