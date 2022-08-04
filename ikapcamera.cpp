#include "ikapcamera.h"

/* This function can be used to register the stream callback function. */
void IKAPC_CC cbStartOfStream(uint32_t eventType, void* pContext);
void IKAPC_CC cbEndOfStream(uint32_t eventType, void* pContext);
void IKAPC_CC cbOnEndOfFrame(uint32_t eventType, void* pContext);
void IKAPC_CC cbOnTimeOut(uint32_t eventType, void* pContext);
void IKAPC_CC cbOnFrameLost(uint32_t eventType, void* pContext);

int ikapCamera::CheckCameraNums()
{
    ITKSTATUS res = ITKSTATUS_OK;
   uint32_t numCameras = 0;

   /* Before using any IKapC methods, the IKapC runtime must be initialized.
   在使用任何 IKapC 方法之前，必须初始化 IKapC 运行时*/
   res = ItkManInitialize();
   if(res!=ITKSTATUS_OK)
   {
       return 0;
   }

   /* Enumerate all camera devices. You must call
   ItkManGetDeviceCount() before creating a device.
   枚举所有相机设备。   你必须在创建设备之前调用ItkManGetDeviceCount()*/
   res = ItkManGetDeviceCount(&numCameras);//res为是否成功，numCameras为设备数量
   if(res!=ITKSTATUS_OK)
   {
       ItkManTerminate();//释放 IKapC 初始化过程中申请的资源
       return 0;
   }

   return numCameras;
}


ikapCamera::ikapCamera(QObject *parent) : QObject(parent)
{

}


/* This function can be used to configure camera, form more examples please see IKapC usage.
该功能可用于配置摄像头，更多示例请参见IKapC用法*/
//打开相机
void ikapCamera::ConfigureCamera(int nCamIndex)
{
     ITKSTATUS res = ITKSTATUS_OK;

     /* Open first CameraLink Camera. */
     ITKDEV_INFO di;//相机信息

     /* Get device info. */
     res = ItkManGetDeviceInfo(nCamIndex, &di);//获取得到的相机信息
     //CHECK(res);

    // printf("Using camera: serial: %s, name: %s, interface: %s.\n", di.SerialNumber, di.FullName, di.DeviceClass);//打印得到的信息

     // Only use CameraLink camera with proper serial number
     if (strcmp(di.DeviceClass, "GigEVision") == 0 && strcmp(di.SerialNumber, "") != 0)//字符串比较
     {
         ITKGIGEDEV_INFO gvInfo;

         /* Open camera. */
         res = ItkDevOpen(nCamIndex, ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE, &g_hCamera);//打开相机
         CHECK(res);
     //    res = ItkManGetGigEDeviceInfo(nCamIndex, &gvInfo);//获取千兆网相机专有设备信息
     }
}

//初始化相机
bool ikapCamera::InitDevices(int nCamIndex)
{
    ITKSTATUS res = ITKSTATUS_OK;
    ITKDEV_INFO di;//相机信息

    res = ItkManGetDeviceInfo(nCamIndex, &di);//获取得到的相机信息
    //CHECK(res);
   // printf("Using camera: serial: %s, name: %s, interface: %s.\n", di.SerialNumber, di.FullName, di.DeviceClass);//打印得到的信息

    // Only use CameraLink camera with proper serial number
    if (strcmp(di.DeviceClass, "GigEVision") == 0 && strcmp(di.SerialNumber, "") != 0)//字符串比较
    {
        ITKGIGEDEV_INFO gvInfo;

        /* Open camera. */
        res = ItkDevOpen(nCamIndex, ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE, &g_hCamera);//打开相机，ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE:使用独占方式
        if(res!=ITKSTATUS_OK)
        {
            return false;
        }
    //    res = ItkManGetGigEDeviceInfo(nCamIndex, &gvInfo);//获取千兆网相机专有设备信息
    }

    return CreateStreamAndBuffer();

}

