#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QMainWindow>
#include "windows.h"
#include "newclothrolldialog.h"
#include <QList>
#include <QLabel>
#include <QGridLayout>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QTimer>
#include "arithmetic.h"
#include "datastorage.h"
#include "plc.h"
#include "showimagelabel.h"
#include <sys/stat.h>
#include <iostream>
#include <vector>
#include <tlhelp32.h>
#include "arithmeticqthread.h"
//#include "dalsa_camera.h"
#include "hisclothdialog.h"
#include "models.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "socketqthread.h"
#include "socketclient.h"
#include "socketserver.h"
#include "hintdialog.h"
#include "ikapcamera.h"
#include <QtCharts>
#include "ChartView.h"
#include "callout.h"


using namespace QtCharts;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *watched, QEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    void LoadConifg();//读取配置文件
    void Init(); //初始化
    bool InitPlc();//初始化PLc
    bool InitCamera();//初始化相机
    bool InitSocket();//初始化socket
    void InitChart();//初始化chart

    /*************************************************
    Function:       openCameraByIndex(int serveID,int cameraID,int configID)
    @brief     根据索引打开相机
    @param serveID： 服务器ID
    @param cameraID：  相机ID
    @param configID：  配置文件ID
    *************************************************/
    //void openCameraByIndex(DalsaCamera *cam,string serveID);
    bool CloseCamera();//释放相机
    bool InitVison(); //初始化图像算法
    void AddDetectImageShow(QString imagePath,QString camserialname,int x,int y,int type,double yData,double size,int IQDtype);
    void AddDetectImageShowPaging(QString imagePath,QString camserialname,int x,int y,int type,double yData);
    void DisplayImage();
    void ReShowDetectImages();
    void DrawClothRoll();
    void DrawClothRoll_Y4000();
    void ShowAllImage();
    void ClearAllView();
    bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);
    bool FilterSubControlInfo(int x,int y,double yData);
    void clearFiles(QString folderFullPath);
    void WriteLog(QString log);
    void LoadHistoryClothRoll();

    //线程
    static void* LoadHistoryClothRollThread(void* pUser);

signals:
    void SendPlcCode(QString code);
    void StartVisualInspectionSignal();
    void UpdateImageShowListSignal(QString imageName,QString camserialname,int x,int y,int type,double yData,int IQDtype);
    void AutoClearViewSignal();

public slots:
    void GetClothRollInfo(QString clothRollNo,QString robotNo,QString oprName);
    void ReceivePhotographCode(int yData);
    void AddDetectImageShowSlot(QString imagePath,QString camserialname,int x,int y,int type,double yData,double size,int IQDtype);
    void RecieveSelectHClothSlot(QString clothNo);
    void HeartBeatSlot();
    void UpdateSocketInfoSlot(int code,double meterLength,QString clothRollNo,int workStatus);
    void UpdatePlcInfoSlot(double meterLength);
    void UpdateSocketImageSlot(QString imageName,QString camserialname,int x,int y,int type,double yData,double size);
    void SendPlcCodeSlot(QString plcCode);
    QList<FlawRecord> SortFlawRecord(QList<FlawRecord> list);
    void SocketCloseVisionSlot();
    void SendMeterInfoSlot();
    void AutoReportSlot();
    //超过N张后，清除视图，再显示最后100张
    void AutoClearViewSlot();

private slots:
    void on_NewClothRollPB_clicked(bool checked);

    void on_pushButton_clicked(bool checked);

    void on_startVisionPB_clicked(bool checked);

    void on_closeVisionPB_clicked(bool checked);

    void on_pushButton_3_clicked(bool checked);

    void on_pushButton_4_clicked(bool checked);

    void UpdateOnceStatistics();

    void on_exportPB_clicked(bool checked);

    void on_exitPB_clicked(bool checked);

    void DataStorageAutoSave();

    void CheckCameraInfo();

    void RestartCameras();

    //历史布卷
    void on_clothHistoryPB_clicked();

    void on_NewClothRollPB_clicked();

    void on_pushButton_clicked();

    void on_headPagePB_clicked();

    void on_prePagePB_clicked();

    void on_nextPagePB_clicked();

    void on_endPagePB_clicked();

    //chart
    void tooltip(QPointF point,bool b);

    void on_btn_Unlock_clicked();

    void on_btn_Lock_clicked();

    void on_comboBox_1_currentIndexChanged(int index);

    void on_comboBox_4_currentIndexChanged(int index);

    void on_clothHistoryPB_clicked(bool checked);

    void on_startVisionPB_clicked();

