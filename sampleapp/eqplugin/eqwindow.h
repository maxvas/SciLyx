#ifndef EQWINDOW_H
#define EQWINDOW_H

#include <QDialog>
#include <docgenwindow.h>

namespace Ui {
class EqWindow;
}

class EqWindow : public DocGenWindow
{
    Q_OBJECT

public:
    explicit EqWindow(QWidget *parent =0);
    QString name();
    QString buttonName();
    QString description();
    void execute();
    ~EqWindow();

private:
    Ui::EqWindow *ui;
    QString eqname;

private slots:
    void ok();

};

#endif // EQWINDOW_H
