#ifndef MATWINDOW_H
#define MAtWINDOW_H

#include <QDialog>
#include <docgenwindow.h>

namespace Ui {
class MatWindow;
}

class MatWindow : public DocGenWindow
{
    Q_OBJECT

public:
    explicit MatWindow(QWidget *parent =0);
    QString name();
    QString buttonName();
    QString description();
    void execute();
    ~MatWindow();

private:
    Ui::MatWindow *ui;
    QString material;

private slots:
    void ok();

};

#endif // MAtWINDOW_H
