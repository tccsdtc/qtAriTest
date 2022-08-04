#include "arithmetic.h"

#include <QCoreApplication>
#include <QDir>
#include <QTime>

Arithmetic::Arithmetic()
{
}

void Arithmetic::Init(QString rootdir)
{
    ack=0;
    flag_exprocess=0;//外部进程存在标志位
    ngOnceAllNum = 0;
    isViOver = false;
    imagefolder = "C:\\waitdetection\\";//一组图的检测路径waitdetection
    snapdir = "C:\\snaptest\\";
    m_root_dir = rootdir.toStdString(); //存图路径
    bwsaveimagefolder = m_root_dir+"Image\\result\\";//检测结果缺陷白色二值化图存储路径
    roiNGimagefolder = m_root_dir+"Image\\roiNG\\";//NG单独小缺陷扣图的存图路径roiNG
    originNGimagefolder = m_root_dir+"Image\\originNG\\";//NG原始图的存图路径originNG
    allOriginImageFolder = m_root_dir + "\\Image\\allIamge\\";

    flag_alert_area=0;//因为缺陷面积过大的紧急停机标志位置位1
    shreshold_alert_area=150;//对缺陷面积过大的阈值设置，面积大于等于400缺陷就是严重了，紧急停机，客户可以控件自行设置
    flag_alert_quanlity=0;//因为缺陷数量过多的紧急停机标志位置位1
    shreshold_alert_quanlity=3;//对缺陷个数过多的阈值设置，个数等于3个缺陷就是严重了，紧急停机，客户可以控件自行设置

    bool result;
    QString path;
    QDir dir;

    //    算法自身调用
    path = QString::fromStdString(imagefolder);
    if (!dir.exists(path))
    {
        result = dir.mkpath(path);
    }
    //    算法自身调用
    path = QString::fromStdString(snapdir);
    if (!dir.exists(path))
    {
        result = dir.mkpath(path);
    }

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
        if (curProcessName == "hook-rd.exe")
        {
            internal_count =internal_count +1;
            //BOOL ret = true;
            if(internal_count==2)
            {
                flag_exprocess = 1;
                break;
            }
        }
        bResult = Process32Next(proHandle, &pInfo);
    }

    tcpSocket1=new QTcpSocket(this);
    connect(tcpSocket1,SIGNAL(readyRead()),this,SLOT(revData1()));
    connect(tcpSocket1,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError1(QAbstractSocket::SocketError)));

    if(flag_exprocess == 1)
        newTcpConnection1();

    flag_exprocess = 1; //主机没有部署检测进程的时候，赋值0，方便写业务代码调试
}

void Arithmetic::SetClothRollNo(QString clothRollNo)
{
    m_clothRollNo = clothRollNo;
}

void Arithmetic::InitPath()
{
    QString path;
    QDir dir;
    bool result;
    //新建布卷号目录
    originNGimagefolder += m_clothRollNo.toStdString() +"\\";
    path = QString::fromStdString(originNGimagefolder);
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
}

void Arithmetic::newTcpConnection1()
{
    if(tcpSocket1->state()!=QAbstractSocket::ConnectedState)
    {
        tcpSocket1->abort();
        tcpSocket1->connectToHost("127.0.0.1",7000);//local socket
    }
}

void Arithmetic::StopVisionInspection()
{
    tcpSocket1->abort();
}

DWORD WINAPI Arithmetic:: ArithmeticThread(LPVOID lpParam)
{
    Arithmetic* phWnd = (Arithmetic*)lpParam;
    phWnd->detetionslot();
    return 0;
}

void Arithmetic::ArithmeticProcessSlot()
{
    detetionslot();
    //    pthread_t ariId;
    //    pthread_create(&ariId,NULL,ArithmeticThread,this);
}


void Arithmetic::displayError1(QAbstractSocket::SocketError)
{
    tcpSocket1->close();
}

void Arithmetic::revData1()
{
    QString data=tcpSocket1->readAll();
    qDebug() << "data1:"<<data<<endl;
    ack = data.toInt();
    //ui->textEdit->setText(data);
}

