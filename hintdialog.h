#ifndef HINTDIALOG_H
#define HINTDIALOG_H

#include <QDialog>

namespace Ui {
class HIntDialog;
}

class HIntDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HIntDialog(QWidget *parent = nullptr);
    ~HIntDialog();

    void SetProgressBarValue(int value);
private:
    Ui::HIntDialog *ui;
};

#endif // HINTDIALOG_H
