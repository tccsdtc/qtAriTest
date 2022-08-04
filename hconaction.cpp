#include "hconaction.h"
#define HCPP_LEGACY_API

void MyHalconExceptionHandler(const HalconCpp::HException& except)
{
    throw except;
}


hconAction::hconAction(QObject* parent) : QObject(parent)
{
    // HalconCpp::HException::InstallHHandler(&MyHalconExceptionHandler);
}


Mat hconAction::HImageToMat(const HalconCpp::HImage &hImg)
{

    cv::Mat mat;
    int channels = hImg.CountChannels()[0].I();
    HalconCpp::HImage hImage = hImg.ConvertImageType("byte");

    Hlong hW = 0, hH = 0;
    HalconCpp::HString cType;

    if (channels == 1) {
        void *r = hImage.GetImagePointer1(&cType, &hW, &hH);
        mat.create(int(hH), int(hW), CV_8UC1);
        memcpy(mat.data, static_cast<unsigned char *>(r), int(hW*hH));
    }
    else if (channels == 3) {
        void *r = NULL, *g = NULL, *b = NULL;

        hImage.GetImagePointer3(&r, &g, &b, &cType, &hW, &hH);
        mat.create(int(hH), int(hW), CV_8UC3);

        std::vector<cv::Mat> vec(3);
        vec[0].create(int(hH), int(hW), CV_8UC1);
        vec[1].create(int(hH), int(hW), CV_8UC1);
        vec[2].create(int(hH), int(hW), CV_8UC1);

        memcpy(vec[2].data, static_cast<unsigned char *>(r), int(hW*hH));
        memcpy(vec[1].data, static_cast<unsigned char *>(g), int(hW*hH));
        memcpy(vec[0].data, static_cast<unsigned char *>(b), int(hW*hH));
        cv::merge(vec, mat);
    }
    return mat;

}

Mat hconAction::HObjectToMat(const HalconCpp::HObject &hObj)
{
    HalconCpp::HImage hImg(hObj);
    return HImageToMat(hImg);
}

HalconCpp::HObject hconAction::MatToHObject(const Mat &image)
{
    HalconCpp::HObject Hobj = HalconCpp::HObject();
    int hgt = image.rows;
    int wid = image.cols;
    int i;
    if (image.type() == CV_8UC3)
    {
        std::vector<Mat> imgchannel;
        split(image, imgchannel);
        Mat imgB = imgchannel[0];
        Mat imgG = imgchannel[1];
        Mat imgR = imgchannel[2];
        uchar* dataR = new uchar[hgt*wid];
        uchar* dataG = new uchar[hgt*wid];
        uchar* dataB = new uchar[hgt*wid];
        for (i = 0; i < hgt; i++)
        {
            memcpy(dataR + wid*i, imgR.data + imgR.step*i, wid);
            memcpy(dataG + wid*i, imgG.data + imgG.step*i, wid);
            memcpy(dataB + wid*i, imgB.data + imgB.step*i, wid);
        }
        HalconCpp::GenImage3(&Hobj, "byte", wid, hgt, (Hlong)dataR, (Hlong)dataG, (Hlong)dataB);
        delete[]dataR;
        delete[]dataG;
        delete[]dataB;
        dataR = NULL;
        dataG = NULL;
        dataB = NULL;
    }
    else if (image.type() == CV_8UC1)
    {
        uchar* data = new uchar[hgt*wid];
        for (i = 0; i < hgt; i++)
            memcpy(data + wid*i, image.data + image.step*i, wid);
        HalconCpp::GenImage1(&Hobj, "byte", wid, hgt, (Hlong)data);
        delete[] data;
        data = NULL;
    }
    return Hobj;
}

HalconCpp::HImage hconAction::MatToHImage(const Mat &image)
{
    HalconCpp::HImage hImg(MatToHObject(image));
    return hImg;
}

