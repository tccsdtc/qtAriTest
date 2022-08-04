#ifndef EXCELHELPER_H
#define EXCELHELPER_H
#include "windows.h"
#include <QList>
#include <QString>
#include <QVariant>
#include <QFile>
#include <QDebug>
#include <QtCore/QCoreApplication>
#include <QAxObject>

class ExcelHelper
{
public:
    ExcelHelper();

    bool InitExcelFile(QString fileName);
    bool WriteLineData(QList<QVariant> rowData);
    bool WriteAllData();
    bool Save();
    bool CloseExcelFile();

private:
    int rowNum;
    QString m_fileName;
    QAxObject *excel;
    QAxObject *workbooks;
    QAxObject *workbook;
    QAxObject *worksheets;
    QAxObject *worksheet;
    QAxObject *range;
    QAxObject *cell;
    QList<QVariant> allRowData;

};

#endif // EXCELHELPER_H
