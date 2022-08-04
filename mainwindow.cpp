#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QPalette>
#include <QColor>
#include <QDebug>
#include <QPoint>
#include <QPainter>
#include <QThread>
#include <QObject>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <thread>
#include <QSettings>
#include <QMessageBox>
#include <QScrollArea>
#include <QScrollBar>
#include <QProcess>
using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init()
{
    CoInitialize(0);
    setWindowState(Qt::WindowMaximized);
    LoadConifg();
    AlarmDelay=3.4;

    qRegisterMetaType<Mat>("Mat");

    if(socketType == 2)
    {
        ui->NewClothRollPB->setEnabled(false);
        ui->startVisionPB->setEnabled(true);
        ui->closeVisionPB->setEnabled(false);
        ui->exportPB->setEnabled(false);
        ui->clothHistoryPB->setEnabled(false);
    }

    //更新UI界面 软件名称和版本号 "布卷编号"
    //ui->softwareNameLB->setText(softwareName);

    m_yData = 0;
    m_imageIndex = 0;
    pageImageIndex = 0;

    showTimer = new QTimer();
    photoTimer = new QTimer();
    plcTimer = new QTimer();
    ReportTimer=new QTimer();
    imageSpaceInterval = 10 ;
    isViRun = false;
    isStartStatus = false;
    handyDataEx = 0;

    socketServer = NULL;
    socketClient = NULL;

    m_startTime = QDateTime::currentDateTime();
    m_recordPath = root_dir+"record\\";

    QDir dir;
    //算法自身调用
    if (!dir.exists(m_recordPath))
    {
        bool result = dir.mkpath(m_recordPath);
    }

    InitSocket();
    InitChart();
    //初始化算法变量
    InitVison();

    if(socketType < 2)
    {
        //初始化PLC
        if(!InitPlc())
        {
            QMessageBox::information(NULL,"Into","串口连接失败，请检查配置！");
        }
    }

    //初始化相机
    if(!InitCamera())
    {
        QMessageBox::information(NULL,"Into","相机连接失败，请检测相机相关！");
    }

    QObject::connect(&newClothRollDialog,SIGNAL(Send(QString,QString,QString)),this,SLOT(GetClothRollInfo(QString,QString,QString)));
    QObject::connect(this,SIGNAL(SendPlcCode(QString)),&plc,SLOT(ReceiveMainWMessage(QString)));
    QObject::connect(this,SIGNAL(UpdateImageShowListSignal(QString,QString,int,int,int,double,int)),this,SLOT(AddDetectImageShowSlot(QString,QString,int,int,int,double,int)));
    QObject::connect(plcTimer,SIGNAL(timeout()),this,SLOT(SendMeterInfoSlot()));
    QObject::connect(showTimer,SIGNAL(timeout()),this,SLOT(UpdateOnceStatistics()));
    QObject::connect(photoTimer,SIGNAL(timeout()),this,SLOT(CheckCameraInfo()));
    QObject::connect(&hisClothDialog,SIGNAL(SendSelectHClothSignal(QString)),this,SLOT(RecieveSelectHClothSlot(QString)));
    QObject::connect(ReportTimer,SIGNAL(timeout()),this,SLOT(AutoReportSlot()));
    QObject::connect(this,SIGNAL(AutoClearViewSignal()),this,SLOT(AutoClearViewSlot()));



    WriteLog("Qt_GC_VI_Insepect is Start!!!");
}

void MainWindow::LoadConifg()
{
    qDebug()<<QCoreApplication::applicationDirPath()+"/config.ini";
    QSettings config(QCoreApplication::applicationDirPath()+"/config.ini",QSettings::IniFormat);

    //Software
    softwareName = config.value("System/softwareName").toString(); //软件名称

    //User
    root_dir = config.value("User/rootDir").toString(); //存图路径
    lineImageNum = config.value("User/linePhotoNumber").toInt();
    pageImageNum = config.value("User/pageImageNumber").toInt();
    comName = config.value("User/comName").toString();
    stopCode = config.value("User/stopCode").toInt();
    resetCode = config.value("User/resetCode").toInt();

    //Camera
    m_cameraName = config.value("Camera/cameraName").toString();
    pixelWidth = config.value("Camera/pixelWidth").toInt();
    pixelHeight = config.value("Camera/pixelHeight").toInt();
    pixelProportion = config.value("Camera/pixelProportion").toDouble(); //实际单位mm/像素px 光源

    //Ari
    ariOnoff = config.value("Arithmetic/ariOnoff").toInt();
    threshold_pixel = config.value("Arithmetic/threshold_pixel").toInt();
    threshold_alert_area = config.value("Arithmetic/threshold_alert_area").toInt();
    threshold_alert_quanlity = config.value("Arithmetic/threshold_alert_quanlity").toInt();
    threshold_alert_ratio = config.value("Arithmetic/threshold_alert_ratio").toInt();
    threshold_alert_lenth = config.value("Arithmetic/threshold_alert_lenth").toInt();
    threshold_alert_width = config.value("Arithmetic/threshold_alert_width").toInt();
    threshold_alert_cujiesha_max = config.value("Arithmetic/threshold_alert_cujiesha_max").toInt();
    threshold_alert_cujiesha_min = config.value("Arithmetic/threshold_alert_cujiesha_min").toInt();
    threshold_length_filter = config.value("Arithmetic/threshold_length_filter").toInt();

    //PLC
    plcIp = config.value("WJ/ip").toString();
    plcPort = config.value("WJ/port").toInt();
        qDebug()<<plcIp<<""<<QString::number(plcPort);

    //socket
    socketType = config.value("Socket/type").toInt();
    socketIp = config.value("Socket/ip").toString();
    socketPort = config.value("Socket/port").toInt();
    socketyDataEx = config.value("Socket/yDataEx").toDouble();
    socketServerPath = config.value("Socket/serverPath").toString();

    //RestartCamera
    m_dRestartCameraTime = config.value("RestartCamera/time",180.0).toDouble();  //重启相机的时间,单位:分(超过这个时间没有触发拍照信号，就重启)
    m_RestartCameraCount = config.value("RestartCamera/count",1).toInt(); //重启相机的数量
    m_RestartCameraNetcardPrefix = config.value("RestartCamera/NetcardPrefix","camera").toString(); //重启相机的网卡前缀。注意:网卡名必须是从前缀+1开始,比如 camera1,camera2
    m_bRestartNetcard = config.value("RestartCamera/RestartNetcard",true).toBool(); //重启相机的数量
    //分厂不同设备标识
    m_GcAddress = config.value("Address/Address","Unknown").toString();

    //直径选择
    m_nDiameter = config.value("User/Diameter",3).toInt();
    if(m_nDiameter==1)
    {
        ui->comboBox_1->setCurrentIndex(0);
    }
    else if(m_nDiameter==2)
    {
        ui->comboBox_1->setCurrentIndex(1);
    }
    else if(m_nDiameter==3)
    {
       ui->comboBox_1->setCurrentIndex(2);
    }
    else if(m_nDiameter==4)
    {
        ui->comboBox_1->setCurrentIndex(3);
    }
    else if(m_nDiameter==5)
    {
       ui->comboBox_1->setCurrentIndex(4);
    }

    m_nDiameter_BlackDot= config.value("User/DiameterBlackDot",1).toInt();
    if(m_nDiameter_BlackDot==0.5)
    {
        ui->comboBox_4->setCurrentIndex(0);
    }
    else if(m_nDiameter_BlackDot==1)
    {
        ui->comboBox_4->setCurrentIndex(1);
    }
    else if(m_nDiameter_BlackDot==1.5)
    {
       ui->comboBox_4->setCurrentIndex(2);
    }
    else if(m_nDiameter_BlackDot==2)
    {
        ui->comboBox_4->setCurrentIndex(3);
    }
    else if(m_nDiameter_BlackDot==2.5)
    {
       ui->comboBox_4->setCurrentIndex(4);
    }

}