int hconAction::FilterAction1(const HalconCpp::HObject& himg, bool isEdge, const double& setFlawSize, const int& IQDflawType, double& FlawSize)
{
    HObject  ho_RegionDynThreshBlackEgde, ho_RegionFillUp;
    HObject  ho_RegionDilation, ho_Image, ho_RegionDetect;
    HObject  ho_ImageMean, ho_RegionDynThresh, ho_ConnectedRegions;
    HObject  ho_SelectedRegions, ho_ConnectedRegions1, ho_SelectedRegions1;
    HObject  ho_RegionDilation1, ho_ImageDilation, ho_RegionDynThresh1;
    HObject  ho_SelectedRegions2, ho_RegionComplement;

    // Local control variables
    HTuple  hv_Area, hv_Row3, hv_Column3;
    HTuple  hv_Width, hv_Height, hv_Mean, hv_Deviation, hv_Min;
    HTuple  hv_Max, hv_Range, hv_centerX_L, hv_centerX_H, hv_meanValue;
    HTuple  hv_Row, hv_Column, hv_Radius, hv_Width1;
    HTuple  hv_Height1, hv_Row2, hv_Column2, hv_Radius2, hv_size,hv_Sum;
    HTuple  hv_Row1, hv_Column1, hv_Radius1, hv_diffSize, hv_meanValue_background;
    HTuple  hv_diffMean, hv_diffMeanRatio, hv_diffresult, hv_result;

    //***********************************************初始化*************************************************
    FlawSize=0;
    ho_Image = himg.Clone();
    try
    {
        //***********************************************准备步骤*************************************************

        //寻找黑边区域
        Threshold(ho_Image, &ho_RegionDynThreshBlackEgde, 0, 8);
        AreaCenter(ho_RegionDynThreshBlackEgde, &hv_Area, &hv_Row3, &hv_Column3);
        if (0 != (hv_Area > 15))
        {
            FillUp(ho_RegionDynThreshBlackEgde, &ho_RegionFillUp);
            DilationCircle(ho_RegionFillUp, &ho_RegionDilation, 5);
            //去除黑色区域
            Complement(ho_RegionDilation, &ho_RegionDetect);
            ReduceDomain(ho_Image, ho_RegionDetect, &ho_Image);
            CropDomain(ho_Image, &ho_Image);
        }

        //切图后计算
        GetImageSize(ho_Image, &hv_Width, &hv_Height);
        Intensity(ho_Image, ho_Image, &hv_Mean, &hv_Deviation);
        MinMaxGray(ho_Image, ho_Image, 0, &hv_Min, &hv_Max, &hv_Range);

        hv_centerX_L = (hv_Width * 1) / 8;
        hv_centerX_H = (hv_Width * 7) / 8;


        //**********************************************缺胶点状+线状，模糊点，鱼眼*********************************************************
//        //滤波
//        MeanImage(ho_Image, &ho_ImageMean, 3, 3);
//        //使用滤波后的图动态分割
//        VarThreshold(ho_ImageMean, &ho_RegionDynThresh, hv_Width, hv_Height, 0.2,20, "dark");
        VarThreshold(ho_Image, &ho_RegionDynThresh, hv_Width, hv_Height, 0.2,20, "dark");
        ClosingCircle(ho_RegionDynThresh, &ho_RegionDynThresh, 3.5);
        FillUp(ho_RegionDynThresh, &ho_RegionDynThresh);

        //去除最周围的区域
        Connection(ho_RegionDynThresh, &ho_ConnectedRegions);
        SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "column", "and", hv_centerX_L,hv_centerX_H);

        //如果是边界上的缺陷，过滤掉长度过大的缺陷（大概率铜箔边）
        if (isEdge)
        {
            SelectShape(ho_SelectedRegions, &ho_SelectedRegions, "height", "and", 1,hv_Height * 0.9);
        }
        Union1(ho_SelectedRegions, &ho_SelectedRegions);

        //得到选中区域的平均灰度，注意此图不是滤波后的图
        GrayFeatures(ho_SelectedRegions, ho_Image, "mean", &hv_meanValue);

        //得到选中区域中最大区域的尺寸
        Connection(ho_SelectedRegions, &ho_ConnectedRegions1);
        SelectShapeStd(ho_ConnectedRegions1, &ho_SelectedRegions1, "max_area", 70);
        //得到尺寸
        SmallestCircle(ho_ConnectedRegions1, &hv_Row2, &hv_Column2, &hv_Radius2);
        TupleSum(hv_Radius2, &hv_Sum);
        hv_size = (hv_Sum * 2) * 0.173;
        FlawSize=hv_size;

        //膨胀这个最大区域，在膨胀后的区域里继续动态分割
        DilationCircle(ho_SelectedRegions1, &ho_RegionDilation1, 6);
        ReduceDomain(ho_Image, ho_RegionDilation1, &ho_ImageDilation);
        GetImageSize(ho_ImageDilation, &hv_Width1, &hv_Height1);
        VarThreshold(ho_ImageDilation, &ho_RegionDynThresh1, hv_Width1, hv_Height1,0.2, 7, "dark");
        FillUp(ho_RegionDynThresh1, &ho_RegionDynThresh1);
        Connection(ho_RegionDynThresh1, &ho_RegionDynThresh1);
        SelectShapeStd(ho_RegionDynThresh1, &ho_SelectedRegions2, "max_area", 70);


        if (0 != (hv_size > setFlawSize))
        {
            //得到浓区域和淡区域的内接圆尺寸
            InnerCircle(ho_SelectedRegions1, &hv_Row, &hv_Column, &hv_Radius);
            InnerCircle(ho_SelectedRegions2, &hv_Row1, &hv_Column1, &hv_Radius1);
            hv_diffSize = hv_Radius1 - hv_Radius;

            //得到背景的平均灰度
            Complement(ho_SelectedRegions, &ho_RegionComplement);
            GrayFeatures(ho_RegionComplement, ho_Image, "mean", &hv_meanValue_background);

            hv_diffMean = hv_meanValue_background - hv_meanValue;
            hv_diffMeanRatio = hv_meanValue_background / hv_diffMean;
            hv_diffresult = hv_diffMean - ((hv_diffMeanRatio * hv_diffMeanRatio) / 2);

            if (0 != (hv_diffresult > 15))
            {
                if (0 != (HTuple(hv_diffresult <= 25).TupleAnd(hv_diffSize >= 5)))
                {
                    hv_result = 0;
                }
                else if (0 != (HTuple(hv_diffresult <= 20).TupleAnd(hv_diffSize >= 4)))
                {
                    hv_result = 0;
                }
                else
                {
                    hv_result = 1;
                }
            }
            else
            {
                if (0 != (HTuple(hv_diffresult > 10).TupleAnd(hv_diffSize <= 2)))
                {
                    hv_result = 1;
                }
                else
                {
                    hv_result = 0;
                }
            }
        }
        else
        {
            hv_result = 0;
        }

        return hv_result;
    }
    catch (HalconCpp::HException &except)
    {
        QString message=QString(except.ErrorText().Text());
        qDebug()<<message;
        return 0;
    }
}


