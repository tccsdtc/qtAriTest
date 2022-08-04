#include "BaseItem.h"
#include <math.h>
#include <QtDebug>
#define PI 3.141592653

qreal BaseItem::LineWidth=2;//控制点初始尺寸
void BaseItem::SetScale(double value)
{
    LineWidth=2/value;
}
BaseItem::BaseItem(QPointF center,ItemType type):center(center),types(type)
{
    setHandlesChildEvents(false);//设置后才能将事件传递到子元素
    noSelected.setColor(QColor(0, 100, 200));
    noSelected.setWidth(LineWidth);
    isSelected.setColor(QColor(0, 180, 0));
    isSelected.setWidth(LineWidth);


    thisPen=noSelected;
    this->setFlags(QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsFocusable);
}
void BaseItem::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    thisPen=isSelected;
    for(int i=1;i<ControlList.length();i++)
    {
        ControlList[i]->setVisible(true);
    }
}
void BaseItem::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    thisPen=noSelected;
    for(int i=1;i<ControlList.length();i++)
    {
        ControlList[i]->setVisible(false);
    }
}



//**************************************矩形*******************************************************
RectangleItem::RectangleItem(qreal x, qreal y, qreal width, qreal height):BaseItem(QPointF(x,y),Rectangle)
{
    ControlList<<new ControlItem(this,center,0);
    ControlList<<new ControlItem(this,center+QPointF(-width/2,-height/2),1);
    ControlList<<new ControlItem(this,center+QPointF(width/2,-height/2),2);
    ControlList<<new ControlItem(this,center+QPointF(width/2,height/2),3);
    ControlList<<new ControlItem(this,center+QPointF(-width/2,height/2),4);
}
QRectF RectangleItem::boundingRect() const
{
    return  QRectF(ControlList[1]->GetPoint(),ControlList[3]->GetPoint());
}
bool RectangleItem::UpDate(int index)
{
    QPointF Pf=ControlList[index]->GetPoint();

    QPointF tmp=Pf-center;
    qreal x=qAbs(tmp.x());
    qreal y=qAbs(tmp.y());

    ControlList[1]->SetPoint(center+QPointF(-x,-y));
    ControlList[2]->SetPoint(center+QPointF(x,-y));
    ControlList[3]->SetPoint(center+QPointF(x,y));
    ControlList[4]->SetPoint(center+QPointF(-x,y));
    return true;
}
void RectangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    thisPen.setWidthF(LineWidth);
    painter->setPen(thisPen);

    QRectF ret(ControlList[1]->GetPoint(),ControlList[3]->GetPoint());
    painter->drawRect(ret);
}
//**************************************矩形 旋转*******************************************************
RectangleRItem::RectangleRItem(qreal x, qreal y, qreal Lenth1, qreal Lenth2, qreal Pi):BaseItem(QPointF(x,y),RectangleR)
{
    angle=Pi;
    lenth1=Lenth1/2;
    lenth2=Lenth2/2;
    Pa1=center+QPointF(0,lenth2);
    Pa2=center+QPointF(lenth1,0);
    Pa3=center+QPointF(0,-lenth2);
    Pa4=center+QPointF(-lenth1,0);
    //中心
    ControlList<<new ControlItem(this,center,0);
    //中心线控制点
    ControlList<<new ControlItem(this,Pa1,1);
    ControlList<<new ControlItem(this,Pa2,2);
    ControlList<<new ControlItem(this,Pa3,3);
    ControlList<<new ControlItem(this,Pa4,4);
}
QRectF RectangleRItem::boundingRect() const
{
    qreal tmp=(lenth1>lenth2?lenth2:lenth1);
    return QRectF(center.x()-tmp,center.y()-tmp,tmp*2,tmp*2);
}
bool RectangleRItem::UpDate(int index)
{
    QPointF Pf=ControlList[index]->GetPoint();

    qreal dx = Pf.x() - center.x();
    qreal dy = Pf.y() - center.y();
    if ( dx >= 0 && dy < 0 )
    {
        angle = atan2( (-1)*(dy), dx );
    }
    else if ( dx < 0 && dy < 0 )
    {
        angle = atan2( (-1)*dy, dx );
    }
    else if ( dx < 0 && dy >= 0 )
    {
        angle = PI*2 + atan2( (-1)*dy, dx );
    }
    else if ( dx >= 0 && dy >= 0 )
    {
        angle = PI*2 - atan2( dy, dx ) ;
    }

    //角度补偿 角度方向逆时针 控制点排列方向 顺时针
    switch (index)
    {
        case 1:
        angle+=PI/2;
            break;
        case 3:
        angle+=PI*3/2;
            break;
        case 4:
        angle+=PI;
            break;
    }

    qreal s = sin(-angle);
    qreal c = cos(-angle);
    if(index==2||index==4)
    {
       lenth1=sqrt(dx*dx+dy*dy);
    }else if (index==1||index==3)
    {
       lenth2=sqrt(dx*dx+dy*dy);
    }


    Pa1=center+QPointF( 0 * c - lenth2 * s,0 * s + lenth2 * c);
    Pa2=center+QPointF( lenth1 * c - 0 * s,lenth1 * s + 0 * c);
    Pa3=center+QPointF( 0 * c + lenth2 * s,0 * s - lenth2 * c);
    Pa4=center+QPointF( -lenth1 * c - 0 * s,-lenth1 * s + 0 * c);

    ControlList[1]->SetPoint(Pa1);
    ControlList[2]->SetPoint(Pa2);
    ControlList[3]->SetPoint(Pa3);
    ControlList[4]->SetPoint(Pa4);
    return true;
}
void RectangleRItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    thisPen.setWidthF(LineWidth);
    painter->setPen(thisPen);
