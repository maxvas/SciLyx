#ifndef VERSIONBROWSERDIALOG_H
#define VERSIONBROWSERDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QTableWidgetItem>

class GitManager;
class CommitInfo;

namespace Ui {
class VersionBrowserDialog;
}

class VersionBrowserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VersionBrowserDialog(QWidget *parent, QString filePath, GitManager *git);
    ~VersionBrowserDialog();

private:
    Ui::VersionBrowserDialog *ui;
    GitManager *git;
    QList<CommitInfo* > commits;
    QString fileName;
    void clearCommmits();

signals:
    void openedOldFile(QString fileName, QString commit, QByteArray data);

private slots:
    void openOldFileSuccess(QString fileName, QString commit, QByteArray data);
    void openOldFileFailure(QString error, QString details);
    void logSuccess(QString data);
    void logFailure(QString error, QString details);
    void on_commitsTable_itemDoubleClicked(QTableWidgetItem *item);
};

#endif // VERSIONBROWSERDIALOG_H