int hconAction::FilterAction2(const HObject &himg, bool isEdge, const double &setFlawSize, const int &IQDflawType, double &FlawSize)
{
    // Local iconic variables
    HObject  ho_RegionDynThreshBlackEgde, ho_RegionFillUp;
    HObject  ho_Image, ho_RegionDilation, ho_RegionDetect;
    HObject  ho_ImageGauss, ho_RegionDark, ho_ConnectedRegions;
    HObject  ho_SelectedRegions, ho_SelectedSizeRegions, ho_RegionLight;
    HObject ho_EmptyObject,ho_SelectedRegions1,ho_RegionDarkDilation;
    HObject ho_ConnectedRegions1,ho_RegionUnion;

    // Local control variables
    HTuple  hv_Contrast, hv_Area, hv_Row3, hv_Column3;
    HTuple  hv_Mean, hv_Deviation, hv_Width, hv_Height, hv_centerX_L;
    HTuple  hv_centerX_H, hv_centerY_L, hv_centerY_H, hv_Min;
    HTuple  hv_Max, hv_Range, hv_AbsThreshold, hv_RegionNum;
    HTuple  hv_Row, hv_Column, hv_Radius, hv_size, hv_ValueDark;
    HTuple  hv_ValueLight, hv_diffMean, hv_result;
    HTuple  hv_Energy, hv_Correlation, hv_Homogeneity;
    HTuple hv_IsEqual;
    HTuple hv_Height1, hv_Width1, hv_Ratio,hv_HeightScale;

    FlawSize=0;
    hv_result==0;
    ho_Image = himg.Clone();
    try
    {
        //***********************************************准备步骤*************************************************
        //寻找黑边区域
        Threshold(ho_Image, &ho_RegionDynThreshBlackEgde, 0, 8);
        GetImageSize(ho_Image, &hv_Width, &hv_Height);

        FillUp(ho_RegionDynThreshBlackEgde, &ho_RegionFillUp);
        DilationCircle(ho_RegionFillUp, &ho_RegionDilation, 5);
        Connection(ho_RegionDilation, &ho_ConnectedRegions1);
        SelectShape(ho_ConnectedRegions1, &ho_SelectedRegions1, (HTuple("column1").Append("column2")),
            "or", HTuple(0).TupleConcat(hv_Width-10), HTuple(10).TupleConcat(hv_Width));
        //去除黑色区域
        GenEmptyObj(&ho_EmptyObject);
        TestEqualObj(ho_EmptyObject, ho_SelectedRegions1, &hv_IsEqual);
        if (0 != (hv_IsEqual.TupleNot()))
        {
          Union1(ho_SelectedRegions1, &ho_RegionUnion);
          Complement(ho_RegionUnion, &ho_RegionDetect);
          ReduceDomain(ho_Image, ho_RegionDetect, &ho_Image);
          CropDomain(ho_Image, &ho_Image);
        }

        //切图后计算
        Intensity(ho_Image, ho_Image, &hv_Mean, &hv_Deviation);
        GetImageSize(ho_Image, &hv_Width, &hv_Height);

        hv_centerX_L = (hv_Width*1)/8;
        hv_centerX_H = (hv_Width*7)/8;
        hv_centerY_L = (hv_Height*1)/8;
        hv_centerY_H = (hv_Height*7)/8;

        //动态分割方法1
        GaussImage(ho_Image, &ho_ImageGauss, 5);
        MinMaxGray(ho_ImageGauss, ho_ImageGauss, 0, &hv_Min, &hv_Max, &hv_Range);
        //**********************************************初筛，分出大面积缺胶，明显脏污等*********************************************************
        //找黑色区域
        hv_AbsThreshold = hv_Range/4;
        if (0 != (hv_AbsThreshold<6))
        {
          hv_AbsThreshold = 6;
        }
        VarThreshold(ho_ImageGauss, &ho_RegionDark, hv_Width, hv_Height, 0.2, hv_AbsThreshold,
            "dark");
        Connection(ho_RegionDark, &ho_ConnectedRegions);
        SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, ((HTuple("area").Append("column")).Append("row")),
            "and", (HTuple(5).TupleConcat(hv_centerX_L)).TupleConcat(hv_centerY_L),
            (HTuple(99999).TupleConcat(hv_centerX_H)).TupleConcat(hv_centerY_H));
        CountObj(ho_SelectedRegions, &hv_RegionNum);
        //用于得到尺寸
        SelectShapeStd(ho_SelectedRegions, &ho_SelectedSizeRegions, "max_area", 70);
        SmallestCircle(ho_SelectedSizeRegions, &hv_Row, &hv_Column, &hv_Radius);
        hv_size = (hv_Radius*2)*0.173;
        FlawSize=hv_size;

        Union1(ho_SelectedRegions, &ho_RegionDark);

        //得到背景区域
        //Complement(ho_RegionDark, &ho_RegionLight);
        DilationCircle(ho_RegionDark, &ho_RegionDarkDilation, 5);
        Difference(ho_RegionDarkDilation, ho_RegionDark, &ho_RegionLight);

        //计算前景区域和背景区域的平均灰度
        GrayFeatures(ho_RegionDark, ho_Image, "mean", &hv_ValueDark);
        GrayFeatures(ho_RegionLight, ho_Image, "mean", &hv_ValueLight);

        hv_diffMean = hv_ValueLight-hv_ValueDark;
        if (0 != (HTuple(hv_diffMean>16).TupleAnd(hv_size>setFlawSize)))
        {
          hv_result = 1;
        }
        else
        {
          hv_result = 0;
        }
    }
    catch (HalconCpp::HException &except)
    {
        QString message="Step1——:"+QString(except.ErrorText().Text());
        qDebug()<<message;
        hv_result = 0;
    }


    if (0 != (hv_result==0))
    {
        try
        {
            //尺寸区域膨胀做纹理运算
            DilationCircle(ho_SelectedSizeRegions, &ho_RegionDilation, 1.5);
            HeightWidthRatio(ho_SelectedSizeRegions, &hv_Height1, &hv_Width1, &hv_Ratio);
            hv_HeightScale = ((hv_Height1*1.0)/hv_Height)*1.0;
            CoocFeatureImage(ho_RegionDilation, ho_Image, 8, "mean", &hv_Energy, &hv_Correlation,
                &hv_Homogeneity, &hv_Contrast);

            if (0 != (HTuple(HTuple(HTuple(HTuple(hv_Contrast>(hv_diffMean*3.5)).TupleAnd(hv_RegionNum<=5)).TupleAnd(hv_size>1)).TupleAnd(hv_diffMean>10)).TupleAnd(hv_HeightScale<0.8)))
            {
              hv_result = 1;
            }
            else
            {
              hv_result = 0;
            }
        }
        catch (HalconCpp::HException &except)
        {
            QString message="Step2——"+QString(except.ErrorText().Text());
            qDebug()<<message;
            hv_result = 0;
        }
    }
    return hv_result;
}

