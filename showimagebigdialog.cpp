#include "showimagebigdialog.h"
#include "ui_showimagebigdialog.h"

ShowImageBigDialog::ShowImageBigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowImageBigDialog)
{
    ui->setupUi(this);

    view = new QGraphicsViews();
    ui->vLayout->addWidget(view);
}

ShowImageBigDialog::~ShowImageBigDialog()
{
    delete ui;
}

void ShowImageBigDialog::SetImage(QString path)
{

//    ui->label->setScaledContents(true);
//    ui->label->setPixmap(path);
//    ui->label->setMaximumSize(this->width(),this->height());
//    ui->label->setMinimumSize(imageSize);

//    ui->label->resize(this->width(),this->height());
//    ui->label->setAlignment(Qt::AlignCenter);
//    pic = QPixmap(path);
//    ui->label->setPixmap(pic.scaled(ui->label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    pic = QImage(path);
    view->DispImage(pic);
}


