#include "socketqthread.h"

SocketQThread::SocketQThread(QObject *parent) : QThread(parent)
{
    socketClient = NULL;
    socketServer = NULL;
}

void SocketQThread::SetSocket(int socketType, QString ip, int port)
{
    m_socketType = socketType;
    m_ip = ip;
    m_port = port;
}

void SocketQThread::run()
{
    if(m_socketType == 0)
    {
        return;
    }
    else if(m_socketType == 1)  //scoket server
    {
        socketServer = new SocketServer();
        socketServer->Init(m_ip,m_port);
        socketServer->StartListen(); //开始监听
    }
    else if(m_socketType == 2) //socket client
    {
        socketClient = new SocketClient();
        socketServer->Init(m_ip,m_port);
        socketClient->ConnectServer(); //开始连接服务器
    }

    while(true)
    {

    }
}
