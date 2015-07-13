#ifndef PASSWORDPROVIDER_H
#define PASSWORDPROVIDER_H

#include <QTcpServer>
#include <QString>

//Простой TCP-сервер, передающий в процесс askpass пароль от GIT
class PasswordProvider : public QTcpServer
{
    Q_OBJECT
public:
    explicit PasswordProvider(QString password);

private:
    QString password;
    void incomingConnection(qintptr handle);

signals:

public slots:

};

#endif // PASSWORDPROVIDER_H