/* This function can be used to create stream and add buffer.
该函数可用于创建流和添加缓冲区*/
bool ikapCamera::CreateStreamAndBuffer()
{
    uint32_t		streamCount = 0;															/*Stream count.							*/
    int64_t		nImageSize = 0;															     /* Image size		数据缓冲区的大小							*/
    char				pixelFormat[16];															/* Image pixel format.					*/
    uint32_t		pixelFormatLen = 16;													/* Image pixel format length.	*/

    // Get stream count  获取流数
    /*假设用户通过 ItkDevGetStreamCount 获得当前可以采集的图像数据流个数为 N(N>0)，则
用 户 可 以 选 择 其 中 一 路 或 者 多 路 完 成 图 像 采 集 ， 在 采 集 图 像 前 必 须 调 用 函 数
ItkDevAllocStream 函数完成数据流的初始化操作。*/
    ITKSTATUS res = ITKSTATUS_OK;
    res = ItkDevGetStreamCount(g_hCamera, &streamCount);   //获取相机数据流的数量
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }

    if (streamCount == 0)//CameraLink 接口相机支持的图像数据流个数可能为 0
    {
        ItkManTerminate(); //释放 IKapC 初始化过程中申请的资源
        return false;
    }

    res = ItkDevGetInt64(g_hCamera,"Width",&nWidth);//得的设备的整形数据
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }

    res = ItkDevGetInt64(g_hCamera,"Height",&nHeight);
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }

    res = ItkDevToString(g_hCamera,"PixelFormat",pixelFormat, &pixelFormatLen);
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }

    if (strcmp(pixelFormat, "Mono8") == 0)  //字符串比较
        nFormat = ITKBUFFER_VAL_FORMAT_MONO8;
    else if (strcmp(pixelFormat, "Mono10") == 0)
        nFormat = ITKBUFFER_VAL_FORMAT_MONO10;
    else if (strcmp(pixelFormat, "Mono10Packed") == 0)
        nFormat = ITKBUFFER_VAL_FORMAT_MONO10PACKED;
    else if (strcmp(pixelFormat, "BayerGR8") == 0)
        nFormat = ITKBUFFER_VAL_FORMAT_BAYER_GR8;
    else if (strcmp(pixelFormat, "BayerRG8") == 0)
        nFormat = ITKBUFFER_VAL_FORMAT_BAYER_RG8;
    else if (strcmp(pixelFormat, "BayerGB8") == 0)
        nFormat = ITKBUFFER_VAL_FORMAT_BAYER_GB8;
    else if (strcmp(pixelFormat, "BayerBG8") == 0)
        nFormat = ITKBUFFER_VAL_FORMAT_BAYER_BG8;
    else
    {
        //相机不支持的像素格式
        ItkManTerminate();
        return false;
    }


    ITKBUFFER hBuffer;//缓冲区句柄
    res = ItkBufferNew(nWidth, nHeight, nFormat, &hBuffer);//建立缓冲区
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }
    g_vectorBuffer.push_back(hBuffer);//放入缓冲句柄列表

    // Get buffer size
    res = ItkBufferGetPrm(hBuffer,ITKBUFFER_PRM_SIZE, &nImageSize);//获取缓冲区的参数
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }
    // apply buffer data
    g_bufferData = (unsigned char*)malloc(nImageSize);  //为缓冲区分配内存
    if (g_bufferData == NULL)
    {
        exit(EXIT_FAILURE);
    }

    /* Allocate stream handle for image grab.
    为图像抓取分配流句柄*/
    res = ItkDevAllocStream(g_hCamera, 0, hBuffer, &g_hStream);  //打开数据流通道
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }

    /*当连续采集或者采集图像序列时，可能会由于相机采集帧率过快导致新采集得到图像数据丢
失或者覆盖原有图像数据。通过增加缓冲区链表中缓冲区的个数可以避免这种情况的发生。*/
    for (int i =1 ;i < STREAM_BUFFER_COUNT;i++)
    {
        res = ItkBufferNew(nWidth, nHeight, nFormat, &hBuffer);
        if(res!=ITKSTATUS_OK)
        {
            return false;
        };
        res = ItkStreamAddBuffer(g_hStream,hBuffer);
        if(res!=ITKSTATUS_OK)
        {
            return false;
        }
        g_vectorBuffer.push_back(hBuffer);
    }

    return true;
}

