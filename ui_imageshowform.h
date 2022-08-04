/********************************************************************************
** Form generated from reading UI file 'imageshowform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGESHOWFORM_H
#define UI_IMAGESHOWFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageShowForm
{
public:
    QGridLayout *gridLayout;
    QLabel *label;

    void setupUi(QWidget *ImageShowForm)
    {
        if (ImageShowForm->objectName().isEmpty())
            ImageShowForm->setObjectName(QString::fromUtf8("ImageShowForm"));
        ImageShowForm->resize(1110, 790);
        gridLayout = new QGridLayout(ImageShowForm);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(ImageShowForm);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);


        retranslateUi(ImageShowForm);

        QMetaObject::connectSlotsByName(ImageShowForm);
    } // setupUi

    void retranslateUi(QWidget *ImageShowForm)
    {
        ImageShowForm->setWindowTitle(QApplication::translate("ImageShowForm", "Form", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ImageShowForm: public Ui_ImageShowForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGESHOWFORM_H
