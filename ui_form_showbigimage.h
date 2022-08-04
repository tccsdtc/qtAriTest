/********************************************************************************
** Form generated from reading UI file 'form_showbigimage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_SHOWBIGIMAGE_H
#define UI_FORM_SHOWBIGIMAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form_ShowBigImage
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *vLayout;

    void setupUi(QWidget *Form_ShowBigImage)
    {
        if (Form_ShowBigImage->objectName().isEmpty())
            Form_ShowBigImage->setObjectName(QString::fromUtf8("Form_ShowBigImage"));
        Form_ShowBigImage->resize(640, 480);
        horizontalLayout = new QHBoxLayout(Form_ShowBigImage);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        vLayout = new QVBoxLayout();
        vLayout->setObjectName(QString::fromUtf8("vLayout"));

        horizontalLayout->addLayout(vLayout);


        retranslateUi(Form_ShowBigImage);

        QMetaObject::connectSlotsByName(Form_ShowBigImage);
    } // setupUi

    void retranslateUi(QWidget *Form_ShowBigImage)
    {
        Form_ShowBigImage->setWindowTitle(QApplication::translate("Form_ShowBigImage", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form_ShowBigImage: public Ui_Form_ShowBigImage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_SHOWBIGIMAGE_H
