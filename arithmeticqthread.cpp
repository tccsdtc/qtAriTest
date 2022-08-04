#include "arithmeticqthread.h"

#include <QDir>
#include <QProcess>
#include <QSettings>
#include <QTime>

#define STOPMACHINE 0    //是否停机，0是不停机，1是停机

const int minmaxNumThreshold = 2;//各选择多少个最大最小缺陷作为停机判断条件
#define isStopMachineByManyMinMaxDefect 0//是否采用多个最大最小缺陷作为停机判断条件

ArithmeticQThread::ArithmeticQThread(QObject *parent) : QThread(parent)
{
    _isStop = true;
    isFirst = true;
    isOvertime = false;
    detection_process_pid1 = 0;
    qRegisterMetaType<Mat>("Mat");
    LoadConifg();

    hAction = new hconAction(this);
    imgQueue=new CircleQueue(50000);
}

void ArithmeticQThread::LoadConifg()
{
    QSettings config(QCoreApplication::applicationDirPath()+"/config.ini",QSettings::IniFormat);
    pixelProportion = config.value("Camera/pixelProportion",0.175).toDouble(); //实际单位mm/像素px 光源
    m_bSaveNgImg = config.value("User/SaveNgImg",false).toBool();
    m_bSaveOkImg = config.value("User/SaveOkImg",false).toBool();
    m_bRun = config.value("User/run",true).toBool();    //是否运行算法

    //选择的直径
    m_nDiameter = config.value("User/Diameter",3).toInt();
    m_nDiameter_BlackDot = config.value("User/DiameterBlackDot",1).toInt();
    m_nThreshold   = config.value("User/Threshold",30).toInt();
}

void ArithmeticQThread::run()
{
    WriteLog("Run is Start!");
    Img_rgb = Mat::zeros(1024, 1024, CV_8UC1);//8位png
    dstImage1 = Mat::zeros(pixelHeight, pixelWidth, CV_8UC1);//CV_8UC3
    Init();
    uint time_lastImg = QDateTime::currentDateTime().toTime_t();
    yData_lastImg = yData;
    while(!_isStop)
    {
        if (!imgQueue->isEmpty())
        {
        //    WriteLog("Take befero.imgList.length:" + QString::number(imgList.length()));
            Mat  imgSrc = imgQueue->getTop().clone();
            qDebug()<<"Queue——"<<QString::number(imgQueue->m_front)<<"_"<<QString::number(imgQueue->m_rear);

     //       WriteLog("Take end.imgList.length:" + QString::number(imgList.length()));
            if (imgSrc.data)
            {
                extraRegioncropClassify(imgSrc);
            }
        }

//        if(HouChuLiImgList.length()> 0)
//        {
//        //    WriteLog("Take befero.HouChuLiImgList.length:" + QString::number(HouChuLiImgList.length()));
//            Mat imgSrc = HouChuLiImgList.takeFirst();
//      //      WriteLog("Take end.HouChuLiImgList.length:" + QString::number(HouChuLiImgList.length()));
//            if(imgSrc.data)
//            {
//                extraRegioncropClassify(imgSrc);
//            }
//        }

        _isVI = false;
        QThread::msleep(1);
    }

    emit DataStorageSignal();

    tcpSocket1->close();
    WriteLog("Run is Stop");
}


//void ArithmeticQThread::run()
//{
//    WriteLog("Run is Start!");
//    Img_rgb = Mat::zeros(1024, 1024, CV_8UC1);//8位png
//    dstImage1 = Mat::zeros(pixelHeight, pixelWidth, CV_8UC1);//CV_8UC3
//    Init();
//    uint time_lastImg = QDateTime::currentDateTime().toTime_t();
//    yData_lastImg = yData;
//    while(!_isStop)
//    {
//        if(imgList.length()> 0)
//        {
//            WriteLog("Take befero.imgList.length:" + QString::number(imgList.length()));
//            Mat imgSrc = imgList.takeFirst();
//            WriteLog("Take end.imgList.length:" + QString::number(imgList.length()));
//            if(imgSrc.data)
//            {
//                DetectionSlot(imgSrc);
//            }
//            yData_lastImg = yData;
//            time_lastImg = QDateTime::currentDateTime().toTime_t();
//        }
//        else if(yData - yData_lastImg >= 4.0 && (QDateTime::currentDateTime().toTime_t()-time_lastImg)>10 )
//        {
//            WriteLog("RestartCameras start yData: "+QString::number(yData)+" yData_lastImg: "+QString::number(yData_lastImg));
//            emit RestartCameras();
////            QDateTime startTime = QDateTime::currentDateTime();
////            while(true)
////            {
////                QDateTime now = QDateTime::currentDateTime();
////                if(startTime.msecsTo(now) > 120000)//
////                {
////                    break;
////                }
////                QCoreApplication::processEvents();//释放调度给其他任务机会
////            }
//            WriteLog("RestartCameras over");
//            yData_lastImg = yData;
//            time_lastImg = QDateTime::currentDateTime().toTime_t();
//        }
//        //临时处理,超过200清空，防止资源累计过卡
//        if(imgList.length()> 200)
//        {
//            WriteLog("imgList.length()> 200,clear");
//            imgList.clear();
//        }

//        if(HouChuLiImgList.length()> 0)
//        {
//        //    WriteLog("Take befero.HouChuLiImgList.length:" + QString::number(HouChuLiImgList.length()));
//            Mat imgSrc = HouChuLiImgList.takeFirst();
//      //      WriteLog("Take end.HouChuLiImgList.length:" + QString::number(HouChuLiImgList.length()));
//            if(imgSrc.data)
//            {
//                extraRegioncropClassify(imgSrc);
//            }
//        }

//        _isVI = false;
//        QThread::msleep(20);
//    }

//    emit DataStorageSignal();

//    tcpSocket1->close();
//    WriteLog("Run is Stop");
//}

void ArithmeticQThread::SetRootPath(QString rootPath)
{
    m_root_dir = rootPath.toStdString();
}

