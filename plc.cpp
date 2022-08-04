#include "plc.h"
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QThread>

Plc::Plc(QObject *parent) : QObject(parent)
{
    filter = true;
    m_serialPort = NULL;
    readTimer = new QTimer(this);
    pre_plcCRN = -1;
    plcRollCirle = 298.2;   //有误差，按照298.2mm来
    clothRollCirle = 153;
    yData = 0;
    m_preTime = QDateTime::currentDateTime();
    isCameraphotograph = false;
    camerInterval = 9000;
    m_nullNumber = 0;
    QObject::connect(this,SIGNAL(OpenSerialPortSignal()),this,SLOT(OpenSerialPort()));
}

Plc::~Plc()
{
    WriteLog("plc is destruct");
}

bool Plc::OpenSerialPort()
{
    qDebug()<<"OpenSerialPort";
    WriteLog("OpenSerialPort");
    if(m_serialPort)
    {
        m_serialPort->close();
    }
    m_serialPort = NULL;
    //开始与Plc建议通信
    m_serialPort = new QSerialPort();

    if(m_serialPort->isOpen())//如果串口已经打开了 先给他关闭了
    {
        m_serialPort->clear();
        m_serialPort->close();
    }

    //设置串口名字 假设我们上面已经成功获取到了 并且使用第一个
    m_serialPort->setPortName(m_comName);

    if(!m_serialPort->open(QIODevice::ReadWrite))//用ReadWrite 的模式尝试打开串口
    {
        WriteLog("serialPort open failed!");
        return false;
    }

    //打开成功
    m_serialPort->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);//设置波特率和读写方向
    m_serialPort->setDataBits(QSerialPort::Data8);		//数据位为8位
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);//无流控制
    m_serialPort->setParity(QSerialPort::NoParity);	//无校验位
    m_serialPort->setStopBits(QSerialPort::OneStop); //一位停止位

    QObject::connect(m_serialPort,SIGNAL(readyRead()),this,SLOT(ReceiveData()));
    QObject::connect(readTimer,SIGNAL(timeout()),this,SLOT(ReadPlcData()));

    WriteLog("serialPort open success!");
    return true;
}

void Plc::SetComName(QString comName,QString rootDir)
{
    m_comName = comName;
    m_rootDir = rootDir;
}

bool Plc::SetCameraphotograph(bool offOn)
{
    isCameraphotograph = offOn;
    if(isCameraphotograph)
    {
        yData = 0;
    }

    return true;
}

double Plc::GetYData()
{
    return yData;
}

void Plc::SendInfo(QString message)
{
    //    QByteArray sendBuf;
    //    qDebug()<<"Write to serial: "<<message;
    //    ConvertStringToHex(message, sendBuf);       //把QString 转换 为 hex

    m_serialPort->write(message.toLatin1());
}



//“1”—>小瑕疵自动复位
//“2”—>织布机停机
//“3”—>小瑕疵手动复位
//“4”—>编码器清零
void Plc:: ReceiveMainWMessage(QString message)
{
    if(message == "4")
    {
        pre_plcCRN = -1;
        filter = true;
    }
    SendInfo(message);
}

void Plc::ReadPlcData()
{
    QString log;
    try {

//        WriteLog("ReadPlcData");

        if(m_serialPort == NULL)
        {
            WriteLog("m_serialPort is NULL");
            return;
        }

        QByteArray info = m_serialPort->readAll();
        log = "plc recevice data :"+(QString)info+",dataLength:"+QString::number(info.count());
//        WriteLog(log);
        if(info.isEmpty())
        {
            log = "PLC is null data";
//            WriteLog(log);
        }
        QByteArray hexData = info.toHex();
//        WriteLog(info.toHex());

        if(hexData.count()< 16)
        {
            m_nullNumber++;
            log = "PLC size < 16";
//            qDebug()<<log;
//            WriteLog(log);
            return ;
        }

        if(hexData[0] != 'e' || hexData[1] != 'e' || hexData[2] != 'e' || hexData[3] != 'e' ||
                hexData[12] != 'f' || hexData[13] != 'f' || hexData[14] != 'f' || hexData[15] != 'f')
        {
            log = "analyse is error";
//            WriteLog(log);
            return;
        }

        QByteArray clothdata;
        //编码解析
        clothdata.append(hexData[10]);
        clothdata.append(hexData[11]);
        clothdata.append(hexData[8]);
        clothdata.append(hexData[9]);
        clothdata.append(hexData[6]);
        clothdata.append(hexData[7]);
        clothdata.append(hexData[4]);
        clothdata.append(hexData[5]);

        bool ok;
        double plcNum = clothdata.toLong(&ok,16);

        log = "clothdata:"+ (QString)clothdata +",plcNum:"+QString::number(plcNum)+",pre_plcCRN:"+QString::number(pre_plcCRN);
//        WriteLog(log);

//        if(filter)
//        {
//            if(plcNum < 300)
//            {
//                filter = false;
//            }
//            else
//            {
//                plcNum = 0;
//            }
//        }

        yData = plcRollCirle * plcNum / 2000 / 1000; //单位为米 除以1000

        if(plcNum != pre_plcCRN)
        {
            pre_plcCRN = plcNum;
        }

        emit UpdatePlcInfoSignal(yData);

        info.clear();
    }
    catch (...) {
        WriteLog("plcProcess error");
    }
}

