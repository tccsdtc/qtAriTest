#include "socketclient.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QJsonObject>
#include <QJsonParseError>
#include <QThread>

SocketClient::SocketClient(QObject *parent) : QObject(parent)
{

}

void WaitNoSleep(qint64 msTime)
{
    QDateTime startTime = QDateTime::currentDateTime();
    while(true)
    {
        QDateTime now = QDateTime::currentDateTime();
        if(startTime.msecsTo(now) > msTime)
        {
            break;
        }
        QCoreApplication::processEvents();//释放调度给其他任务机会
    }
}

void SocketClient::Init(QString ip,int port)
{
    m_ip = ip;
    m_port = port;

    m_tcpClientSocket = new QTcpSocket();
    autoConnect = new QTimer(this);
    autoConnect->setInterval(1200);
    connect(autoConnect,SIGNAL(timeout()),this,SLOT(AutoConnectSlot()));   //TCC
    connect(m_tcpClientSocket,&QTcpSocket::connected,this,&SocketClient::ConnectSlot);
    connect(m_tcpClientSocket,&QTcpSocket::readyRead,this,&SocketClient::RecieveSlot);
    connect(m_tcpClientSocket,&QTcpSocket::disconnected,this,&SocketClient::DisconnectedSlot);
}

void SocketClient::InitPlc(QString ip,int port,int stopCode,int resetCode)
{
    m_ip = ip;
    m_port = port;
    m_stopCode = stopCode;
    m_resetCode = resetCode;
    m_bNeedReset = false;
    m_yData_LastStop = 0.0;

    m_tcpClientSocket = new QTcpSocket();
    autoConnect = new QTimer(this);
    autoConnect->setInterval(1200);
    connect(autoConnect,SIGNAL(timeout()),this,SLOT(AutoConnectSlot()));
    connect(m_tcpClientSocket,&QTcpSocket::connected,this,&SocketClient::ConnectSlot);
    connect(m_tcpClientSocket,&QTcpSocket::readyRead,this,&SocketClient::RecievePlcSlot);
    connect(m_tcpClientSocket,&QTcpSocket::disconnected,this,&SocketClient::DisconnectedSlot);
}

void SocketClient::RecievePlcSlot()
{
    if( m_bNeedReset && ( m_yData - m_yData_LastStop >= 2.0) )
    {
        SendMessageToServer("#0110000");
        QThread::msleep(100);
        m_bNeedReset = false;
    }
    QByteArray receiveArray = m_tcpClientSocket->readAll();
    //WriteLog("RecievePlcSlot: "+receiveArray.toHex());
    QString meg = (QString)receiveArray;
    QStringList list = meg.split(',');
    if(list[0].length() >= 12)
    {
        double pulse = list[0].mid(2,10).toDouble();

        //    qDebug()<<QString::number(m_wjIndex)<<"-Pulse:"<<QString::number(pulse)<<",m_prePulse"<<QString::number(m_prePulse);
        if(pulse > 0)
        {
            double yData = 300 * pulse / 2000 / 1000; //单位为米 除以1000
            //WriteLog("yData: "+QString::number(yData));
            emit UpdatePlcInfoSignal(yData);
            m_yData = yData;
        }
    }
}

//“1”—>小瑕疵自动复位
//“2”—>织布机停机
//“3”—>小瑕疵手动复位
//“4”—>编码器清零
void SocketClient:: ReceiveMainWMessage(QString message)
{
    QString msg;
    msg = message;
    if(message == "4")
    {
        msg = "$011A+0000000000";
        m_yData_LastStop = 0.0;
        m_bNeedReset = false;
    }else if(message == QString::number(2))
    {
           SendMessageToServer("#0111101");
            QThread::msleep(20);
           SendMessageToServer("#0111201");

           WaitNoSleep(600);

           SendMessageToServer("#0111100");
            QThread::msleep(20);
           SendMessageToServer("#0111200");
        return;
    }else if(message == QString::number(m_resetCode))
    {
        qDebug()<<"resetCode";
        SendMessageToServer("#0110000");
        return;
    }

    SendMessageToServer(msg);
}