void ArithmeticQThread::Init()
{
    ack=0;
    socketTimeOutNum = 0;
    flag_exprocess=0;//外部进程存在标志位
    yData = 0.0;
    yData_lastImg = 0.0;
    //    ngOnceAllNum = 0;
    isViOver = false;

    //imagefolder = "C:\\staticwaitdetetion\\";//固定缺陷图来验证当前环境运行情况下检测服务正常，判断是否因为文件漏检或者漏扫描的逻辑错误//20210111pm7，试验结果，漏检的缺陷正常检测出的，说明逻辑流程上有漏检。
    _isVI = false;
    flag_alert_area=0;//因为缺陷面积过大的紧急停机标志位置位1
    flag_alert_quanlity=0;//因为缺陷数量过多的紧急停机标志位置位1

    //报表用
    m_nCheckOutTotal=0;
    m_nNum1=0;
    m_nNum2=0;
    m_nNum3=0;

    bool result;
    QString path;
    QDir dir;

    int internal_count=0;
    PROCESSENTRY32 pInfo;
    pInfo.dwSize = sizeof(PROCESSENTRY32);
    HANDLE proHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    BOOL bResult = Process32First(proHandle, &pInfo);
    if (!bResult)
    {
        //return false;
    }
    QString curProcessName;
    while (bResult)
    {
        curProcessName = QString("%1").arg(QString::fromUtf16(reinterpret_cast<const unsigned short *>(pInfo.szExeFile)));
        //qDebug() << curProcessName;
        if (curProcessName == "Rockhook-rd.exe")
        {
            internal_count =internal_count +1;
            //BOOL ret = true;
            if(internal_count >= 1)
            {
                flag_exprocess = 1;
                break;
            }
        }
        bResult = Process32Next(proHandle, &pInfo);
    }

    tcpSocket1=new QTcpSocket();//tcpSocket1=new QTcpSocket(this);
    connect(tcpSocket1,SIGNAL(readyRead()),this,SLOT(revData1()));
    connect(tcpSocket1,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError1(QAbstractSocket::SocketError)));

    if(flag_exprocess == 1)
    {
        tcpSocket1->abort();
        tcpSocket1->connectToHost("127.0.0.1",7000);//newTcpConnection1();
    }

    flag_exprocess = 1; //主机没有部署检测进程的时候，赋值0，方便写业务代码调试
}

void ArithmeticQThread::SetClothRollNo(QString clothRollNo)
{
    m_clothRollNo = clothRollNo;
}

void ArithmeticQThread::SetImagePixel(int width, int height)
{
    pixelWidth = width;
    pixelHeight = height;
}

void ArithmeticQThread::InitPath()
{
    QString path;
    QDir dir;
    bool result;

    imagefolder = "C:\\waitdetection\\";//一组图的检测路径waitdetection
    snapdir = "C:\\snaptest\\";

    bwsaveimagefolder = m_root_dir+"Image\\result\\";//检测结果缺陷白色二值化图存储路径
    originNGimagefolder = m_root_dir+"Image\\originNG\\";//NG原始图的存图路径originNG
    originOkimagefoler = m_root_dir+"Image\\originOk\\";//NG原始图的存图路径originNG
    allOriginImageFolder = m_root_dir + "\\Image\\allIamge\\";
    reportFolder= m_root_dir + "\\Report\\";

    if(m_socketType < 2)
    {
        roiNGimagefolder = m_root_dir+"Image\\roiNG\\";//NG单独小缺陷扣图的存图路径roiNG
        guiroiNGimagefolder = m_root_dir+"Image\\guiroiNG\\";//NG单独小缺陷扣图的存图路径guiroiNG，只是为了展示不得用于数据采集建库20210805am10
    }
    else
    {
        roiNGimagefolder = m_socketServerPath+"Image\\roiNG\\";
        guiroiNGimagefolder = m_socketServerPath+"Image\\guiroiNG\\";//NG单独小缺陷扣图的存图路径guiroiNG，只是为了展示不得用于数据采集建库20210805am10
    }

    //新建布卷号目录
    path = QString::fromStdString(imagefolder);
    if (!dir.exists(path))
    {
        result = dir.mkpath(path);
    }
    path = QString::fromStdString(snapdir);
    if (!dir.exists(path))
    {
        result = dir.mkpath(path);
    }

    originNGimagefolder += m_clothRollNo.toStdString() +"\\";
    path = QString::fromStdString(originNGimagefolder);
    if (!dir.exists(path))
    {
        result = dir.mkpath(path);
    }

    originOkimagefoler += m_clothRollNo.toStdString() +"\\";
    path = QString::fromStdString(originOkimagefoler);
    if (!dir.exists(path))
    {
        result = dir.mkpath(path);
    }

    allOriginImageFolder += m_clothRollNo.toStdString()+"\\";
    path = QString::fromStdString(allOriginImageFolder);
    if (!dir.exists(path))
    {
        result = dir.mkpath(path);
    }

    bwsaveimagefolder += m_clothRollNo.toStdString()+"\\";
    path = QString::fromStdString(bwsaveimagefolder);
    if (!dir.exists(path))
    {
        result = dir.mkpath(path);
    }

    roiNGimagefolder += m_clothRollNo.toStdString()+"\\";
    path = QString::fromStdString(roiNGimagefolder);
    if (!dir.exists(path))
    {
        result = dir.mkpath(path);
    }
    //NG单独小缺陷扣图的存图路径guiroiNG，只是为了展示不得用于数据采集建库20210805am10
    guiroiNGimagefolder += m_clothRollNo.toStdString()+"\\";
    path = QString::fromStdString(guiroiNGimagefolder);
    if (!dir.exists(path))
    {
        result = dir.mkpath(path);
    }

    //报表地址
    reportFolder+= m_clothRollNo.toStdString()+"\\";
    path=QString::fromStdString(reportFolder);
    if (!dir.exists(path))
    {
        result = dir.mkpath(path);
    }


}

