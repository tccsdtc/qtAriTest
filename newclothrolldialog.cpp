#include "newclothrolldialog.h"
#include "ui_newclothrolldialog.h"
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>
#include <QRegExpValidator>

NewClothRollDialog::NewClothRollDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewClothRollDialog)
{
    ui->setupUi(this);
    ui->clothRollNoLE->setValidator(new QRegExpValidator(QRegExp("[0-9]*-?[0-9]*")));
    ui->clothRollNoLE->installEventFilter(this);
    ui->machineNoLE->installEventFilter(this);
    ui->operaNameLE->installEventFilter(this);
//    QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd__hhmmss");
//    ui->clothRollNoLE->setText(timeStr);
}

NewClothRollDialog::~NewClothRollDialog()
{
    delete ui;
}

bool NewClothRollDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->clothRollNoLE && event->type()==QEvent::MouseButtonPress){
        CallSoftKey();
    }else if(watched==ui->machineNoLE && event->type()==QEvent::MouseButtonPress){
        CallSoftKey();
    }else if(watched==ui->operaNameLE && event->type()==QEvent::MouseButtonPress){
        CallSoftKey();
    }
    return QWidget::eventFilter(watched,event);
}

void NewClothRollDialog::CallSoftKey()
{
    PVOID OldValue = nullptr;
    BOOL bRet = Wow64DisableWow64FsRedirection(&OldValue);
    QString csProcess = "C:\\Windows\\System32\\osk.exe";
    QString params="";
    ShellExecute(nullptr, L"open", (LPCWSTR)csProcess.utf16(), (LPCWSTR)params.utf16(), nullptr, SW_SHOWNORMAL);
    if (bRet)
    {
        Wow64RevertWow64FsRedirection(OldValue);
    }
}

//确认
void NewClothRollDialog::on_pushButton_clicked()
{
    if(ui->clothRollNoLE->text().isEmpty())
    {
        QMessageBox::information(NULL,"Info","Cloth No is NULL");
        return;
    }
    emit Send(ui->clothRollNoLE->text(),ui->machineNoLE->text(),ui->operaNameLE->text());
    this->close();
}

//取消
void NewClothRollDialog::on_pushButton_2_clicked()
{

}
