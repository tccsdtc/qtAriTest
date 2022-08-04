#ifndef MODELS_H
#define MODELS_H

#include <QPoint>
#include <QString>

/********************************************************
*
*缺陷图片展示
*
********************************************************/
struct ImageShowStruct
{
    QString imagePath;
    QString camserialname;
    int x;
    int y;
    int num;
};

struct FlawPoint
{
    int type;
    QPoint point;
    double yData;
};

struct ClothInfo
{
    QString ClothNo;
    double Length;
    int FlawNum;
    QString StartTime;
    QString EndTime;
};

struct FlawRecord
{
    QString imgPath;
    QString camera;
    QString flawTime;
    int type;
    int IQDtype;
    int x;
    int y;
    double yData;
};

#endif // MODELS_H