void ArithmeticQThread::newTcpConnection1()
{
    if(tcpSocket1->state()!=QAbstractSocket::ConnectedState)
    {
        tcpSocket1->abort();
        tcpSocket1->connectToHost("127.0.0.1",7000);//local socket
    }
}

void ArithmeticQThread::StopVisionInspection()
{
    tcpSocket1->close();
    //tcpSocket1->abort();
}

void ArithmeticQThread::SetThreshold(QString cameraName,int thresholdPixel,int thresholdArea,int thresholdQuanlity,int thresholdRatio,
                                     int thresholdLenth,int thresholdWidth,int thresholdAlertCujieshaMax,int thresholdAlertCujieshaMin,
                                     int stopCode,double restartCameraTime)
{
    m_cameraName = cameraName;
    threshold_pixel = thresholdPixel;
    threshold_alert_area = thresholdArea;
    threshold_alert_quanlity = thresholdQuanlity;
    threshold_alert_ratio = thresholdRatio;
    threshold_alert_lenth = thresholdLenth;
    threshold_alert_width = thresholdWidth;
    threshold_alert_cujiesha_max = thresholdAlertCujieshaMax;//450;//参数
    threshold_alert_cujiesha_min = thresholdAlertCujieshaMin;
    m_stopCode = stopCode;
    m_dRestartCameraTime = restartCameraTime;
}

void ArithmeticQThread::SetSocketParam(int socketType, cv::String serverPath)
{
    m_socketType = socketType;
    m_socketServerPath = serverPath;
}

void ArithmeticQThread::displayError1(QAbstractSocket::SocketError)
{
    WriteLog("TcpSocket is abnormal close!");
    tcpSocket1->close();
}

void ArithmeticQThread::revData1()
{
    QString data=tcpSocket1->readAll();
    //    WriteLog("data1:"+data);
    ack = data.toInt();
    //ui->textEdit->setText(data);
}

void ArithmeticQThread::SetViFlag()
{
    _isVI = true;
}

void ArithmeticQThread::StartVi()
{
    _isStop = false;
}
void ArithmeticQThread::StopVi()
{
    qDebug()<<"receieve stop signal!";
    _isStop = true;
}

bool ArithmeticQThread::copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist)
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

bool ArithmeticQThread::moveFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist)
{
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        qDebug()<<"File is not exists";
        return false;
    }

    //if(!QFile::copy(sourceDir, toDir))
    //{
    //qDebug()<<"File can't copy";
    //return false;
    //}

    //if(!QFile::remove(sourceDir))
    //{
    // return false;
    //}

    DosMoveFile(sourceDir,toDir);
    //end if
    //if(!QFile::rename(sourceDir, toDir))
    //{
    //  return false;
    //}
    return true;
}

void ArithmeticQThread::clearFiles(QString folderFullPath)
{
    QDir dir(folderFullPath);
    dir.setFilter(QDir::Files);
    int fileCount = dir.count();
    for (int i = 0; i < fileCount; i++)
        dir.remove(dir[i]);
}