void Plc::ReceiveData()
{
    if(!readTimer->isActive())
    {
        readTimer->start(1000);
    }
}

void Plc::receiveInfo()
{
    qDebug()<<"receiveInfo:"<<QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
}

void Plc::Error()
{
    qDebug()<<"Error";
}

void Plc::ConvertStringToHex(const QString &str, QByteArray &byteData)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    byteData.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertCharToHex(hstr);
        lowhexdata = ConvertCharToHex(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        byteData[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    byteData.resize(hexdatalen);
}

//char 转为 16进制
char Plc::ConvertCharToHex(char ch)
{
    /*
        0x30等于十进制的48，48也是0的ASCII值，，
        1-9的ASCII值是49-57，，所以某一个值－0x30，，
        就是将字符0-9转换为0-9

        */
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}

void Plc::WriteLog(QString message)
{
    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("yyyy-MM-dd hh:mm:ss zzz");
    QString log ="["+timeStr+"]:"+message+"\n";

    //测试
    qDebug()<<log;

    QString logPath = m_rootDir+"log\\";
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

//方法已弃用
bool Plc::StartPlcMonitor()
{
    //    QObject::connect(m_serialPort,SIGNAL(readyRead()),this,SLOT(receiveInfo()));
    //    QObject::connect(m_serialPort,SIGNAL(errorOccurred()),this,SLOT(Error()));

    plcStatus = true;

    //开启接受线程
    std::thread receiveid(Plc_Receive_Thread,this);
    //    int ret = pthread_create(&receiveid, NULL, Plc_Receive_Thread, this);

    return true;
}

void* Plc::Plc_Receive_Thread(void *pUser)
{
    if (NULL == pUser)
    {
        return NULL;
    }

    Plc* pHwnd = (Plc*)pUser;

    pHwnd->PlcProcess();
}

void Plc::PlcProcess()
{
    WriteLog("Start Plc Process");
    QString log;
    while(plcStatus)
    {
        try {

            WriteLog("plcProcess circle start");
            QThread::msleep(1000);

            if(m_nullNumber > 10)
            {
                log = "PLC is abnormal";
                WriteLog(log);
                //            qDebug()<<log;
                emit OpenSerialPortSignal();
                QThread::msleep(1000);
                continue;
            }

            if(m_serialPort == NULL)
            {
                WriteLog("m_serialPort is NULL");
                continue;
            }

            QByteArray info = m_serialPort->readAll();
            log = "plc recevice data :"+(QString)info;
            WriteLog(log);
            qDebug()<<log;
            if(info.isEmpty())
            {
                m_nullNumber++;
                log = "PLC is null data";
                WriteLog(log);
                //            qDebug()<<log;
                continue;
            }
            QByteArray hexData = info.toHex();

            if(hexData.count()< 8)
            {
                m_nullNumber++;
                log = "PLC size < 8";
                WriteLog(log);
                qDebug()<<log;
                continue;
            }

            QByteArray clothdata;

            //编码解析
            clothdata.append(hexData[6]);
            clothdata.append(hexData[7]);
            clothdata.append(hexData[4]);
            clothdata.append(hexData[5]);
            clothdata.append(hexData[2]);
            clothdata.append(hexData[3]);
            clothdata.append(hexData[0]);
            clothdata.append(hexData[1]);

            bool ok;
            double plcNum = clothdata.toLong(&ok,16);

            log = "clothdata:"+ (QString)clothdata +",plcNum:"+QString::number(plcNum)+",pre_plcCRN:"+QString::number(pre_plcCRN);
            WriteLog(log);
            qDebug()<<log;

            yData = plcRollCirle * plcNum  / 2000 / 1000; //单位为米 除以1000

            if(plcNum != pre_plcCRN)
            {
                pre_plcCRN = plcNum;
                QDateTime now = QDateTime::currentDateTime();
                int x = m_preTime.msecsTo(now);
                //                qDebug()<<"time ex is "<<x;
                if(x > camerInterval)
                {
                    m_preTime = now;
                    if(isCameraphotograph)
                    {
                        //                        qDebug()<<"Start Photo";
                        emit SendPhotographCode(yData);
                    }
                    //                m_preTime = now;
                }
            }

            //        qDebug()<<"yData is "<<yData;

            //这里面的协议 你们自己定义就行  单片机发什么 代表什么 我们这里简单模拟一下
            //QString 转十六进制 QString toInt(&ok,16); //bool ok
            //QString hexstr = QString::number(hexnum,16);//表示转换成16进制存入字符串

            //if(hexData == "269afeff")
            //        {
            //            //do something

            //        }
            //        else if(hexData  == "0x100001")
            //        {
            //            //do something
            //        }

            //m_serialPort->clear();
            qDebug()<<"**************************************************";
            m_nullNumber =0;
            WriteLog("plcProcess circle end");
        } catch (EXCEPINFO ex) {
            WriteLog("plcProcess error:"+(QString)ex.wCode);
        }
    }
}

