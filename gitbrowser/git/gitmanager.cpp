#include "gitmanager.h"

#include <QDebug>
#include <QDir>
#include <QTextCodec>
#include <QProcess>
#include <QCoreApplication>
#include <iostream>
#include "shell.h"

using namespace std;

GitManager::GitManager(QString workingDirectory) :
    QObject(0), inProcess(false), workingDirectory(workingDirectory)
{
    console = new Shell(workingDirectory);
    connect(console, SIGNAL(finished(int,QByteArray)), this, SLOT(onFinished(int,QByteArray)));
    connect(console, SIGNAL(output(QByteArray)), this, SLOT(onOutput(QByteArray)));
    gitCommand = "git";
}

void GitManager::setRepoParams(RepoParams *params)
{
    remote = params->login+"@"+params->url;
    //Устанавливаем переменные окружения
//    QStringList env;
#ifdef _WIN32
    console->write("DISPLAY=10\n");
    console->write("$");
#endif
    console->write("GIT_AUTHOR_NAME="+params->author+"\n");
    console->write("$");
    console->write("GIT_AUTHOR_EMAIL="+params->email+"\n");
    console->write("$");
    console->write("GIT_COMMITTER_NAME="+params->author+"\n");
    console->write("$");
    console->write("GIT_COMMITTER_EMAIL="+params->email+"\n");
    console->write("$");
    console->write("GIT_SSH_COMMAND=\"ssh -o BatchMode=yes -o ConnectTimeout=5 -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no\"\n");
    console->write("$");
//    console->write("echo 'ssh -o BatchMode=yes -o ConnectTimeout=5 -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no $*' > ssh\n");
//    console->write("$");
//    console->write("chmod +x ssh\n");
//    console->write("$");
//    console->write("GIT_TRACE=1 GIT_SSH='./ssh'\n");
//    console->write("$");
}

GitManager::~GitManager()
{
    delete console;
}

//Запуск git
void GitManager::start(QStringList args, bool quotes)
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
            console->write("cd "+QCoreApplication::applicationDirPath()+"\n");
            console->write("$");
        }else
        {
            console->write("cd "+workingDirectory+"\n");
            console->write("$");
        }
    }
    cout<<endl;
    QString command = gitCommand;
    foreach (QString arg, args) {
        if (quotes)
            command += QString(" ") +"\""+arg+"\"";
        else
            command += QString(" ") +arg;
    }
    cout<<command.toStdString()<<endl;
    console->write(command+"\n");
    console->write("$");
    this->command = args[0];
    inProcess = true;
}

//Слот, вызываемый по завершении git
void GitManager::onFinished(int code, QByteArray output)
{
//    cout<<QString::fromUtf8(output).toStdString()<<"\n";
//    cout.flush();
    inProcess = false;
//    console->write("$");
    if (output.startsWith("git \"commit\""))
    {
        emit commitSuccess();
        return;
    }
    if (output.startsWith("git push"))
    {
        if (code==0)
            emit pushSuccess();
        else
            emit pushFailure("Ошибка при отправке изменений на сервер", QString::fromUtf8(output));
        return;
    }
    if (output.startsWith("git pull"))
    {
        if (code==0)
            emit pullSuccess();
        else
            emit pullFailure("Ошибка при получении изменений с сервера", QString::fromUtf8(output));
        return;
    }
    if (output.startsWith("git \"add\""))
    {
        if (code==0)
            emit addSuccess();
        else
            emit addFailure("Ошибка при добавлении изменений в локальную базу данных (репозиторий)", QString::fromUtf8(output));
    }
    if (output.startsWith("git \"show\""))
    {
        if (code==0)
            emit showSuccess(workingDirectory+"/"+temp_show_fileName, temp_show_commit, output);
        else
            emit showFailure("Ошибка при получении старой версии файла из репозитория", QString::fromUtf8(output));
    }
    if (output.startsWith("git \"log\""))
    {
        if (code==0)
            emit logSuccess(QString::fromUtf8(output));
        else
            emit logFailure("Ошибка при получении списка версий файла", QString::fromUtf8(output));
    }
    if (output.startsWith("git \"clone\""))
    {
        if (code==0)
            emit cloneSuccess();
        else
            emit cloneFailure("Ошибка при клонировании репозитория", QString::fromUtf8(output));
    }
    if (output.startsWith("git \"remote\""))
    {
        if (code==0)
            emit remoteSuccess(QString::fromUtf8(output));
        else
            emit remoteFailure("Ошибка при получении информации о репозитории", QString::fromUtf8(output));
    }
    if (output.startsWith("(ssh"))
    {
        QList<QByteArray> lines = output.split('\n');
        if (lines.count()>1)
        {
            if (lines[1].startsWith("ok"))
            {
                emit checkConnectionSuccess();
                return;
            }
            emit checkConnectionFailure("Не удалось подключиться к серверу", lines[lines.count()-1]);
            return;
        }
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

void GitManager::gitError(QString error)
{
    qDebug()<<"Error!";
    qDebug()<<error;
}

void GitManager::pull()
{
    start(QStringList()<<"pull"<<"origin", false);
}

void GitManager::push()
{
    start(QStringList()<<"push"<<"origin"<<"master", false);
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

void GitManager::checkConnection()
{
    console->write("(ssh -o BatchMode=yes -o ConnectTimeout=5 max@192.168.2.10 echo ok 2>&1)\n");
    console->write("$");
}

void GitManager::onOutput(QByteArray output)
{
    QFile logFile("shell.txt");
    logFile.open(QFile::Append);
    logFile.write(output);
    logFile.close();
}
