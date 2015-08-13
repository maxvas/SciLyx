#include "gitmanager.h"

#include <QDebug>
#include <QDir>
#include <QTextCodec>
#include <QProcess>
#include <QCoreApplication>
#include <iostream>

using namespace std;

GitManager::GitManager(QString workingDirectory) :
    QObject(0), inProcess(false), workingDirectory(workingDirectory)
{
    connect(&git, SIGNAL(finished(int)), this, SLOT(onFinished(int)));
    connect(&git, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(&git, SIGNAL(started()), this, SLOT(onStarted()));
    connect(&git, SIGNAL(error(QProcess::ProcessError)), this, SLOT(gitError(QProcess::ProcessError)));
    git.setProcessChannelMode(QProcess::MergedChannels);
    QString homeDir = QDir::homePath();
#ifdef _WIN32
    gitCommand = QDir::currentPath()+"/git/bin/git.exe";
#else
    gitCommand = "git";
#endif
}

void GitManager::setRepoParams(RepoParams *params)
{
    remote = params->login+"@"+params->url;
    //Устанавливаем переменные окружения
    QStringList env;
#ifdef _WIN32
    env<<"DISPLAY=10";
#endif
    env<<"GIT_AUTHOR_NAME="+params->author;
    env<<"GIT_AUTHOR_EMAIL="+params->email;
    env<<"GIT_COMMITTER_NAME="+params->author;
    env<<"GIT_COMMITTER_EMAIL="+params->email;
//#ifdef __unix__
//    env<<"GIT_SSH="+homeDir+"/lims/ssh";
//#endif
    git.setEnvironment(env);
}

GitManager::~GitManager()
{
    git.terminate();
}

//Запуск git
void GitManager::start(QStringList args)
{
    if (inProcess)
        return;
    if (args.count()==0)
    {
        return;
    } else
    {
        if (args[0]=="clone")
        {
            git.setWorkingDirectory(QCoreApplication::applicationDirPath());
        }else
        {
            git.setWorkingDirectory(workingDirectory);
        }
    }
    output.clear();
    QString command = gitCommand;
    foreach (QString arg, args) {
        command += QString(" ") + "\""+arg+"\"";
    }
    cout<<command.toStdString();
    git.start(gitCommand, args);
    git.setTextModeEnabled(true);
    this->command = args[0];
    inProcess = true;
}

//Слот для чтения стандартного вывода команды git
void GitManager::onReadyRead()
{
    QByteArray newData = git.readAll();
    newData += git.readAllStandardError();
    newData += git.readAllStandardOutput();
    if (command=="clone")
    {
        cout<<QString(newData).toStdString()<<"\n";
        cout.flush();
    }
    output += newData;
}

//Слот, вызываемый по завершении git
void GitManager::onFinished(int code)
{
    cout<<QString(output).toStdString()<<"\n";
    cout.flush();
    inProcess = false;
    if (command=="commit")
    {
        emit commitSuccess();
        return;
    }
    if (command=="push")
    {
        if (code==0)
            emit pushSuccess();
        else
            emit pushFailure("Ошибка при отправке изменений на сервер", QString::fromUtf8(output));
        return;
    }
    if (command=="pull")
    {
        if (code==0)
            emit pullSuccess();
        else
            emit pullFailure("Ошибка при получении изменений с сервера", QString::fromUtf8(output));
        return;
    }
    if (command=="add")
    {
        if (code==0)
            emit addSuccess();
        else
            emit addFailure("Ошибка при добавлении изменений в локальную базу данных (репозиторий)", QString::fromUtf8(output));
    }
    if (command=="show")
    {
        if (code==0)
            emit showSuccess(workingDirectory+"/"+temp_show_fileName, temp_show_commit, output);
        else
            emit showFailure("Ошибка при получении старой версии файла из репозитория", QString::fromUtf8(output));
    }
    if (command=="log")
    {
        if (code==0)
            emit logSuccess(QString::fromUtf8(output));
        else
            emit logFailure("Ошибка при получении списка версий файла", QString::fromUtf8(output));
    }
    if (command=="clone")
    {
        if (code==0)
            emit cloneSuccess();
        else
            emit cloneFailure("Ошибка при клонировании репозитория", QString::fromUtf8(output));
    }
    if (command=="remote")
    {
        if (code==0)
            emit remoteSuccess(QString::fromUtf8(output));
        else
            emit remoteFailure("Ошибка при получении информации о репозитории", QString::fromUtf8(output));
    }
}

void GitManager::onRemoteShowOriginSuccess(QString output)
{
    disconnect(this, SIGNAL(remoteSuccess(QString)), this, SLOT(onRemoteShowOriginSuccess(QString)));
    disconnect(this, SIGNAL(remoteFailure(QString,QString)), this, SLOT(onRemoteShowOriginFailure(QString,QString)));
    QStringList ls = output.split("\n");
    if (ls.length()<3)
    {
        emit checkRemoteAddrFailure("Ошибка", output);
    }
    QString fetchUrl = ls[1].trimmed();
    if (!fetchUrl.startsWith("Fetch URL: "))
    {
        emit checkRemoteAddrFailure("Ошибка", output);
    }
    fetchUrl = fetchUrl.mid(11);
    if (temp_checkRemoteAddr_remote==fetchUrl)
        emit checkRemoteAddrSuccess();
    else
        emit checkRemoteAddrFailure("Ошибка проверки адреса репозитория", output);
}

void GitManager::onRemoteShowOriginFailure(QString error, QString details)
{
    disconnect(this, SIGNAL(remoteSuccess(QString)), this, SLOT(onRemoteShowOriginSuccess(QString)));
    disconnect(this, SIGNAL(remoteFailure(QString,QString)), this, SLOT(onRemoteShowOriginFailure(QString,QString)));
    emit checkRemoteAddrFailure("Ошибка проверки адреса репозитория: "+error, details);
}

void GitManager::gitError(QProcess::ProcessError error)
{
    qDebug()<<"Error!";
    QString str1 = git.errorString();
    QTextCodec *ibmCodec = QTextCodec::codecForName("CP1251");
    qDebug()<<ibmCodec->fromUnicode(str1);
}

void GitManager::pull()
{
    start(QStringList()<<"pull");
}

void GitManager::push()
{
    start(QStringList()<<"push"<<"origin"<<"master");
}

void GitManager::commit()
{
    start(QStringList()<<"commit"<<"-m"<<"\"commit\"");
}

void GitManager::add()
{
    start(QStringList()<<"add"<<"-A"<<".");
}

void GitManager::showOldFile(QString fileName, QString commit)
{
    temp_show_fileName = fileName;
    temp_show_commit = commit;
    start(QStringList()<<"show"<<commit+":"+fileName);
}

void GitManager::log(QString fileName)
{
    start(QStringList()<<"log"<<"--"<<fileName);
}

void GitManager::clone(QString remote)
{
#ifdef __unix__
    QString homeDir = QDir::homePath();
    QFileInfo sshInfo(homeDir+"/lims/ssh");
    if (!sshInfo.exists())
    {
        QFile ssh(homeDir+"/lims/ssh");
        ssh.open(QFile::ReadWrite);
        ssh.write("ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no $*");
        ssh.flush();
        ssh.close();
        ssh.setPermissions(QFile::ExeOther | QFile::ExeUser | QFile::ExeOwner | QFile::ExeGroup | QFile::WriteUser | QFile::ReadUser | QFile::ReadOther | QFile::ReadOwner);
    }
#endif
    start(QStringList()<<"clone"<<remote<<workingDirectory<<"--progress");
}

void GitManager::remoteShowOrigin()
{
    start(QStringList()<<"remote"<<"show"<<"origin");
}

void GitManager::checkRemoteAddr(QString remote)
{
    connect(this, SIGNAL(remoteSuccess(QString)), this, SLOT(onRemoteShowOriginSuccess(QString)));
    connect(this, SIGNAL(remoteFailure(QString,QString)), this, SLOT(onRemoteShowOriginFailure(QString,QString)));
    temp_checkRemoteAddr_remote = remote;
}

void GitManager::onStarted()
{
    if (command=="clone")
    {
        git.write("yes\n");
    }
}
