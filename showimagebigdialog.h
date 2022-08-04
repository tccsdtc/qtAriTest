#ifndef SHOWIMAGEBIGDIALOG_H
#define SHOWIMAGEBIGDIALOG_H

#include <QDialog>
#include<QGraphicsViews.h>

namespace Ui {
class ShowImageBigDialog;
}

class ShowImageBigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowImageBigDialog(QWidget *parent = nullptr);
    ~ShowImageBigDialog();
    void SetImage(QString path);

private:
    Ui::ShowImageBigDialog *ui;
    QImage pic;

    QGraphicsViews* view;   //显示窗口
};

#endif // SHOWIMAGEBIGDIALOG_H
