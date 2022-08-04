#ifndef HCONACTION_H
#define HCONACTION_H

#include <QObject>
#include"HalconCpp.h"
#include"Halcon.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgproc/types_c.h"
#include <opencv2/opencv.hpp>
#include"qdebug.h"

using namespace HalconCpp;
using namespace cv;

class hconAction : public QObject
{
    Q_OBJECT
public:
    explicit hconAction(QObject *parent = nullptr);

    Mat HImageToMat(const HalconCpp::HImage& hImg);
    Mat HObjectToMat(const HalconCpp::HObject& hObj);

    HalconCpp::HObject MatToHObject(const Mat &image);
    HalconCpp::HImage MatToHImage(const Mat &image);


    //setFlawSize是缺胶的标准尺寸
    int FilterAction1(const HalconCpp::HObject& himg, bool isEdge, const double& setFlawSize, const int& IQDflawType, double& FlawSize);

    //setFlawSize是黑点(异物)的标准尺寸
    int FilterAction2(const HalconCpp::HObject& himg, bool isEdge, const double& setFlawSize, const int& IQDflawType, double& FlawSize);

    int FilterAction3(const HalconCpp::HObject& himg, bool isEdge, const double& setFlawSize, const int& IQDflawType, double& FlawSize);

    //两个setFlawSize分别是缺胶点的标准尺寸和异物的标准尺寸
    int FilterAction5(const HalconCpp::HObject& himg, bool isEdge, const double& setFlawSize1,const double& setFlawSize2, const int& IQDflawType, double& FlawSize);

    //setFlawSize是黑点(异物)的标准尺寸
    int FilterAction6(const HalconCpp::HObject& himg, bool isEdge, const double& setFlawSize, const int& IQDflawType, double& FlawSize);


    int FilterAction7(const HalconCpp::HObject& himg, bool isEdge, const double& setFlawSize1, const int& IQDflawType, double& FlawSize);



private:
    HalconCpp::HImage img_halcon;
    Mat img_cv;

signals:

};

#endif // HCONACTION_H
