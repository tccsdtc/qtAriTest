#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>

class SocketServer : public QObject
{
    Q_OBJECT
public:
    explicit SocketServer(QObject *parent = nullptr);

    void Init(QString ip,int port);  //初始化Socket
    bool StartListen();  //开始监听]
    bool SendMessageToClinet(QString message); //发送Socket数据
    QStringList getContentFromParenthese(QString content);

signals:
    void UpdateSocketImageSignal(QString imageName,QString camserialname,int x,int y,int type,double yData);
    void SendPlcCodeSignal(QString plcCode);

public slots:
    void NewConnectSlot(); //新连接Socket
    void RecieveSlot();  //接收数据
    void WriteLog(QString message);

private:
    void AnalyticalData(QByteArray receieveData,QString ip,int port); //解析数据

public:
    double yData;
    QList<QTcpSocket*> m_ClientList; //客户端列表
    QString m_clothNo;  //布卷号
    int m_workStatus;  //0 代表不工作，1代表工作
    QString stopCode;

private:
    QString m_Ip;   //ip地址
    int m_Port;     //端口

    QTcpServer* m_TcpServer;  //服务端

};

#endif // SOCKETSERVER_H
