#ifndef DATASTORAGE_H
#define DATASTORAGE_H
#include "excelhelper.h"

#include <QAxObject>
#include <QList>

//目前采用xls存储方式
class DataStorage
{
public:
    DataStorage();

    void InitReport(QString reportPath);
    void InitReportHead(QList<QVariant> head);
    void WriteData(QList<QVariant> line);
    void SaveReport();
    void CloseReport();

private:
    ExcelHelper *xlsFile;
};

#endif // DATASTORAGE_H
