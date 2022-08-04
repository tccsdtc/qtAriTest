//#pragma execution_character_set("utf-8")
#include "dalsa_camera.h"
#include <QDebug>
#include <QString>
#include <QString>
#include <QTime>
//cv::String imagefolder1 = "D:\\program\\JuShi\\Image\\waitdetection\\";//一组图的检测路径waitdetection

DalsaCamera::DalsaCamera()
{
    Xfer = NULL;
    m_pFeature = NULL;
    m_pBuffers = NULL;
    m_iImageWidth = 0;
    m_iImageHeight = 0;
    bIsInitDevices = false;
    isFinishSavePic = false;
    m_yData = "";

//    std::vector<cv::String> imagenames;
//    std::cout <<imagefolder1<<std::endl;
//    cv::glob(imagefolder1,imagenames,false);
//    std::cout<<"imagenames.size():"<<imagenames.size()<<std::endl;


   // picFlag = 0;
}

DalsaCamera::~DalsaCamera()
{

}

bool DalsaCamera::CheckCameraNums(list<string> &serverNameList)
{
    serverNameList.clear();
    int serverCount = SapManager::GetServerCount();
    if (serverCount == 0)
    {
        printf("No device found!\n");
        return false;
    }

    //2.获取服务器名称***************************
      // Scan the boards to find those that support acquisition
    BOOL serverFound = FALSE;
    BOOL cameraFound = FALSE;
    //选择服务器**********************
    for (int serverIndex = 0; serverIndex < serverCount; serverIndex++)
    {
        //4.ResourceAcq帧捕获硬件
        if (SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcq) != 0)
        {
            char serverName[CORSERVER_MAX_STRLEN];
            SapManager::GetServerName(serverIndex, serverName, sizeof(serverName));
            printf("%d: %s\n", serverIndex, serverName);
            serverFound = TRUE;
        }
        else if (SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcqDevice) != 0)
        {
            //1：Linea_M8192-7um_1
            char serverName[CORSERVER_MAX_STRLEN];
            SapManager::GetServerName(serverIndex, serverName, sizeof(serverName));
            if (strstr(serverName, CAMERA_LINK_SERVER_NAME_PREFIX) > (char*)0)
                continue;
            serverNameList.push_back(serverName);
//            printf("%d: %s\n", serverIndex, serverName);
            cameraFound = TRUE;
        }
    }

    // At least one acquisition server must be available
    if (!serverFound && !cameraFound)
    {
        printf("No acquisition server found!\n");
        return false;
    }
}


bool DalsaCamera::searchServer(std::vector<std::string>& vec_acqServerName)
{
    vec_acqServerName.clear();
    vec_acqServerName.resize(0);
        printf("Sapera Console Grab Example (C++ version)\n");
    //1.获取可用的Sapera服务器的数量
    int serverCount = SapManager::GetServerCount();
    if (serverCount == 0)
    {
        printf("No device found!\n");
        return false;
    }


    //2.获取服务器名称***************************
      // Scan the boards to find those that support acquisition
    BOOL serverFound = FALSE;
    BOOL cameraFound = FALSE;
    //选择服务器**********************
    for (int serverIndex = 0; serverIndex < serverCount; serverIndex++)
    {
        //4.ResourceAcq帧捕获硬件
        if (SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcq) != 0)
        {
            char serverName[CORSERVER_MAX_STRLEN];
            SapManager::GetServerName(serverIndex, serverName, sizeof(serverName));
            printf("%d: %s\n", serverIndex, serverName);
            serverFound = TRUE;
        }
        else if (SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcqDevice) != 0)
        {
            //1：Linea_M8192-7um_1
            char serverName[CORSERVER_MAX_STRLEN];
            SapManager::GetServerName(serverIndex, serverName, sizeof(serverName));
            if (strstr(serverName, CAMERA_LINK_SERVER_NAME_PREFIX) > (char*)0)
                continue;
            vec_acqServerName.push_back(serverName);
            printf("%d: %s\n", serverIndex, serverName);
            cameraFound = TRUE;
        }
    }

    // At least one acquisition server must be available
    if (!serverFound && !cameraFound)
    {
        printf("No acquisition server found!\n");
        return false;
    }
    //根据输入索引获得服务器名字
   // SapManager::GetServerName(1, acqServerName, CORSERVER_MAX_STRLEN);//默认获得第一个索引服务器名字
}

