#include "circlequeue.h"



CircleQueue::CircleQueue(unsigned int size)
{
    m_front = 0;
    m_rear = 0;
    m_size = size + 1;
    m_data = new QList<Mat>[m_size + 1];
}

CircleQueue::   ~CircleQueue()
{
    delete[] m_data;
}

bool CircleQueue:: isEmpty()
{
    return m_front == m_rear;
}

bool CircleQueue::isFull()
{
    return m_front == (m_rear + 1) % m_size;
}

void CircleQueue::push(Mat data)
{
    if (isFull())
    {
        throw new exception("The queue is full，cannot keep pushing.");
    }
    m_data->insert(m_rear,data);
    m_rear = (m_rear + 1) % m_size;
}

void CircleQueue::pop()
{
    if (isEmpty())
    {
        throw new exception("The queue is empty，cannot keep poping.");
    }
    m_front = (m_front + 1) % m_size;
}

void CircleQueue::popall()
{
    if (isEmpty())
    {
        throw new exception("The queue is empty，cannot keep poping.");
    }
    while (m_front != m_rear)
        m_front = (m_front + 1) % m_size;
}

int CircleQueue::size()
{
    return (m_rear + m_size - m_front) % m_size;
}

Mat CircleQueue::getTop()
{
    if (isEmpty())
    {
        throw new exception("The queue is empty，cannot access the top element.");
    }
    Mat mat=m_data->at(m_front);
    m_front = (m_front + 1) % m_size;
    return mat;
}

cv::Mat CircleQueue::showTop()
{
    if (isEmpty())
    {
        throw new exception("The queue is empty，cannot access the top element.");
    }
    return m_data->at(m_front);    
}
