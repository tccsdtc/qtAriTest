#include "excelhelper.h"

#include <QDir>

ExcelHelper::ExcelHelper()
{

}

bool ExcelHelper::InitExcelFile(QString fileName)
{
    HANDLE x ;
    m_fileName = fileName;
    rowNum = 1;

    //连接excel
    excel = new QAxObject("Excel.Application");
    //是否可视化excel
//    excel->dynamicCall("SetVisible(bool Visible)", true);
    //是否弹出警告窗口
//    excel->setProperty("DisplayAlerts", false);

    //获取工作簿集合
    workbooks = excel->querySubObject("WorkBooks");
    //新建一个工作簿
    workbooks->dynamicCall("Add");
    //获取当前工作簿
    workbook = excel->querySubObject("ActiveWorkBook");
    //获取工作表格集合
    worksheets = workbook->querySubObject("Sheets");
    //获取当前工作表格1，即sheet1
    worksheet  = worksheets->querySubObject("Item(int)", 1);

    return true;
}

bool ExcelHelper::WriteLineData(QList<QVariant> rowData)
{
    QString pos = "A"+QString::number(rowNum)+":G"+QString::number(rowNum);
    //选取范围
    range = worksheet->querySubObject("Range(const QString)", pos);
    //批量写入
    range->dynamicCall("SetValue(const QVariant&", QVariant(rowData));
    rowNum++;
    return true;
}

bool ExcelHelper::Save()
{
    //保存到文件
    workbook->dynamicCall("SaveAS(const QString&)", QDir::toNativeSeparators(m_fileName));
    return true;
}

bool ExcelHelper::CloseExcelFile()
{
//    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
    delete  excel;
    excel = NULL;
    return true;
}