bool MainWindow::InitPlc()
{
//    QObject::connect(&plc,SIGNAL(SendPhotographCode(int)),this,SLOT(ReceivePhotographCode(int)));
//    QObject::connect(&plc,SIGNAL(UpdatePlcInfoSignal(double)),this,SLOT(UpdatePlcInfoSlot(double)));//UpdateMeterLength

//    //开启PLC
//    plc.SetComName(comName,root_dir);
//    if(!plc.OpenSerialPort())
//    {
//        return false;
//    }

    plc.InitPlc(plcIp,plcPort,stopCode,resetCode);
    plc.ConnectServer();
    QObject::connect(&plc,SIGNAL(UpdatePlcInfoSignal(double)),this,SLOT(UpdatePlcInfoSlot(double)));//UpdateMeterLength
    return true;
}

bool MainWindow::InitCamera()
{
    //开启摄像机//TCCTCC
    IKapCameraList.clear();
    int CameraNum=ikapCamera::CheckCameraNums();
    qDebug()<<"CameraNum:"<<QString::number(CameraNum);

    for (int i = 0; i < CameraNum; i++)
    {
        ikapCamera* camera= new ikapCamera();
        if(!camera->InitDevices(i))
        {
            continue;
        }
        if(!camera->OpenVideoStream())
        {
            continue;
        }
        QObject::connect(camera,SIGNAL(SendCameraInfo(Mat)),ariThread,SLOT(ReceiveCameraSlot(Mat)));
        IKapCameraList.append(camera);
    }

    if (IKapCameraList.count() ==1)
    {
        return true;
    }
    else
    {
        return false;
    }
     //com口异常算为设备，后续了解
}

void MainWindow::RestartCameras()
{
    WriteLog("RestartCameras Start");

    //停止拍照
    for(int num = 0;num < IKapCameraList.count();num++)
    {
        IKapCameraList[num]->bIsSavePic = false;
    }
    //关闭相机
    CloseCamera();

//    //QThread::msleep(1);  等待10秒
    QDateTime startTime1 = QDateTime::currentDateTime();
    while(true)
    {
        QDateTime now1 = QDateTime::currentDateTime();
        if(startTime1.msecsTo(now1) > 10000)
        {
            break;
        }
        QCoreApplication::processEvents();//释放调度给其他任务机会
    }

    //打开相机
    if(!InitCamera())
    {
        WriteLog("RestartCameras Fail");
        return;
    }
    //开始拍照
    for(int num = 0;num < IKapCameraList.count();num++)
    {
        IKapCameraList[num]->bIsSavePic = true;
    }

    WriteLog("RestartCameras Success");
}

bool MainWindow::InitSocket()
{
    if(socketType == 0)
    {
        setWindowTitle("GC_VI_Inspect");
        return true;
    }
    else if(socketType == 1)  //scoket server
    {
        setWindowTitle("GC_VI_Inspect_Server");
        socketServer = new SocketServer();
        socketServer->Init(socketIp,socketPort);
        socketServer->StartListen(); //开始监听
        QObject::connect(socketServer,SIGNAL(UpdateSocketImageSignal(QString,QString,int,int,int,double,double)),this,SLOT(UpdateSocketImageSlot(QString,QString,int,int,int,double,double)));
        QObject::connect(socketServer,SIGNAL(SendPlcCodeSignal(QString)),&plc,SLOT(ReceiveMainWMessage(QString)));
        socketServer->stopCode = QString::number(stopCode);
    }
    else if(socketType == 2) //socket client
    {
        setWindowTitle("GC_VI_Inspect_Client");
        socketClient = new SocketClient();
        socketClient->Init(socketIp,socketPort);
        socketClient->ConnectServer(); //开始连接服务器

        QObject::connect(socketClient,SIGNAL(UpdateSocketInfoSignal(int,double,QString,int)),this,SLOT(UpdateSocketInfoSlot(int,double,QString,int)));
        QObject::connect(socketClient,SIGNAL(CloseVision()),this,SLOT(SocketCloseVisionSlot()));

        heartBeat = new QTimer(this);
        heartBeat->setInterval(100);

        QObject::connect(heartBeat,SIGNAL(timeout()),this,SLOT(HeartBeatSlot()));
        heartBeat->start();
    }
    //    socketThread = new SocketQThread(this);
    //    socketThread->SetSocket(socketType,socketIp,socketPort);
    //    socketThread->start();
    return true;
}

void MainWindow::InitChart()
{
    m_chart=new QChart;

    m_series=new QScatterSeries;
    m_series->setName("OK");
    m_series->setMarkerSize(9);//设置节点大小
 //   m_series->setUseOpenGL(true);//提高加载速度,但无法选中点

    m_seriesFlaw = new QScatterSeries;
    m_seriesFlaw->setName("NG");
    m_seriesFlaw->setMarkerSize(9);//设置节点大小
    m_seriesFlaw->setColor(QColor(255,0,0));

    m_seriesRemind=new QLineSeries;
    m_seriesRemind->setName("setName");
    m_seriesRemind->setColor(QColor(0,255,0));
    m_seriesRemind->append(-2,-AlarmDelay);
    m_seriesRemind->append(-132,-AlarmDelay);

    m_chart->addSeries(m_series);
    m_chart->addSeries(m_seriesFlaw);
    m_chart->addSeries(m_seriesRemind);
    m_chart->createDefaultAxes();

    m_axisX=new QValueAxis;
    m_axisX->setRange(-2,132);
    m_axisX->setGridLineVisible(true);
    m_axisX->setTickCount(5);     //标记的个数
    m_axisX->setMinorTickCount(0); //次标记的个数

    m_axisY=new QValueAxis;
    m_axisY->setRange(0,60);
    m_axisY->setGridLineVisible(true);
    m_axisY->setTickCount(20);
    m_axisY->setMinorTickCount(0);

    m_chart->setAxisX(m_axisX,m_series);
    m_chart->setAxisY(m_axisY,m_series);
    m_chart->setAxisX(m_axisX,m_seriesFlaw);
    m_chart->setAxisY(m_axisY,m_seriesFlaw);
    m_chart->setAxisX(m_axisX,m_seriesRemind);
    m_chart->setAxisY(m_axisY,m_seriesRemind);

//    m_chart->addAxis(m_axisX,Qt::AlignTop);
//    m_chart->addAxis(m_axisY,Qt::AlignLeft);
//    m_series->attachAxis(m_axisX);
//    m_series->attachAxis(m_axisY);

    m_chart->legend()->hide();

    m_chartView=new ChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    ui->rl->addWidget(m_chartView);

//    ui->chart1->setChart(m_chart);
//    ui->chart1->setRenderHint(QPainter::Antialiasing);

 //   connect(m_series, SIGNAL(doubleClicked(QPointF)), this, SLOT(tooltip(QPointF)));
    m_tooltip = 0;
    connect(m_series, SIGNAL(hovered(QPointF,bool)), this, SLOT(tooltip(QPointF,bool)));
    connect(m_seriesFlaw, SIGNAL(hovered(QPointF,bool)), this, SLOT(tooltip(QPointF,bool)));
}