bool DalsaCamera::searchCamera(std::string vec_acqServerName, std::vector<UINT32>& vec_acqDeviceNumber, std::vector<std::string>& vec_cameraName)
{
    vec_cameraName.clear();
    vec_cameraName.resize(0);
    vec_acqDeviceNumber.clear();
    vec_acqDeviceNumber.resize(0);
    int deviceCount = SapManager::GetResourceCount(vec_acqServerName.c_str(), SapManager::ResourceAcq);
    cameraCount = (deviceCount == 0) ? SapManager::GetResourceCount(vec_acqServerName.c_str(), SapManager::ResourceAcqDevice) : 0;

    //printf("\nSelect the acquisition device (or 'q' to quit)");
    //printf("\n..............................................\n");

    if (deviceCount == 0)
    {
        //1:A1
        for (int cameraIndex = 0; cameraIndex < cameraCount; cameraIndex++)
        {
            char cameraName[CORPRM_GETSIZE(CORACQ_PRM_LABEL)];
            SapManager::GetResourceName(vec_acqServerName.c_str(), SapManager::ResourceAcqDevice, cameraIndex, cameraName, sizeof(cameraName));
           // printf("%d: %s\n", cameraIndex + 1, cameraName);
            vec_cameraName.push_back(cameraName);
            vec_acqDeviceNumber.push_back(cameraIndex);
            //std::cout << "138" << std::endl;
        }
    }

    if (cameraCount == 0)
    {
        printf("\n NO cameras");
        return false;
    }

    return true;
}

bool DalsaCamera::searchConfig(std::vector<std::string>& str_config)
{
    str_config.clear();
    str_config.resize(0);
    str_config.push_back("NoFile");
    //三、配置文件
    char configPath[MAX_PATH];
    configPath[0] = '\0';
    GetEnvironmentVariable("SAPERADIR", configPath, sizeof(configPath));
    std::cout << "configPath =" << configPath << std::endl;//configPath =C:\Program Files\Teledyne DALSA\Sapera

    configPath[sizeof(configPath) - 1] = '\0';
    CorStrncat(configPath, "\\CamFiles\\User\\", sizeof(configPath));
    std::cout << "configPath =" << configPath << std::endl;//configPath =C:\Program Files\Teledyne DALSA\Sapera\CamFiles\User\

    char findPath[MAX_PATH];
    CorStrncpy(findPath, configPath, MAX_PATH);
    CorStrncat(findPath, "*.ccf", sizeof(findPath));
    std::cout << "findPath =" << findPath << std::endl;//C:\Program Files\Teledyne DALSA\Sapera\CamFiles\User\*.ccf

    HANDLE fhandle;
    WIN32_FIND_DATA fdata;
    if ((fhandle = FindFirstFile(findPath, &fdata)) == INVALID_HANDLE_VALUE)
    {
        if (cameraCount == 0)
        {
            printf("No config file found.\nUse CamExpert to generate a config file before running this example.\n");
            return FALSE;
        }
    }

    fhandle = FindFirstFile(findPath, &fdata);

    // Try to find the last letter to choose

    int configFileMenuCount = 0;
    char lastCharMenu = 'x';
    do
    {
        // Use numbers 0 to 9, then lowercase letters if there are more than 10 files
        int configFileMenuShow = configFileMenuCount + 1;
        if (configFileMenuCount > 9)
            lastCharMenu = (char)(configFileMenuShow - 10 + 'a');
        configFileMenuCount++;
    } while (FindNextFile(fhandle, &fdata) && configFileMenuCount < MAX_CONFIG_FILES);
    FindClose(fhandle);

    printf("\nSelect the config file (or 'q' to quit)");
    printf("\n.......................................\n");
    if (deviceCount == 0 && cameraCount != 0)
    {
        printf("0: No config File.\n");
    }

    fhandle = FindFirstFile(findPath, &fdata);							//find first file
    BOOL moreFilesAvailable = TRUE;

    int configFileCount = 1;
    while ((fhandle != INVALID_HANDLE_VALUE && moreFilesAvailable) && configFileCount < MAX_CONFIG_FILES)
    {
        // Use numbers 0 to 9, then lowercase letters if there are more than 10 files
        //if (configFileCount <= 9)
        //    printf("%d: %s\n", configFileCount, fdata.cFileName);
        //else
        //    printf("%c: %s\n", configFileCount - 10 + 'a', fdata.cFileName);
        CorStrncpy(configFileNames[configFileCount], fdata.cFileName, sizeof(configFileNames[configFileCount]));
        std::string ccf = configPath + std::string(fdata.cFileName);
        str_config.push_back(ccf);
        configFileCount++;
        moreFilesAvailable = FindNextFile(fhandle, &fdata);
    }

    FindClose(fhandle);

}


