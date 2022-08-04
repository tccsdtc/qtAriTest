#include "QGraphicsScenes.h"
#include <QGraphicsSceneMouseEvent>

QGraphicsScenes::QGraphicsScenes(QObject *parent) : QGraphicsScene(parent)
{
    PolygonFlg = false;//多边形标志
}

//开始画多边形
void QGraphicsScenes::startCreate()
{
    PolygonFlg = true;
    Plist.clear();
}

//重写鼠标按下事件，为画多边形使用
void QGraphicsScenes::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (PolygonFlg)//在画多边形时重写鼠标左右键功能
    {
        QPointF p(event->scenePos().x(), event->scenePos().y());

        switch ( event->buttons() )
        {
        case Qt::LeftButton: {
            Plist.push_back(p);
            emit updatePoint(p, Plist, false);
        } break;
        case Qt::RightButton: {
            if (Plist.size() >= 3) {
                emit updatePoint(p, Plist, true);
                emit createFinished();
                PolygonFlg = false;
                Plist.clear();
            }
        } break;
        default: break;
        }
    } 
    else//如不是多边型则无需改写
    {
        QGraphicsScene::mousePressEvent(event);
    }
}
