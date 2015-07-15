#ifndef CONFIGUREDIALOG_H
#define CONFIGUREDIALOG_H

#include <QDialog>

namespace Ui
{
    class ConfigureDialog;
}

class ConfigureDialog : public QDialog
{
    Q_OBJECT
public:
    ConfigureDialog(QWidget *parent = 0);
    ~ConfigureDialog();

private:
    Ui::ConfigureDialog * ui;
    void closeEvent(QCloseEvent *event);

};

#endif // CONFIGUREDIALOG_H
