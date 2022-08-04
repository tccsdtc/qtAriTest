#include "hintdialog.h"
#include "ui_hintdialog.h"

HIntDialog::HIntDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HIntDialog)
{
    ui->setupUi(this);
}

HIntDialog::~HIntDialog()
{
    delete ui;
}

void HIntDialog::SetProgressBarValue(int value)
{
    ui->progressBar->setValue(value);
}