void ArithmeticQThread::extraRegioncropClassify(Mat dst_rgb_Contour)
{
    mtx.lock();
 //  WriteLog("extraRegioncropClassifyStop is Start.");
    QTime t1;
    t1.start();

 //   Mat dst_rgb_Contour=img.clone();
    int area = dst_rgb_Contour.cols*dst_rgb_Contour.rows;

    double widthReal = pixelProportion * dst_rgb_Contour.cols;
    double heightReal = pixelProportion * dst_rgb_Contour.rows;

    TestType=2;
    if(TestType==1)
    {
        //在线情况针对是一张图片停机，在线停机为节约时间直接获得第一个缺陷即代表当前整个图片，一卷布检测完后，再对所有缺陷识别报总体表
        QFile dataFile("C:\\snaptest\\analysisresult\\result.txt");
        {
            imagesave_str = "C:\\snaptest\\analysisresult\\"+QString::number(0)+".bmp";
            //fileindex_region = fileindex_region+1;
            //imwrite(imagesave_str.toLatin1().data(), max_dst_rgb_Contour);//存储成bmp格式图片
            imwrite(imagesave_str.toLatin1().data(), dst_rgb_Contour);//存储成bmp格式图片

            if (dataFile.open(QFile::WriteOnly|QIODevice::Text))
            {
                QTextStream data(&dataFile);
                //在线情况针对是一张图片停机，在线停机为节约时间直接获得第一个缺陷即代表当前整个图片，一卷布检测完后，再对所有缺陷识别报总体表
                data << QString::number(1);
            }
            dataFile.close();
            tcpSocket1->write("2");

            QDateTime startTime2 = QDateTime::currentDateTime();
            while(true)
            {
                QDateTime now2 = QDateTime::currentDateTime();
                if(startTime2.msecsTo(now2) > 300)//300
                {
                    WriteLog("Ari 2 is TimeOut.Ack is "+ QString::number(ack));
                    isOvertime =true;
                    break;
                }

                QCoreApplication::processEvents();//释放调度给其他任务机会
                if(ack==7)
                {
                    ack = 0;
                    isOvertime =false;
                    break;
                }
            }

            if(isOvertime == false)
            {
                if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
                {
                    QTextStream data(&dataFile);
                    QStringList fonts;
                    QString line;

                    //if(index == 0)
                    {
                        line = data.readLine();
                        line.remove('\n');
                        fonts<<line;
                        object_type[0]=line.toInt()+1;
                        WriteLog("detection type:"+QString::number(object_type[0]));
                    }
                }
                dataFile.close();
                //停机代码
                IQDflawType = object_type[0];
            }
        }
    }

    double proSize=0;
    if(TestType==2)
    {
        /*
    1.缺胶点状+线状，模糊点，鱼眼
    2.异物，黑色线状
    3.填料颗粒(白色点状)
    4.填料颗粒(黑白相间线状)
    5.问题边
    6.黑点
    7.正常图
    8.折痕
    9.大面积缺胶
    */
        /**********************************************************IQD分类后的后处理************************************************************/
        bool IsEdge;

        if (IQDflawType == 1)
        {
            IsEdge=false;
            HalconCpp::HObject himg = hAction->MatToHObject(dst_rgb_Contour);
            flawType = hAction->FilterAction1(himg, IsEdge, 3, IQDflawType, FlawSize);
            proSize=FlawSize;
        }
        if (IQDflawType == 2)
        {
            HalconCpp::HObject himg = hAction->MatToHObject(dst_rgb_Contour);
            flawType = hAction->FilterAction2(himg, IsEdge,1, IQDflawType, FlawSize);
            proSize=FlawSize;
        }

        if (IQDflawType == 3)
        {
            HalconCpp::HObject himg = hAction->MatToHObject(dst_rgb_Contour);
            flawType = hAction->FilterAction3(himg, IsEdge,1, IQDflawType, FlawSize);
        }

        if (IQDflawType == 4)
        {
            flawType = 1;
        }

        if (IQDflawType == 5)
        {
            IsEdge=true;
            HalconCpp::HObject himg = hAction->MatToHObject(dst_rgb_Contour);
            flawType = hAction->FilterAction5(himg ,IsEdge, 3,1, IQDflawType,FlawSize);
            proSize=FlawSize;
        }

        if (IQDflawType == 6)
        {
            HalconCpp::HObject himg = hAction->MatToHObject(dst_rgb_Contour);
            flawType = hAction->FilterAction6(himg, IsEdge,1, IQDflawType, FlawSize);
            proSize=FlawSize;
        }

        if (IQDflawType == 7)
        {
            HalconCpp::HObject himg = hAction->MatToHObject(dst_rgb_Contour);
            flawType = hAction->FilterAction7(himg, IsEdge,3, IQDflawType, FlawSize);
            proSize=FlawSize;
        }
    }

    if(flawType==0)
    {
        TestNum_0++;
    }

    if(flawType==1)
    {
        TestNum_1++;
    }



    qDebug()<<QString::number(flawType)<<"__"<<QString::number(TestNum_0)<<"__"<<QString::number(TestNum_1)<<"__"<<QString::number(TestNum_0+TestNum_1); //测试用
    //Img0_old(r).copyTo(dst_rgb_Contour);
    //HalconCpp::HImage himg = hAction->MatToHImage(dst_rgb_Contour);
    //flawType=hAction->FilterAction3(himg,m_nDiameter,m_nThreshold);
    /********************************************************************************************************************************************/


    QString timeGuiRoi = QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");
    double yDataTemp = yData;
    double areaReal = area * pixelProportion * pixelProportion;
    imagesave_str =  QString::fromStdString(roiNGimagefolder) +m_cameraName+"_"+timeGuiRoi +
            "_"+QString::number(1)+"_"+QString::number(2)+"_"+QString::number(yDataTemp,'f',2)+"_"+QString::number(IQDflawType)+
            "_"+QString::number(areaReal,'f',2)+"_"+QString::number(widthReal,'f',2)+"_"+QString::number(heightReal,'f',2)+
            "_"+QString::number(flawType)+".bmp";
//    if(flawType == 1)
    {
        WriteLog(imagesave_str);
    }
    imwrite(imagesave_str.toLatin1().data(), dst_rgb_Contour);//存储成bmp格式图片
    emit UpdateImageShowListSignal(imagesave_str,m_cameraName,1,2,flawType,3,proSize,IQDflawType);

    qDebug()<<"时间"<< t1.elapsed()<<"ms";
    mtx.unlock();
 //   WriteLog("extraRegioncropClassifyStop is Over.");
}

void ArithmeticQThread::WriteLog(QString message)
{
    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("yyyy-MM-dd hh:mm:ss zzz");
    QString log ="["+timeStr+"]:"+message+"\n";
    qDebug()<<log; //测试用

    QString logPath = QString::fromStdString(m_root_dir)+"log\\"+time.toString("yyyyMMdd\\");
    QDir dir;
    if(!dir.exists(logPath))
    {
        dir.mkpath(logPath);
    }
    logPath += time.toString("hh")+".txt";
    QFile outFile(logPath);
    outFile.open(QIODevice::ReadWrite | QIODevice::Append);
    outFile.write(log.toLatin1());
    outFile.close();
}

bool ArithmeticQThread::DosRemoveFile(QString path)
{
    QString cmdstring = "del /s/f/q " + path;
    QStringList arguments;
    arguments << "/c";
    arguments << cmdstring;
    QProcess::startDetached( "cmd", arguments );

    return true;
}

bool ArithmeticQThread::DosMoveFile(QString src,QString dst)
{
    QString cmdstring = "move " + src + " " + dst;
    QStringList arguments;
    arguments << "/c";
    arguments << cmdstring;
    QProcess::startDetached( "cmd", arguments );

    return true;
}

void ArithmeticQThread::ReceiveCameraSlot(Mat imgSrc)
{
    WriteLog("Receive Camera!");
    imgList.append(imgSrc);
}

