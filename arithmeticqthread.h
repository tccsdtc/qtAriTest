#ifndef ARITHMETICQTHREAD_H
#define ARITHMETICQTHREAD_H
#pragma execution_character_set("utf-8")

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include "windows.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <sys/stat.h>
#include <iostream>
#include <vector>
#include <tlhelp32.h>
#include "datastorage.h"
#include "models.h"
#include "hconaction.h"
#include "circlequeue.h"//环形队列
#include <QtConcurrent>

using namespace cv;
using namespace std;

struct SizeSorter
{
    SizeSorter(double area_ = 0.0, int index_ = -1)
        :area(area_)
        , index(index_)
    {

    }
    SizeSorter(const SizeSorter& sorter)
        :area(sorter.area)
        , index(sorter.index){}
    double area;
    int index;
    static bool cmp(const SizeSorter& sorter1, const SizeSorter& sorter2)
    {
        return sorter1.area <= sorter2.area;
    }
};

class ArithmeticQThread : public QThread
{
    Q_OBJECT
public:
    explicit ArithmeticQThread(QObject *parent = nullptr);

    void Init();
    void LoadConifg();

    void newTcpConnection1();
    void InitPath();
    void SetRootPath(QString rootPath);
    void SetClothRollNo(QString clothRollNo);
    void SetImagePixel(int width,int height);
    void StopVisionInspection();
    void SetThreshold(QString cameraName,int thresholdPixel,int thresholdArea,int thresholdQuanlity,int thresholdRatio,int thresholdLenth,int thresholdWidth,
                      int thresholdAlertCujieshaMax,int thresholdAlertCujieshaMin,int stopCode,double restartCameraTime=180);
    void SetSocketParam(int socketType,cv::String serverPath);
    void StopMachine(int size,vector<Point> maxContour);
    void StopMachineTraverse(int size,vector<Point> maxContour);
    void StopMachineReflect(int size,vector<Point> maxContour);
    void KillAriExe();
    bool RestartAriExe();
    int CheckROIType(QString imgPath);

    //长条形分割
    int getCropLength(const std::vector<int>& lens, const cv::Rect & rect);
    std::vector<cv::Rect> cutDefect(cv::Mat img, const std::vector<cv::Point>& ctr, int thresh = 350, double rateThresh = 6.0);
    void adjustRect(cv::Mat img, cv::Rect & rect);

    //ariTest
    void getAllFile();


protected:
    void run() override;

private:
    bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);
    bool moveFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);
    void clearFiles(QString folderFullPath);

    //ariTest
    void extraRegioncropClassify(Mat img);

    void WriteLog(QString message);
    bool DosRemoveFile(QString path);
    bool DosMoveFile(QString src,QString dst);

    bool findProcessandByQProcess(const QString &strExe);
    void killProcessandByQProcess();

//    bool RestartAriExe();

signals:
    void SendPlcCode(QString code);
    void UpdateImageShowListSignal(QString imagePath,QString camserialname,int x,int y,int type,double yData,double size,int IQDtype);
    void DataStorageSignal();
    void RestartCameras();

public slots:
    void ReceiveCameraSlot(Mat imgSrc);     //接收图片槽函数
    void DetectionSlot(Mat imgSrc);         //处理图片函数
    //图像算法
    void displayError1(QAbstractSocket::SocketError);
    void revData1();
//    void detetionslot();
    void SetViFlag();
    void StartVi();
    void StopVi();

