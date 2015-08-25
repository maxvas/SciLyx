#ifndef GITMANAGER_H
#define GITMANAGER_H

#include <QObject>
#include <QString>
#include <QQueue>
#include "../reposettings.h"

class Shell;

class GitManager : public QObject
{
    Q_OBJECT
public:
    explicit GitManager(QString workingDirectory);
    void setRepoParams(RepoParams *params);
    ~GitManager();

private:
    Shell *console;
    bool inProcess;//Работает ли сейчас git
    QString workingDirectory;
    QString remote;//Адрес удаленного репозитория
    QString command;//команда, выполняемая git
    void start(QStringList args);//Запуск git
    QString temp_show_fileName;
    QString temp_show_commit;
    QString temp_checkRemoteAddr_remote;
    QString gitCommand;

signals:
    void pullSuccess();//Сигнал успешного завершения pull
    void pullFailure(QString error, QString details);//Сигнал об ошибке во время pull
    void pushSuccess();//Сигнал успешного завершения push
    void pushFailure(QString error, QString details);//Сигнал об ошибке во время push
    void commitSuccess();//Сигнал успешного завершения commit
    void commitFailure(QString error, QString details);//Сигнал об ошибке во время commit
    void addSuccess();//Сигнал успешного завершения add
    void addFailure(QString error, QString details);//Сигнал об ошибке во время add
    void showSuccess(QString fileName, QString commit, QByteArray data);//Сигнал успешного завершения show. При это возвращаются данные, возвращаемые командой
    void showFailure(QString error, QString details);//Сигнал об ошибке во время show
    void logSuccess(QString output);
    void logFailure(QString error, QString details);
    void cloneSuccess();
    void cloneFailure(QString error, QString details);
    void remoteSuccess(QString output);
    void remoteFailure(QString error, QString details);
    void checkRemoteAddrSuccess();
    void checkRemoteAddrFailure(QString error, QString details);
    void checkConnectionSuccess();
    void checkConnectionFailure(QString error, QString details);

public slots:
    void pull();//Запуск git pull
    void push();//git push
    void commit();//git commit
    void add();//git add -A .
    void showOldFile(QString fileName, QString commit);
    void log(QString fileName);
    void clone(QString remote);
    void remoteShowOrigin();
    void checkRemoteAddr(QString remote);
    void checkConnection();

private slots:
    void onOutput(QByteArray output);//Слот для чтения стандартного вывода команды git
    void onFinished(int code, QByteArray output);//Слот, вызываемый по завершении git
    void onRemoteShowOriginSuccess(QString output);
    void onRemoteShowOriginFailure(QString error, QString details);
    void gitError(QString error);

};

#endif // GITMANAGER_H
