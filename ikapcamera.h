#ifndef IKAPCAMERA_H
#define IKAPCAMERA_H
#pragma execution_character_set("utf-8")

#include <QObject>
#include <stdlib.h>
#include <malloc.h>
#include <Windows.h>
#include <stdio.h>
#include <vector>
#include "IKapC.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <QTime>
#include <QDebug>

#define									STREAM_BUFFER_COUNT			5
#define CHECK( errc ) if ( ITKSTATUS_OK != errc ) printErrorAndExit( errc )

using namespace cv;

class ikapCamera : public QObject
{
    Q_OBJECT
public: 
    static int CheckCameraNums();

    explicit ikapCamera(QObject *parent = nullptr);


    /* This function can be used to configure camera, form more examples please see IKapC usage. */
    void ConfigureCamera(int nCamIndex);
    bool InitDevices(int nIndex);

    /* This function can be used to create stream and add buffer. */
    bool CreateStreamAndBuffer();

    /* This function can be used to configure stream, form more examples please see IKapC usage. */
    bool ConfigureStream();
    bool OpenVideoStream();

    /* This function will be line trigger.*/
    bool SetLineTrigger();

    bool CloseCamera();
    /* This function will be unregister callback. */
    void UnRegisterCallback();

    /* This method demonstrates how to retrieve the IKapC error message
    for the last failed function call. */
    void printErrorAndExit(ITKSTATUS errc);

    void WriteLog(QString message);

    bool Convert2ToMat(int64_t w,int64_t h,int64_t format, unsigned char * pData);
    void RGB2BGR( unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight);

public:
    ITKDEVICE							g_hCamera = NULL;  // 相机设备句柄
    ITKSTREAM							g_hStream = NULL;//数据流句柄
    std::vector <ITKBUFFER>	g_vectorBuffer;//缓冲区句柄列表
    int											g_nCurFrameIndex = 0;
    unsigned char*						g_bufferData = NULL;

    int64_t		nWidth = 0;																	/* Image width.								*/
    int64_t		nHeight = 0;																	/* Image height.							*/
    uint32_t      nFormat = ITKBUFFER_VAL_FORMAT_MONO8;		/* Image format		图像格式		*/

    bool bIsSavePic = false;
    Mat srcImage;



signals:
    void SendCameraInfo(Mat);
};

#endif // IKAPCAMERA_H