//void SocketClient:: ReceiveMainWMessage(QString message)
//{
//    QString msg;
//    msg = message;
//    if(message == "4")
//    {3
//        msg = "$011A+0000000000";
//        m_yData_LastStop = 0.0;
//        m_bNeedReset = false;
//    }else if(message == QString::number(m_stopCode))
//    {
//        if( !m_bNeedReset && (m_yData - m_yData_LastStop >= 2.0) )      //停机，报警，灯常亮
//        {
//            qDebug()<<"m_yData_LastStop"<<QString::number(m_yData_LastStop)<<"m_yData"<<QString::number(m_yData);
//            m_yData_LastStop = m_yData;
//            m_bNeedReset = true;
//            SendMessageToServer("#0110007");
//            QThread::msleep(100);
//            SendMessageToServer("#0110002");
//            QThread::msleep(100);
//        }
//        return;
//    }else if(message == QString::number(m_resetCode))
//    {
//        qDebug()<<"resetCode";
//        SendMessageToServer("#0110000");
//        return;
//    }

//    SendMessageToServer(msg);
//}

bool SocketClient::ConnectServer()
{
    //主动和服务器建立连接
    m_tcpClientSocket->connectToHost(QHostAddress(m_ip),m_port);
    bool ret = m_tcpClientSocket->waitForConnected(1000);
    if(!ret)
    {
        int num = 3;
        while(num--) //重新连接3次
        {
            m_tcpClientSocket->connectToHost(QHostAddress(m_ip),m_port);
            ret = m_tcpClientSocket->waitForConnected(1000);

            if(ret)
            {
                return true;
            }
        //    autoConnect->start();
        }
    }

    return true;
}

void SocketClient::AutoConnectSlot()
{
    qDebug()<<"connect socketserver!!!";
    m_tcpClientSocket->connectToHost(QHostAddress(m_ip),m_port);
    bool ret = m_tcpClientSocket->waitForConnected(1000);
    WriteLog("connect socketserver::"+QString::number(ret)+" ip:"+m_ip+" port:"+QString::number(m_port));
}

void SocketClient::SendMessageToServer(QString message)
{
    m_tcpClientSocket->write(message.toUtf8().data());
    m_tcpClientSocket->flush();
    //WriteLog("SendMessageToServer: "+message);
}

void SocketClient::ConnectSlot()
{
    qDebug()<<"connect success!";
    autoConnect->stop();
}

void SocketClient::RecieveSlot()
{
    QByteArray receiveArray = m_tcpClientSocket->readAll();
    QStringList receiveJsonList = getContentFromParenthese(receiveArray);

    foreach(QString receive,receiveJsonList)
    {
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

            switch (code) {
            case 0: //心跳
            {
                double meterLength = receiveJson.object().value("meterLength").toDouble();
                QString clothRollNo = receiveJson.object().value("clothRollNo").toString();
                int workStatus = receiveJson.object().value("workStatus").toInt();
                emit UpdateSocketInfoSignal(code,meterLength,clothRollNo,workStatus);
                break;
            }
            case 1://开始
            {
                QString clothRollNo = receiveJson.object().value("clothRollNo").toString();
                emit UpdateSocketInfoSignal(code,NULL,clothRollNo,NULL);
                break;
            }
            case 2://结束
            {
                emit UpdateSocketInfoSignal(code,NULL,NULL,NULL);
                break;
            }
            }
        }
    }
}

void SocketClient::DisconnectedSlot()
{
    emit CloseVision();
    //    QMessageBox::information(NULL,"Info","Disconnect Server! Please Startup Software!");
    qDebug()<<"Disconnect Server! Please Startup Software!";
//    autoConnect->start();

}

QStringList SocketClient::getContentFromParenthese(QString content)
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

void SocketClient::WriteLog(QString message)
{
    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("yyyy-MM-dd hh:mm:ss zzz");
    QString log ="["+timeStr+"]:"+message +"\n";

    qDebug()<<log;

    QString logPath = "./log/";
    QDir dir;
    if(!dir.exists(logPath))
    {
        dir.mkpath(logPath);
    }
    logPath += time.toString("yyyyMMdd")+".txt";
    QFile outFile(logPath);
    outFile.open(QIODevice::ReadWrite | QIODevice::Append);
    outFile.write(log.toLatin1());
}


