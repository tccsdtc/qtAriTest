/********************************************************************************
** Form generated from reading UI file 'newclothrolldialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWCLOTHROLLDIALOG_H
#define UI_NEWCLOTHROLLDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewClothRollDialog
{
public:
    QLabel *label_4;
    QWidget *widget;
    QLabel *label;
    QLineEdit *clothRollNoLE;
    QLabel *label_2;
    QLineEdit *machineNoLE;
    QLabel *label_3;
    QLineEdit *operaNameLE;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QDialog *NewClothRollDialog)
    {
        if (NewClothRollDialog->objectName().isEmpty())
            NewClothRollDialog->setObjectName(QString::fromUtf8("NewClothRollDialog"));
        NewClothRollDialog->resize(409, 337);
        label_4 = new QLabel(NewClothRollDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(150, 10, 81, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_4->setFont(font);
        widget = new QWidget(NewClothRollDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 60, 381, 201));
        widget->setMinimumSize(QSize(300, 0));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 20, 60, 16));
        clothRollNoLE = new QLineEdit(widget);
        clothRollNoLE->setObjectName(QString::fromUtf8("clothRollNoLE"));
        clothRollNoLE->setGeometry(QRect(130, 10, 221, 41));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(40, 80, 60, 16));
        machineNoLE = new QLineEdit(widget);
        machineNoLE->setObjectName(QString::fromUtf8("machineNoLE"));
        machineNoLE->setGeometry(QRect(130, 70, 221, 41));
        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 140, 60, 16));
        operaNameLE = new QLineEdit(widget);
        operaNameLE->setObjectName(QString::fromUtf8("operaNameLE"));
        operaNameLE->setGeometry(QRect(130, 130, 221, 41));
        pushButton_2 = new QPushButton(NewClothRollDialog);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(220, 270, 150, 50));
        pushButton_2->setMinimumSize(QSize(150, 50));
        pushButton = new QPushButton(NewClothRollDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(30, 270, 150, 50));
        pushButton->setMinimumSize(QSize(150, 50));

        retranslateUi(NewClothRollDialog);
        QObject::connect(pushButton_2, SIGNAL(clicked(bool)), NewClothRollDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(NewClothRollDialog);
    } // setupUi

    void retranslateUi(QDialog *NewClothRollDialog)
    {
        NewClothRollDialog->setWindowTitle(QApplication::translate("NewClothRollDialog", "\346\226\260\345\273\272\345\270\203\345\215\267\345\257\271\350\257\235\346\241\206", nullptr));
        label_4->setText(QApplication::translate("NewClothRollDialog", "\346\226\260\345\273\272\345\270\203\345\215\267", nullptr));
        label->setText(QApplication::translate("NewClothRollDialog", "\345\270\203\345\215\267\347\274\226\345\217\267\357\274\232", nullptr));
        label_2->setText(QApplication::translate("NewClothRollDialog", "\346\234\272\345\231\250\347\274\226\345\217\267\357\274\232", nullptr));
        label_3->setText(QApplication::translate("NewClothRollDialog", "\346\223\215\344\275\234\345\267\245\344\272\272\357\274\232", nullptr));
        pushButton_2->setText(QApplication::translate("NewClothRollDialog", "\345\217\226\346\266\210", nullptr));
        pushButton->setText(QApplication::translate("NewClothRollDialog", "\347\241\256\350\256\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewClothRollDialog: public Ui_NewClothRollDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWCLOTHROLLDIALOG_H
