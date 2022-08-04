#ifndef BASEITEM_H
#define BASEITEM_H

#include <QObject>
#include "ControlItem.h"
#include <QList>

//**************************************基类***************************************************
class BaseItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    enum ItemType
    {
        Circle = 0,         // 圆
        Ellipse,            // 椭圆
        Concentric_Circle,  // 同心圆
        Pie,                // 饼
        Chord,              // 和弦
        Rectangle,          // 矩形
        RectangleR,          // 矩形
        Square,             // 正方形
        Polygon,            // 多边形
        Round_End_Rectangle,// 圆端矩形
        Rounded_Rectangle   // 圆角矩形
    };

    virtual bool UpDate(int index)=NULL;
    static void SetScale(double value);
    QList<ControlItem* > ControlList;

protected:
    BaseItem(QPointF center,ItemType type);
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;
protected:
    QPointF center;
    ItemType types;
    QPen isSelected;
    QPen noSelected;
    QPen thisPen;
    static qreal LineWidth;//控制点初始尺寸
};
//**************************************矩形*******************************************************
class RectangleItem: public BaseItem
{
    Q_OBJECT
public:
    RectangleItem(qreal x, qreal y, qreal width, qreal height);

protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
};
//**************************************矩形 旋转*******************************************************
class RectangleRItem: public BaseItem
{
    Q_OBJECT
public:
    RectangleRItem(qreal x, qreal y, qreal Lenth1, qreal Lenth2,qreal Pi);

protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
protected:
    qreal angle=0;
    qreal lenth1=0;
    qreal lenth2=0;

    QPointF Pa1;
    QPointF Pa2;
    QPointF Pa3;
    QPointF Pa4;
};
//**************************************椭圆*******************************************************
class EllipseItem: public RectangleRItem
{
    Q_OBJECT
public:
    EllipseItem(qreal x, qreal y, qreal Lenth1, qreal Lenth2,qreal Pi);
protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
};
//**************************************圆*******************************************************
class CircleItem:public BaseItem
{
    Q_OBJECT
public:
    CircleItem(qreal x, qreal y, qreal R);
protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    qreal Radius;
};
//**************************************同心圆*********************************************************
class ConcentricCircleItem:public BaseItem
{
    Q_OBJECT
public:
    ConcentricCircleItem(qreal x, qreal y, qreal RadiusMin, qreal RadiusMax);
protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    qreal RadiusMin;
    qreal RadiusMax;
};
//**************************************饼图*********************************************************
class PieItem:public BaseItem
{
    Q_OBJECT
public:
    PieItem(qreal x, qreal y,qreal radius, qreal startAng, qreal endAng);
protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    qreal Radius;
    qreal StartAng;
    qreal EndAng;
};
//**************************************多边形*********************************************************
class PolygonItem:public BaseItem
{
    Q_OBJECT
public:
    PolygonItem();
    QPointF getCentroid(QList<QPointF> list);
    void getMaxLength();
public slots:
    void pushPoint(QPointF p, QList<QPointF> list, bool isCenter);
protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
protected:
    qreal Radius;
    bool Finished;
};
//**************************************圆端矩形*********************************************************
class RoundRectangle1Item:public BaseItem
{
    Q_OBJECT
public:
    RoundRectangle1Item(qreal x, qreal y, qreal width, qreal height);
protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
};
//**************************************圆角矩形*********************************************************
class RoundRectangle2Item:public BaseItem
{
    Q_OBJECT
public:
    RoundRectangle2Item(qreal x, qreal y, qreal width, qreal height,qreal radius);
protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    qreal Width;
    qreal Height;
    qreal Radius;
};








#endif // BASEITEM_H