/* This function can be used to configure stream, form more examples please see IKapC usage.
该函数可用于配置流，更多示例请参见 IKapC 用法。*/
bool  ikapCamera::ConfigureStream()
{
    ITKSTATUS	res = ITKSTATUS_OK;
    uint32_t			xferMode = ITKSTREAM_VAL_TRANSFER_MODE_SYNCHRONOUS_WITH_PROTECT;				/* Transfer image in asynchronous mode.以异步模式传输图像 */
    uint32_t         startMode = ITKSTREAM_VAL_START_MODE_NON_BLOCK;
    uint32_t			timeOut = 2000;		/* Image transfer timeout.					*/

    /* Set block mode which means the grab will not be stopped before an entire image
    come into buffer.
    设置块模式，这意味着抓取不会在整个图像之前停止
    进入缓冲区。*/
    res = ItkStreamSetPrm(g_hStream, ITKSTREAM_PRM_START_MODE, &startMode);//设置数据流句柄参数
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }

    res = ItkStreamSetPrm(g_hStream, ITKSTREAM_PRM_TRANSFER_MODE, &xferMode);
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }

    res = ItkStreamSetPrm(g_hStream, ITKSTREAM_PRM_TIME_OUT, &timeOut);
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }

    /* Register callback which will be called at the begin of stream.注册将在流开始时调用的回调。 */
    res = ItkStreamRegisterCallback(g_hStream, ITKSTREAM_VAL_EVENT_TYPE_START_OF_STREAM, cbStartOfStream, g_hStream);
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }
    /* Register callback which will be called at the end of stream. 注册将在流结束时调用的回调*/
    res = ItkStreamRegisterCallback(g_hStream, ITKSTREAM_VAL_EVENT_TYPE_END_OF_STREAM, cbEndOfStream, g_hStream);
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }

    /* Register callback which will be called at the end of one image completely. 注册回调，将在一张图像的末尾完全调用*/
    res = ItkStreamRegisterCallback(g_hStream, ITKSTREAM_VAL_EVENT_TYPE_END_OF_FRAME, cbOnEndOfFrame, this);
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }

    //超时回调
    res  = ItkStreamRegisterCallback(g_hStream, ITKSTREAM_VAL_EVENT_TYPE_TIME_OUT, cbOnTimeOut, g_hStream);
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }

    //丢帧回调
    res  = ItkStreamRegisterCallback(g_hStream, ITKSTREAM_VAL_EVENT_TYPE_FRAME_LOST, cbOnFrameLost, g_hStream);
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }
     return true;
}

bool ikapCamera::OpenVideoStream()
{
    ITKSTATUS	res = ITKSTATUS_OK;
//    if(!SetLineTrigger())//设置相机模式
//    {
//        return false;
//    }

    if(!ConfigureStream())//注册回调函数
    {
        return false;
    }

    res = ItkStreamStart(g_hStream, ITKSTREAM_CONTINUOUS);//开始取流
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }
    return true;
}

/* This function will be line trigger.*/
bool ikapCamera::SetLineTrigger()
{
    ITKSTATUS res = ITKSTATUS_OK;
    // Turn off frame trigger
    res = ItkDevFromString(g_hCamera,"TriggerSelector","FrameStart");
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }
    res = ItkDevFromString(g_hCamera,"TriggerMode","Off");
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }

    // Trigger selector
    res = ItkDevFromString(g_hCamera,"TriggerSelector","LineStart");
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }

    // Set trigger mode
    res = ItkDevFromString(g_hCamera,"TriggerMode","On");
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }

    // Select trigger source(Line1、Line2、RotaryEncoder1)
    res = ItkDevFromString(g_hCamera,"TriggerSource","Line1");
    if(res!=ITKSTATUS_OK)
    {
        return false;
    }

    return true;
}

bool ikapCamera::CloseCamera()
{
    ITKSTATUS	res = ITKSTATUS_OK;


    /* Stop grab image.停止抓取图像 */
    ItkStreamStop(g_hStream);

    /* UnRegister callback.取消注册回调 */
    UnRegisterCallback();

    /* Free stream and buffer.释放流和缓冲区 */
    for (auto it = g_vectorBuffer.begin(); it != g_vectorBuffer.end(); it++)
    {
        ItkStreamRemoveBuffer(g_hStream, *it);
        ItkBufferFree(*it);
    }
    std::vector<ITKBUFFER>().swap(g_vectorBuffer);//这样做可以释放g_vectorBuffer的内存，详细原理百度
    ItkDevFreeStream(g_hStream);
    res = ItkDevClose(g_hCamera);
    if(res!=ITKSTATUS_OK)//设置相机模式
    {
        return false;
    }

    if (g_bufferData != NULL)
        free(g_bufferData);

    bIsSavePic=false;
    /* Shut down the pylon runtime system. Don't call any IKapC method after
    calling ItkManTerminate().
    关闭 pylon 运行时系统，不要在调用ItkManTerminate()后调用任何IKapC 方法*/
    ItkManTerminate();

    return true;
}


/* This function will be unregister callback.
此函数将取消注册回调*/
void ikapCamera::UnRegisterCallback()
{
    ITKSTATUS	res = ITKSTATUS_OK;
    res = ItkStreamUnregisterCallback(g_hStream, ITKSTREAM_VAL_EVENT_TYPE_START_OF_STREAM);
    res = ItkStreamUnregisterCallback(g_hStream, ITKSTREAM_VAL_EVENT_TYPE_END_OF_STREAM);
    res = ItkStreamUnregisterCallback(g_hStream, ITKSTREAM_VAL_EVENT_TYPE_END_OF_FRAME);
    res = ItkStreamUnregisterCallback(g_hStream, ITKSTREAM_VAL_EVENT_TYPE_TIME_OUT);
    res = ItkStreamUnregisterCallback(g_hStream, ITKSTREAM_VAL_EVENT_TYPE_FRAME_LOST);
}