int hconAction::FilterAction3(const HObject &himg, bool isEdge, const double &setFlawSize, const int &IQDflawType, double &FlawSize)
{
    // Local iconic variables
    HObject  ho_RegionDynThreshBlackEgde, ho_RegionFillUp;
    HObject  ho_Image, ho_RegionDilation, ho_RegionDetect;
    HObject  ho_ImageGauss, ho_RegionDark, ho_ConnectedRegions;
    HObject  ho_SelectedRegions;

    // Local control variables
    HTuple  hv_Area_Light, hv_Area, hv_Row;
    HTuple  hv_Column, hv_Mean, hv_Deviation, hv_Width, hv_Height;
    HTuple  hv_Min, hv_Max, hv_Range, hv_Row_Light, hv_Column_Light;
    HTuple  hv_result;

    ho_Image = himg.Clone();
    try
    {
        //***********************************************准备步骤*************************************************
        //寻找黑边区域
        Threshold(ho_Image, &ho_RegionDynThreshBlackEgde, 0, 8);
        AreaCenter(ho_RegionDynThreshBlackEgde, &hv_Area, &hv_Row, &hv_Column);
        if (0 != (hv_Area>15))
        {
            FillUp(ho_RegionDynThreshBlackEgde, &ho_RegionFillUp);
            DilationCircle(ho_RegionFillUp, &ho_RegionDilation, 5);
            //去除黑色区域
            Complement(ho_RegionDilation, &ho_RegionDetect);
            ReduceDomain(ho_Image, ho_RegionDetect, &ho_Image);
            CropDomain(ho_Image, &ho_Image);
        }

        //切图后计算
        Intensity(ho_Image, ho_Image, &hv_Mean, &hv_Deviation);
        GetImageSize(ho_Image, &hv_Width, &hv_Height);

        //动态分割方法1
        GaussImage(ho_Image, &ho_ImageGauss, 5);
        MinMaxGray(ho_ImageGauss, ho_ImageGauss, 0, &hv_Min, &hv_Max, &hv_Range);

        //找白色区域
        VarThreshold(ho_ImageGauss, &ho_RegionDark, hv_Width/3, hv_Height/3, 0.2, 6, "light");
        Connection(ho_RegionDark, &ho_ConnectedRegions);
        SelectShapeStd(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
        AreaCenter(ho_SelectedRegions, &hv_Area_Light, &hv_Row_Light, &hv_Column_Light);

        if (0 != (hv_Area_Light>4))
        {
            hv_result = 1;
        }
        else
        {
            hv_result = 0;
        }
    }
    catch (HalconCpp::HException &except)
    {
        QString message=QString(except.ErrorText().Text());
        qDebug()<<message;
        hv_result= 0;
    }
    return hv_result;
}


int hconAction::FilterAction5(const HObject &himg, bool isEdge, const double &setFlawSize1, const double &setFlawSize2, const int &IQDflawType, double &FlawSize)
{
    HObject  ho_RegionDynThreshBlackEgde, ho_RegionFillUp;
    HObject  ho_RegionDilation, ho_Image, ho_RegionDetect;
    HObject  ho_RegionDynThreshLight, ho_RegionDynThreshDrak;
    HObject  ho_SelectedRegions, ho_SelectedRegionsMaxSize, ho_RegionComplement;

    HTuple  hv_size, hv_result, hv_Area;
    HTuple  hv_Row3, hv_Column3, hv_Width, hv_Height, hv_Mean;
    HTuple  hv_Deviation, hv_Min, hv_Max, hv_Range, hv_Area_Light;
    HTuple  hv_Row_Light, hv_Column_Light, hv_Row_MaxSize, hv_Column_MaxSize;
    HTuple  hv_Radius_MaxSize, hv_Compactness, hv_meanValue;
    HTuple  hv_meanValue_background, hv_diffMean;

    FlawSize=0;
    hv_result==0;
    ho_Image = himg.Clone();

    try
    {
        //***********************************************准备步骤*************************************************

        //寻找黑边区域
        Threshold(ho_Image, &ho_RegionDynThreshBlackEgde, 0, 13);
        AreaCenter(ho_RegionDynThreshBlackEgde, &hv_Area, &hv_Row3, &hv_Column3);
        if (0 != (hv_Area>15))
        {
          FillUp(ho_RegionDynThreshBlackEgde, &ho_RegionFillUp);
          DilationCircle(ho_RegionFillUp, &ho_RegionDilation, 5);
          //去除黑色区域
          Complement(ho_RegionDilation, &ho_RegionDetect);
          ReduceDomain(ho_Image, ho_RegionDetect, &ho_Image);
          CropDomain(ho_Image, &ho_Image);
        }

        //切图后计算
        GetImageSize(ho_Image, &hv_Width, &hv_Height);
        Intensity(ho_Image, ho_Image, &hv_Mean, &hv_Deviation);
        MinMaxGray(ho_Image, ho_Image, 0, &hv_Min, &hv_Max, &hv_Range);

        //**********************************************缺胶点状+线状，模糊点，鱼眼*********************************************************
        //滤波
        //mean_image (Image, ImageMean, 3, 3)
        //median_image (Image, Image, 'circle', 1, 'mirrored')
        //动态分割出白色区域
        VarThreshold(ho_Image, &ho_RegionDynThreshLight, hv_Width/2, hv_Height/2, 0.2,
            20, "light");
        ClosingCircle(ho_RegionDynThreshLight, &ho_RegionDynThreshLight, 1.5);
        FillUp(ho_RegionDynThreshLight, &ho_RegionDynThreshLight);
        Connection(ho_RegionDynThreshLight, &ho_RegionDynThreshLight);
        SelectShapeStd(ho_RegionDynThreshLight, &ho_RegionDynThreshLight, "max_area",
            70);

        AreaCenter(ho_RegionDynThreshLight, &hv_Area_Light, &hv_Row_Light, &hv_Column_Light);
        if (0 != (hv_Area_Light>25))
        {
          hv_result = 1;
        }

        if (0 != (hv_result==0))
        {
          VarThreshold(ho_Image, &ho_RegionDynThreshDrak, hv_Width, hv_Height, 0.2,
              15, "dark");
          ClosingCircle(ho_RegionDynThreshDrak, &ho_RegionDynThreshDrak, 2.5);
          FillUp(ho_RegionDynThreshDrak, &ho_RegionDynThreshDrak);
          Connection(ho_RegionDynThreshDrak, &ho_RegionDynThreshDrak);
          //select_shape (RegionDynThreshDrak, SelectedRegions, ['area'], 'and', [10], [99999])
          //如果是边界上的缺陷，过滤掉长度过大的缺陷（大概率铜箔边）
          if (isEdge)
          {
            SelectShape(ho_RegionDynThreshDrak, &ho_SelectedRegions, (HTuple("height").Append("rectangularity")),
                "or", (HTuple(1).Append(0)), (hv_Height*0.9).TupleConcat(0.75));
          }
          //得到选中区域中最大区域的尺寸
          SelectShapeStd(ho_SelectedRegions, &ho_SelectedRegionsMaxSize, "max_area",
              70);
          SmallestCircle(ho_SelectedRegionsMaxSize, &hv_Row_MaxSize, &hv_Column_MaxSize,
              &hv_Radius_MaxSize);
          hv_size = (hv_Radius_MaxSize*2)*0.173;
          FlawSize=hv_size;

          //如果尺寸小于缺胶标准尺寸，则简单判断是否是毛丝
          if (0 != (hv_size<setFlawSize1))
          {
            Compactness(ho_SelectedRegionsMaxSize, &hv_Compactness);
            if (0 != (HTuple(hv_Compactness>1.6).TupleAnd(hv_size>setFlawSize2)))
            {
              hv_result = 1;
            }
            else
            {
              hv_result = 0;
            }

            //如果尺寸大于缺胶标准尺寸，则判断灰度值差异
          }
          else
          {
            //得到平均灰度值
            Union1(ho_SelectedRegions, &ho_SelectedRegions);
            GrayFeatures(ho_SelectedRegions, ho_Image, "mean", &hv_meanValue);
            //得到背景平均灰度值
            Complement(ho_SelectedRegions, &ho_RegionComplement);
            GrayFeatures(ho_RegionComplement, ho_Image, "mean", &hv_meanValue_background);
            hv_diffMean = hv_meanValue_background-hv_meanValue;
            if (0 != (hv_diffMean>20))
            {
              hv_result = 1;
            }
            else
            {
              hv_result = 0;
            }
          }
        }
    return hv_result;
    }
    catch (HalconCpp::HException &except)
    {
        QString message=QString(except.ErrorText().Text());
        qDebug()<<message;
        return 0;
    }
}


int hconAction::FilterAction6(const HalconCpp::HObject& himg, bool isEdge,const double& setFlawSize ,const int& IQDflawType, double& FlawSize)
{
    HObject  ho_RegionDynThreshBlackEgde;
    HObject  ho_Image, ho_RegionFillUp, ho_RegionDilation, ho_RegionDetect;
    HObject  ho_ImageMean, ho_RegionDynThresh, ho_ConnectedRegions;
    HObject  ho_SelectedRegions, ho_ConnectedRegions1, ho_SelectedRegions1;
    HObject  ho_RegionComplement;

    // Local control variables
    HTuple  hv_Mean, hv_Deviation, hv_Min, hv_Max;
    HTuple  hv_Range, hv_Width, hv_Height, hv_centerX_L, hv_centerX_H;
    HTuple  hv_centerY_L, hv_centerY_H, hv_isDarkFlaw, hv_strDL;
    HTuple  hv_meanValue, hv_Row, hv_Column, hv_Radius, hv_resultRadius;
    HTuple  hv_meanValue_background, hv_diffMean, hv_result;

    ho_Image = himg.Clone();
    try
    {
        //寻找黑边区域
        Threshold(ho_Image, &ho_RegionDynThreshBlackEgde, 0, 8);
        FillUp(ho_RegionDynThreshBlackEgde, &ho_RegionFillUp);
        DilationCircle(ho_RegionFillUp, &ho_RegionDilation, 5);
        //去除黑色区域
        Complement(ho_RegionDilation, &ho_RegionDetect);
        ReduceDomain(ho_Image, ho_RegionDetect, &ho_Image);
        CropDomain(ho_Image, &ho_Image);

        //切图后重新计算
        Intensity(ho_Image, ho_Image, &hv_Mean, &hv_Deviation);
        MinMaxGray(ho_Image, ho_Image, 0, &hv_Min, &hv_Max, &hv_Range);
        //得到长宽
        GetImageSize(ho_Image, &hv_Width, &hv_Height);

        hv_centerX_L = (hv_Width * 1) / 8;
        hv_centerX_H = (hv_Width * 7) / 8;

        //得到长宽
        GetImageSize(ho_Image, &hv_Width, &hv_Height);

        //滤波
        MeanImage(ho_Image, &ho_ImageMean, 1, 1);
        //median_image (Image, Image, 'circle', 1, 'mirrored')
        //使用滤波后的图动态分割
        VarThreshold(ho_ImageMean, &ho_RegionDynThresh, hv_Width, hv_Height, 0.2, 20, "dark");
    //去除最周围的区域
        Connection(ho_RegionDynThresh, &ho_ConnectedRegions);
        SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "column", "and", hv_centerX_L, hv_centerX_H);
        //如果是边界上的缺陷，过滤掉长度过大的缺陷（大概率铜箔边）
        if (isEdge)
        {
            SelectShape(ho_SelectedRegions, &ho_SelectedRegions, "height", "and", 1,hv_Height * 0.9);
        }
        Union1(ho_SelectedRegions, &ho_SelectedRegions);
        //得到选中区域的平均灰度，注意此图不是滤波后的图
        GrayFeatures(ho_SelectedRegions, ho_Image, "mean", &hv_meanValue);
        //得到选中区域中最大区域的尺寸
        Connection(ho_SelectedRegions, &ho_ConnectedRegions1);
        SelectShapeStd(ho_ConnectedRegions1, &ho_SelectedRegions1, "max_area", 70);

        SmallestCircle(ho_SelectedRegions1, &hv_Row, &hv_Column, &hv_Radius);
        hv_resultRadius = (hv_Radius * 2) * 0.173;
        FlawSize=hv_resultRadius;

        //得到背景的平均灰度
        Complement(ho_SelectedRegions, &ho_RegionComplement);
        GrayFeatures(ho_RegionComplement, ho_Image, "mean", &hv_meanValue_background);

        hv_diffMean = hv_meanValue_background - hv_meanValue;

        if (0 != (hv_resultRadius > setFlawSize))
        {
            if (0 != (hv_diffMean > 32))
            {
                hv_result = 1;
            }
            else
            {
                hv_result = 0;
            }
        }
        else
        {
            hv_result = 0;
        }
    }
    catch (HalconCpp::HException &except)
    {
        QString message=QString(except.ErrorText().Text());
        qDebug()<<message;
        hv_result= 0;
    }
    return hv_result;
}

