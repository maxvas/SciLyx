#include "shell.h"

#include <QDebug>
#include <QDir>
#include <QApplication>

Shell::Shell(QString workingDirectory):
    mExitCodeRecieving(false), mIsFinished(false), mNextComandFlag(true)
{
    connect(&sh, SIGNAL(finished(int)), this, SLOT(onFinished(int)));
    connect(&sh, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    sh.setProcessChannelMode(QProcess::MergedChannels);
#ifdef __WIN32
    sh.setWorkingDirectory(workingDirectory);
//    sh.start(QDir::currentPath()+"/git/bin/sh.exe", QStringList()<<"--login"<<"-i");
    sh.start("\"C:\\Program Files\\Git\\bin\\sh.exe\"", QStringList()<<"--login"<<"-i");
#elif __unix__
    sh.start("sh");
#endif
}

Shell::~Shell()
{
    sh.terminate();
    sh.waitForFinished();
}

void Shell::dequeue()
{
    if (mQueue.isEmpty())
        return;
    QString str = mQueue.dequeue();
    if (str=="$")
    {
        mNextComandFlag = true;
        return;
    }
    if (!mIsFinished && !mNextComandFlag)
    {
        directWrite(str);
        return;
    }
    if (!mIsFinished && mNextComandFlag)
    {
        mQueue.push_front(str);
        return;
    }
    if (mIsFinished && !mNextComandFlag)
    {
        mQueue.push_front(str);
        return;
    }
    if (mIsFinished && mNextComandFlag)
    {
        directWrite(str);
        mNextComandFlag = false;
        mIsFinished = false;
        return;
    }
}

void Shell::processQueue()
{
    dequeue();
    QApplication::processEvents();
    while (!mNextComandFlag && !mQueue.isEmpty())
    {
        dequeue();
        QApplication::processEvents();
    }
}

void Shell::directWrite(QString str)
{
    QByteArray data;
    data.append(str.toUtf8());
    sh.write(data);
}

void Shell::write(QString str)
{
    mQueue.push_back(str);
    processQueue();
}

void Shell::onReadyRead()
{
    QByteArray data = sh.readAll();
    emit output(data);
    mOutput += data;
    if (mExitCodeRecieving && mOutput.endsWith("$ "))
    {
        int pos = mOutput.indexOf("\n");
        mOutput = mOutput.mid(qMin(pos+1, data.length()));
        pos = mOutput.indexOf("\n");
        mOutput = mOutput.left(pos);
        int code = mOutput.toInt();
        emit finished(code, mOutputAccepted);
        mOutput = "";
        mOutputAccepted = "";
        mExitCodeRecieving = false;
        mIsFinished = true;
        processQueue();
        return;
    }
    if (data.endsWith("$ "))
    {
        int pos = mOutput.lastIndexOf("\n");
        if (pos>0)
        {
            mOutput = mOutput.left(pos);
        }
        pos = mOutput.lastIndexOf("\n");
        if (pos>0)
        {
            mOutput = mOutput.left(pos);
        }
        pos = mOutput.lastIndexOf("\n");
        if (pos>0)
        {
            mOutput = mOutput.left(pos);
        }
        mOutputAccepted = mOutput;
        mOutput = "";
        mExitCodeRecieving = true;
        directWrite("echo $?\n");
    }
}

void Shell::onFinished(int code)
{
    emit error("sh was terminated");
}

void Shell::onError()
{
    emit error(sh.errorString());
}
