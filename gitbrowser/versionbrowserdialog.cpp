#include "versionbrowserdialog.h"
#include "ui_versionbrowserdialog.h"
#include <QProcess>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <QList>
#include "commitinfo.h"
#include "gitmanager.h"

VersionBrowserDialog::VersionBrowserDialog(QWidget *parent, QString filePath, GitManager *git) :
    QDialog(parent),
    ui(new Ui::VersionBrowserDialog),
    git(git),
    fileName(filePath)
{
    ui->setupUi(this);
    connect(git, SIGNAL(logSuccess(QString)), this, SLOT(logSuccess(QString)));
    connect(git, SIGNAL(logFailure(QString,QString)), this, SLOT(logFailure(QString,QString)));
    git->log(filePath);
    ui->commitsTable->setColumnCount(2);
    ui->commitsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->commitsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->commitsTable->setHorizontalHeaderLabels(QStringList()<<"Дата изменения"<<"Автор");
    ui->commitsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

VersionBrowserDialog::~VersionBrowserDialog()
{
    delete ui;
    clearCommmits();
}

void VersionBrowserDialog::clearCommmits()
{
    foreach (CommitInfo *info, commits) {
        delete info;
    }
}

void VersionBrowserDialog::openOldFileSuccess(QString fileName, QString commit, QByteArray data)
{
    close();
    disconnect(git, SIGNAL(showSuccess(QString,QString,QByteArray)), this, SLOT(openOldFileSuccess(QString,QString,QByteArray)));
    disconnect(git, SIGNAL(showFailure(QString,QString)), this, SLOT(openOldFileFailure(QString,QString)));
    emit openedOldFile(fileName, commit, data);
}

void VersionBrowserDialog::openOldFileFailure(QString error, QString details)
{
    disconnect(git, SIGNAL(showSuccess(QString,QString,QByteArray)), this, SLOT(openOldFileSuccess(QString,QString,QByteArray)));
    disconnect(git, SIGNAL(showFailure(QString,QString)), this, SLOT(openOldFileFailure(QString,QString)));
    //TODO: сделать обработку ошибки
}

void VersionBrowserDialog::logSuccess(QString data)
{
    QStringList ls = data.split("commit ");
    foreach (QString str, ls) {
        if (str.length()==0)
            continue;
        commits.push_front(new CommitInfo(str));
    }
    ui->commitsTable->setRowCount(commits.count());
    for (int i=0; i<commits.count(); i++)
    {
        ui->commitsTable->setRowHeight(i, 25);
        QTableWidgetItem *it = new QTableWidgetItem(commits[i]->dt.toString("yyyy-MM-dd HH:mm:ss"));
        ui->commitsTable->setItem(i, 0, it);
        it = new QTableWidgetItem(commits[i]->author);
        ui->commitsTable->setItem(i, 1, it);
    }
}

void VersionBrowserDialog::on_commitsTable_itemDoubleClicked(QTableWidgetItem *item)
{
    connect(git, SIGNAL(showSuccess(QString,QString,QByteArray)), this, SLOT(openOldFileSuccess(QString,QString,QByteArray)));
    connect(git, SIGNAL(showFailure(QString,QString)), this, SLOT(openOldFileFailure(QString,QString)));

    CommitInfo* commitInfo = commits[item->row()];
    git->showOldFile(fileName, commitInfo->commit);
}

void VersionBrowserDialog::logFailure(QString error, QString details)
{
    //TODO: сделать обработку ошибки
}