void ArithmeticQThread::DetectionSlot(Mat imgSrc)
{
    WriteLog("lock.arithmetic slot is start!");
    mtx.lock();
    //    WriteLog("arithmetic slot is start!");
    QDateTime time = QDateTime::currentDateTime();
    timeStr = time.toString("yyyyMMddHHmmsszzz");

//只存图
    if(!m_bRun)
    {
        imagesave_str = QString::fromStdString(originOkimagefoler)+ m_cameraName+"_"+timeStr+"_Y_"+QString::number(yData,'f',2) +".bmp";
        imwrite(imagesave_str.toLatin1().data(), imgSrc);//存储成bmp格式图片
        mtx.unlock();
        return;
    }

    cv::Mat channels[3];

    Mat Img0;
    Mat Img0_png;
    Mat Img;
    //Mat Img_rgb;
    //Mat Img_rgb_view;
    vector<Vec4i> hierarchy_region;
    vector<vector<Point>> allContours_region;

    if (flag_exprocess==1)//1表示外部检测进程存在,并且一组2张的图像成功存储在waitdetection文件中
    {
        newTcpConnection1();//通过socket申请连接外部进程，如果已经连接就不用再次连接
        //判断
        if(imgSrc.empty())
        {
            WriteLog("img empty");
        }

        if(!imgSrc.data)
        {
            WriteLog("img data is null");
        }

        //不光每个区域检测前清零，每张图检测前停机标志量也要清零，
        flag_alert_area=0;//因为缺陷面积过大的紧急停机标志位置位1
        flag_alert_quanlity=0;//因为缺陷数量过多的紧急停机标志位置位1
        flag_alert_ratio=0;//因为缺陷畸形比的紧急停机标志位置位1
        flag_alert_lenth=0;//因为缺陷长度的紧急停机标志位置位1
        flag_alert_width=0;//因为缺陷宽度的紧急停机标志位置位1
        maxIndex = -1;
        minIndex = -1;
        isMaxProcess = false;
        isMinProcess = false;

        vecSizes.clear();
        vecUndetectedIndex.clear();

        Mat img = imgSrc.clone();
        if(img.cols != pixelWidth || img.rows != pixelHeight)
        {
            WriteLog("size is failed");
            return;
        }

        //    cv::split(img, channels);
        //    cv::Mat Img0_old = channels[1];
        Img0_old = img;

        WriteLog("resize before");
        cv::resize(Img0_old, Img_rgb, Img_rgb.size(), INTER_NEAREST);

        cv::imwrite("C:\\snaptest\\0.png", Img_rgb);
        WriteLog("check1:"+QString::number(tcpSocket1->write("1001")));

        QDateTime startTime = QDateTime::currentDateTime();

        isOvertime = false;
        while(true)
        {
            QDateTime now = QDateTime::currentDateTime();
            if(startTime.msecsTo(now) > 500)//2500
            {
                WriteLog("Ari 1 is Time Out.Ack:"+QString::number(ack));
                isOvertime = true;
                if(socketTimeOutNum > 2)
                {
                    WriteLog("Reconnect socket!!!");
                    RestartAriExe();
                    tcpSocket1->abort();
                    tcpSocket1=new QTcpSocket();//tcpSocket1=new QTcpSocket(this);//重新连接应该跟关闭qt软件重新打开方式完整初始化再连接 20210806pm11
                    connect(tcpSocket1,SIGNAL(readyRead()),this,SLOT(revData1()));
                    connect(tcpSocket1,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError1(QAbstractSocket::SocketError)));
                    tcpSocket1->connectToHost("127.0.0.1",7000);
                    QThread::msleep(20);
                    WriteLog("Reconnect check:"+QString::number(tcpSocket1->write("1001")));
                }
                else
                {
                    socketTimeOutNum++;
                }
                break;
            }

            QCoreApplication::processEvents();//释放调度给其他任务机会
            if(ack==6)
            {
                //            WriteLog("ack:"+QString::number(ack));
                ack = 0;
                socketTimeOutNum = 0;
                isOvertime = false;
                break;
            }
        }

        if(isOvertime == false)
        {
            WriteLog("Start!Load 0_predict.png");
            QString str_file1 = "C:\\snaptest\\0_predict.png";
            string string_file1 = str_file1.toStdString();
            Img = imread(string_file1,0); //结果
            WriteLog("End!Load 0_predict.png");

            if(threshold_pixel > 255)
            {
                WriteLog("threshold_pixel > 255");
                QSettings config(QCoreApplication::applicationDirPath()+"/config.ini",QSettings::IniFormat);
                qDebug()<<"configPath:"<<QCoreApplication::applicationDirPath()+"/config.ini";
                threshold_pixel = config.value("Arithmetic/threshold_pixel").toInt();
//                threshold_alert_area = config.value("Arithmetic/threshold_alert_area").toInt();
                threshold_alert_quanlity = config.value("Arithmetic/threshold_alert_quanlity").toInt();
//                threshold_alert_ratio = config.value("Arithmetic/threshold_alert_ratio").toInt();
//                threshold_alert_lenth = config.value("Arithmetic/threshold_alert_lenth").toInt();
//                threshold_alert_width = config.value("Arithmetic/threshold_alert_width").toInt();
//                threshold_alert_cujiesha_max = config.value("Arithmetic/threshold_alert_cujiesha_max").toInt();
//                threshold_alert_cujiesha_min = config.value("Arithmetic/threshold_alert_cujiesha_min").toInt();
            }

            for( int j = 0; j < Img.rows ; j++ )
            {
                for( int i = 0; i < Img.cols; i++ )
                {
                    uchar pixel = Img.at<uchar>(j, i);
                    if(pixel>threshold_pixel)//2
                        Img.at<uchar>(j, i) = 255;
                    else
                        Img.at<uchar>(j, i) = 0;
                }
            }

//            WriteLog("ari end");

            WriteLog("Edge before0!");
            cv::resize(Img, dstImage1, dstImage1.size(), INTER_NEAREST);  //指定size的大小
            WriteLog("Edge before1!");
            offset_x_leftedge_yanbu = 0;
            Mat dstImage_leftedge_bw;
            Rect rect(0, 0, int(pixelWidth/10), int(pixelHeight/10)); //区域的左上角点的坐标为（0,0,宽度,高度）
            Mat Img0_old_rect = Img0_old(rect);

            WriteLog("Edge is start!");
            //主机侧拍黑边情况
            if(m_socketType == 1)
            {
                cv::threshold(Img0_old_rect,dstImage_leftedge_bw,20,255,cv::THRESH_BINARY);
                for( int j = 0; j < dstImage_leftedge_bw.cols ; j++ )
                {
                    uchar  left_tmp= dstImage_leftedge_bw.at<uchar>(100,j);
                    if(left_tmp==0)//
                    {
                        offset_x_leftedge_yanbu =offset_x_leftedge_yanbu+1;
                    }
                }
            }
            if(m_socketType == 2)
            {
                cv::threshold(Img0_old_rect,dstImage_leftedge_bw,200,255,cv::THRESH_BINARY);
                for( int j = 0; j < dstImage_leftedge_bw.cols ; j++ )
                {
                    uchar  left_tmp= dstImage_leftedge_bw.at<uchar>(100,j);
                    if(left_tmp>0)//
                    {
                        offset_x_leftedge_yanbu =offset_x_leftedge_yanbu+1;
                    }
                }
            }
            WriteLog("Edge is over!");

            //抠图每个缺陷区域小图并按照序号进行存储，同时填充缺陷x，y图内的位置
            Mat dstImage1_bw;
            cv::threshold(dstImage1,dstImage1_bw,0,255,cv::THRESH_BINARY);
            findContours( dstImage1_bw, allContours_region, hierarchy_region, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, Point(0, 0));
            WriteLog("allContours_region.size():"+QString::number(allContours_region.size()));
            WriteLog("start move img");

            //保存源图
            if(allContours_region.size()>=1)//如果是缺陷图，当前原始图再保存到D:\\originNG文件夹中，为以后研究从减小数据筛选工作量做准备
            {
                //保存到NG源图
                imagesave_str = QString::fromStdString(originNGimagefolder)+ m_cameraName+"_" +timeStr+"_Y_"+QString::number(yData,'f',2) +".bmp";
               //WriteLog(imagesave_str);
                if(m_bSaveNgImg)
                    imwrite(imagesave_str.toLatin1().data(), img);//存储成bmp格式图片
            }
            else
            {
                //保存到OK源图
               imagesave_str = QString::fromStdString(originOkimagefoler)+ m_cameraName+"_"+timeStr+"_Y_"+QString::number(yData,'f',2) +".bmp";
               if(m_bSaveOkImg)
                    imwrite(imagesave_str.toLatin1().data(), img);//存储成bmp格式图片
            }

            WriteLog("end move img");
            //对缺陷数量过多的阈值设置，个数等于3个缺陷就是严重了，紧急停机
            if(allContours_region.size()>=threshold_alert_quanlity)
            {
                flag_alert_quanlity=1;//因为缺陷数量过多的紧急停机标志位置位1
            }

            double maxArea = 0;
            double minArea = 0;
            if(allContours_region.size()>0)//如果存在至少一个区域数量 20210905am11
            {
                maxArea = cv::contourArea(allContours_region[0]);//大小比较前初始复制第一个元素
                minArea = cv::contourArea(allContours_region[0]);//大小比较前初始复制第一个元素
            }
            for(size_t i = 0; i < allContours_region.size(); i++)
            {
                double area = cv::contourArea(allContours_region[i]);
                if(area >= maxArea)
                {
                    maxArea = area;
                    //                    maxContour = allContours_region[i];
                    maxIndex = i;
                }
                if(area <= minArea)//20210905am11前提一张图至少有2个检出区域20210828pm1，最小的考虑布污
                {
                    minArea = area;//前提一张图至少有2个检出区域20210828pm1，最小的考虑布污
                    //                    minContour = allContours_region[i];//前提一张图至少有2个检出区域20210828pm1，最小的考虑布污
                    minIndex = i;
                }
                vecSizes.push_back(SizeSorter(area, i));
            }

#if isStopMachineByManyMinMaxDefect
            sort(vecSizes.begin(), vecSizes.end(), SizeSorter::cmp);//vecSizes按面积从小到大排序

            if (vecSizes.size() <= minmaxNumThreshold*2)
            {
                for (auto& sizeSorter : vecSizes)
                {
                    vecUndetectedIndex.push_back(sizeSorter.index);
                }
            }
            else
            {
                for(int index = 0; index < minmaxNumThreshold; ++index)
                {
                    vecUndetectedIndex.push_back(vecSizes[index].index);
                }

                for(int index = minmaxNumThreshold-1; index >= 0; --index)
                {
                    vecUndetectedIndex.push_back(vecSizes[vecSizes.size()-1-index].index);
                }
            }
#endif

            for(int i_region=0; i_region < allContours_region.size() && i_region < threshold_alert_quanlity; i_region++)
            {

            }

            if(allContours_region.size() >= threshold_alert_quanlity)
            {
            }
            WriteLog("end roi img");

            //            WriteLog("BW img");
        }
    }
    //    WriteLog("arithmetic slot is end!");
    mtx.unlock();
    WriteLog("unlock.arithmetic slot is end!");
}

