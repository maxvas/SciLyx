#ifndef PASSWORDRECIEVER_H
#define PASSWORDRECIEVER_H

#include <QObject>

class QTcpSocket;

class PasswordReciever : public QObject
{
    Q_OBJECT
public:
    explicit PasswordReciever(QObject *parent = 0);

private:
    QTcpSocket *socket;

signals:

private slots:
    void onReadyRead();

};

#endif // PASSWORDRECIEVER_H
