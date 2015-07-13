#include "passwordprovider.h"
#include <QTcpSocket>

PasswordProvider::PasswordProvider(QString password) :
    QTcpServer(0), password(password)
{
    this->listen(QHostAddress::LocalHost, 3136);
}

void PasswordProvider::incomingConnection(qintptr handle)
{
    QTcpSocket *socket = new QTcpSocket;
    socket->setSocketDescriptor(handle);
    socket->waitForConnected();
    socket->write(QString(password).toUtf8());
    socket->flush();
    socket->disconnectFromHost();
    socket->deleteLater();
}