void ArithmeticQThread::StopMachine(int size,vector<Point> maxContour)
{
    if(m_socketType == 1)//反射工位
    {
        StopMachineReflect(size,maxContour);
    }
    else if(m_socketType == 2)//透射工位   //7.17修改前代码
    {
        StopMachineTraverse(size,maxContour);
    }
}


//透射当前一张图所有缺陷ui展示完后马上停机，不在循环内停机是避免客户没有看到所有缺陷ui不全，误认为剩下缺陷没有检测到
//1、	停机疵点(停机粗节纱、胶结经污、号数不对、稀密弄、布污)，只要检测到，均需要停机
//2、	报警疵点(含松紧经、松纬)大于等于5个，需要停机
//3、	报警疵点(含小粗节纱)大于等于20处，可判定为浆斑起毛、密集性散丝需要停机
//4、	报警疵点(含散烂边)计算像素点大于等于10000像素(1万像素)需要停机
//5、	报警疵点(含断经断纬短纬、中等粗节纱、纬纱弯曲、经起毛、纬向移纱、布皱)大于等于3处停机
void ArithmeticQThread::StopMachineTraverse(int size,vector<Point> maxContour)
{
    if(size>=1)//前提是缺陷图，而不是正常图
    {
        if(object_type[0]==1)
        {
            if((cv::contourArea(maxContour)>=40)){
                flawType = 1;
            }
            if((cv::contourArea(maxContour)>=6000)&&size>=2)//10000
            {
                flawType = 1;
            }
        }    
        if(flawType == 1)
        {
            QString log = "Alarm is SendPlcCode : " + QString::number(m_stopCode);
            WriteLog(log);
       //     emit SendPlcCode(QString::number(m_stopCode));
        }
    }
}


