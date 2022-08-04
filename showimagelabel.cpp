#include "showimagelabel.h"

#include <QColor>

ShowImageLabel::ShowImageLabel(QWidget *parent): QLabel(parent)
{
    image = new QLabel(this);
    text = new QLabel(this);
    num = new QLabel(this);
    size = new QLabel(this);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255, 0, 0));
    this->setAutoFillBackground(true);  //一定要这句，否则不行
    this->setPalette(palette);
    palette.setColor(QPalette::Foreground, QColor(255, 255, 255));
    text->setPalette(palette);
    palette.setColor(QPalette::Foreground, Qt::red);
    num->setPalette(palette);
    size->setPalette(palette);
}

ShowImageLabel::~ShowImageLabel()
{
    image= NULL;
    text = NULL;
    num = NULL;
    size = NULL;
}

void ShowImageLabel::Init()
{

}

void ShowImageLabel::SetImagePath(QString filePath)
{
    m_filePath = filePath;
    //QImage image = QImage(m_filePath);
    //image = image.mirrored(true, false); //水平翻转
    //pic = QPixmap::fromImage(image);

    pic = QPixmap(m_filePath);
}

void ShowImageLabel::SetImageText(QString text)
{
    m_text = text;
}

void ShowImageLabel::SetImageSize(double size,int IQDTpye)
{
    m_size=size;
    m_IQDTpye=IQDTpye;
}

void ShowImageLabel::SetImageNum(int num)
{
    m_num = num;
}

void ShowImageLabel::SetImageType(int type)
{
    m_type = type;
    if(type == 0)
    {
        QPalette palette;
        palette.setColor(QPalette::Background, QColor(0, 0, 255));
        this->setAutoFillBackground(true);  //一定要这句，否则不行
        this->setPalette(palette);
        palette.setColor(QPalette::Foreground, QColor(255, 255, 255));
        text->setPalette(palette);
    }
}


void ShowImageLabel::ShowImage()
{
    image->setScaledContents(true);
    image->setPixmap(pic);
    image->setMaximumSize(this->width(),this->height()-20);
    image->setMinimumSize(this->width(),this->height()-20);
    image->show();
    text->move(0,this->height()-20);
    text->setMinimumSize(this->width(),0);
    text->setText(m_text);
    text->show();
    num->move(this->width()-30,0);
    num->setMinimumSize(30,0);
    num->setText(QString::number(m_num));
    num->show();
    size->move(3,0);
    size->setMinimumSize(80,0);
    size->setText(QString::number(m_size,'f',1)+"mm  T:"+QString::number(m_IQDTpye));
    size->show();
}

void ShowImageLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
//    Form_ShowBigImage showView(this);
//    showView.SetImage(m_filePath);
//    showView.exec();

    ShowImageBigDialog imageBig(this);
    imageBig.SetImage(m_filePath);
    imageBig.exec();
}
