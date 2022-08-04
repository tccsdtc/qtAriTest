#include "socketserver.h"

#include <QJsonParseError>
#include <QJsonObject>
#include <QTime>
#include <QDir>

SocketServer::SocketServer(QObject *parent) : QObject(parent)
{

}

void SocketServer::Init(QString ip,int port)
{
    m_Ip = ip;
    m_Port = port;

    m_TcpServer = NULL;
    //    tcpSocket = NULL;

    //监听套接字   指定父对象的目的，让其自动回收空间
    m_TcpServer = new QTcpServer(this);

    connect(m_TcpServer,&QTcpServer::newConnection,this,&SocketServer::NewConnectSlot);
}

bool SocketServer::StartListen()
{
    if(!m_TcpServer->listen(QHostAddress(m_Ip),m_Port)){
        QString error = "[Listen Fail]:" + m_TcpServer->errorString();
        QMessageBox::information(NULL,"Info",error);
    }
    return true;
}

bool SocketServer::SendMessageToClinet(QString message)
{
    if(m_ClientList.length()< 1)
    {
        return true;
    }

    foreach(QTcpSocket* client,m_ClientList)
    {
        //给对方发送数据，使用套接字是tcpSocket
        client->write(message.toUtf8().data());
    }

    return true;
}

void SocketServer::NewConnectSlot()
{
    QTcpSocket* tcpSocket;
    tcpSocket = m_TcpServer->nextPendingConnection();

    //add
    m_ClientList.append(tcpSocket);

    //    QObject::connect(tcpSocket,&QTcpSocket::readyRead,this,&SocketServer::RecieveSlot);

    connect(tcpSocket,&QTcpSocket::readyRead,[=](){
        //从通信套接字中取出内容
        QByteArray receiveArray = tcpSocket->readAll();
//        qDebug()<<"SocketServer - receiveArray:"<<receiveArray;
        QString ip = tcpSocket->peerAddress().toString();
        qint16  port = tcpSocket->peerPort();
        QStringList receiveJsonList = getContentFromParenthese(receiveArray);

        foreach(QString receive,receiveJsonList)
        {
//            qDebug()<<"SocketServer JsonDes:"<<receive;
            QJsonParseError err_rpt;
            QJsonDocument receiveJson = QJsonDocument::fromJson(receive.toLatin1(), &err_rpt);

            if(err_rpt.error != QJsonParseError::NoError)
            {
                qDebug() << "Json Format is false.";
                return;
            }
            else    //JSON格式正确
            {
                int code = receiveJson.object().value("code").toInt();
                if(code == 0)
                {
                    QString codeStr = "{\"code\":0,\"meterLength\":" + QString::number(yData)+",\"clothRollNo\":\""+m_clothNo+"\",\"workStatus\":"+QString::number(m_workStatus)+"}";
//                    WriteLog(codeStr);
                    tcpSocket->write(codeStr.toUtf8().data());
                }
                else if(code == 3)
                {
                    //                WriteLog();
                    QString imageName = receiveJson.object().value("imageName").toString(); //解析出文件名
                    QString camserialname = receiveJson.object().value("camserialname").toString();
                    int x = receiveJson.object().value("x").toInt();
                    int y = receiveJson.object().value("y").toInt();
                    int type = receiveJson.object().value("type").toInt();
                    double yData = receiveJson.object().value("yData").toDouble();
                    emit UpdateSocketImageSignal(imageName,camserialname,x,y,type,yData);
                }
                else if(code == 4)
                {
                    QString plcCode = receiveJson.object().value("plcCode").toString(); //解析出文件名
                    emit SendPlcCodeSignal(plcCode);
                }
            }
        }
    });

    connect(tcpSocket,&QTcpSocket::disconnected,[=](){

        for (int i=0;i< m_ClientList.length();i++) {
            if(tcpSocket->peerAddress().toString() == m_ClientList[i]->peerAddress().toString() &&
                    tcpSocket->peerPort() == m_ClientList[i]->peerPort())
            {
                m_ClientList.removeAt(i);
//                emit SendPlcCodeSignal(stopCode);
//                QMessageBox::information(NULL,"Info","分控机异常，停止布卷！！！请确认分控机连接到服务器再操作！");
            }
        }
    });

//    QMessageBox::information(NULL,"Info","分控机连接到服务，可以开始操作！！！");
}

void SocketServer::RecieveSlot()
{

}

void SocketServer::WriteLog(QString message)
{
    qDebug()<<"SocketServer:"+message;
    //    QDateTime time = QDateTime::currentDateTime();
    //    QString timeStr = time.toString("yyyy-MM-dd hh:mm:ss zzz");
    //    QString log ="["+timeStr+"]:"+message+"\n";
    //    qDebug()<<log; //测试用

    //    QString logPath = QString::fromStdString(m_root_dir)+"log\\";
    //    QDir dir;
    //    if(!dir.exists(logPath))
    //    {
    //        dir.mkpath(logPath);
    //    }
    //    logPath += time.toString("yyyyMMdd")+".txt";
    //    QFile outFile(logPath);
    //    outFile.open(QIODevice::ReadWrite | QIODevice::Append);
    //    outFile.write(log.toLatin1());
}

QStringList SocketServer::getContentFromParenthese(QString content)
{
    QStringList contents;
    int m = 0, n = 0;
    bool isJson = false;
    for (int i = 0; i < content.length(); i++) {
        if (content[i] == '{') {
            if (!isJson) {
                m = i;
            }
            isJson = true;
        }
        if (content[i] == '}') {
            if (isJson) {
                n = i;
                contents.append(content.mid(m, n-m+1));
//                qDebug()<<content.mid(m, n-m + 1);
                isJson = false;
            }
        }
    }
    return contents;
}
