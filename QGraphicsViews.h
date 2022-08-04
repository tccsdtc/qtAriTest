#ifndef QGraphicsViews_H
#define QGraphicsViews_H

#include <QGraphicsView>
#include <QMouseEvent>
#include "QGraphicsScenes.h"

class QGraphicsViews : public QGraphicsView//继承于QGraphicsView：一种基于图形项（Graphics Item）的模型/视图模式，这种方式可以在一个场景中绘制大量图元项，且每个图元项都是可选择、可交互的。
{
public:
    QGraphicsViews(QWidget *parent = nullptr);

    void DispImage(QImage& Image);
    void SetToFit();
    void ClearObj();
    QGraphicsScenes* scene;

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;
private:
    void ZoomFrame(double value);
    void GetFit();
    double ZoomValue=1;
    double ZoomFit=1;
    double PixX=0;
    double PixY=0;
    QPixmap image;
    QGraphicsPixmapItem* item;
private:
    int MRSize=8;//拖拽区域大小
    QColor color1=QColor(255, 255, 255);        //颜色1
    QColor color2=QColor(100, 100, 100, 150);    //颜色2
    QPixmap bgPix=QPixmap(64, 64);              //背景图片
    void drawBg();
};

#endif // QGraphicsViews_H
