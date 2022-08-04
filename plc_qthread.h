#ifndef PLC_QTHREAD_H
#define PLC_QTHREAD_H

#include <QThread>

class Plc_QThread : public QThread
{
    Q_OBJECT
public:
    explicit Plc_QThread(QThread *parent = nullptr);
    void InitPlc(QString comName);
protected:
    void run() override;
signals:

};

#endif // PLC_QTHREAD_H
