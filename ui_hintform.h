/********************************************************************************
** Form generated from reading UI file 'hintform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HINTFORM_H
#define UI_HINTFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HintForm
{
public:
    QLabel *label;
    QProgressBar *progressBar;

    void setupUi(QWidget *HintForm)
    {
        if (HintForm->objectName().isEmpty())
            HintForm->setObjectName(QString::fromUtf8("HintForm"));
        HintForm->resize(251, 118);
        label = new QLabel(HintForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 30, 131, 16));
        progressBar = new QProgressBar(HintForm);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(70, 60, 118, 23));
        progressBar->setValue(24);

        retranslateUi(HintForm);

        QMetaObject::connectSlotsByName(HintForm);
    } // setupUi

    void retranslateUi(QWidget *HintForm)
    {
        HintForm->setWindowTitle(QApplication::translate("HintForm", "Form", nullptr));
        label->setText(QApplication::translate("HintForm", "\345\216\206\345\217\262\345\270\203\345\215\267\346\255\243\345\234\250\345\212\240\350\275\275\344\270\255\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HintForm: public Ui_HintForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HINTFORM_H
