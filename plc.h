#ifndef PLC_H
#define PLC_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <thread>
#include <QtSerialPort/QSerialPort>
#include "windows.h"

class Plc : public QObject
{
    Q_OBJECT
public:
    explicit Plc(QObject *parent = nullptr);
    ~Plc();

    void PlcProcess();
    void SetComName(QString comName,QString rootDir);
    bool StartPlcMonitor();
    bool SetCameraphotograph(bool offOn);
    double GetYData();


private:
    void ConvertStringToHex(const QString &str, QByteArray &byteData);
    char ConvertCharToHex(char ch);
    void SendInfo(QString info);
    void WriteLog(QString message);

    static void* Plc_Receive_Thread(void* pUser);

signals:
    void SendPhotographCode(int yData);
    void OpenSerialPortSignal();
    void UpdatePlcInfoSignal(double meterLength);

public slots:
    bool OpenSerialPort();
    void ReceiveMainWMessage(QString message);
    void ReadPlcData();
    void ReceiveData();
    void receiveInfo();
    void Error();

public:
    int pre_plcCRN;

private:
    bool filter;
    std::thread plcThread;
    bool plcStatus;

    QSerialPort *m_serialPort;
    QString m_comName;
    int camerInterval;

    bool isCameraphotograph;
    QDateTime m_preTime;
    QTimer *readTimer;

    double plcRollCirle;
    double clothRollCirle;
    double yData;

    QString m_rootDir;
    int m_nullNumber;
};

#endif // PLC_H
