#ifndef CIRCLEQUEUE_H
#define CIRCLEQUEUE_H
#pragma execution_character_set("utf-8")

#include <QObject>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class CircleQueue : public QObject
{
    Q_OBJECT
public:
    CircleQueue(unsigned int size);
    ~CircleQueue();

    bool isEmpty();
    bool isFull();
    void push(Mat data);
    void pop();
    void popall();
    int size();
    Mat showTop(); //只显示 指针不后移
    Mat getTop();


public:
    unsigned int	        m_size;
    int				            m_front;
    int				            m_rear;
    QList<Mat>*	        m_data;
signals:

};

#endif // CIRCLEQUEUE_H