bool DalsaCamera::InitDevices(string acqServerName, UINT32 acqDeviceNumber, std::string configFilename)
{
    m_serverName = acqServerName;
//    if(acqDeviceNumber == NULL)
//    {
    SapLocation loc(acqServerName.c_str());
//    }
//    else
//        SapLocation loc(acqServerName.c_str(), acqDeviceNumber);

    //创建服务器或设备对象
    if (SapManager::GetResourceCount(acqServerName.c_str(), SapManager::ResourceAcq) > 0)
    {
        //构造函数
        Acq = SapAcquisition(loc, configFilename.c_str());
        //资源分配，会附带一个Trash资源
        m_pBuffers = new SapBufferWithTrash(2, &Acq);
        //构造显示资源
        //View = SapView(&Buffers, SapHwndAutomatic);
        //获取区域到缓冲区域转变
        //AcqToBuf = SapAcqToBuf(&Acq, &Buffers, XferCallback, &View);
        AcqToBuf = SapAcqToBuf(&Acq, m_pBuffers, XferCallback, this);
        Xfer = &AcqToBuf;
        m_pFeature = new SapFeature(AcqDevice.GetLocation());

        // Create acquisition object
        if (!Acq.Create())
        {
            std::cout << "!Acq.Create()" << std::endl;
            freeHandles();
            return false;
        }

    }
    else if (SapManager::GetResourceCount(acqServerName.c_str(), SapManager::ResourceAcqDevice) > 0)
    {
        if (strcmp(configFilename.c_str(), "NoFile") == 0)
        {
            std::cout << "strcmp(configFilename" << std::endl;
            AcqDevice = SapAcqDevice(loc, FALSE);
        }
        else
        {
            std::cout << "else strcmp(configFilename" << std::endl;
            std::cout << "configFilename = " << configFilename << std::endl;
            AcqDevice = SapAcqDevice(loc, configFilename.c_str());
        }

        m_pBuffers = new SapBufferWithTrash(2, &AcqDevice);
       // View = SapView(&Buffers, SapHwndAutomatic);
      //  AcqDeviceToBuf = SapAcqDeviceToBuf(&AcqDevice, &Buffers, XferCallback, &View);
        AcqDeviceToBuf = SapAcqDeviceToBuf(&AcqDevice, m_pBuffers, XferCallback, this);
        m_pFeature = new SapFeature(AcqDevice.GetLocation());
        Xfer = &AcqDeviceToBuf;

        // Create acquisition object
        if (!AcqDevice.Create())
        {
            std::cout << "!AcqDevice.Create()" << std::endl;
            freeHandles();
            return false;
        }
    }


    if (m_pBuffers && !*m_pBuffers && !m_pBuffers->Create())
    {
        freeHandles();
        return false;
    }


    // Create transfer object
    if (Xfer && !*Xfer && !Xfer->Create())
    {
        std::cout << "Xfer && !Xfer->Create()" << std::endl;
        freeHandles();
        return false;
    }

    // Create view object
    //if (!View.Create())
    //{
    //    std::cout << "!View.Create()" << std::endl;
    //    FreeHandles();
    //    return;
    //}

    if (m_pFeature && !*m_pFeature && !m_pFeature->Create())
    {
        freeHandles();
        return false;
    }

    m_iImageWidth = m_pBuffers->GetWidth();
    m_iImageHeight = m_pBuffers->GetHeight();
    captureImg.create(m_iImageHeight, m_iImageWidth, CV_8UC1);
    
//    SetDeviceTriggerMode(0); //设置触发模式

    bIsInitDevices = true;
    return true;
}