//反射工位当前一张图所有缺陷ui展示完后马上停机，不在循环内停机是避免客户没有看到所有缺陷ui不全，误认为剩下缺陷没有检测到
//1、停机疵点(停机粗节纱、胶结经污、停机经起毛、布污)，只要检测到，均需要停机
//2、报警疵点(含小粗节纱)大于等于20处，可判定为浆斑起毛、密集性散丝需要停机
//3、报警疵点(含散烂边)计算像素点大于等于10000像素(1万像素)需要停机
//4、报警疵点(含中等粗节纱、松紧经、经起毛、布皱)大于等于3处停机
//5、停机疵点破洞，目前还没有收集到，后续补充
void ArithmeticQThread::StopMachineReflect(int size,vector<Point> maxContour)
{
    if(size>=1)//前提是缺陷图，而不是正常图
    {
        if(object_type[0]==1)
        {
            if((cv::contourArea(maxContour)>=40))
            {
                flawType = 1;
            }          
        }
        if(flawType == 1)
        {
            QString log = "Alarm is SendPlcCode : " + QString::number(m_stopCode);
            WriteLog(log);
            emit SendPlcCode(QString::number(m_stopCode));
        }
    }
}

void ArithmeticQThread::KillAriExe()
{
    bool status =findProcessandByQProcess("Rockhook-rd.exe");
    killProcessandByQProcess();
    WriteLog("kill Ari exe.");
}

int ArithmeticQThread::CheckROIType(QString imgPath)
{
    if (flag_exprocess==1)//1表示外部检测进程存在,并且一组2张的图像成功存储在waitdetection文件中
    {
        newTcpConnection1();//通过socket申请连接外部进程，如果已经连接就不用再次连接

        QFile dataFile("C:\\snaptest\\analysisresult\\result.txt");
        QString imagesave_str = "C:\\snaptest\\analysisresult\\0.bmp";
        //Mat dst_rgb_Contour = img;
        //imwrite(imagesave_str.toLatin1().data(), dst_rgb_Contour);//存储成bmp格式图片
        QFile::remove("C:\\snaptest\\analysisresult\\0.bmp");
        bool ret = QFile::copy(imgPath,imagesave_str);
        if(!ret)
        {
            WriteLog("ImgFile remove failed!");
        }

        if (dataFile.open(QFile::WriteOnly|QIODevice::Text))
        {
            QTextStream data(&dataFile);
            data<<QString::number(1);
        }
        dataFile.close();

        WriteLog("check3:" + QString::number(tcpSocket1->write("2")));
        QDateTime startTime2 = QDateTime::currentDateTime();

        while(true)
        {
            QDateTime now2 = QDateTime::currentDateTime();
            if(startTime2.msecsTo(now2) > 180)//300
            {
                WriteLog("Ari 3 is TimeOut.Ack is "+ QString::number(ack));
                isOvertime =true;
                break;
            }

            QCoreApplication::processEvents();//释放调度给其他任务机会
            if(ack==7)
            {
                ack = 0;
                isOvertime =false;
                break;
            }
        }

        if(isOvertime == false)
        {
            if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
            {
                QTextStream data(&dataFile);
                QStringList fonts;
                QString line;

                //if(index == 0)
                {
                    line = data.readLine();
                    line.remove('\n');
                    fonts<<line;
                    object_type[0]=line.toInt()+1;
                    WriteLog("detection type:"+QString::number(object_type[0]));
                }
            }
            dataFile.close();
            return object_type[0];
            //停机代码
        }
        return -1;
    }
    return -1;
}

bool ArithmeticQThread::findProcessandByQProcess(const QString &strExe)
{
    bool bResult = false;
    // 判断进程是否存在
    QProcess tasklist;
    tasklist.start("tasklist",
                   QStringList() << "/NH"
                   << "/FO" << "CSV"
                   << "/FI" << QString("IMAGENAME eq %1").arg(strExe));
    tasklist.waitForFinished();
    QString strOutput = tasklist.readAllStandardOutput();
    //qDebug() << strOutput;
    QStringList strOutputList= strOutput.split( "\n" );
    qDebug() <<strOutputList.at(0);
    //qDebug() <<strOutputList.at(1);
    QString process_str1 = strOutputList.at(0);
    //QString process_str2 = strOutputList.at(1);
    //如果进程存在
    if (strOutput.startsWith(QString("\"%1").arg(strExe)))
    {
        QStringList strlist1 = process_str1.split(",");
        //QStringList strlist2 = process_str2.split(",");
        QString str1 = strlist1.at(1).toLocal8Bit().data();
        //QString str2 = strlist2.at(1).toLocal8Bit().data();
        detection_process_pid1 = (str1.replace("\"","")).toInt();
        //detection_process_pid2 = (str1.replace("\"","")).toInt();
        qDebug() << detection_process_pid1;
        //qDebug() << detection_process_pid2;
        //getUsedMem(detection_process_pid1);
        //getUsedMem(detection_process_pid2);
        bResult = true;
    }
    return bResult;
}

void ArithmeticQThread::killProcessandByQProcess()
{
    QString cmd;
    cmd = QString("taskkill /F /PID %1 /T").arg(detection_process_pid1);
    QProcess p1(0);
    p1.start(cmd);
    p1.waitForStarted();
    p1.waitForFinished();
#if 0
    cmd = QString("taskkill /F /PID %1 /T").arg(detection_process_pid2);
    QProcess p2(0);
    p2.start(cmd);
    p2.waitForStarted();
    p2.waitForFinished();
#endif
}

bool ArithmeticQThread::RestartAriExe()
{
    bool status =findProcessandByQProcess("Rockhook-rd.exe");
    //    if(status==true)
    {
        killProcessandByQProcess();
        WriteLog("kill Ari exe.");
        QThread::msleep(10);

        QProcess process;
        process.start("C:\\Window\\startgo.bat");
        process.waitForFinished();
        WriteLog("restart Ari exe.");
        QThread::msleep(10000);
    }

    return true;
}

