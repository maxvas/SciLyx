#ifndef REPOMANAGER_H
#define REPOMANAGER_H

#include <QDialog>

namespace Ui {
class RepoSettings;
}

class RepoParams
{
public:
    QString configFile();
    bool readFromConfig();
    bool saveToConfig();
    QString url;
    QString login;
    QString author;
    QString email;
};

class RepoSettings : public QDialog
{
    Q_OBJECT

public:
    explicit RepoSettings(QWidget *parent);
    ~RepoSettings();
    void startEdit(RepoParams *params);
    bool loadParams();

private:
    Ui::RepoSettings *ui;
    QString fileName;
    RepoParams *mParams;
    void updateView();
    bool isChanged();

signals:
    void changed();

private slots:
    void on_pbOk_clicked();
    void on_pbCancel_clicked();
};

#endif // REPOMANAGER_H
