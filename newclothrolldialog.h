#ifndef NEWCLOTHROLLDIALOG_H
#define NEWCLOTHROLLDIALOG_H

#include <QDialog>
#include "windows.h"

namespace Ui {
class NewClothRollDialog;
}

class NewClothRollDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewClothRollDialog(QWidget *parent = nullptr);
    ~NewClothRollDialog();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    void CallSoftKey();

signals:
    void Send(QString clothNo,QString robotNo,QString oprName);

public slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::NewClothRollDialog *ui;

    QString macNo;
};

#endif // NEWCLOTHROLLDIALOG_H
