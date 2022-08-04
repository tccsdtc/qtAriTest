/********************************************************************************
** Form generated from reading UI file 'showimagebigdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWIMAGEBIGDIALOG_H
#define UI_SHOWIMAGEBIGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ShowImageBigDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *vLayout;

    void setupUi(QDialog *ShowImageBigDialog)
    {
        if (ShowImageBigDialog->objectName().isEmpty())
            ShowImageBigDialog->setObjectName(QString::fromUtf8("ShowImageBigDialog"));
        ShowImageBigDialog->resize(1053, 863);
        horizontalLayout = new QHBoxLayout(ShowImageBigDialog);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        vLayout = new QVBoxLayout();
        vLayout->setObjectName(QString::fromUtf8("vLayout"));

        horizontalLayout->addLayout(vLayout);


        retranslateUi(ShowImageBigDialog);

        QMetaObject::connectSlotsByName(ShowImageBigDialog);
    } // setupUi

    void retranslateUi(QDialog *ShowImageBigDialog)
    {
        ShowImageBigDialog->setWindowTitle(QApplication::translate("ShowImageBigDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShowImageBigDialog: public Ui_ShowImageBigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWIMAGEBIGDIALOG_H
