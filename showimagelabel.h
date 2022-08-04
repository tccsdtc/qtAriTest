#ifndef SHOWIMAGELABEL_H
#define SHOWIMAGELABEL_H
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include "showimagebigdialog.h"

class ShowImageLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ShowImageLabel(QWidget *parent = nullptr);
    ~ShowImageLabel();
    void Init();
    void SetImagePath(QString filePath);
    void SetImageText(QString text);
    void SetImageSize(double size,int IQDTpye);
    void SetImageNum(int num);
    void SetImageType(int type);
    void ShowImage();
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
public:
    int m_type;
    QString m_filePath;
    QString m_text;
    int m_num;
    double m_size;
    int m_IQDTpye;
    QLabel *image;
    QLabel *text;
    QLabel *num;
    QLabel *size;
    QPixmap pic;
};

#endif // SHOWIMAGELABEL_H