bool MainWindow::InitVison()
{
    ariThread = new ArithmeticQThread(this);
    ariThread->SetImagePixel(pixelWidth,pixelHeight);
    ariThread->m_cameraName = m_cameraName;
    ariThread->SetThreshold(m_cameraName,threshold_pixel,threshold_alert_area,threshold_alert_quanlity,threshold_alert_ratio,
                            threshold_alert_lenth,threshold_alert_width,threshold_alert_cujiesha_max,
                            threshold_alert_cujiesha_min,stopCode,m_dRestartCameraTime);

    QObject::connect(this,SIGNAL(StartVisualInspectionSignal()),ariThread,SLOT(SetViFlag()));//一组图采集缓存snaptest文件夹好后，触发槽函数检测
    QObject::connect(ariThread,SIGNAL(SendPlcCode(QString)),this,SLOT(SendPlcCodeSlot(QString)));
    QObject::connect(ariThread,SIGNAL(UpdateImageShowListSignal(QString,QString,int,int,int,double,double,int)),this,SLOT(AddDetectImageShowSlot(QString,QString,int,int,int,double,double,int)));
    QObject::connect(ariThread,SIGNAL(DataStorageSignal()),this,SLOT(DataStorageAutoSave()));
    connect(ariThread,SIGNAL(RestartCameras()),this,SLOT(RestartCameras()),Qt::BlockingQueuedConnection);

    return true;
}

//void MainWindow::openCameraByIndex(DalsaCamera* cam,string serverName)//TCCTCC
//{
//    //初始化
//    if (cam->InitDevices(serverName,NULL,"NoFile"))
//    {
//        //打开视频流
//        if (!cam->OpenVideoStream())
//        {
//            qDebug()<<"错误, 打开视频流错误";
//        }
//        else
//        {
//            std::cout << "The video stream turns on normally" << std::endl;
//        }
//    }
//    else
//    {
//        // MsgBox("错误", "打开相机错误");
//        std::cout << "open camera failed" << std::endl;
//    }
//}