/* This function can be used to register the stream callback function. */
void IKAPC_CC cbStartOfStream(uint32_t eventType, void* pContext)
{
    printf("On start of stream. \n");
}

void IKAPC_CC cbEndOfStream(uint32_t eventType, void* pContext)
{
     printf("On end of stream. \n");
}

//
void IKAPC_CC cbOnEndOfFrame(uint32_t eventType, void* pContext)
{
    ikapCamera* iKapCam=(ikapCamera*)pContext;
    printf("On end of frame. \n");
     ITKSTATUS res = ITKSTATUS_OK;
     unsigned bufferStatus = 0;
     int64_t     nImageSize = 0;
     ITKBUFFER  hBuffer = iKapCam->g_vectorBuffer[iKapCam->g_nCurFrameIndex];
 //     ITKBUFFER  hBuffer = iKapCam->g_vectorBuffer[0];

     if(iKapCam->bIsSavePic)
     {
         res = ItkBufferGetPrm(hBuffer,ITKBUFFER_PRM_STATE,&bufferStatus);//获取缓冲区的参数（缓冲区状态）
         if(res!=ITKSTATUS_OK)
         {
             return;
         }

         //ITKBUFFER_VAL_STATE_FULL: 满状态，意味着当前缓冲区已经被相机采集到的图像数据填满
         //缓冲区非满，在采集千兆网相机时有可能因为网络丢包导致无法采集完整的一帧图像
         if (bufferStatus ==  ITKBUFFER_VAL_STATE_FULL ||  bufferStatus ==ITKBUFFER_VAL_STATE_UNCOMPLETED )
         {
             // Save buffer
             /*
             res = ItkBufferSave(hBuffer,g_saveFileName,ITKBUFFER_VAL_TIFF);
             CHECK(res);
             */

             // Read buffer
             res = ItkBufferGetPrm(hBuffer,ITKBUFFER_PRM_SIZE, &nImageSize);//获取缓冲区的参数（缓冲区大小）
             if(res!=ITKSTATUS_OK)
             {
                 return;
             }
             res = ItkBufferRead(hBuffer,0,iKapCam->g_bufferData,(uint32_t)nImageSize);
             if(res!=ITKSTATUS_OK)
             {
                 return;
             }

            iKapCam->Convert2ToMat(iKapCam->nWidth,iKapCam->nHeight,iKapCam->nFormat,iKapCam->g_bufferData);

             //g_bufferData可用于转换为Mat
         }
//         iKapCam->g_nCurFrameIndex++;
//         iKapCam->g_nCurFrameIndex =  iKapCam->g_nCurFrameIndex % STREAM_BUFFER_COUNT;
     }
      iKapCam->g_nCurFrameIndex++;
      iKapCam->g_nCurFrameIndex =  iKapCam->g_nCurFrameIndex % STREAM_BUFFER_COUNT;

}


void IKAPC_CC cbOnTimeOut(uint32_t eventType, void* pContext)
{
     printf("on time out. \n");
}

void IKAPC_CC cbOnFrameLost(uint32_t eventType, void* pContext)
{
    printf("on frame lost. \n");
}

/* This function demonstrates how to retrieve the error message for the last failed
function call.
此函数演示如何检索上次失败的错误消息函数调用。*/
void ikapCamera::printErrorAndExit(ITKSTATUS errc)
{
    WriteLog("Error Code:"+QString::number(errc));
    ItkManTerminate();  /* Releases all resources. */
}

void ikapCamera::WriteLog(QString message)
{
    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("yyyy-MM-dd hh:mm:ss zzz");
    QString log ="["+timeStr+"]:"+message+"\n";
    qDebug()<<log; //测试用
}

bool ikapCamera::Convert2ToMat(int64_t w, int64_t h, int64_t format, unsigned char *pData)
{
    if ( format == ITKBUFFER_VAL_FORMAT_MONO8 )
    {
        srcImage = cv::Mat(h, w, CV_8UC1, pData);
    }
    else if ( format == ITKBUFFER_VAL_FORMAT_BAYER_GR8 )
    {
        //RGB2BGR(pData, w,h);//彩色mat用BGR转换，暂时不用
        srcImage = cv::Mat(h, w, CV_8UC3, pData);
    }
    else
    {
        printf("unsupported pixel format\n");
        return false;
    }

    if ( NULL == srcImage.data )
    {
        return false;
    }

    emit SendCameraInfo(srcImage.clone());

    return true;
}

void ikapCamera::RGB2BGR(unsigned char *pRgbData, unsigned int nWidth, unsigned int nHeight)
{

}