private:
    Ui::MainWindow *ui;
    /*
     * 图片展示
    */
    QString softwareName;
    QList<QLabel*> imageList; //展示图片列表
    QList<ShowImageLabel*> imageList_backup; //展示图片列表,备份最后N张用

    QList<FlawPoint> pointList; //缺陷点列表
    int lineImageNum; //一行展示图片数量
    int imageSpaceInterval; //行间距
    QString m_GcAddress;
    int m_nBackupNum=10;    //保留图片张数

    //chart
    QChart* m_chart;
    QScatterSeries* m_series;
    QScatterSeries* m_seriesFlaw;
    QLineSeries* m_seriesRemind;//  提醒用的当前位置线
    QValueAxis* m_axisX;
    QValueAxis* m_axisY;
    ChartView* m_chartView;
    Callout *m_tooltip;

    int pageImageNum; //一页展示图片数量
    QGridLayout *pLayout;
    QSize imageSize; //展示图片大小
    int pageImageMax; //图片一页最大张数
    int showWidgetWidth;
    int showHeightWidth;
    NewClothRollDialog newClothRollDialog;
    HisClothDialog hisClothDialog; //历史布卷对话框
    HIntDialog hintDialog; //历史布卷进度提示
    bool isHisCloth;

    int m_imageIndex; //图片序号
    int m_historyTotalNum;
    int pageImageIndex; //当前页图片序号
    int pageIndex;//当前页
    int pageTotal;//页总数

    QString m_clothRollNo;
    QString m_robotNo;
    QString m_oprName;
    DataStorage dataStorage;
    QString m_recordPath;

    bool isViRun;

    QList<FlawRecord> flawRecordList;

    bool isStartStatus;
    QString m_selectClothRollNo;

    /*
    ******PLC
    */
    //Plc plc;
    SocketClient plc;
    QString plcIp;
    int plcPort;
    double m_yData; //拍照时y轴坐标
    QString comName;
    int stopCode;
    int resetCode;
    QTimer *plcTimer;

    /*
    ******相机
    */
    list<string> serverNameList;
  //  QList<DalsaCamera*> cameraList;

    QList<ikapCamera*> IKapCameraList;
    bool isover;

    QString root_dir; //存图路径

    QTimer *showTimer;
    QTimer *photoTimer;
    QString CameraOneSeriname;
    QString CameraTowSeriname;

    QString m_cameraName;
    QString m_cameraName2;
    int pixelWidth;
    int pixelHeight;
    double pixelProportion;

    /*
    *  算法变量
    */
    ArithmeticQThread* ariThread;
    QDateTime m_startTime;
    int ariOnoff;
    int threshold_pixel;
    int threshold_alert_area;//参数，对缺陷面积过大的阈值设置，面积大于等于400缺陷就是严重了，紧急停机，客户可以控件自行设置
    int threshold_alert_quanlity;//参数，对缺陷个数过多的阈值设置，个数等于3个缺陷就是严重了，紧急停机，客户可以控件自行设置
    int threshold_alert_ratio;//参数，缺陷畸形比的阈值设置，紧急停机，客户可以控件自行设置
    int threshold_alert_lenth;//参数，长度的阈值设置，紧急停机，客户可以控件自行设置
    int threshold_alert_width;//参数，宽度的阈值设置，紧急停机，客户可以控件自行设置
    int threshold_alert_cujiesha_max;//450;//参数
    int threshold_alert_cujiesha_min;
    int threshold_length_filter;

     /*
     *  socket通信
     */
    SocketServer* socketServer; //socket服务端
    SocketClient* socketClient; //socket客户端

    int socketType;      //socket类型
    QString socketIp;  //socketIP地址
    int socketPort;  //socketPort端口
    double socketyDataEx; //y轴间距
    double handyDataEx; //人工补偿y轴间距
    QString socketServerPath; //socketServer路径

    QTimer* heartBeat; //心跳定时器

    //重启相机的参数
    double m_dRestartCameraTime;    //重启相机的时间,单位:分(超过这个时间没有触发拍照信号，就重启)
    int m_RestartCameraCount;   //重启相机的数量
    QString m_RestartCameraNetcardPrefix;   //重启相机的网卡前缀。注意:网卡名必须是从前缀+1开始,比如 camera1,camera2
    bool m_bRestartNetcard;

    //选择的直径
    int m_nDiameter;   //缺胶
    int m_nDiameter_BlackDot;   //黑点

    //报警延迟距离(m)
    double AlarmDelay=3.4;

    //报警列表
    QList<double> AlarmList;

    //报表功能
    QTimer *ReportTimer;
    QString LastReportTime;
    int LastIndex;



    //ariTest
    QString pathname;



};
#endif // MAINWINDOW_H