public:
    CircleQueue* imgQueue;

    bool isFirst;
    bool isOvertime;
    //图像算法
    QList<Mat> imgList;
    QTcpSocket *tcpSocket1;
    int socketTimeOutNum;
    //图像算法
    int ack;
    int flag_exprocess;//外部进程存在标志位
    double yData ;// 当前Y坐标
    double costtime;
    double costtime1;
    double costtime2;
    double costtime3;
    double costtime4;
    bool _isStop;
    bool _isVI;
    int pixelWidth;
    int pixelHeight;

    int m_stopCode;
    double yData_lastImg;   //上一次收到图像时的米数
    double pixelProportion;
    bool m_bSaveNgImg;
    bool m_bSaveOkImg;
    bool m_bRun;

    cv::String imagefolder;//一组图的检测路径waitdetection
    cv::String snapdir;
    cv::String m_root_dir; //存图路径
    cv::String bwsaveimagefolder;//检测结果缺陷白色二值化图存储路径
    cv::String roiNGimagefolder;//NG单独小缺陷扣图的存图路径roiNG
    cv::String guiroiNGimagefolder;//NG单独小缺陷扣图的存图路径guiroiNG，只是为了展示不得用于数据采集建库20210805am10
    cv::String originNGimagefolder;//NG原始图的存图路径originNG
    cv::String originOkimagefoler;
    cv::String allOriginImageFolder;

    float defectarrayresult[3][100];//一组图的检测结果，当前一组图包含左右摄像头2张图.第一维度数组表示第几张，第二维度数组第一元素存储缺陷个数，第二第三等后续每个缺陷的坐标x数值和y数值

    int threshold_pixel;
    int threshold_alert_area;//参数，对缺陷面积过大的阈值设置，面积大于等于400缺陷就是严重了，紧急停机，客户可以控件自行设置
    int threshold_alert_quanlity;//参数，对缺陷个数过多的阈值设置，个数等于3个缺陷就是严重了，紧急停机，客户可以控件自行设置
    int threshold_alert_ratio;//参数，缺陷畸形比的阈值设置，紧急停机，客户可以控件自行设置
    int threshold_alert_lenth;//参数，长度的阈值设置，紧急停机，客户可以控件自行设置
    int threshold_alert_width;//参数，宽度的阈值设置，紧急停机，客户可以控件自行设置
    int threshold_alert_cujiesha_max;//450;//参数，
    int threshold_alert_cujiesha_min;

    int object_type[100];//当前缺陷类型

    //停机规则集合列表，标志位显示
    int flag_alert_area=0;//因为缺陷面积过大的紧急停机标志位置位1
    int flag_alert_quanlity=0;//因为缺陷数量过多的紧急停机标志位置位1
    int flag_alert_ratio=0;//因为缺陷畸形比的紧急停机标志位置位1
    int flag_alert_lenth=0;//因为缺陷长度的紧急停机标志位置位1
    int flag_alert_width=0;//因为缺陷宽度的紧急停机标志位置位1


    int maxIndex = -1;
    int minIndex = -1;
    bool isMaxProcess = false;
    bool isMinProcess = false;

    int flawType = 0;
    int IQDflawType = 0;
    double FlawSize;

    int offset_x_leftedge_yanbu;

    QString m_clothRollNo;
    QString origintmpfromsourceDir;
    QString imageName;
    QString timeStr;
    QString imagesave_str;
    double m_yData;

    //double m_yData;
//    int ngOnceAllNum;
    bool isViOver;
    QMutex mtx;
    Mat Img0_old;
    Mat Img_rgb;// = Mat::zeros(512, 512, CV_8UC1);//8位png
    Mat dstImage1;// = Mat::zeros(pixelHeight, pixelWidth, CV_8UC1);//CV_8UC3
    //socket
    int m_socketType;
    cv::String m_socketServerPath;

    //carema
    QString m_cameraName;
    double m_dRestartCameraTime;

    int detection_process_pid1;//检测进程第一个pid获取值

    // 多选缺陷
    vector<SizeSorter> vecSizes;
    vector<int> vecUndetectedIndex;

    //选择的直径
    int m_nDiameter;   //缺胶点
    int m_nDiameter_BlackDot;   //黑点

    //config设定的灰度阈值
    int m_nThreshold;

    //报表统计
    int m_nCheckOutTotal;    //检出总数
    int m_nNum1, m_nNum2, m_nNum3;    //各类型数量
    cv::String reportFolder;  //报表生成地址

    //ariTest
    QString pathname;
    QList<Mat> HouChuLiImgList;
    int TestType=2;//1:只分类，2:只后处理   3:分类+后处理
    int TestNum_0=0;
    int TestNum_1=0;

private:
    hconAction* hAction;

};

#endif // ARITHMETICQTHREAD_H
