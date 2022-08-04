#ifndef SOCKETQTHREAD_H
#define SOCKETQTHREAD_H

#include <QObject>
#include <QThread>
#include "socketclient.h"
#include "socketserver.h"

class SocketQThread : public QThread
{
    Q_OBJECT
public:
    explicit SocketQThread(QObject *parent = nullptr);

    void SetSocket(int socketType,QString ip,int port);
protected:
    void run() override;
signals:

public slots:

private:
    int m_socketType;
    QString m_ip;
    int m_port;

    SocketClient* socketClient;
    SocketServer* socketServer;
};

#endif // SOCKETQTHREAD_H
