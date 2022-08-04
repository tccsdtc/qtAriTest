#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QMessageBox>
#include <QHostAddress>
#include <QTimer>

class SocketClient : public QObject
{
    Q_OBJECT
public:
    explicit SocketClient(QObject *parent = nullptr);

    void Init(QString ip,int port); //初始化
    void InitPlc(QString ip,int port,int stopCode,int resetCode); //初始化
    bool ConnectServer(); //连接服务器
    void SendMessageToServer(QString message); //发送消息
    QStringList getContentFromParenthese(QString content);

    void WriteLog(QString message);
signals:
    void UpdateSocketInfoSignal(int code,double meterLength,QString clothRollNo,int workStatus);
    void CloseVision();
    void UpdatePlcInfoSignal(double meterLength);

public slots:
    void ConnectSlot(); //连接反馈槽函数
    void RecieveSlot(); //接收数据槽函数
    void RecievePlcSlot(); //接受plc数据槽函数
    void DisconnectedSlot(); //断开槽函数
    void AutoConnectSlot(); //自动重连槽函数
    void ReceiveMainWMessage(QString message);//接收主界面数据

private:
    QString m_ip;
    int m_port;
    int m_stopCode;
    bool m_bNeedReset;
    int m_resetCode;

    double m_yData; //当前米数
    double m_yData_LastStop;    //停机米数

    QTcpSocket* m_tcpClientSocket;  //socket套接字
    QTimer* autoConnect;//自动重连
};

#endif // SOCKETCLIENT_H
