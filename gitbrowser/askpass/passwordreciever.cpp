#include "passwordreciever.h"
#include <QTcpSocket>
#include <iostream>
#include <QCoreApplication>
#include <QHostAddress>

PasswordReciever::PasswordReciever(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket();
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    socket->connectToHost(QHostAddress::LocalHost, 3136);
}

void PasswordReciever::onReadyRead()
{
    QString str = socket->readAll();
    std::cout<<str.toStdString();
    std::cout.flush();
    QCoreApplication::exit();
}
