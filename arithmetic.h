#ifndef ARITHMETIC_H
#define ARITHMETIC_H
#include <QObject>
#include <QTcpSocket>
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
#include <mutex>

using namespace cv;
using namespace std;

class Arithmetic:public QObject
{
    Q_OBJECT
public:
    Arithmetic();
    void Init(QString roodir);

    void newTcpConnection1();
    void InitPath();
    void SetClothRollNo(QString clothRollNo);
    void StopVisionInspection();

//    static void* ArithmeticThread(void* pUser);
    static DWORD WINAPI ArithmeticThread(LPVOID lpParam);

private:
    bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);
    bool moveFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);
    void clearFiles(QString folderFullPath);

    void WriteLog(QString message);

signals:
    void SendPlcCode(QString code);
    void UpdateImageShowListSignal(QString imagePath,QString camserialname,int x,int y,int num);
    void DataStorageSignal();

public slots:
    void ArithmeticProcessSlot();
    //图像算法
    void displayError1(QAbstractSocket::SocketError);
    void revData1();
    void detetionslot();

public:
    //图像算法
    QTcpSocket *tcpSocket1;
    //图像算法
    int ack;
    int flag_exprocess;//外部进程存在标志位
    double costtime;
    double costtime2;
    double costtime3;
    double costtime4;

    cv::String imagefolder;//一组图的检测路径waitdetection
    cv::String snapdir;
    cv::String m_root_dir; //存图路径
    cv::String bwsaveimagefolder;//检测结果缺陷白色二值化图存储路径
    cv::String roiNGimagefolder;//NG单独小缺陷扣图的存图路径roiNG
    cv::String originNGimagefolder;//NG原始图的存图路径originNG
    cv::String allOriginImageFolder;
    float defectarrayresult[3][100];//一组图的检测结果，当前一组图包含左右摄像头2张图.第一维度数组表示第几张，第二维度数组第一元素存储缺陷个数，第二第三等后续每个缺陷的坐标x数值和y数值

    int flag_alert_area;//因为缺陷面积过大的紧急停机标志位置位1
    int shreshold_alert_area;//对缺陷面积过大的阈值设置，面积大于等于400缺陷就是严重了，紧急停机，客户可以控件自行设置
    int flag_alert_quanlity;//因为缺陷数量过多的紧急停机标志位置位1
    int shreshold_alert_quanlity;//对缺陷个数过多的阈值设置，个数等于3个缺陷就是严重了，紧急停机，客户可以控件自行设置

    QString m_clothRollNo;

    int ngOnceAllNum;
    bool isViOver;
    mutex mtx;
};

#endif // ARITHMETIC_H
