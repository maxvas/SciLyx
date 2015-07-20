#ifndef SYNCDIALOG_H
#define SYNCDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QProgressBar>

class SyncDialog : public QLabel
{
    Q_OBJECT

public:
    explicit SyncDialog(QWidget *parent = 0);
    ~SyncDialog();

private:
    QLabel *titleLabel;
    QMovie *syncImage;
    QLabel *iconLabel;
    QProgressBar *progress;

public slots:
    void start(QString titleLabel, bool withProgressBar = false);
    void stop();
    void setProgress(int value);
    void showError(QString errorString);
    void clear();
};

#endif // SYNCDIALOG_H