bool MainWindow::CloseCamera()
{
    for(int num = 0;num < IKapCameraList.count();num++)
    {
        IKapCameraList[num]->CloseCamera();
    }
    return true;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    //showWidgetWidth = ui->groupBox_2->width() - 30;
    //showHeightWidth = ui->groupBox_2->height();
    //imageSize.setWidth((showWidgetWidth-(lineImageNum+1)*imageSpaceInterval)/lineImageNum);
    //imageSize.setHeight((showWidgetWidth-(lineImageNum+1)*imageSpaceInterval)/lineImageNum);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->rl && event->type() == QEvent::Paint)

        DrawClothRoll();
    return QWidget::eventFilter(watched, event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::question(this,"Info","是否关闭软件？",QMessageBox::Yes | QMessageBox::No);
    if(ret == QMessageBox::Yes)
    {
        CloseCamera();
        WriteLog("关闭软件");
        return QWidget::closeEvent(event);
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::AddDetectImageShowSlot(QString imagePath,QString camserialname,int x,int y,int type,double yData,double size,int IQDtype)
{
    if(socketType == 2 && socketClient != NULL)
    {
        QString imageName = imagePath.mid(imagePath.lastIndexOf("\\")+1);
        QString message = "{\"code\":3,\"imageName\":\""+imageName+"\",\"camserialname\":\""+m_cameraName+"\","
                +"\"x\":"+QString::number(x)+",\"y\":"+QString::number(y)+",\"type\":"+QString::number(type)+",\"yData\":"+QString::number(yData-socketyDataEx)+"}";
        socketClient->SendMessageToServer(message);
    }
//    AddDetectImageShowPaging(imagePath,m_cameraName,x,y,type,yData-socketyDataEx);
    AddDetectImageShow(imagePath,m_cameraName,x,y,type,yData-socketyDataEx,size,IQDtype);
}

void MainWindow::AddDetectImageShow(QString imagePath,QString camserialname,int x,int y,int type,double yData,double size,int IQDtype)
{
    qDebug()<<imagePath<<imagePath;
    if(socketType == 1)
    {
        //        if(!FilterSubControlInfo(x,y,yData))
        //        {
        //            WriteLog("Filter Point: " + imagePath );
        //            return ;
        //        }
    }

    if(!isHisCloth)
    {
        FlawRecord flawRecord;
        flawRecord.yData = yData;
        flawRecord.x = x;
        flawRecord.y = y;
        flawRecord.camera = camserialname;
        flawRecord.imgPath = imagePath;
        flawRecord.flawTime = QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");
        flawRecordList.append(flawRecord);
    }
    //qDebug()<<"AddDetectImageShow:"<<QString::number(x)<<QString::number(y)<<QString::number(yData);
    m_imageIndex++;
    //X单位是cm，Y单位是m
    QString imageText = nullptr;

    double yy=y* pixelProportion/1000 + yData;
    double xx=x * pixelProportion/10;
//    y = y* pixelProportion/1000 + yData;//单位换算
//    x = x * pixelProportion/10; //单位换算
//    imageText = " "+camserialname +" X:"+QString::number(x)+" Y:"+QString::number(y* pixelProportion/1000 + yData,'f',1);
    imageText = " "+camserialname +" X:"+QString::number(xx,'f',1)+" Y:"+QString::number(yy,'f',1);
        //qDebug()<<imageText;
        //qDebug()<<QString::number(flawRecordList.count())<<QString::number(pageImageNum);
    //页面图片展示
    pageTotal = flawRecordList.count() / pageImageNum;
    if(flawRecordList.count() % pageImageNum > 0)
    {
        pageTotal++;
    }
//    ui->totalPageLB->setText(QString::number(pageTotal));

    int row,col;//坐标 行，列
    row = imageList.count() / lineImageNum; //行
    col = imageList.count() - row * lineImageNum;  //列

    QPoint startPoint;
    startPoint.setX(col*(imageSize.width()+imageSpaceInterval)+ 10);
    startPoint.setY(row*(imageSize.height()+imageSpaceInterval) + 20);

    ShowImageLabel *image = new ShowImageLabel(ui->scrollAreaWidgetContents);
    image->SetImageType(type);
    image->SetImagePath(imagePath);
    image->SetImageText(imageText);
    image->SetImageSize(size,IQDtype);
    image->SetImageNum(m_imageIndex);
    image->setMaximumSize(imageSize);
    image->setMinimumSize(imageSize);
    image->move(startPoint);
    image->show();
    image->ShowImage();
    imageList.append(image);
//    if(imageList.count()>(1000-m_nBackupNum))
//    {
//        imageList_backup.append(image);
//    }

    ui->scrollAreaWidgetContents->setMinimumSize(0,row*(imageSize.height()+imageSpaceInterval) + 20+imageSize.height()+30);
    if(!ui->scrollArea->verticalScrollBar()->isSliderDown())
        ui->scrollArea->verticalScrollBar()->setValue(row*(imageSize.height()+imageSpaceInterval) + 20+imageSize.height()+30);

    //Map 数据展示
    QPointF point(xx,yy);
    if(type==0)
    {
        m_series->append(point);
    }
    else
    {
        m_seriesFlaw->append(point);
        AlarmList.append(yy);
    }

//    if(imageList.count()>5000)
//    {
//        emit AutoClearViewSignal();
//    }


//    QValueAxis *axisY = dynamic_cast<QValueAxis*>(this->m_chart->axisY(m_series));
//    const double yMin = axisY->min();
//    const double yMax = axisY->max();
//    double difference=yMax-yMin;

//    if(y>yMax)
//    {
//        m_chart->axisY()->setRange(yMin+difference,yMax+difference);
//    }

    if(isHisCloth)
    {
        int value = 100 * m_imageIndex / m_historyTotalNum;
        hintDialog.SetProgressBarValue(value);

        if(m_imageIndex == m_historyTotalNum)
        {
            hintDialog.hide();
        }
    }
}

void MainWindow::AutoClearViewSlot()
{
    //删除控件
    QList<ShowImageLabel*> images = ui->scrollAreaWidgetContents->findChildren<ShowImageLabel*>();
    foreach (ShowImageLabel* image, images)
    {
        delete image;
    }
    int imageIndex=m_imageIndex-m_nBackupNum;
//    foreach (ShowImageLabel* image, imageList_backup)
//    {
//        int row,col;//坐标 行，列
////        row = imageList.count() / lineImageNum; //行
////        col = imageList.count() - row * lineImageNum;  //列
//        row =imageIndex / lineImageNum; //行
//        col = imageIndex - row * lineImageNum;  //列

//        QPoint startPoint;
//        startPoint.setX(col*(imageSize.width()+imageSpaceInterval)+ 10);
//        startPoint.setY(row*(imageSize.height()+imageSpaceInterval) + 20);

//  //      ShowImageLabel *image = new ShowImageLabel(ui->scrollAreaWidgetContents);
//        image->SetImageType(image->m_type);
//        image->SetImagePath(image->m_filePath);
//        image->SetImageText(image->m_text);
//        image->SetImageSize(image->m_size);
//        image->SetImageNum(imageIndex++);
//        image->setMaximumSize(imageSize);
//        image->setMinimumSize(imageSize);
//        image->move(startPoint);
//        image->show();
//        image->ShowImage();
//        imageList.append(image);
//    }
    imageList.clear();
    imageList_backup.clear();
    flawRecordList.clear();
    m_series->clear();
    m_seriesFlaw->clear();
}

void MainWindow::AddDetectImageShowPaging(QString imagePath, QString camserialname, int x, int y, int type, double yData)
{
    if(socketType == 1)
    {
    }

    if(!isHisCloth)
    {
        FlawRecord flawRecord;
        flawRecord.yData = yData;
        flawRecord.x = x;
        flawRecord.y = y;
        flawRecord.camera = camserialname;
        flawRecord.imgPath = imagePath;
        flawRecord.flawTime = QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");
        flawRecordList.append(flawRecord);
    }

    pageImageIndex++;
    if(pageImageIndex > pageImageNum)
    {
        //删除控件
        QList<ShowImageLabel*> images = ui->scrollAreaWidgetContents->findChildren<ShowImageLabel*>();
        foreach (ShowImageLabel* image, images)
        {
            delete image;
        }
        pageImageIndex = 1;
        pageIndex++;
        pageTotal++;
    }

    m_imageIndex = (pageIndex -1)* pageImageNum + pageImageIndex;
    //X单位是cm，Y单位是m
    QString imageText = nullptr;

    y = y* pixelProportion/1000 + yData;//单位换算
    x = x * pixelProportion/10; //单位换算
    imageText = " "+camserialname +" X:"+QString::number(x)+" Y:"+QString::number(y);
    //    qDebug()<<imageText;

    //页面图片展示
    pageTotal = flawRecordList.count() / pageImageNum;
    if(flawRecordList.count() % pageImageNum > 0)
    {
        pageTotal++;
    }
//    ui->totalPageLB->setText(QString::number(pageTotal));

    int row,col;//坐标 行，列
    row = (pageImageIndex-1) / (lineImageNum); //行
    col = (pageImageIndex-1) - row * lineImageNum;  //列

    QPoint startPoint;
    startPoint.setX(col*(imageSize.width()+imageSpaceInterval)+ 10);
    startPoint.setY(row*(imageSize.height()+imageSpaceInterval) + 20);

    ShowImageLabel *image = new ShowImageLabel(ui->scrollAreaWidgetContents);
    image->SetImageType(type);
    image->SetImagePath(imagePath);
    image->SetImageText(imageText);
    image->SetImageNum(m_imageIndex);
    image->setMaximumSize(imageSize);
    image->setMinimumSize(imageSize);
    image->move(startPoint);
    image->show();
    image->ShowImage();
//    imageList.append(image);

    ui->scrollAreaWidgetContents->setMinimumSize(0,row*(imageSize.height()+imageSpaceInterval) + 20+imageSize.height()+30);
    ui->scrollArea->verticalScrollBar()->setValue(row*(imageSize.height()+imageSpaceInterval) + 20+imageSize.height()+30);


    //Map 数据展示
    double X = x;
    double Y = y;
    int point_x = (X/10)*20 +35;
    int point_y = (Y/100)*35+25;
    if(point_x > 295)
    {
        point_x = 295;
    }
    qDebug()<<"添加点坐标 X:"<<point_x<<",Y:"<<point_y;

    FlawPoint p;
    p.type = type;
    p.point = QPoint(point_x,point_y);
    p.yData = yData;
    pointList.append(p);
}

void MainWindow::DisplayImage()
{

}

bool MainWindow::FilterSubControlInfo(int x, int y, double yData)
{
    foreach(FlawPoint point,pointList)
    {
        if(point.point.x() == x)
        {
            if(yData < point.yData + threshold_length_filter && yData > point.yData - threshold_length_filter)
            {
                return false;
            }
        }
    }
    return true;
}

void MainWindow::ReShowDetectImages()
{

}

/********************************************************
*
********       画布卷地图
*
********************************************************/
void MainWindow::DrawClothRoll()
{


}


void MainWindow::ClearAllView()
{
    //删除控件
    QList<ShowImageLabel*> images = ui->scrollAreaWidgetContents->findChildren<ShowImageLabel*>();
    foreach (ShowImageLabel* image, images)
    {
        delete image;
    }
    //清除列表
    imageList.clear();
    flawRecordList.clear();
//    pointList.clear();
    m_series->clear();
    m_seriesFlaw->clear();


}

void MainWindow::GetClothRollInfo(QString clothRollNo, QString robotNo,QString oprName)
{
    m_clothRollNo = clothRollNo;
    m_robotNo = robotNo;
    m_oprName = oprName;

    if(socketType < 2)
    {
        ui->NewClothRollPB->setEnabled(false);
        ui->startVisionPB->setEnabled(true);
        ui->exportPB->setEnabled(false);
    }
}

void MainWindow::ReceivePhotographCode(int yData)
{
    //调用海康摄像头拍照
    m_yData = yData;

    isover = true;
    //    for(int num = 0;num < cameraList.count();num++)
    //    {
    //        QDateTime current_date_time = QDateTime::currentDateTime();
    //        QString now = current_date_time.toString("yyyyMMddHHmmsszzz");
    //        QString pathdir = "C:\\waitdetection\\";
    //        cameraList[num]->SaveImage(pathdir);

    //        if(cameraList[num]->isSaveSuccess == false)
    //        {
    //            isover = false;
    //        }
    //    }

    if(isover)
    {
        qDebug("Pototh is Over");
        //        emit StartVisualInspectionSignal();
    }
}

void MainWindow::RecieveSelectHClothSlot(QString clothNo)
{
    m_selectClothRollNo = clothNo;
    m_imageIndex = 0;

    ClearAllView();

    hintDialog.SetProgressBarValue(0);
    flawRecordList.clear();

    QString histroyClothInfoPath = root_dir +"Image\\guiroiNG\\" + m_selectClothRollNo +"\\clothInfo.ini";
    qDebug()<<histroyClothInfoPath;

    ClothInfo clothInfo;
    //check clothinfo file
    if(QFile::exists(histroyClothInfoPath))
    {
        //读取配置文件
        QSettings config(histroyClothInfoPath,QSettings::IniFormat);

        //Software
        clothInfo.ClothNo = config.value("ClothNo").toString(); //布卷号
        clothInfo.Length = config.value("Length").toDouble();
        clothInfo.FlawNum = config.value("FlawNum").toInt();
        clothInfo.StartTime =config.value("StartTime").toString();
        clothInfo.EndTime = config.value("EndTime").toString();
    }

    m_clothRollNo = clothInfo.ClothNo;
    m_historyTotalNum = clothInfo.FlawNum;
    //展示统计信息
    ui->clothNoLB->setText(clothInfo.ClothNo);
    ui->clothLengthLB->setText(QString::number(clothInfo.Length));
    ui->clothNGNumberLB->setText(QString::number(clothInfo.FlawNum));


    std::thread receiveid(LoadHistoryClothRollThread,this);
    receiveid.detach();

    hintDialog.show();

    WriteLog("历史布卷接受线程结束");
//    LoadHistoryClothRoll();
}

void *MainWindow::LoadHistoryClothRollThread(void *pUser)
{
    if(pUser == NULL)
    {
        return NULL;
    }
    MainWindow* pHWnd = (MainWindow*)pUser;
    pHWnd->LoadHistoryClothRoll();
}

QList<FlawRecord> MainWindow::SortFlawRecord(QList<FlawRecord> list)
{
    for(int i=0;i<list.length()-1;i++)
    {
        for(int j=i+1;j<list.length();j++)
        {
            if(list[i].flawTime.toDouble() > list[j].flawTime.toDouble()) //大于则交换
            {
                FlawRecord temp;
                temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }
    return list;
}

void MainWindow::SocketCloseVisionSlot()
{
    on_closeVisionPB_clicked(true);
}

void MainWindow::SendMeterInfoSlot()
{
    plc.SendMessageToServer("#012");
}

//自动报表
void MainWindow::AutoReportSlot()
{
    QString time = QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");
    //    QString resoucePath = root_dir+"record\\"+m_clothRollNo+"_"+time+".xlsx";

    //新建报表

    QString record = QString::fromStdString(ariThread->reportFolder)+m_clothRollNo+"_"+time+".xlsx";
//    QList<QVariant> head;
//    head.append("布卷编号");
//    head.append("统计开始时间");
//    head.append("统计结束时间");
//    head.append("检出总数");
//    head.append("缺胶报警数");
//    head.append("填料颗粒");
//    head.append("异物报警数");

//    dataStorage.InitReport(record);
//    dataStorage.InitReportHead(head);


//    QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
//    QList<QVariant> data;
//    data.append(m_clothRollNo);
//    data.append(LastReportTime);
//    data.append(now);
//    data.append(ariThread->m_nCheckOutTotal);
//    data.append(ariThread->m_nNum1);
//    data.append(ariThread->m_nNum2);
//    data.append(ariThread->m_nNum3);
//    dataStorage.WriteData(data);

//    dataStorage.SaveReport();
//    dataStorage.CloseReport();

//    LastReportTime=now;
//    ariThread->m_nCheckOutTotal=0;
//    ariThread->m_nNum1=0;
//    ariThread->m_nNum2=0;
//    ariThread->m_nNum3=0;


    QList<QVariant> head;
    head.append("铜箔编号");
    head.append("瑕疵时间");
    head.append("NG序号");
    head.append("疵点类型");
    head.append("X轴布长位置");
    head.append("Y轴布长位置");

    dataStorage.InitReport(record);
    dataStorage.InitReportHead(head);

    if(flawRecordList.length() < 1)
    {
        return;
    }
    else
    {
        int index =1;
        for(int num = LastIndex ; num < flawRecordList.count();num++)
        {
            FlawRecord flawRecord=flawRecordList[num];
            QList<QVariant> linedata;
            if(flawRecord.type != 0)
            {
                linedata.append(m_clothRollNo);
                linedata.append(QDateTime::fromString(flawRecord.flawTime, "yyyyMMddHHmmsszzz").toString("yyyy-MM-dd HH:mm:ss"));
                linedata.append(index++);
                if(flawRecord.type == 1)
                    linedata.append("缺胶");
                if(flawRecord.type == 2)
                    linedata.append("填料颗粒");
                if(flawRecord.type == 3)
                    linedata.append("异物");
                linedata.append(flawRecord.x);
                linedata.append(flawRecord.y);

                dataStorage.WriteData(linedata);
            }
        }

        LastIndex=flawRecordList.count();

//        foreach(FlawRecord flawRecord,flawRecordList)
//        {
//            QList<QVariant> linedata;
//            if(flawRecord.type != 0)
//            {

//                linedata.append(m_clothRollNo);
//                linedata.append(QDateTime::fromString(flawRecord.flawTime, "yyyyMMddHHmmsszzz").toString("yyyy-MM-dd HH:mm:ss"));
//                linedata.append(index++);
//                if(flawRecord.type == 1)
//                    linedata.append("缺胶");
//                if(flawRecord.type == 2)
//                    linedata.append("填料颗粒");
//                if(flawRecord.type == 3)
//                    linedata.append("异物");
//                linedata.append(flawRecord.x);
//                linedata.append(flawRecord.y);

//                dataStorage.WriteData(linedata);
//            }
//        }
    }

    dataStorage.SaveReport();
    dataStorage.CloseReport();



    //    QString dstPath= dirName + "\\" + m_clothRollNo+".xlsx";
    //    copyFileToPath(resoucePath,dstPath,false);

}



void MainWindow::HeartBeatSlot()
{
    if(socketClient != NULL)
    {
        socketClient->SendMessageToServer("{\"code\":0}");
    }
}

void MainWindow::UpdateSocketInfoSlot(int code, double meterLength, QString clothRollNo, int workStatus)
{
    //WriteLog("code:"+QString::number(code)+",meterLength:"+QString::number(meterLength)+",clothRollNo:"+clothRollNo+",workStatus:"+QString::number(workStatus));
    //qDebug()<<"code"<<QString::number(code)<<"meterLength"<<QString::number(meterLength)<<"clothRollNo"<<clothRollNo<<"workStatus"<<QString::number(workStatus);
    switch (code) {
    case 0:
    {
        m_yData = meterLength;
        m_clothRollNo = clothRollNo;
        if(workStatus == 1)
        {
            on_startVisionPB_clicked(true);
        }
        //        else if(workStatus == 0)
        //        {
        //            on_closeVisionPB_clicked(true);
        //        }
        break;
    }
    case 1:
    {
        m_clothRollNo = clothRollNo;
        on_startVisionPB_clicked(true);
        break;
    }
    case 2:
    {
        on_closeVisionPB_clicked(true);
        break;
    }
    }
}

void MainWindow::UpdatePlcInfoSlot(double meterLength)
{
    m_yData = meterLength + handyDataEx;
}

void MainWindow::UpdateSocketImageSlot(QString imageName, QString camserialname, int x, int y, int type, double yData,double size)
{
    QString imagePath = root_dir + "Image\\guiroiNG\\"+m_clothRollNo+"\\"+imageName;
    AddDetectImageShow(imagePath,camserialname,x,y,type,yData,size,0);
}

void MainWindow::SendPlcCodeSlot(QString plcCode)
{
    if(socketType < 2)
    {
        emit SendPlcCode(plcCode);
    }
    else
    {
        QString code = "{\"code\":4,\"plcCode\":\""+plcCode+"\"}";
        socketClient->SendMessageToServer(code);
    }
}

void MainWindow::DataStorageAutoSave()
{
    isViRun = false;

    //        if(m_imageIndex == 0)
    //        {
    //            QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    //            QList<QVariant> data;
    //            data.append(m_clothRollNo);
    //            data.append(0);
    //            data.append("-");
    //            data.append(0);
    //            data.append(0);
    //            data.append("Camera1,2");
    //            data.append(now);
    //            dataStorage.WriteData(data);
    //        }

    //        //自动保存报表
    //        dataStorage.SaveReport();
    //        dataStorage.CloseReport();

    if(socketType < 2)
    {
        //创建配置文件
//        QFile file;
//        QString configPath = root_dir + "Image\\guiroiNG\\"+ m_clothRollNo + "\\clothInfo.ini";
//        if(file.exists(configPath))
//        {
//            QDateTime now = QDateTime::currentDateTime();
//            file.setFileName(configPath);
//            file.open(QIODevice::ReadWrite | QIODevice::Append);
//            QString configLength = "Length=" + QString::number(m_yData,'f',2) + "\n";
//            file.write(configLength.toLatin1());
//            QString configFlawNum = "FlawNum=" + QString::number(m_imageIndex) + "\n";
//            file.write(configFlawNum.toLatin1());
//            QString endTime = "EndTime="+now.toString("yyyy/MM/dd HH:mm:ss zzz")+"\n";
//            file.write(endTime.toLatin1());
//            file.close();
//        }
        QSettings config(root_dir + "Image\\guiroiNG\\"+ m_clothRollNo + "\\clothInfo.ini",QSettings::IniFormat);
        config.setValue("Length",QString::number(m_yData,'f',2));
        int preFlawNum = config.value("FlawNum",0).toInt();
        config.setValue("FlawNum",preFlawNum + m_imageIndex);
        QDateTime now = QDateTime::currentDateTime();
        config.setValue("EndTime",now.toString("yyyy/MM/dd HH:mm:ss zzz"));
    }

    ui->exportPB->setEnabled(true);
}

void MainWindow::CheckCameraInfo()
{
//    if(cameraList.count() < 1)
//    {
//        return;
//    }

//    isover = true;
//    for(int num = 0;num < cameraList.count();num++)
//    {
//        if(cameraList[num]->isFinishSavePic == false)
//        {
//            isover = false;
//        }
//    }

//    if(isover)
//    {
//        for(int num = 0;num < cameraList.count();num++)
//        {
//            cameraList[num]->isFinishSavePic = false;
//        }

//        if(ariOnoff == 1)
//        {
//            emit StartVisualInspectionSignal();
//        }
//    }
}

void MainWindow::on_NewClothRollPB_clicked(bool checked)
{
    Q_UNUSED(checked);

    //选择文件夹
       QString curPath=QCoreApplication::applicationDirPath(); //获取应用程序的路径
   //  QString curPath=QDir::currentPath();//获取系统当前目录
   //调用打开文件对话框打开一个文件

       QString dlgTitle="选择一个目录"; //对话框标题
       QString selectedDir=QFileDialog::getExistingDirectory(this,dlgTitle,curPath,QFileDialog::ShowDirsOnly);
       if (!selectedDir.isEmpty())
       {
           ui->lineEdit_path->setText(selectedDir);
           pathname = selectedDir;
           ariThread->pathname=pathname;
       }

}

//测试按钮
void MainWindow::on_pushButton_clicked(bool checked)
{
    isover = true;
    Q_UNUSED(checked);

    emit StartVisualInspectionSignal();
}

void MainWindow::ShowAllImage()
{

}

//开始检测
void MainWindow::on_startVisionPB_clicked(bool checked)
{
    Q_UNUSED(checked);

    if(socketType == 1)
    {
        if(socketServer->m_ClientList.length() < 1)
        {
            QMessageBox::information(this,"Info","无客户端连接,请保证客户端正常开启！");
            return ;
        }
    }

    if(isStartStatus)
    {
        return;
    }
    else
    {
        isStartStatus = true;
    }

    if(socketType < 2)
    {
        ui->startVisionPB->setEnabled(true);
    }

    ariThread->IQDflawType = ui->lineEdit->text().trimmed().toInt();

    ui->clothNoLB->setText(m_clothRollNo);

    showWidgetWidth = ui->groupBox_2->width() - 30;
    showHeightWidth = ui->groupBox_2->height();
    imageSize.setWidth((showWidgetWidth-(lineImageNum+1)*imageSpaceInterval)/lineImageNum);
    imageSize.setHeight((showWidgetWidth-(lineImageNum+1)*imageSpaceInterval)/lineImageNum);

    qDebug()<<"imageSize:"<<imageSize;

    QString path = "C:\\waitdetection\\";
    clearFiles(path);

    if(socketType < 2)
    {
        //wj96复位
        emit SendPlcCode(QString::number(resetCode));
        QThread::sleep(1);
        int ret = QMessageBox::question(this,"Ask","布卷长度是否清零?",QMessageBox::Yes | QMessageBox::No);
        if(ret == QMessageBox::Yes)
        {
            qDebug()<< "plc clear";
            //plc->编码器清零
            emit SendPlcCode("4");
            //清除报警列表
            AlarmList.clear();
        }
    }

    //清空绘图控件
    ClearAllView();

    //主分配分指令
    if(socketType == 1 && socketServer != NULL)
    {
        socketServer->m_clothNo = m_clothRollNo;
        socketServer->m_workStatus = 1;
        QString code = "{\"code\":1,\"clothRollNo\":\""+m_clothRollNo+"\"}";
        socketServer->SendMessageToClinet(code);

    }

    //当此检测数量置为0
    m_imageIndex = 0;
    pageImageIndex = 0;
    pageIndex = 1;
    pageTotal = 1;
    isHisCloth = false;

    ariThread->isViOver = false;
    ariThread->_isStop = false;
    //新建算法记录文件夹
    ariThread->SetRootPath(root_dir);
    ariThread->SetSocketParam(socketType,socketServerPath.toStdString());
    ariThread->SetClothRollNo(m_clothRollNo);
    ariThread->InitPath();
    ariThread->start();

    if(socketType < 2)
    {    //创建配置文件
        QSettings config(root_dir + "Image\\guiroiNG\\"+ m_clothRollNo + "\\clothInfo.ini",QSettings::IniFormat);
        QDateTime now = QDateTime::currentDateTime();
        config.setValue("StartTime",now.toString("yyyy/MM/dd HH:mm:ss zzz"));
        config.setValue("ClothNo",m_clothRollNo);
        config.setValue("pixelProportion",QString::number(pixelProportion));
        config.setValue("Address",m_GcAddress);
    }

    isViRun = true;
    showTimer->start(200);

    LastIndex=0;

    if(socketType < 2)
    {
        ui->NewClothRollPB->setEnabled(false);
        ui->closeVisionPB->setEnabled(true);
        ui->clothHistoryPB->setEnabled(false);
        ui->exportPB->setEnabled(false);
    }
    qDebug()<<"startVisionPB Over";

    ariThread->getAllFile();
}

//停止检测
void MainWindow::on_closeVisionPB_clicked(bool checked)
{
    if(!isStartStatus)
    {
        return ;
    }

    if(socketType < 2)
    {
        int ret = QMessageBox::question(this,"Info",QString::fromLocal8Bit("stop?"),QMessageBox::Yes | QMessageBox::No);
        if(ret == QMessageBox::No)
        {
            return;
        }
    }

    Q_UNUSED(checked);

    ui->meterExlineEdit->setText("0");
    handyDataEx=0;

    if(socketType < 2)
    {
        ui->closeVisionPB->setEnabled(false);
    }

    //主分配分指令
    if(socketType == 1 && socketServer != NULL)
    {
        socketServer->m_workStatus = 0;
        QString code = "{\"code\":2}";
        socketServer->SendMessageToClinet(code);
    }

    //停止拍照
    for(int num = 0;num < IKapCameraList.count();num++)
    {
        IKapCameraList[num]->bIsSavePic = false;
    }

    ariThread->isViOver = true;
    ariThread->_isStop = true;

    if(socketType < 2)
        plcTimer->stop();
    showTimer->stop();
    photoTimer->stop();
 //   ReportTimer->stop();

    //plc->编码器清零
    //    emit SendPlcCode("4");

    if(socketType < 2)
    {
        ui->NewClothRollPB->setEnabled(true);
        ui->closeVisionPB->setEnabled(false);
        ui->clothHistoryPB->setEnabled(true);
    }

    isStartStatus = false;
}

void MainWindow::on_pushButton_3_clicked(bool checked)
{
    ClearAllView();
}

bool MainWindow::copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    //toDir.replace("\\","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }
    /*
    QDir *createfile = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }
*/
    //end if
    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}

void MainWindow::clearFiles(QString folderFullPath)
{
    QDir dir(folderFullPath);
    dir.setFilter(QDir::Files);
    int fileCount = dir.count();
    for (int i = 0; i < fileCount; i++)
        dir.remove(dir[i]);
}

void MainWindow::on_pushButton_4_clicked(bool checked)
{
    emit StartVisualInspectionSignal();
}

//定时器刷新统计界面数据
void MainWindow::UpdateOnceStatistics()
{
    ui->clothLengthLB->setText(QString::number(m_yData,'f',2));
    ui->clothNGNumberLB->setText(QString::number(m_imageIndex));

//    for(int num = 0;num < cameraList.count();num++)//TCCTCC
//    {
//        cameraList[num]->m_yData = QString::number(m_yData,'f',2).toStdString();
//    }
    ariThread->yData = m_yData;
    if(socketType == 1)
    {
        if(socketServer != NULL)
            socketServer->yData = m_yData;
    }

    //m_yData=m_yData+0.05;    //家里测试用
    /***************************************更新map界限*****************************************/
    if(!ui->cBox_updataLimit->checkState())
    {
        if((m_yData-27)<0)
        {
            m_chart->axisY()->setRange(0,30);
        }else
        {
            m_chart->axisY()->setRange(m_yData-27,m_yData+3);
        }

        QPointF point0(-2,m_yData-AlarmDelay);
        QPointF point1(132,m_yData-AlarmDelay);
        m_seriesRemind->replace(0,point0);
        m_seriesRemind->replace(1,point1);
    }

      /***************************************报警*****************************************/
    if(AlarmList.length()>0)
    {
        if(AlarmList.at(0)<=(m_yData-AlarmDelay+0.55))
        {
            double d=AlarmList.takeFirst();
            emit SendPlcCode(QString::number(2));
        }
    }
}

void MainWindow::on_exportPB_clicked(bool checked)
{
    Q_UNUSED(checked);

    QString dirName =  QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         "",
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);

    if(dirName.isNull())
    {
        return;
    }

    QString time = QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");
    //    QString resoucePath = root_dir+"record\\"+m_clothRollNo+"_"+time+".xlsx";

    //新建报表

    QString record = dirName+"/"+m_clothRollNo+"_"+time+".xlsx";
    QList<QVariant> head;
    head.append("布卷编号");
    head.append("NG序号");
    head.append("报警类型");
    head.append("X轴布长位置");
    head.append("Y轴布长位置");
    head.append("相机ID");
    head.append("创建日期");
    dataStorage.InitReport(record);
    dataStorage.InitReportHead(head);

    if(flawRecordList.length() < 1)
    {
        QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        QList<QVariant> data;
        data.append(m_clothRollNo);
        data.append(0);
        data.append("-");
        data.append(0);
        data.append(0);
        data.append("Camera");
        data.append(now);
        dataStorage.WriteData(data);
    }
    else
    {
        int index =1;
        foreach(FlawRecord flawRecord,flawRecordList)
        {

            QList<QVariant> linedata;
            linedata.append(m_clothRollNo);
            linedata.append(index++);
            if(flawRecord.type == 1)
            {
                linedata.append("停机记录");
            }
            else
                linedata.append("亮灯记录");

            linedata.append(flawRecord.x);
            linedata.append(flawRecord.y);
            linedata.append(flawRecord.camera);
            linedata.append(QDateTime::fromString(flawRecord.flawTime, "yyyyMMddHHmmsszzz").toString("yyyy-MM-dd HH:mm:ss"));
            dataStorage.WriteData(linedata);
        }
    }


    dataStorage.SaveReport();
    dataStorage.CloseReport();


    //    QString dstPath= dirName + "\\" + m_clothRollNo+".xlsx";
    //    copyFileToPath(resoucePath,dstPath,false);
}

void MainWindow::on_exitPB_clicked(bool checked)
{

}



void MainWindow::LoadHistoryClothRoll()
{
//    flawRecordList.clear();

//    QString histroyClothInfoPath = root_dir +"Image\\roiNG\\" + m_selectClothRollNo +"\\clothInfo.ini";
//    qDebug()<<histroyClothInfoPath;

//    ClothInfo clothInfo;
//    //check clothinfo file
//    if(QFile::exists(histroyClothInfoPath))
//    {
//        //读取配置文件
//        QSettings config(histroyClothInfoPath,QSettings::IniFormat);

//        //Software
//        clothInfo.ClothNo = config.value("ClothNo").toString(); //布卷号
//        clothInfo.Length = config.value("Length").toDouble();
//        clothInfo.FlawNum = config.value("FlawNum").toInt();
//        clothInfo.StartTime =config.value("StartTime").toString();
//        clothInfo.EndTime = config.value("EndTime").toString();
//    }

//    m_clothRollNo = clothInfo.ClothNo;
//    //展示统计信息
//    ui->clothNoLB->setText(clothInfo.ClothNo);
//    ui->clothLengthLB->setText(QString::number(clothInfo.Length));
//    ui->clothNGNumberLB->setText(QString::number(clothInfo.FlawNum));


    QString histroyImgPath = root_dir +"Image\\guiroiNG\\" + m_selectClothRollNo+"\\";
    QDir dir(histroyImgPath);
    if(!dir.exists())
        return;

    QStringList filters;
    filters<<QString("*.bmp");
    dir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
    dir.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式

    //获取图片列表
    QStringList imgList;
    imgList = dir.entryList(QDir::Files);

//    m_imageIndex = 0;
//    pageIndex = 1;
    //int num = 1;
    //展示历史数据
    foreach(QString img , imgList)
    {
        //命名规则一
        //int num = img.split("~").at(0).toInt();
        //        QString camserialname = img.split("~").at(0);
        //        QStringList roiInfo = img.split("~").at(1).split("_");
        //        int x = roiInfo.at(1).toInt();
        //        int y = roiInfo.at(2).toInt();
        //        double yData = roiInfo.at(3).toDouble();
        //        int type = roiInfo.at(4).split(".").at(0).toInt();

        //命名规则二
        FlawRecord flawRecord;
        flawRecord.imgPath = root_dir +"Image\\guiroiNG\\" + m_selectClothRollNo+"\\"+img;
        flawRecord.camera = img.split("_").at(0);
        flawRecord.flawTime = img.split("_").at(1);
        flawRecord.x = img.split("_").at(2).toInt();
        flawRecord.y = img.split("_").at(3).toInt();
        flawRecord.yData = img.split("_").at(4).toDouble();
        flawRecord.IQDtype = img.split("_").at(5).split(".").at(0).toInt();
        flawRecord.type = img.split("_").at(9).split(".").at(0).toInt();
        //int num = roiInfo.at(5).split(".").at(0).toInt();
        flawRecordList.append(flawRecord);
    }


    //排序
    //    for(int i=0;i<flawRecordList.length()-1;i++)
    //    {
    //        for(int j=i+1;j<flawRecordList.length();j++)
    //        {
    //            if(flawRecordList[i].flawTime.toDouble() > flawRecordList[j].flawTime.toDouble()) //大于则交换
    //            {
    //                FlawRecord temp;
    //                temp = flawRecordList[i];
    //                flawRecordList[i] = flawRecordList[j];
    //                flawRecordList[j] = temp;
    //            }
    //        }
    //    }
    flawRecordList = SortFlawRecord(flawRecordList);

    foreach(FlawRecord flawRecord,flawRecordList)
    {
        QThread::msleep(5);
        emit UpdateImageShowListSignal(flawRecord.imgPath,flawRecord.camera,flawRecord.x,flawRecord.y,flawRecord.type,flawRecord.yData, flawRecord.IQDtype);
    }

    WriteLog("结束添加照片");

//    ui->exportPB->setEnabled(true);
}

void MainWindow::on_clothHistoryPB_clicked()
{
    isHisCloth = true;
    showWidgetWidth = ui->groupBox_2->width() - 30;
    showHeightWidth = ui->groupBox_2->height();
    imageSize.setWidth((showWidgetWidth-(lineImageNum+1)*imageSpaceInterval)/lineImageNum);
    imageSize.setHeight((showWidgetWidth-(lineImageNum+1)*imageSpaceInterval)/lineImageNum);

    QString histroyPath = root_dir +"Image\\guiroiNG\\";
    QDir dir(histroyPath);
    if(!dir.exists())
    {
        QMessageBox::information(this,"Info","无历史数据");
        return;
    }

    QStringList dirList;
    dirList = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
//    qDebug()<<dirList;

    QList<ClothInfo> clothInfoList;
    foreach(QString dir, dirList)
    {
        QString histroyClothInfoPath = histroyPath + dir +"\\clothInfo.ini";
        qDebug()<<histroyClothInfoPath;
        //check clothinfo file
        if(QFile::exists(histroyClothInfoPath))
        {
            //读取配置文件
            QSettings config(histroyClothInfoPath,QSettings::IniFormat);
            ClothInfo clothInfo;
            //Software
            clothInfo.ClothNo = config.value("ClothNo").toString(); //布卷号
            clothInfo.Length = config.value("Length").toDouble();
            clothInfo.FlawNum = config.value("FlawNum").toInt();
            clothInfo.StartTime =config.value("StartTime").toString();
            clothInfo.EndTime = config.value("EndTime").toString();
            clothInfoList.append(clothInfo);
        }
    }

    hisClothDialog.SetClothHistoryInfo(clothInfoList);
    hisClothDialog.exec();
}

void MainWindow::on_NewClothRollPB_clicked()
{

}

void MainWindow::on_pushButton_clicked()
{
    handyDataEx = ui->meterExlineEdit->text().toDouble();
    qDebug()<<"handyDataEx:"<<handyDataEx;
}


void MainWindow::on_headPagePB_clicked()
{

}

void MainWindow::on_prePagePB_clicked()
{

}

void MainWindow::on_nextPagePB_clicked()
{

}

void MainWindow::WriteLog(QString message)
{
    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("yyyy-MM-dd hh:mm:ss zzz");
    QString log = "[" + timeStr + "]:" + message + "\n";
    qDebug() << log; //测试用

    QString logPath = root_dir + "log\\" + time.toString("yyyyMMdd\\");
    QDir dir;
    if (!dir.exists(logPath))
    {
        dir.mkpath(logPath);
    }
    logPath += time.toString("yyyyMMdd") + ".txt";
    QFile outFile(logPath);
    outFile.open(QIODevice::ReadWrite | QIODevice::Append);
    outFile.write(log.toLatin1());
    outFile.close();
}

void MainWindow::on_endPagePB_clicked()
{

}

//显示index
//void MainWindow::tooltip(QPointF point, bool b)
//{
//    QVector<QPointF> points=m_series->pointsVector();

//    QVector<QPointF>::Iterator it=std::find(points.begin(),points.end(),point);
//    int index1 = std::distance(std::begin(points), it);

//    if (m_tooltip == 0)
//        m_tooltip = new Callout(m_chart);

//    if (b) {
//     //   m_tooltip->setText(QString("X: %1 \nY: %2 ").arg(point.x()).arg(i));
//                m_tooltip->setText(QString("X: %1 \nY: %2 \nIndex:%3 ").arg(point.x()).arg(point.y()).arg(index1));
//        m_tooltip->setAnchor(point);
//        m_tooltip->setZValue(11);
//        m_tooltip->updateGeometry();
//        m_tooltip->show();
//    } else {
//        m_tooltip->hide();
//    }
//}

//不显示index
void MainWindow::tooltip(QPointF point, bool b)
{
    if (m_tooltip == 0)
        m_tooltip = new Callout(m_chart);

    if (b) {
        m_tooltip->setText(QString("X: %1 \nY: %2").arg(point.x()).arg(point.y()));
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}


void MainWindow::on_btn_Unlock_clicked()
{
    if(ui->lineEdit_password->text()=="333444")
    {
        ui->comboBox_1->setEnabled(true);
        ui->comboBox_4->setEnabled(true);
    }
    ui->lineEdit_password->setText("");
}


void MainWindow::on_btn_Lock_clicked()
{
    ui->comboBox_1->setEnabled(false);
    ui->comboBox_4->setEnabled(false);
}


void MainWindow::on_comboBox_1_currentIndexChanged(int index)
{
    int diameter=0;
    if(index==0)
    {
        diameter=1;
    }
    else if(index==1)
    {
        diameter=2;
    }
    else if(index==2)
    {
        diameter=3;
    }
    else if(index==3)
    {
        diameter=4;
    }
    else if(index==4)
    {
        diameter=5;
    }

 //   ariThread->m_nDiameter=diameter;
    QSettings config(QCoreApplication::applicationDirPath()+"/config.ini",QSettings::IniFormat);
    config.setValue("User/Diameter",QVariant(diameter));
}


void MainWindow::on_comboBox_4_currentIndexChanged(int index)
{
//    int diameter=0;
//    if(index==0)
//    {
//        diameter=0.5;
//    }
//    else if(index==1)
//    {
//        diameter=1;
//    }
//    else if(index==2)
//    {
//        diameter=1.5;
//    }
//    else if(index==3)
//    {
//        diameter=2;
//    }
//    else if(index==4)
//    {
//        diameter=2.5;
//    }

//    ariThread->m_nDiameter_BlackDot=diameter;
//    QSettings config(QCoreApplication::applicationDirPath()+"/config.ini",QSettings::IniFormat);
//    config.setValue("User/DiameterBlackDot",QVariant(diameter));
}


void MainWindow::on_clothHistoryPB_clicked(bool checked)
{

}


void MainWindow::on_startVisionPB_clicked()
{

}

