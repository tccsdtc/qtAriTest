#include "hisclothdialog.h"
#include "ui_hisclothdialog.h"

#include <QDir>
#include <QStandardItemModel>
#include <QDebug>
#include <QSettings>

HisClothDialog::HisClothDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HisClothDialog)
{
    ui->setupUi(this);
    Init();
}

HisClothDialog::~HisClothDialog()
{
    delete ui;
}

void HisClothDialog::SetClothHistoryInfo(QList<ClothInfo> clothInfoList)
{
    m_clothInfoList = clothInfoList;
    for(int row = 0; row < m_clothInfoList.length();row++)
    {
        model->setItem(row,0,new QStandardItem(m_clothInfoList[row].ClothNo));
        model->setItem(row,1,new QStandardItem("-"));
        model->setItem(row,2,new QStandardItem(QString::number(m_clothInfoList[row].Length)));
        model->setItem(row,3,new QStandardItem(QString::number(m_clothInfoList[row].FlawNum)));
        model->setItem(row,4,new QStandardItem(m_clothInfoList[row].StartTime));
        model->setItem(row,5,new QStandardItem(m_clothInfoList[row].EndTime));
    }
}

void HisClothDialog::Init()
{
    model = new QStandardItemModel();
    model->setColumnCount(6);
    model->setHeaderData(0,Qt::Horizontal,"布卷号");
    model->setHeaderData(1,Qt::Horizontal,"布卷类型");
    model->setHeaderData(2,Qt::Horizontal,"布卷长度");
    model->setHeaderData(3,Qt::Horizontal,"瑕疵数量");
    model->setHeaderData(4,Qt::Horizontal,"开始时间");
    model->setHeaderData(5,Qt::Horizontal,"结束时间");

    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(4,200);
    //设置选中时为整行选中
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QDateTime time = QDateTime::currentDateTime();

    ui->stratdateTimeEdit->setDateTime(time);
    ui->enddateTimeEdit->setDateTime(time);

}

void HisClothDialog::on_pushButton_3_clicked()
{
    this->close();
}

void HisClothDialog::on_SelectHistoryPB_clicked()
{
    this->hide();
    //    QStandardItem *seletItem = QTableView::selectionModel();
    int curRow = ui->tableView->currentIndex().row();
    QModelIndex indextemp = model->index(curRow,0);
    QString clothNo = model->data(indextemp).toString();
    emit SendSelectHClothSignal(clothNo);

//    this->hide();
}

void HisClothDialog::on_selectPB_clicked()
{
    if(ui->timeRB->isChecked())
    {
        QString startTime = ui->stratdateTimeEdit->dateTime().toString("yyyyMMddHHmmsszzz");
        QString endTime = ui->enddateTimeEdit->dateTime().toString("yyyyMMddHHmmsszzz");

        model->clear();
        model->setColumnCount(6);
        model->setHeaderData(0,Qt::Horizontal,"布卷号");
        model->setHeaderData(1,Qt::Horizontal,"布卷类型");
        model->setHeaderData(2,Qt::Horizontal,"布卷长度");
        model->setHeaderData(3,Qt::Horizontal,"瑕疵数量");
        model->setHeaderData(4,Qt::Horizontal,"开始时间");
        model->setHeaderData(5,Qt::Horizontal,"结束时间");

        ui->tableView->setColumnWidth(4,200);

        int index=0;
        for(int row = 0; row < m_clothInfoList.length();row++)
        {
            if(QDateTime::fromString(m_clothInfoList[row].StartTime,"yyyy/MM/dd HH:mm:ss zzz").toString("yyyyMMddHHmmsszzz").toDouble() > startTime.toDouble()
                    &&QDateTime::fromString(m_clothInfoList[row].EndTime,"yyyy/MM/dd HH:mm:ss zzz").toString("yyyyMMddHHmmsszzz").toDouble() < endTime.toDouble())
            {
                model->setItem(index,0,new QStandardItem(m_clothInfoList[row].ClothNo));
                model->setItem(index,1,new QStandardItem("-"));
                model->setItem(index,2,new QStandardItem(QString::number(m_clothInfoList[row].Length)));
                model->setItem(index,3,new QStandardItem(QString::number(m_clothInfoList[row].FlawNum)));
                model->setItem(index,4,new QStandardItem(m_clothInfoList[row].StartTime));
                model->setItem(index,5,new QStandardItem(m_clothInfoList[row].EndTime));
                index++;
            }
        }
    }
    else if(ui->clothNoRB->isChecked())
    {
        QString startTime = ui->stratdateTimeEdit->dateTime().toString("yyyyMMddHHmmsszzz");
        QString endTime = ui->enddateTimeEdit->dateTime().toString("yyyyMMddHHmmsszzz");

        model->clear();
        model->setColumnCount(6);
        model->setHeaderData(0,Qt::Horizontal,"布卷号");
        model->setHeaderData(1,Qt::Horizontal,"布卷类型");
        model->setHeaderData(2,Qt::Horizontal,"布卷长度");
        model->setHeaderData(3,Qt::Horizontal,"瑕疵数量");
        model->setHeaderData(4,Qt::Horizontal,"开始时间");
        model->setHeaderData(5,Qt::Horizontal,"结束时间");

        ui->tableView->setColumnWidth(4,200);

        int index=0;
        for(int row = 0; row < m_clothInfoList.length();row++)
        {
            if(m_clothInfoList[row].ClothNo == ui->lineEdit->text())
            {
                model->setItem(index,0,new QStandardItem(m_clothInfoList[row].ClothNo));
                model->setItem(index,1,new QStandardItem("-"));
                model->setItem(index,2,new QStandardItem(QString::number(m_clothInfoList[row].Length)));
                model->setItem(index,3,new QStandardItem(QString::number(m_clothInfoList[row].FlawNum)));
                model->setItem(index,4,new QStandardItem(m_clothInfoList[row].StartTime));
                model->setItem(index,5,new QStandardItem(m_clothInfoList[row].EndTime));
                index++;
            }
        }
    }
}
