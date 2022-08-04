#include "QGraphicsViews.h"
#include <QScrollBar>
#include <QDebug>
#include "ControlItem.h"
//#include "BaseItem.h"

QGraphicsViews::QGraphicsViews(QWidget *parent) : QGraphicsView(parent)
{
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);//解决拖动是背景图片残影
    //设置视图的拖拽模式:1、NoDrag（没有任何反应，鼠标事件将被忽略）,
    //2、ScrollHandDrag(光标变为指向手，然后拖动鼠标将滚动滚动条，此模式在交互和非交互模式下均有效)
    //3、RubberBandDrag(将出现矩形块，拖动鼠标将设置矩形的大小，并选中矩形覆盖的所有项目，非交互视图禁用此模式)
    setDragMode(QGraphicsView::ScrollHandDrag);
    drawBg();
    // 隐藏水平/竖直滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setBackgroundBrush(Qt::gray);//设置背景图像

    // 设置场景范围
    //setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);
    setSceneRect(-25000, -25000, 50000, 50000);

    // 反锯齿
    setRenderHints(QPainter::Antialiasing);

    //Qt绘图中，QGraphicsView的作用相当于画板，QGraphicsScene等效于画布，QGraphicsItem为画布上的画。
/*1.将Pixmap添加至QGraphicsPixmapItem；
2.将QGraphicsPixmapItem添加至QGraphicsScene；
3.将QGraphicsScene添加至QGraphicsView；*/
    item=new QGraphicsPixmapItem;
    scene=new QGraphicsScenes;
    scene->addItem(item);
    this->setScene(scene);
}

//显示图片
void QGraphicsViews::DispImage(QImage &Image)
{
    image=QPixmap::fromImage(Image);
    item->setPixmap(image);

    GetFit();
}

//设置缩放
void QGraphicsViews::SetToFit()
{
    ZoomFrame(ZoomFit); //ZoomFit默认值为1
    QScrollBar *pHbar =  this->horizontalScrollBar();
    pHbar->setSliderPosition(PixX);
    QScrollBar *pVbar = this->verticalScrollBar();
    pVbar->setSliderPosition(PixY);
}

//清除所有图元（ROI）
void QGraphicsViews::ClearObj()
{
    foreach(auto item,scene->items())
    {
        if(item->type()==10)
        {
           delete item;
        }
    }
}


void QGraphicsViews::ZoomFrame(double value)
{
    double tmp=value/ZoomValue;
    ZoomValue*=tmp;
    ControlItem::SetScale(ZoomValue);
  //  BaseItem::SetScale(ZoomValue);
    this->scale(tmp,tmp);//比例
}

void QGraphicsViews::GetFit()
{    
    if(this->width()<1||image.width()<1)
    {
        return;
    }
    //图片自适应方法
    double winWidth=this->width();
    double winHeight=this->height();
    double ScaleWidth=(image.width()+1)/winWidth;//图片宽/窗口宽
    double ScaleHeight=(image.height()+1)/winHeight;//图片长/窗口长
    double row1,column1;
    double s=0;
    if(ScaleWidth>=ScaleHeight)
    {

        row1= -(1) * ((winHeight * ScaleWidth) - image.height()) / 2;
        column1 = 0  ;
        s=1/ScaleWidth;
    }
    else
    {
        row1= 0;
        column1 = -(1.0) * ((winWidth * ScaleHeight) - image.width()) / 2 ;
        s=1/ScaleHeight;
    }

    if(ZoomFit!=s||PixX!=column1*s)
    {
        ZoomFit=s;
        PixX=column1*s;
        PixY=row1*s;
        SetToFit();
    }
}

//画背景图
void QGraphicsViews::drawBg()
{
    bgPix.fill(color1);
    QPainter painter(&bgPix);
    painter.fillRect(0, 0, 32, 32, color2);
    painter.fillRect(32, 32, 32, 32, color2);
    painter.end();
    //this->update();
}

void QGraphicsViews::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
}
void QGraphicsViews::resizeEvent(QResizeEvent *event)
{
    GetFit();
    QGraphicsView::resizeEvent(event);
}
void QGraphicsViews::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
}
void QGraphicsViews::mouseDoubleClickEvent(QMouseEvent *event)
{
    SetToFit();
    QGraphicsView::mouseDoubleClickEvent(event);
}
void QGraphicsViews::wheelEvent(QWheelEvent *event)//滚轮事件
{

    if((event->delta() > 0)&&(ZoomValue >= 50))//最大放大到原始图像的50倍
    {
        return;
    }
    else if((event->delta() < 0)&&(ZoomValue <= 0.01))//图像缩小到自适应大小之后就不继续缩小
    {
        return;
    }
    else
    {
        double tmp=ZoomValue;
        if(event->delta() > 0)//鼠标滚轮向前滚动
        {
            tmp*=1.1;//每次放大10%，这个*是乘法
        }
        else
        {
            tmp*=0.9;//每次缩小10%，这个*是乘法
        }
        ZoomFrame(tmp);
        if(event->delta() > 0)
        {
           // moveBy(-event->pos().x()*qrealOriginScale*0.1, -event->pos().y()*qrealOriginScale*0.1);//使图片缩放的效果看起来像是以鼠标所在点为中心进行缩放的
        }
        else
        {
            //moveBy(event->pos().x()*qrealOriginScale*0.1, event->pos().y()*qrealOriginScale*0.1);//使图片缩放的效果看起来像是以鼠标所在点为中心进行缩放的
        }
    }

}

void QGraphicsViews::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
//    painter->drawTiledPixmap(rect, bgPix);//绘制背景曾
}



