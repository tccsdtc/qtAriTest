#include "ChartView.h"
#include <QApplication>
#include <QValueAxis>

ChartView::ChartView(QChart *chart, QWidget *parent)
    : QChartView(chart, parent)
{
	m_isPress = false;
	m_ctrlPress = false;
	m_alreadySaveRange = false;
	m_coordItem = nullptr;

//	this->setDragMode(QGraphicsView::RubberBandDrag);
//    this->setMouseTracking(false);
    setCursor(QCursor(Qt::PointingHandCursor)); //设置鼠标指针为手指形
}

ChartView::~ChartView()
{
}

//void ChartView::mousePressEvent(QMouseEvent *event)
//{
//    if (event->button() == Qt::LeftButton)
//    {
//        m_lastPoint = event->pos();
//        m_isPress = true;
//    }
//}

//void ChartView::mouseMoveEvent(QMouseEvent *event)
//{
//    if (!m_coordItem)
//    {
//        m_coordItem = new QGraphicsSimpleTextItem(this->chart());
//        m_coordItem->setZValue(5);
//        m_coordItem->setPos(100, 60);
//        m_coordItem->show();
//    }
//    const QPoint curPos = event->pos();
//    QPointF curVal = this->chart()->mapToValue(QPointF(curPos));
//    QString coordStr = QString("X = %1, Y = %2").arg(curVal.x()).arg(curVal.y());
//    m_coordItem->setText(coordStr);

//    if (m_isPress)
//    {
//        QPoint offset = curPos - m_lastPoint;
//        m_lastPoint = curPos;
//        if (!m_alreadySaveRange)
//        {
//            this->saveAxisRange();
//            m_alreadySaveRange = true;
//        }
//        this->chart()->scroll(-offset.x(), offset.y());
//    }
//}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPress = false;
    if (event->button() == Qt::RightButton)
    {
        if (m_alreadySaveRange)
        {
            this->chart()->axisX()->setRange(m_xMin, m_xMax);
            this->chart()->axisY()->setRange(m_yMin, m_yMax);
        }
    }
}

void ChartView::wheelEvent(QWheelEvent *event)
{
        const double factor = 1.5;//缩放比例

        if (!m_alreadySaveRange)
        {
            this->saveAxisRange();
            m_alreadySaveRange = true;
        }

        if (m_ctrlPress)
        {//Y轴
            const QPoint curPos = event->pos();
            QPointF curVal = this->chart()->mapToValue(QPointF(curPos));
            QValueAxis *axisY = dynamic_cast<QValueAxis*>(this->chart()->axisY());
            const double yMin = axisY->min();
            const double yMax = axisY->max();
            const double yCentral = curVal.y();
            double difference;
            double bottomOffset;
            double topOffset;
            if (event->delta() > 0)
            {//放大
                difference=(yMax-yMin);
                if(difference<=20)
                    return;

                bottomOffset = 1.0 / factor * (yCentral - yMin);
                topOffset = 1.0 / factor * (yMax - yCentral);
            }
            else
            {//缩小
                bottomOffset = 1.0 * factor * (yCentral - yMin);
                topOffset = 1.0 * factor * (yMax - yCentral);
            }
            this->chart()->axisY()->setRange(yCentral - bottomOffset, yCentral + topOffset);
        }
        else
        {
            //Y轴
            QValueAxis *axisY = dynamic_cast<QValueAxis*>(this->chart()->axisY());
            const double yMin = axisY->min();
            const double yMax = axisY->max();

            double difference;
            double bottomOffset;
            double topOffset;

            difference=(yMax-yMin)/2;
            if (event->delta() > 0)
            {//放大
                bottomOffset = yMin+difference;
                topOffset = yMax+difference;
            }
            else
            {//缩小
                if(yMin<=0)
                    return;
                bottomOffset = yMin-difference;
                topOffset = yMax-difference;
            }

            this->chart()->axisY()->setRange( bottomOffset,  topOffset);
        }
}

void ChartView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
    {
        m_ctrlPress = true;
    }
}

void ChartView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
    {
        m_ctrlPress = false;
    }
}

void ChartView::saveAxisRange()
{
    QValueAxis *axisX = dynamic_cast<QValueAxis*>(this->chart()->axisX());
    m_xMin = axisX->min();
    m_xMax = axisX->max();
    QValueAxis *axisY = dynamic_cast<QValueAxis*>(this->chart()->axisY());
    m_yMin = axisY->min();
    m_yMax = axisY->max();
}

