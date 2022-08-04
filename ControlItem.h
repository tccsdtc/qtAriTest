#ifndef CONTROLITEM_H
#define CONTROLITEM_H

#include <QObject>
#include <QAbstractGraphicsShapeItem>
#include <QPointF>
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QCursor>
#include <QKeyEvent>
#include <QList>
#include <QDebug>


class ControlItem : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT
public:
    explicit ControlItem(QGraphicsItemGroup* parent, QPointF p, int type);
    QPointF GetPoint();
    QPointF GetBefPoint(){return BefPoint;}
    void SetPoint(QPointF p);
    qreal dX(){return dx;};
    qreal dY(){return dy;};

    static void SetScale(double value);
protected:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;


private:
    QPen myPen = this->pen();
    QPointF point;
    int pointType;
    static QRectF MQRect;//控制点矩形框
    static qreal ScaleValue;//控制点初始尺寸
    static qreal LineWidth;//控制点初始尺寸
    qreal dx;
    qreal dy;
    QPointF BefPoint;
};

#endif // CONTROLITEM_H