void Arithmetic::detetionslot()
{
    try {
        WriteLog("arithmetic slot is start!");
        //    isOverIamgeAuto = false;
        //    qDebug("Receive Signal after Slot");
        cv::Mat channels[3];
        Mat Img0;
        Mat Img0_png;
        Mat Img;
        Mat Img_rgb;
        Mat Img_rgb_view;
        vector<Vec4i> hierarchy;
        vector<vector<Point>> allContours;
        vector<Vec4i> hierarchy_region;
        vector<vector<Point>> allContours_region;
        int fileindex = 0;
        int fileindex_region = 0;//局部缺陷抠图存储
        int flag_alert_area=0;//因为缺陷面积过大的紧急停机标志位置位，每一组图像检测前清0
        int flag_alert_quanlity=0;//因为缺陷数量过多的紧急停机标志位置位，，每一组图像检测前清0

        //    qDebug() << "get signal for detetionslot";
        //    qDebug() << "flag_exprocess:"<<flag_exprocess;
        //    qDebug() << "isover:"<<isover;

        //if ((flag_exprocess==1)&&(isover==true))//1表示外部检测进程存在,并且一组2张的图像成功存储在waitdetection文件中
        if (flag_exprocess==1)//1表示外部检测进程存在,并且一组2张的图像成功存储在waitdetection文件中
        {
            newTcpConnection1();//通过socket申请连接外部进程，如果已经连接就不用再次连接
            //cv::String imagefolder = "C:\\waitdetection\\";//一组图的检测路径waitdetection
            //cv::String bwsaveimagefolder = "D:\\result\\";//检测结果缺陷白色二值化图存储路径
            //cv::String roiNGimagefolder = "D:\\roiNG\\";//NG单独小缺陷扣图的存图路径roiNG
            //cv::String originNGimagefolder = "D:\\originNG\\";//NG原始图的存图路径originNG
            vector<cv::String> imagenames;
            cv::glob(imagefolder,imagenames,false);
            qDebug()<<"imagenames.size():"<<imagenames.size();

            for(int fileindex=0;fileindex<imagenames.size();fileindex++)//当前项目，waitdetection文件夹只会出现一组包含2张图
            {
                double start = static_cast<double>(GetTickCount());
                String str_file0 = imagenames[fileindex];
                Mat img = imread(str_file0);
                cv::split(img, channels);
                cv::Mat Img0_old = channels[1];

                //             Mat img = imread(str_file0);
                //             cv::Mat Img0_old= 0

                Mat Img_rgb = Mat::zeros(512, 512, CV_8UC1);//8位png
                cv::resize(Img0_old, Img_rgb, Img_rgb.size(), INTER_NEAREST);
                //cvtColor(Img_rgb, Img0_png, CV_BGR2GRAY);
                Img0_png = Img_rgb.clone();
                cv::imwrite("C:\\snaptest\\0.png", Img0_png);
                tcpSocket1->write("1");
                QTime t2;
                t2.start();
                while(t2.elapsed()<2500)
                {
                    QCoreApplication::processEvents();//释放调度给其他任务机会
                    if(ack==6)
                    {
                        qDebug() << "ack:"<<ack<<endl;
                        ack = 0;
                        break;
                    }
                    //                if(isOverAuto == true)
                    //                {
                    //                    break;
                    //                }
                }
                QString str_file1 = "C:\\snaptest\\0_predict.png";
                string string_file1 = str_file1.toStdString();
                Img = imread(string_file1,0);
                Img_rgb_view = Img_rgb.clone();
                for( int j = 0; j < Img.rows ; j++ )
                {
                    for( int i = 0; i < Img.cols; i++ )
                    {
                        uchar pixel = Img.at<uchar>(j, i);
                        if(pixel>2)//2
                            Img.at<uchar>(j, i) = 255;
                        else
                            Img.at<uchar>(j, i) = 0;
                    }
                }
                //            namedWindow("marked", WINDOW_NORMAL);
                //            imshow("marked", Img);
                //costtime = ((double)GetTickCount() - start) / getTickFrequency()/1000.0;
                //QString printcosttime;
                //printcosttime = printcosttime + "costtime(ms):"+QString("%1").arg(costtime);
                cout << "1:" << costtime<< "ms" << endl;
                findContours( Img, allContours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, Point(0, 0) );
                QString origintmpfromsourceDir = QString::fromStdString(imagenames[fileindex]);
                QString tmpfromsourceDir = QString::fromStdString(imagenames[fileindex]);
                //QString extractstr = tmpfromsourceDir.remove(0,29);
                //QString extractstr = tmpfromsourceDir.remove(0,16);
                QString extractstr = tmpfromsourceDir.remove(0,16);
                //QString tmptosourceDir = QString::fromStdString(saveimagefolder)+extractstr;
                QString bw_tmpfromsourceDir =extractstr;

                QString imagesave_str = QString::fromStdString(bwsaveimagefolder)+bw_tmpfromsourceDir;
                QString imageName = origintmpfromsourceDir.split("\\").at(2);
                //            qDebug()<<"imageName:"<<imageName;
                QString cam_serialname = imageName.split("_").at(0);//从图像文件名获取当前图像的相机编号（前11个字符），这样方便判断相机是左还是右
                //            qDebug()<<"cam_serialname"<<cam_serialname;
                //cvtColor(Img_rgb, Img_rgb, CV_BGR2RGB);
                Mat dstImage1 = Mat::zeros(3000, 8192, CV_8UC3);
                cv::resize(Img, dstImage1, dstImage1.size(), INTER_NEAREST);  //指定size的大小

                //            cv:string imagePath = allOriginImageFolder+;

                if(allContours.size()>=0)//不管正常图还是缺陷图，二值化检测结果图都保存到D:\\result文件夹中
                {
                    //qDebug()<<"tmpfromsourceDir:"<<tmpfromsourceDir;
                    //qDebug()<<"tmptosourceDir:"<<tmptosourceDir;
                    //qDebug()<<"imagesave_str:"<<imagesave_str;
                    //qDebug()<<"bw_tmpfromsourceDir:"<<bw_tmpfromsourceDir;
                    //QString tmptosourceDir = QString::fromStdString(saveimagefolder)+extractstr;//注意每个文件夹不同相应要改"G:\gc\collect_data\cam1_data\100.33_1469.jpg"
                    //copyFileToPath(origintmpfromsourceDir,tmptosourceDir, false);//复制原图

                    imwrite(imagesave_str.toLatin1().data(), dstImage1);//指定文件夹存储二值化结果图
                }

                if(allContours.size()>=1)//如果是缺陷图，当前原始图再保存到D:\\originNG文件夹中，为以后研究从减小数据筛选工作量做准备
                {

                    QString tmptosourceDir = QString::fromStdString(originNGimagefolder)+extractstr;//注意每个文件夹不同相应要改"G:\gc\collect_data\cam1_data\100.33_1469.jpg"
                    copyFileToPath(origintmpfromsourceDir,tmptosourceDir, false);//复制原图
                }

                //存储目录allOriginImageFolder  剪切复制
                moveFileToPath(origintmpfromsourceDir,QString::fromStdString(allOriginImageFolder)+imageName, false);//复制原图

                //            costtime2 = ((double)cvGetTickCount() - start) / cvGetTickFrequency()/1000.0;
                //            printcosttime = printcosttime + "costtime(ms):"+QString("%1").arg(costtime2);
                //            cout << "2:" << costtime2<< "ms" << endl;

                //抠图每个缺陷区域小图并按照序号进行存储，同时填充缺陷x，y图内的位置
                Mat dstImage1_bw;
                cv::threshold(dstImage1,dstImage1_bw,0,255,cv::THRESH_BINARY);
                findContours( dstImage1_bw, allContours_region, hierarchy_region, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, Point(0, 0) );
                qDebug()<<"allContours_region.size():"<<allContours_region.size();
                //对缺陷数量过多的阈值设置，个数等于3个缺陷就是严重了，紧急停机
                if(allContours_region.size()>=shreshold_alert_quanlity)
                {
                    flag_alert_quanlity=1;//因为缺陷数量过多的紧急停机标志位置位1
                    //                emit SendPlcCode("2"); //TSYL
                }
                //            qDebug()<<"allContours_region.size():"<<allContours_region.size();
                for(int i_region=0; i_region<allContours_region.size(); i_region++)
                {
                    //对缺陷面积过大的阈值设置，面积大于等于400缺陷就是严重了，紧急停机
                    if(contourArea(allContours_region[i_region])>=shreshold_alert_area)
                    {
                        flag_alert_area=1;//因为缺陷面积过大的紧急停机标志位置位1
                        //                    emit SendPlcCode("2"); //TSYL
                    }
                    //此处对缓存待检测文件夹c盘下的snaptest的一组图（左右相机2张图）进行扫描检测的缺陷位置和缺陷区域xy坐标数值
                    defectarrayresult[fileindex][0]=allContours_region.size();//先保存缺陷区域的个数

                    //cout<<allContours[i_Contours]<<endl;
                    defectarrayresult[fileindex][i_region+1]=allContours_region[i_region][0].x;
                    defectarrayresult[fileindex][i_region+2]=allContours_region[i_region][0].y;

                    //cout<<allContours[i]<<endl;
                    //                qDebug() << "i_region:"<<i_region<<endl;
                    if (contourArea(allContours_region[i_region])>10)//不要噪音
                    {
                        qDebug()<< "test";
                        cv::Rect r = cv::boundingRect(allContours_region.at(i_region));
                        Mat dst_rgb_Contour;
                        //qDebug() << "r.x:"<<r.x<<endl;
                        //qDebug() << "r.y:"<<r.y<<endl;
                        //qDebug() << "r.width:"<<r.width<<endl;
                        //qDebug() << "r.height:"<<r.height<<endl;
                        r.x = r.x-50;
                        r.y = r.y-50;
                        if((r.x)<=0)//溢出
                        {
                            r.x=0;
                            r.width = r.width+0+20;
                        }
                        else{
                            r.width = r.width+50+20;
                        }
                        if((r.y)<=0)//溢出
                        {
                            r.y=0;
                            r.height = r.height+0+40;
                        }
                        else{
                            r.height = r.height+50+40;
                        }

                        if((r.width+r.x)>=8192)//溢出
                        {
                            r.width=(r.width-(r.width+r.x-8192)-1);
                        }
                        if((r.height+r.y)>=3000)//溢出
                        {
                            r.height=(r.height-(r.height+r.y-3000)-1);
                        }
                        //qDebug() << "2r.x:"<<r.x<<endl;
                        //qDebug() << "2r.y:"<<r.y<<endl;
                        //qDebug() << "2r.width:"<<r.width<<endl;
                        //qDebug() << "2r.height:"<<r.height<<endl;
                        ngOnceAllNum++;
                        Img0_old(r).copyTo(dst_rgb_Contour);
                        //imshow("dst_rgb_Contour",dst_rgb_Contour);//
                        QString imagesave_str = "";
                        fileindex_region = fileindex_region+1;
                        //imagesave_str = "C:\\waitdetection\\"+QString::number(fileindex_region)+".bmp";
                        //缺陷截图roiNG存图文件名字规则:路径=相机编号+"_"+x+"_"+y+"_"+序号.bmp
                        imagesave_str =  QString::fromStdString(roiNGimagefolder) +cam_serialname+"_"+QString::number(allContours_region[i_region][0].x)+"_"+QString::number(allContours_region[i_region][0].y)+"_"+QString::number(fileindex_region)+".bmp";//"D:\\roiNG\\";//NG单独小缺陷扣图的存图路径roiNG
                        //imagesave_str =  QString::fromStdString(roiNGimagefolder) +cam_serialname+"_"+QString::number(ngOnceAllNum)+".bmp";
                        imwrite(imagesave_str.toLatin1().data(), dst_rgb_Contour);//存储成bmp格式图片

                        //UI界面展示
                        emit UpdateImageShowListSignal(imagesave_str,cam_serialname,allContours_region[i_region][0].x,allContours_region[i_region][0].y,ngOnceAllNum);
                    }
                }
                //            costtime3 = ((double)cvGetTickCount() - start) / cvGetTickFrequency()/1000.0;
                //            printcosttime = printcosttime + "costtime(ms):"+QString("%1").arg(costtime3);
                //            cout << "3:" << costtime3<< "ms" << endl;
                //数据库，绘图坐标API及时对defectarrayresult数组缺陷数据进行存储和坐标绘图显示

                //缓存图片检测处理完后，请删除左右2张图片文件，然后删除缺陷的截图文件
            }

            //        clearFiles(QString::fromStdString(imagefolder));
            if(isViOver)
            {
                emit DataStorageSignal();
            }

        }
        WriteLog("arithmetic slot is end!");

    } catch (Exception ex) {
        WriteLog(QString::fromStdString(ex.msg));
    }
}

bool Arithmetic::copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist)
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

bool Arithmetic::moveFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist)
{
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }

    //end if
    if(!QFile::rename(sourceDir, toDir))
    {
        return false;
    }
    return true;
}

void Arithmetic::clearFiles(QString folderFullPath)
{
    QDir dir(folderFullPath);
    dir.setFilter(QDir::Files);
    int fileCount = dir.count();
    for (int i = 0; i < fileCount; i++)
        dir.remove(dir[i]);
}

void Arithmetic::WriteLog(QString message)
{
    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("yyyy-MM-dd hh:mm:ss zzz");
    QString log ="["+timeStr+"]:"+message+"\n";

    QString logPath = QString::fromStdString(m_root_dir)+"log\\";
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
