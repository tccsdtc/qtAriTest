#include "datastorage.h"

DataStorage::DataStorage()
{
    xlsFile = NULL;
}

void DataStorage::InitReport(QString reportPath)
{
    xlsFile = new ExcelHelper();
    xlsFile->InitExcelFile(reportPath);
}

void DataStorage::InitReportHead(QList<QVariant> head)
{
    xlsFile->WriteLineData(head);
}

void DataStorage::WriteData(QList<QVariant> line)
{
    xlsFile->WriteLineData(line);
}

void DataStorage::SaveReport()
{
    xlsFile->Save();
}

void DataStorage::CloseReport()
{
    xlsFile->CloseExcelFile();
    xlsFile = NULL;
}
