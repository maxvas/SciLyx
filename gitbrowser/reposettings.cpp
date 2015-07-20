#include "reposettings.h"
#include "ui_reposettings.h"
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QTextStream>

#define UNUSED(expr) do { (void)(expr); } while (0)

RepoSettings::RepoSettings(QWidget *parent) :
    QDialog(parent), ui(new Ui::RepoSettings)
{
    ui->setupUi(this);
}

RepoSettings::~RepoSettings()
{
    delete ui;
}

void RepoSettings::startEdit(RepoParams *params)
{
    mParams = params;
    updateView();
    show();
}

bool RepoSettings::loadParams()
{
    return mParams->readFromConfig();
}

void RepoSettings::updateView()
{
    ui->txtUrl->setText(mParams->url);
    ui->txtLogin->setText(mParams->login);
    ui->txtAuthor->setText(mParams->author);
    ui->txtEmail->setText(mParams->email);
}

bool RepoSettings::isChanged()
{
    return ui->txtUrl->text()!=mParams->url ||
            ui->txtLogin->text()!=mParams->login ||
            ui->txtAuthor->text()!=mParams->author ||
            ui->txtEmail->text()!=mParams->email;
}

QString RepoParams::configFile()
{
    QString home = QDir::homePath();
    return home + "/SciLyx/repo.conf";
}

bool RepoParams::readFromConfig()
{
    QString configFileName = configFile();
    QFile config(configFileName);
    if (!config.open(QFile::ReadOnly))
        return false;
    bool hasUrl = false, hasLogin = false, hasAuthor = false, hasEmail = false;
    while (!config.atEnd())
    {
        QString line = config.readLine();
        QStringList ls = line.split("=");
        if (ls.count()!=2)
        {
            return false;
        }
        QString varName = ls[0];
        QString varValue = ls[1];
        varName = varName.trimmed();
        varValue = varValue.trimmed();
        if (varName=="url")
        {
            url = varValue;
            hasUrl = true;
        }
        if (varName=="login")
        {
            login = varValue;
            hasLogin = true;
        }
        if (varName=="author")
        {
            author = varValue;
            hasAuthor = true;
        }
        if (varName=="email")
        {
            email = varValue;
            hasEmail = true;
        }
    }
    return hasUrl && hasLogin && hasAuthor && hasEmail;
}

bool RepoParams::saveToConfig()
{
    QDir sciLyxDir(QDir::homePath()+"/SciLyx");
    if (!sciLyxDir.exists())
    {
        QDir homeDir = QDir::home();
        if (!homeDir.mkdir("SciLyx"))
        {
            return false;
        }
    }
    QFile config(configFile());
    if (!config.open(QFile::WriteOnly))
        return false;
    QTextStream ostream(&config);
    ostream<<"url = "<<url<<"\r\n";
    ostream<<"login = "<<login<<"\r\n";
    ostream<<"author = "<<author<<"\r\n";
    ostream<<"email = "<<email<<"\r\n";
    config.close();
    return true;
}

void RepoSettings::on_pbOk_clicked()
{
    if (!isChanged())
    {
        close();
        return;
    }
    mParams->url = ui->txtUrl->text();
    mParams->login = ui->txtLogin->text();
    mParams->author = ui->txtAuthor->text();
    mParams->email = ui->txtEmail->text();
    mParams->saveToConfig();
    emit changed();
    close();
}

void RepoSettings::on_pbCancel_clicked()
{
    close();
}