bool DalsaCamera::freeHandles()
{
    if (bIsInitDevices==true)
    {
        std::cout << "bIsInitDevices " << bIsInitDevices<<std::endl;
        //unregister the acquisition callback
        Acq.UnregisterCallback();

        // Destroy transfer object
        if (Xfer && *Xfer && !Xfer->Destroy())
            return FALSE;


        // Destroy buffer object
        //if (!Buffers.Destroy()) return FALSE;
        if (m_pBuffers && *m_pBuffers && !m_pBuffers->Destroy())
            return FALSE;


        // Destroy acquisition object
        if (!Acq.Destroy()) 
            return FALSE;

        // Destroy acquisition object
        if (!AcqDevice.Destroy()) 
            return FALSE;
    }
  
    //if (Xfer)				
    //    delete Xfer;
    if (m_pBuffers)			
    {
        delete m_pBuffers;
        m_pBuffers = NULL;
    }
    if (m_pFeature)			
    {
        delete m_pFeature;
        m_pFeature = NULL;
    }

    return true;
}

void DalsaCamera::XferCallback(SapXferCallbackInfo* pInfo)
{
    DalsaCamera* pDlg = (DalsaCamera*)pInfo->GetContext();
    if (pInfo->IsTrash())
    {
        //qDebug()<< "IsTrash";
    }
    else
    {
        if (pDlg->bIsSavePic)
        {
            pDlg->m_pBuffers->GetAddress((void**)&(pDlg->TMPBUF));
            memcpy(pDlg->captureImg.data, pDlg->TMPBUF, pDlg->m_iImageWidth * pDlg->m_iImageHeight);
            std::string path;
            path = pDlg->waitdetectionPath + pDlg->m_serverName + "~" +
                   QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz").toStdString()+".bmp"; //可增加米长数据
            qDebug()<< QString::fromStdString(path);
            emit pDlg->SendCameraInfo(pDlg->captureImg.clone());
            //pDlg->SaveImage(pDlg->captureImg, path);
            pDlg->isFinishSavePic = true;
        }
    }
}

bool DalsaCamera::OpenVideoStream()
{
    bIsOpenVideo = true;
    Xfer->Grab();
    return bIsOpenVideo;
}

bool DalsaCamera::CloseVideoStream()
{
    if (bIsOpenVideo == true)
    {
        Xfer->Freeze();
        if (!Xfer->Wait(5000))
        {
            printf("Grab could not stop properly.\n");
            Xfer->Abort();
            return false;
        }
        else
        {
            bIsOpenVideo = false;
            printf("Close VideoStream.\n");
            return true;
        }
    }
    else
    {
        printf("The video stream is not on.\n");
        return false;
    }

}

bool DalsaCamera::CloseCamera()
{
    CloseVideoStream();
    return freeHandles();
}

void DalsaCamera::SaveImage(cv::Mat& pic, std::string& path)
{
    //std::cout << "camId = " << camId << std::endl;;
//    SYSTEMTIME st = { 0 };
//    GetLocalTime(&st);  //获取当前时间 可精确到ms
    //char name[256] = { 0 };
    //sprintf(name, "%d_%02d_%02d_%02d-%02d_%02d_%d.bmp", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);//将年份格式输出到t_y变量中
    //std::string picName = "D:/program/JuShi/Image/waitdetection/"  + std::to_string(camId) + "-"+  std::string(name);
    //std::string picName = waitdetectionPath  + std::to_string(camId) + "-"+  std::string(name);
   //std::string picName = "./img/pic"  + std::to_string(camId) + "-"+  std::string(name);
    //std::cout << "picName = " << picName << std::endl;
    cv::imwrite(path, pic);
}

void  DalsaCamera::SetDeviceLineFrequency(double LineRate_str)
{
    m_linerate = LineRate_str;
    double LineRatemaxValue, LineRateminValue;
    AcqDevice.GetFeatureInfo("AcquisitionLineRate", m_pFeature);
    m_pFeature->GetMax(&LineRatemaxValue);
    m_pFeature->GetMin(&LineRateminValue);
    if (m_linerate > LineRatemaxValue)
    {
        m_linerate = LineRatemaxValue;
    }
    if (m_linerate < LineRateminValue)
    {
        m_linerate = LineRateminValue;
    }
    AcqDevice.SetFeatureValue("AcquisitionLineRate", m_linerate);
}

void  DalsaCamera::SetDeviceTriggerMode(int mode)
{
    m_triggermode =mode;
    AcqDevice.SetFeatureValue("TriggerMode", m_triggermode); 
}

void DalsaCamera:: getPath(cv::String waitPath)
{
    waitdetectionPath = waitPath;
}
