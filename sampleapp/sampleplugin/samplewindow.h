#ifndef SAMPLEWINDOW_H
#define SAMPLEWINDOW_H

#include <QDialog>
#include <docgenwindow.h>

namespace Ui {
class SampleWindow;
}

class SampleWindow : public DocGenWindow
{
    Q_OBJECT

public:
    explicit SampleWindow(QWidget *parent =0);
    QString name();
    QString buttonName();
    QString description();
    void execute();
    ~SampleWindow();

private:
    Ui::SampleWindow *ui;

};

#endif // SAMPLEWINDOW_H
