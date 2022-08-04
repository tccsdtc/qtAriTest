#ifndef HISCLOTHDIALOG_H
#define HISCLOTHDIALOG_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "models.h"
#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class HisClothDialog;
}

class HisClothDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HisClothDialog(QWidget *parent = nullptr);
    ~HisClothDialog();

    void SetClothHistoryInfo(QList<ClothInfo> clothInfoList);

signals:
    void SendSelectHClothSignal(QString clothNo);
private:

    void Init(); //初始化
private slots:
    void on_pushButton_3_clicked();

    void on_SelectHistoryPB_clicked();

    void on_selectPB_clicked();

private:
    Ui::HisClothDialog *ui;

    QStandardItemModel *model;
    QList<ClothInfo> m_clothInfoList;
};

#endif // HISCLOTHDIALOG_H