//    qreal x = cos(angle)*lenth1;
//    qreal y = sin(angle)*lenth1;
//    //根据中心点求角点
//    QPointF P1=Pa1+QPointF(-x,y);
//    QPointF P2=Pa1+QPointF(x,-y);
//    QPointF P3=center-(P1-center);
//    QPointF P4=center-(P2-center);
//    //画边线
//    painter->drawLine(P1,P2);
//    painter->drawLine(P2,P3);
//    painter->drawLine(P3,P4);
//    painter->drawLine(P4,P1);
    //画角度方向线
    painter->drawLine(center,Pa2);

    painter->save();
    painter->translate(center);
    painter->rotate(-angle*180/PI);
    //painter-> drawEllipse(QRectF(-lenth1,-lenth2,lenth1*2,lenth2*2));
    painter->drawRect(QRectF(-lenth1,-lenth2,lenth1*2,lenth2*2));
    painter->restore();

}
//**************************************椭圆*******************************************************
EllipseItem::EllipseItem(qreal x, qreal y, qreal Lenth1, qreal Lenth2, qreal Pi):RectangleRItem(x,y,Lenth1,Lenth2,Pi)
{

}
void EllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    thisPen.setWidthF(LineWidth);
    painter->setPen(thisPen);
    painter->drawLine(center,Pa2);

    painter->save();
    painter->translate(center);
    painter->rotate(-angle*180/PI);
    painter-> drawEllipse(QRectF(-lenth1,-lenth2,lenth1*2,lenth2*2));
    painter->restore();
}
//**************************************圆*******************************************************
CircleItem::CircleItem(qreal x, qreal y, qreal R):BaseItem(QPointF(x,y),Circle)
{
    Radius=R;
    ControlList<<new ControlItem(this,center,0);
    ControlList<<new ControlItem(this,center+QPointF(R,0),1);
}
QRectF CircleItem::boundingRect() const
{
    return QRectF(center.x() - Radius, center.y() - Radius, Radius * 2, Radius * 2);
}
bool CircleItem::UpDate(int index)
{
    QPointF Pf=ControlList[index]->GetPoint();
    QPointF tmp=Pf-center;
    Radius=sqrt(tmp.x()*tmp.x()+tmp.y()*tmp.y());
    return true;
}
void CircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    thisPen.setWidthF(LineWidth);
    painter->setPen(thisPen);
    painter-> drawEllipse(QRectF(center.x()-Radius,center.y()-Radius,Radius*2,Radius*2));
}
//**************************************同心圆*********************************************************
ConcentricCircleItem::ConcentricCircleItem(qreal x, qreal y, qreal radiusMin, qreal radiusMax):BaseItem(QPointF(x,y),Concentric_Circle)
{
    RadiusMax=radiusMax;
    RadiusMin=radiusMin>radiusMax?radiusMax:radiusMin;
    ControlList<<new ControlItem(this,center,0);
    ControlList<<new ControlItem(this,center+QPointF(RadiusMin,0),1);
    ControlList<<new ControlItem(this,center+QPointF(RadiusMax,0),2);
}
QRectF ConcentricCircleItem::boundingRect() const
{
    return QRectF(center.x() - RadiusMax, center.y() - RadiusMax, RadiusMax * 2, RadiusMax * 2);
}
bool ConcentricCircleItem::UpDate(int index)
{
    QPointF Pf=ControlList[index]->GetPoint();
    QPointF tmp=Pf-center;
    qreal R=sqrt(tmp.x()*tmp.x()+tmp.y()*tmp.y());
    if(index==1)
    {
        if(R>RadiusMax)
        return false;
        RadiusMin=R;
    }else if(index==2)
    {
        if(R<RadiusMin)
        return false;
        RadiusMax=R;
    }
    return true;
}
void ConcentricCircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    thisPen.setWidthF(LineWidth);
    painter->setPen(thisPen);

    QPainterPath pth;
    pth.addEllipse(QRectF(center.x()-RadiusMin,center.y()-RadiusMin,RadiusMin*2,RadiusMin*2));
    pth.addEllipse(QRectF(center.x()-RadiusMax,center.y()-RadiusMax,RadiusMax*2,RadiusMax*2));
    painter->setBrush(QBrush(QColor(0, 160, 230,50)));
    painter->drawPath(pth);
}
//**************************************饼图********细节需要优化*************************************************
PieItem::PieItem(qreal x, qreal y, qreal radius, qreal startAng, qreal endAng):BaseItem(QPointF(x,y),Pie)
{
    Radius=radius;
    StartAng=startAng;
    EndAng=endAng; //endAng>StartAng?StartAng:
    qreal ss = sin(StartAng);
    qreal cs = cos(StartAng);
    qreal se = sin(EndAng);
    qreal ce = cos(EndAng);
    ControlList<<new ControlItem(this,center,0);
    ControlList<<new ControlItem(this,center+QPointF(Radius * cs,-Radius * ss),1);
    ControlList<<new ControlItem(this,center+QPointF(Radius * ce,-Radius * se),2);
}
QRectF PieItem::boundingRect() const
{
    return QRectF(center.x() - Radius, center.y() - Radius, Radius * 2, Radius * 2);
}
bool PieItem::UpDate(int index)
{
    QPointF Pf=ControlList[index]->GetPoint();

    qreal dx = Pf.x() - center.x();
    qreal dy = Pf.y() - center.y();
    Radius=sqrt(dx*dx+dy*dy);
    qreal atmp=0;
    if ( dx >= 0 && dy < 0 )
    {
        atmp = atan2( (-1)*(dy), dx );
    }
    else if ( dx < 0 && dy < 0 )
    {
        atmp = atan2( (-1)*dy, dx );
    }
    else if ( dx < 0 && dy >= 0 )
    {
        atmp = PI*2 + atan2( (-1)*dy, dx );
    }
    else if ( dx >= 0 && dy >= 0 )
    {
        atmp = PI*2 - atan2( dy, dx );
    }

    if(index==1)
    {
//        if(atmp<EndAng)
//            return false;
        StartAng=atmp;
    }else if(index==2)
    {
//        if(atmp>StartAng)
//            return false;
        EndAng=atmp;
    }

    qreal ss = sin(StartAng);
    qreal cs = cos(StartAng);
    qreal se = sin(EndAng);
    qreal ce = cos(EndAng);

    ControlList[1]->SetPoint(center+QPointF(Radius * cs,-Radius * ss));
    ControlList[2]->SetPoint(center+QPointF(Radius * ce,-Radius * se));
    return true;
}
void PieItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    thisPen.setWidthF(LineWidth);
    painter->setPen(thisPen);
    QRectF ret(center.x() - Radius, center.y() - Radius, Radius * 2, Radius * 2);
    painter->drawPie(ret, 16*EndAng*180/PI, 16*((StartAng-EndAng)*180/PI));
}
//**************************************多边形*********************************************************
PolygonItem::PolygonItem():BaseItem(QPointF(0,0),Polygon)
{
    ControlList<<new ControlItem(this,center,0);
    ControlList[0]->setVisible(false);
    Finished=false;
}
QPointF PolygonItem::getCentroid(QList<QPointF> list)
{
    qreal x = 0;
    qreal y = 0;
    for (auto &temp : list)
    {
        x += temp.x();
        y += temp.y();
    }
    x = x/list.size();
    y = y/list.size();
    return QPointF(x,y);
}
void PolygonItem::getMaxLength()
{
    QVector<qreal> vec;
    for (int i=1;i<ControlList.length();i++)
    {
        qreal dis = sqrt(pow(center.x() - ControlList[i]->x(), 2) + pow(center.y() - ControlList[i]->y(), 2));
        vec.append(dis);
    }

    qreal ret = 0;
    for (auto &temp : vec)
    {
        if (temp > ret) {
            ret = temp;
        }
    }
    Radius = ret;
}
void PolygonItem::pushPoint(QPointF p, QList<QPointF> list, bool isCenter)
{
    if (!Finished)
    {
        //this->setSelected(true);
        center = getCentroid(list);
        getMaxLength();
        if (isCenter)
        {
            ControlList[0]->SetPoint(center);
            ControlList[0]->setVisible(true);
            Finished=true;
        } else
        {
            auto tmp=new ControlItem(this,p,ControlList.length());
            tmp->setVisible(true);
            ControlList<<tmp;
        }
        this->update();
    }
}
QRectF PolygonItem::boundingRect() const
{
    return QRectF(center.x() - Radius, center.y() - Radius, Radius * 2, Radius * 2);
}
bool PolygonItem::UpDate(int index)
{
    QList<QPointF> list;
    for(int i=1;i<ControlList.length();i++)
    {
        list<<ControlList[i]->GetPoint();
    }
    center = getCentroid(list);
    ControlList[0]->SetPoint(center);
    return true;
}
void PolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    thisPen.setWidthF(LineWidth);
    painter->setPen(thisPen);

    if (Finished)
    {
        for (int i = 1; i < ControlList.length() - 1; i++)
        {
            painter->drawLine(ControlList[i]->GetPoint(), ControlList[i+1]->GetPoint());
        }
        painter->drawLine(ControlList[ControlList.length() - 1]->GetPoint(), ControlList[1]->GetPoint());
    } else
    {
        for (int i = 1; i < ControlList.length() - 1; i++)
        {
            painter->drawLine(ControlList[i]->GetPoint(), ControlList[i+1]->GetPoint());
        }
    }
}
//**************************************圆端矩形*********************************************************
RoundRectangle1Item::RoundRectangle1Item(qreal x, qreal y, qreal width, qreal height):BaseItem(QPointF(x,y),Round_End_Rectangle)
{
    ControlList<<new ControlItem(this,center,0);
    ControlList<<new ControlItem(this,center+QPointF(width/2,height/2),1);
}
QRectF RoundRectangle1Item::boundingRect() const
{
    auto m_edge=ControlList[1]->GetPoint();
    QRectF ret = QRectF(center.x() - m_edge.x() - m_edge.y(),center.y() - m_edge.y(),
                        abs(m_edge.x()) * 2 + abs(m_edge.y()) * 2,abs(m_edge.y()) * 2);
    if ( m_edge.x() >= 0 && m_edge.y() < 0 )
    {
        ret.moveTo(center.x() - m_edge.x() + m_edge.y(), center.y() + m_edge.y());
    }
    else if ( m_edge.x() < 0 && m_edge.y() < 0 )
    {
        ret.moveTo(center.x() + m_edge.x() + m_edge.y(), center.y() + m_edge.y());
    }
    else if ( m_edge.x() < 0 && m_edge.y() >= 0 )
    {
        ret.moveTo(center.x() + m_edge.x() - m_edge.y(), center.y() - m_edge.y());
    }
    else if ( m_edge.x() >= 0 && m_edge.y() >=0 )
    {
        ret.moveTo(center.x() - m_edge.x() - m_edge.y(), center.y() - m_edge.y());
    }

    return ret;
}
bool RoundRectangle1Item::UpDate(int index)
{
    return true;
}
void RoundRectangle1Item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    thisPen.setWidthF(LineWidth);
    painter->setPen(thisPen);

    QPointF left_top, left_bottom, right_top, right_bottom;

    auto m_edge=ControlList[1]->GetPoint();
    if ( m_edge.x() >= 0 && m_edge.y() < 0 )
    {
        left_top = QPointF(center.x() - m_edge.x() + m_edge.y(), m_edge.y());
        left_bottom = QPointF(center.x() - m_edge.x() + m_edge.y(), (-1) * m_edge.y());
        right_top = QPointF(center.x() + m_edge.x() - m_edge.y(), m_edge.y());
        right_bottom = QPointF(center.x() + m_edge.x() - m_edge.y(), (-1) * m_edge.y());
    }
    else if ( m_edge.x() < 0 && m_edge.y() < 0 )
    {
        left_top = QPointF(center.x() + m_edge.x() + m_edge.y(), m_edge.y());
        left_bottom = QPointF(center.x() + m_edge.x() + m_edge.y(), (-1) * m_edge.y());
        right_top = QPointF(center.x() - m_edge.x() - m_edge.y(), m_edge.y());
        right_bottom = QPointF(center.x() - m_edge.x() - m_edge.y(), (-1) * m_edge.y());
    }
    else if ( m_edge.x() < 0 && m_edge.y() >= 0 )
    {
        left_top = QPointF(center.x() + m_edge.x() - m_edge.y(), (-1) * m_edge.y());
        left_bottom = QPointF(center.x() + m_edge.x() - m_edge.y(), m_edge.y());
        right_top = QPointF(center.x() - m_edge.x() + m_edge.y(), (-1) * m_edge.y());
        right_bottom = QPointF(center.x() - m_edge.x() + m_edge.y(), m_edge.y());
    }
    else if ( m_edge.x() >= 0 && m_edge.y() >=0 )
    {
        left_top = QPointF(center.x() - m_edge.x() - m_edge.y(), (-1) * m_edge.y());
        left_bottom = QPointF(center.x() - m_edge.x() - m_edge.y(), m_edge.y());
        right_top = QPointF(center.x() + m_edge.x() + m_edge.y(), (-1) * m_edge.y());
        right_bottom = QPointF(center.x() + m_edge.x() + m_edge.y(), m_edge.y());
    }

    int radius = abs(m_edge.y());
    QPointF deltax(radius, 0);
    QPointF deltay(0, radius);

    painter->drawLine(left_top + deltax, right_top - deltax);
    painter->drawLine(left_bottom + deltax, right_bottom - deltax);
    painter->drawLine(left_top + deltay, left_bottom - deltay);
    painter->drawLine(right_top + deltay, right_bottom - deltay);

    painter->drawArc(QRectF(left_top, QSizeF(radius*2, radius*2)), -180 * 16, -90 * 16);
    painter->drawArc(QRectF(left_bottom-deltay*2, QSizeF(radius*2, radius*2)), -90 * 16, -90 * 16);
    painter->drawArc(QRectF(right_top-deltax*2, QSizeF(radius*2, radius*2)), 0 * 16, 90 * 16);
    painter->drawArc(QRectF(right_bottom-deltax*2-deltay*2, QSizeF(radius*2, radius*2)), 0 * 16, -90 * 16);
}
//**************************************圆角矩形*********************************************************
RoundRectangle2Item::RoundRectangle2Item(qreal x, qreal y, qreal width, qreal height, qreal radius):BaseItem(QPointF(x,y),Rounded_Rectangle)
{
    Width=width;
    Height=height;
    Radius=radius;
    ControlList<<new ControlItem(this,center,0);
    ControlList<<new ControlItem(this,center+QPointF(width/2,height/2),1);
    ControlList<<new ControlItem(this,center+QPointF(width/2,-height/2),2);
}
QRectF RoundRectangle2Item::boundingRect() const
{
    return QRectF(center-QPointF(Width/2,Height/2),center+QPointF(Width/2,Height/2));
}
bool RoundRectangle2Item::UpDate(int index)
{
    QPointF Pf=ControlList[1]->GetPoint();
    QPointF Pfc=ControlList[2]->GetPoint();

    if(index==1)
    {
        //设置1号控制点
        qreal tmpx=Pf.x() <= Radius?Radius:Pf.x();
        qreal tmpy=Pf.y() <= Radius?Radius:Pf.y();
        ControlList[1]->SetPoint(QPointF(tmpx,tmpy));

        //更新尺寸
        Width=qAbs(tmpx-center.x())*2;
        Height=qAbs(tmpy-center.y())*2;

        //控制点2位置被动更新 需要用旧的位置 懒的修改了 有时间再改
        qreal retX = 0;
        qreal retY = 0;
        if ( tmpx == Pfc.x() )
        {
            retX = tmpx;
            retY = (-1) * tmpy + Radius;
        } else {
            retX = tmpx - Radius;
            retY = (-1) * tmpy;
        }
        ControlList[2]->SetPoint(QPointF(retX,retY));
    }
    if(index==2)
    {
        //获取控制点2坐标
        Pfc=ControlList[2]->GetBefPoint();
        qreal dx=ControlList[2]->dX();
        qreal dy=ControlList[2]->dY();
        qreal retX = 0;
        qreal retY = 0;
        if ( Pf.x() == Pfc.x() &&(-1) * Pf.y() != Pfc.y() )
        {
            retX = Pfc.x();
            retY = Pfc.y() + dy;
        }
        else if ( Pf.x() != Pfc.x() &&(-1) * Pf.y() == Pfc.y() )
        {
            retX = Pfc.x() + dx;
            retY = Pfc.y();
        }
        else if ( Pf.x() == Pfc.x() &&(-1) * Pf.y() == Pfc.y() )
        {
            if ( abs(dx) >= abs(dy) )
            {
                retX = Pfc.x() + dx;
                retY = Pfc.y();
            } else {
                retX = Pfc.x();
                retY = Pfc.y() + dy;
            }
        }
        if ( retX > Pf.x() )
        {
            retX = Pf.x();
        } else if ( retX < 0 )
        {
            retX = 0;
        }
        if ( retY < (-1) * Pf.y() )
        {
            retY = (-1) * Pf.y();
        } else if ( retY > 0 )
        {
            retY = 0;
        }


        //设定控制点2
        ControlList[2]->SetPoint(QPointF(retX,retY));
        {
            //更新半径
            qreal dx = 0;
            qreal dy = 0;
            qreal absX = abs(Pf.x());
            qreal absY = abs(Pf.y());

            if ( Pfc.x() >= 0 && Pfc.y() < 0 )
            {
                dx = absX - Pfc.x();
                dy = absY + Pfc.y();
            }
            else if ( Pfc.x() < 0 && Pfc.y() < 0 )
            {
                dx = absX + Pfc.x();
                dy = absY + Pfc.y();
            }
            else if ( Pfc.x() < 0 && Pfc.y() >= 0 )
            {
                dx = absX + Pfc.x();
                dy = absY - Pfc.y();
            }
            else if ( Pfc.x() >= 0 && Pfc.y() >= 0 )
            {
                dx = absX - Pfc.x();
                dy = absY - Pfc.y();
            }
            Radius = dx >= dy ? dx : dy;
            Radius=Radius<0?0:Radius;
        }
    }

    return true;
}
void RoundRectangle2Item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    thisPen.setWidthF(LineWidth);
    painter->setPen(thisPen);

    QRectF ret(center-QPointF(Width/2,Height/2),center+QPointF(Width/2,Height/2));
    painter->drawRoundedRect(ret, Radius, Radius);
}
