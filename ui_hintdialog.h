/********************************************************************************
** Form generated from reading UI file 'hintdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HINTDIALOG_H
#define UI_HINTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>

QT_BEGIN_NAMESPACE

class Ui_HIntDialog
{
public:
    QLabel *label;
    QProgressBar *progressBar;

    void setupUi(QDialog *HIntDialog)
    {
        if (HIntDialog->objectName().isEmpty())
            HIntDialog->setObjectName(QString::fromUtf8("HIntDialog"));
        HIntDialog->resize(221, 117);
        label = new QLabel(HIntDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 30, 131, 16));
        progressBar = new QProgressBar(HIntDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(50, 60, 118, 23));
        progressBar->setValue(24);

        retranslateUi(HIntDialog);

        QMetaObject::connectSlotsByName(HIntDialog);
    } // setupUi

    void retranslateUi(QDialog *HIntDialog)
    {
        HIntDialog->setWindowTitle(QApplication::translate("HIntDialog", "Dialog", nullptr));
        label->setText(QApplication::translate("HIntDialog", "\345\216\206\345\217\262\345\270\203\345\215\267\350\277\233\345\272\246\345\212\240\350\275\275\344\270\255:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HIntDialog: public Ui_HIntDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HINTDIALOG_H
