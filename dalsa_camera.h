//#pragma execution_character_set("utf-8")
#ifndef DALSA_CAMERA
#define DALSA_CAMERA

#pragma once
#include <QObject>
#include "stdio.h"
#include "conio.h"
#include "math.h"
#include "SapClassBasic.h"
//#include "ExampleUtils.h"
#include<iostream>
#include <string>
#include<vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include <time.h>
#include <thread>
#include <windows.h>    
using namespace std;
using namespace cv;

#define CAMERA_LINK_SERVER_NAME_PREFIX "CameraLink_"
#define MAX_CONFIG_FILES   36       // 10 numbers + 26 lowercase letters
static char configFileNames[MAX_CONFIG_FILES][MAX_PATH] = { 0 };

class DalsaCamera : public QObject
{
    Q_OBJECT

public:
	DalsaCamera();
	~DalsaCamera();

private:
	//UINT32   acqDeviceNumber = 0;//设备索引,默认为0
	//char* acqServerName = new char[CORSERVER_MAX_STRLEN]; //服务器名称
	//char* configFilename = new char[MAX_PATH];  //配置文件
	//vector<char*>vec_acqServerName;
	//vector<char*>vec_configFilename;
	char serverName[CORSERVER_MAX_STRLEN];
	//char* configFilename = new char[MAX_PATH];  //配置文件
	/// /////////////////////////////////////////////Sap类
	int								cameraCount = 0;			//相机数量
	int								deviceCount;				//设备数量
	SapAcquisition					Acq;
	SapAcqDevice					AcqDevice;
	//SapBufferWithTrash Buffers;	
	SapBuffer						* m_pBuffers;// Buffers;
	SapTransfer						AcqToBuf;
	SapTransfer						AcqDeviceToBuf;		
	SapTransfer						* Xfer;// = NULL;
	//SapView							View;
	//SapAcqDevice					* m_pCamera;
	SapFeature						* m_pFeature;

public:
	/////////////////////////////////////////////////图像参数变量
	cv::Mat			 captureImg;
	int			     m_iImageWidth;
	int              m_iImageHeight;			//图像宽高
	double		     m_linerate;				//线频率
	BYTE			 * TMPBUF;
	UINT32			 m_triggermode;				//触发模式


	bool			bIsInitDevices;  			//相机开启标志位
	bool			bIsOpenVideo = false;		//视频流标志位
	
	bool			bIsSavePic =false;
	int				picID=0;					//相机序号
	int				test = 0;
	//cv::Mat			testMat;
	bool			isFinishSavePic;
    std::string     waitdetectionPath;
    std::string     m_yData;
    std::string     m_serverName;

signals:
    void SendCameraInfo(Mat imgSrc);


	/***************************封类:: 成员函数*************************************/
public:
    //检测相机数量
    static bool CheckCameraNums(list<string>& serverNameList);
    /**********************************
	Function:  searchServer(vector<char*>vec_acqServerName)
	@brief:    搜索服务器名称
	***********************************/
    bool searchServer(std::vector<std::string>& vec_acqServerName);

	/**********************************
	Function:  bool searchCamera(char* vec_acqServerName, UINT32  acqDeviceNumber);
	@brief:    搜索相机序号和名称
	***********************************/
	bool searchCamera(std::string vec_acqServerName, std::vector<UINT32>& vec_acqDeviceNumber, std::vector<std::string>& vec_cameraName);

	/**********************************
	Function:  bool searchConfig(char* vec_acqServerName, UINT32  acqDeviceNumber);
	@brief:    搜索配置文件
	***********************************/
	bool searchConfig(std::vector<std::string>& str_config);

	/**********************************
	Function:  InitDevices()
	@brief:    初始化相机
	***********************************/
    bool InitDevices(string acqServerName, UINT32 acqDeviceNumber, std::string configFilename);

	/*************************************************
	Function:       freeHandles
	@brief        释放资源
	*************************************************/
	bool freeHandles();

    /*************************************************
	Function:       XferCallback
	@brief        相机回调函数，显示和计算帧率
	@param pInfo：回调信息
	*************************************************/
	static void XferCallback(SapXferCallbackInfo* pInfo);


	/**********************************
	Function:  OpenVideoStream()
	@brief:    打开视频流
	***********************************/
	bool OpenVideoStream();

	/**********************************
	Function:  CloseVideoStream()
	@brief:    关闭视频流
	***********************************/
	bool CloseVideoStream();


	/**********************************
	Function:  closeCamera()
	@brief:    关闭相机
	***********************************/
    bool CloseCamera();

	/**********************************
	Function:  SaveImage()
	@brief:    保存图片
	***********************************/
    void SaveImage(cv::Mat &pic, std::string &path);

    void getPath(cv::String waitPath);

	/**********************************
	Function:  SetDeviceLineFrequency(int LineRate_str);
	@brief:    设置线频率
	***********************************/
	void			SetDeviceLineFrequency(double LineRate_str);

	/**********************************
	Function:  SetDeviceTriggerMode();
	@brief:    设置触发模式
	***********************************/
	void			SetDeviceTriggerMode(int mode);

};




#endif
