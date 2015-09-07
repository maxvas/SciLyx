#ifndef SHELL_H
#define SHELL_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QByteArray>
#include <QQueue>

class Shell : public QObject
{
    Q_OBJECT
public:
    explicit Shell(QString workingDirectory = QString());
    ~Shell();

private:
    QProcess sh;//Процесс консоли (sh)
    bool mExitCodeRecieving;
    QByteArray mOutput;//Все что выведено в консоль при выполнении текущей команды
    QByteArray mOutputAccepted;//Все что выведено в консоль при выполнении текущей команды
    QQueue<QString> mQueue;
    bool mIsFinished;
    bool mNextComandFlag;
    void dequeue();
    void processQueue();
    void directWrite(QString str);

signals:
    void output(QByteArray data);
    void finished(int code, QByteArray mOutput);
    void error(QString error);

public slots:
    void write(QString str);

private slots:
    void onReadyRead();//Слот для чтения стандартного вывода команды sh
    void onFinished(int code);//Слот, вызываемый по завершении sh
    void onError(QProcess::ProcessError);//Слот, вызываемый в случае непредвиденных ошибок
    void onStarted();

};

#endif // SHELL_H