int ArithmeticQThread::getCropLength(const std::vector<int>& lens, const cv::Rect & rect)
{
    int maxRange = std::max(rect.width, rect.height);
    size_t index = lens.size();
    for (auto i = 0; i < lens.size(); ++i)
    {
        if (lens[i] > maxRange/* + 40*/)
        {
            index = i;
            break;
        }
    }

    int len = 0;

    if (index == lens.size())
    {
        len = lens[lens.size()-1];// maxRange + 100;
    }
    else if(index == 0) //小于第一个范围，则取3倍长度
    {
        len = 3 * maxRange;
    }
    else
    {
        len = lens[index];
    }

    return len;
}

std::vector<cv::Rect> ArithmeticQThread::cutDefect(cv::Mat img, const std::vector<cv::Point>& ctr, int thresh, double rateThresh)
{
    std::vector<cv::Rect> rects;

    cv::Rect rect = cv::boundingRect(ctr);
    bool bVeryLong = false;//超长缺陷

    if (rect.width > thresh || rect.height > thresh)
    {
        double rate = (rect.width > rect.height) ? (double(rect.width) / rect.height) : (double(rect.height) / rect.width);
        if (rate > rateThresh)
        {
            bVeryLong = true;
        }
    }
    if (!bVeryLong)
    {
        cv::Rect crop;
        vector<int> lens{ /*50, 100, 200,*/ 350 };  //增加100

        //     int len = getCropLength(lens, rect);
             int len = 200;

        crop.width = len;
        crop.height = len;
        crop.x = rect.x - (len - rect.width) / 2;
        crop.y = rect.y - (len - rect.height) / 2;
        adjustRect(img, crop);
        rects.push_back(crop);
    }
    else
    {
//         rect.x -= 20;
//         rect.y -= 20;
//         rect.width += 40;
//         rect.height += 40;
//         adjustRect(img, rect);

        cv::Rect rect_/* = rect*/;
        bool bVertical = (rect.width < rect.height);//缺陷是否为纵向方向
        if (bVertical)
        {
            rect.x -= (thresh - rect.width) / 2;
            rect.width = thresh;

//            thresh *= 2;    //长条形减少分割数目
            int count = ceil(double(rect.height) / thresh);
            rect.y -= (count * thresh - rect.height)/2;

            rect_ = rect;
            //rect_.width = /*rect.width*/thresh;
            rect_.height = thresh;
            int start = rect.y;
//             if (start < 0)
//             {
//                 start = 0;
//             }
//             int count = ceil(double(rect.height + (start - rect.y)) / thresh);

            for (int i = 0; i < count; ++i)
            {
                rect_.y = start + i * thresh;

                adjustRect(img, rect_);
                rects.push_back(rect_);
            }
        }
        else
        {
            rect.y -= (thresh - rect.height) / 2;
            rect.height = thresh;

//            thresh *= 2;    //长条形减少分割数目
            int count = ceil(double(rect.width) / thresh);
            rect.x -= (count * thresh - rect.width) / 2;

            rect_ = rect;
            rect_.width = thresh;
            //rect_.height = thresh/*rect.height*/;
            int start = rect.x;// -20;
//             if (start < 0)
//             {
//                 start = 0;
//             }
//             int count = ceil(double(rect.width + (start - rect.x)) / thresh);
            for (int i = 0; i < count; ++i)
            {
                rect_.x = start + i * thresh;

                adjustRect(img, rect_);
                rects.push_back(rect_);
            }
        }
    }

    return rects;
}

void ArithmeticQThread::adjustRect(cv::Mat img, cv::Rect & rect)
{
    if (rect.x < 0)
    {
        rect.width = std::min(img.cols, rect.width);
        rect.x = 0;
    }
    if (rect.x + rect.width >= img.cols)
    {
        rect.x = std::max(0, img.cols - rect.width);
        rect.width = img.cols - rect.x - 1;
    }

    if (rect.y < 0)
    {
        rect.height = std::min(img.rows, rect.height);
        rect.y = 0;
    }
    if (rect.y + rect.height >= img.rows)
    {
        rect.y = std::max(0, img.rows - rect.height);
        rect.height = img.rows - rect.y - 1;
    }
}

void ArithmeticQThread::getAllFile()
{
    QtConcurrent::run([=]()
    {
        QDir dir(pathname);
        dir.setFilter(QDir::Files|QDir::Hidden|QDir::NoSymLinks);
        dir.setSorting(QDir::Time);

        QFileInfoList list = dir.entryInfoList(QDir::Filter::Files, QDir::SortFlag::Name);  //只列出文件（不包含文件夹）、这些文件按名称排序。
        for (int i = 0; i < list.size(); ++i)
        {
            QFileInfo fileInfo = list.at(i);
            string imgPath=fileInfo.absoluteFilePath().toStdString();
            qDebug()<<fileInfo.absoluteFilePath();
            Mat imgSrc = imread(imgPath,0); //结果
//           HouChuLiImgList.append(imgSrc);
            try
            {
                imgQueue->push(imgSrc);
            }
            catch (const std::exception&)
            {
                qDebug() << "Queue is full";
            }
            QThread::msleep(5);
        }
    });

//    QDir dir(pathname);
//    dir.setFilter(QDir::Files|QDir::Hidden|QDir::NoSymLinks);
//    dir.setSorting(QDir::Time);

//    QFileInfoList list = dir.entryInfoList();
//    for (int i = 0; i < list.size(); ++i)
//    {
//        QFileInfo fileInfo = list.at(i);
//        string imgPath=fileInfo.absoluteFilePath().toStdString();
//        qDebug()<<fileInfo.absoluteFilePath();
//        Mat imgSrc = imread(imgPath,0); //结果
////        HouChuLiImgList.append(imgSrc);
//        imgQueue->push(imgSrc);
////        QThread::msleep(15);
//    }
}