int hconAction::FilterAction7(const HObject &himg, bool isEdge, const double &setFlawSize1, const int &IQDflawType, double &FlawSize)
{
    // Local iconic variables
    HObject  ho_RegionDynThreshBlackEgde, ho_RegionFillUp;
    HObject  ho_RegionDilation, ho_Image, ho_RegionDetect;
    HObject  ho_ImageMean, ho_RegionDynThresh, ho_ConnectedRegions;
    HObject  ho_SelectedRegions, ho_ConnectedRegions1, ho_SelectedRegions1;
    HObject  ho_RegionDilation1, ho_ImageDilation, ho_RegionDynThresh1;
    HObject  ho_SelectedRegions2, ho_RegionComplement, ho_RegionDynThreshLight;
    HObject  ho_ConnectedRegionsLight, ho_SelectedRegionsLight;

    // Local control variables
    HTuple  hv_Area, hv_Row3, hv_Column3;
    HTuple  hv_Width, hv_Height, hv_Mean, hv_Deviation, hv_Min;
    HTuple  hv_Max, hv_Range, hv_centerX_L, hv_centerX_H, hv_meanValue;
    HTuple  hv_Row2, hv_Column2, hv_Radius2, hv_size, hv_Width1;
    HTuple  hv_Height1, hv_Row, hv_Column, hv_Radius, hv_Row1;
    HTuple  hv_Column1, hv_Radius1, hv_diffSize, hv_meanValue_background;
    HTuple  hv_diffMean, hv_diffMeanRatio, hv_diffresult, hv_result;
    HTuple  hv_Area_Light, hv_Row4, hv_Column4;

    FlawSize=0;
    hv_result==0;
    ho_Image = himg.Clone();
    try
    {
        //***********************************************准备步骤*************************************************

        //寻找黑边区域
        Threshold(ho_Image, &ho_RegionDynThreshBlackEgde, 0, 8);
        AreaCenter(ho_RegionDynThreshBlackEgde, &hv_Area, &hv_Row3, &hv_Column3);
        if (0 != (hv_Area>15))
        {
          FillUp(ho_RegionDynThreshBlackEgde, &ho_RegionFillUp);
          DilationCircle(ho_RegionFillUp, &ho_RegionDilation, 5);
          //去除黑色区域
          Complement(ho_RegionDilation, &ho_RegionDetect);
          ReduceDomain(ho_Image, ho_RegionDetect, &ho_Image);
          CropDomain(ho_Image, &ho_Image);
        }

        //切图后计算
        GetImageSize(ho_Image, &hv_Width, &hv_Height);
        Intensity(ho_Image, ho_Image, &hv_Mean, &hv_Deviation);
        MinMaxGray(ho_Image, ho_Image, 0, &hv_Min, &hv_Max, &hv_Range);

        hv_centerX_L = (hv_Width*1)/8;
        hv_centerX_H = (hv_Width*7)/8;


        //**********************************************缺胶点状+线状，模糊点，鱼眼*********************************************************
        //滤波
        MeanImage(ho_Image, &ho_ImageMean, 3, 3);
        //使用滤波后的图动态分割
        VarThreshold(ho_ImageMean, &ho_RegionDynThresh, hv_Width, hv_Height, 0.2, 20,
            "dark");
        ClosingCircle(ho_RegionDynThresh, &ho_RegionDynThresh, 3.5);
        FillUp(ho_RegionDynThresh, &ho_RegionDynThresh);
        //去除最周围的区域
        Connection(ho_RegionDynThresh, &ho_ConnectedRegions);
        SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "column", "and", hv_centerX_L,
            hv_centerX_H);
        //如果是边界上的缺陷，过滤掉长度过大的缺陷（大概率铜箔边）
        if (isEdge)
        {
          SelectShape(ho_SelectedRegions, &ho_SelectedRegions, "height", "and", 1,
              hv_Height*0.9);
        }
        Union1(ho_SelectedRegions, &ho_SelectedRegions);
        //得到选中区域的平均灰度，注意此图不是滤波后的图
        GrayFeatures(ho_SelectedRegions, ho_Image, "mean", &hv_meanValue);
        //得到选中区域中最大区域的尺寸
        Connection(ho_SelectedRegions, &ho_ConnectedRegions1);
        SelectShapeStd(ho_ConnectedRegions1, &ho_SelectedRegions1, "max_area", 70);
        //得到尺寸
        SmallestCircle(ho_SelectedRegions1, &hv_Row2, &hv_Column2, &hv_Radius2);
        hv_size = (hv_Radius2*2)*0.173;
        FlawSize=hv_size;

        //膨胀这个最大区域，在膨胀后的区域里继续动态分割
        DilationCircle(ho_SelectedRegions1, &ho_RegionDilation1, 6);

        ReduceDomain(ho_Image, ho_RegionDilation1, &ho_ImageDilation);
        GetImageSize(ho_ImageDilation, &hv_Width1, &hv_Height1);
        VarThreshold(ho_ImageDilation, &ho_RegionDynThresh1, hv_Width1, hv_Height1,
            0.2, 7, "dark");
        FillUp(ho_RegionDynThresh1, &ho_RegionDynThresh1);
        Connection(ho_RegionDynThresh1, &ho_RegionDynThresh1);
        SelectShapeStd(ho_RegionDynThresh1, &ho_SelectedRegions2, "max_area", 70);

        if (0 != (hv_size>setFlawSize1))
        {
          //得到浓区域和淡区域的内接圆尺寸
          InnerCircle(ho_SelectedRegions1, &hv_Row, &hv_Column, &hv_Radius);
          InnerCircle(ho_SelectedRegions2, &hv_Row1, &hv_Column1, &hv_Radius1);

          hv_diffSize = hv_Radius1-hv_Radius;
          //diffSize := Radius/(Radius1-Radius)

          //得到背景的平均灰度
          Complement(ho_SelectedRegions, &ho_RegionComplement);
          GrayFeatures(ho_RegionComplement, ho_Image, "mean", &hv_meanValue_background);

          //为了应对背景亮暗不同时，疵点对比度标准也动态调整，背景越亮对比度标准越高
          hv_diffMean = hv_meanValue_background-hv_meanValue;
          hv_diffMeanRatio = hv_meanValue_background/hv_diffMean;
          hv_diffresult = hv_diffMean-((hv_diffMeanRatio*hv_diffMeanRatio)/2);

          if (0 != (hv_diffresult>15))
          {
            if (0 != (HTuple(hv_diffresult<=25).TupleAnd(hv_diffSize>=5)))
            {
              hv_result = 0;
            }
            else if (0 != (HTuple(hv_diffresult<=20).TupleAnd(hv_diffSize>=4)))
            {
              hv_result = 0;
            }
            else
            {
              hv_result = 1;
            }
          }
          else
          {
            if (0 != (HTuple(hv_diffresult>10).TupleAnd(hv_diffSize<=2)))
            {
              hv_result = 1;
            }
            else
            {
              hv_result = 0;
            }
          }
        }
        else
        {
          hv_result = 0;
        }
    }
    catch (HalconCpp::HException &except)
    {
        QString message="Step1——:"+QString(except.ErrorText().Text());
        qDebug()<<message;
        hv_result = 0;
    }


    if (0 != (hv_result==0))
    {
        try
        {
            VarThreshold(ho_ImageMean, &ho_RegionDynThreshLight, hv_Width, hv_Height,
                         0.2, 18, "light");
            ClosingCircle(ho_RegionDynThreshLight, &ho_RegionDynThreshLight, 1.5);
            FillUp(ho_RegionDynThreshLight, &ho_RegionDynThreshLight);
            Connection(ho_RegionDynThreshLight, &ho_ConnectedRegionsLight);
            SelectShape(ho_ConnectedRegionsLight, &ho_SelectedRegionsLight, "column",
                        "and", hv_centerX_L, hv_centerX_H);
            SelectShapeStd(ho_SelectedRegionsLight, &ho_SelectedRegionsLight, "max_area",
                           70);
            AreaCenter(ho_SelectedRegionsLight, &hv_Area_Light, &hv_Row4, &hv_Column4);
            if (0 != (hv_Area_Light>10))
            {
                hv_result = 1;
            }
            else
            {
                hv_result = 0;
            }
        }
        catch (HalconCpp::HException &except)
        {
            QString message="Step2——:"+QString(except.ErrorText().Text());
            qDebug()<<message;
            hv_result = 0;
        }
    }
    return hv_result;
}



