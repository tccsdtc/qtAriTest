#include "mainwindow.h"

#include <QApplication>
#include <Dbghelp.h>
#include <QDir>

bool SingleOne()
{
//    QString strKey = "QT_GC_VI_Inspect";
//    LPCWSTR wstrKey = strKey.toStdWString().c_str();
//    HANDLE hMutex = CreateMutex(NULL,FALSE,wstrKey);
//    if(GetLastError() == ERROR_ALREADY_EXISTS)
//    {
//        //如果互斥体存在，说明程序已经有实例在运行了,释放资源然后关闭本实例
//        if(hMutex){
//            CloseHandle(hMutex);
//            hMutex = NULL;
//        }
//        return false;
//    }
    return true;
}

static LONG WINAPI exceptionCallback(struct _EXCEPTION_POINTERS* exceptionInfo)
{
    QString savePath = QCoreApplication::applicationDirPath() + "/dump/";
    QDir dir(savePath);
    if (!dir.exists() && !dir.mkpath(savePath)) {
        qApp->exit(E_UNEXPECTED);
        return EXCEPTION_EXECUTE_HANDLER;
    }

    savePath.append("assit_");
    savePath.append(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"));
    savePath.append(".dmp");

    HANDLE dump = CreateFileW(savePath.toStdWString().c_str(), GENERIC_WRITE,
        0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == dump) {
        qApp->exit(E_UNEXPECTED);
        return EXCEPTION_EXECUTE_HANDLER;
    }

    MINIDUMP_EXCEPTION_INFORMATION miniDumpExceptionInfo;
    miniDumpExceptionInfo.ExceptionPointers = exceptionInfo;
    miniDumpExceptionInfo.ThreadId = GetCurrentThreadId();
    miniDumpExceptionInfo.ClientPointers = TRUE;
    DWORD idProcess = GetCurrentProcessId();
    MiniDumpWriteDump(GetCurrentProcess(), idProcess, dump,
        MiniDumpNormal, &miniDumpExceptionInfo, NULL, NULL);

    CloseHandle(dump);

    QString strText = QString::number(exceptionInfo->ExceptionRecord->ExceptionCode,16);
    QMessageBox::critical(NULL,"error",strText);

    qApp->exit(E_UNEXPECTED);
    return EXCEPTION_EXECUTE_HANDLER;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!SingleOne())
        return -1;
    SetUnhandledExceptionFilter(exceptionCallback);
    MainWindow w;
    w.setWindowIcon(QIcon("\\resource\\image\\vi.ico"));
    w.show();
    return a.exec();
}
