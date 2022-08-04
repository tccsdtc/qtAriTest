/********************************************************************************
** Form generated from reading UI file 'hisclothdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HISCLOTHDIALOG_H
#define UI_HISCLOTHDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HisClothDialog
{
public:
    QGridLayout *gridLayout;
    QWidget *widget_3;
    QGridLayout *gridLayout_2;
    QWidget *widget_4;
    QGridLayout *gridLayout_3;
    QLabel *label;
    QDateTimeEdit *stratdateTimeEdit;
    QLabel *label_2;
    QDateTimeEdit *enddateTimeEdit;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QRadioButton *timeRB;
    QRadioButton *clothNoRB;
    QPushButton *selectPB;
    QTableView *tableView;
    QWidget *widget;
    QWidget *widget_2;
    QPushButton *SelectHistoryPB;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QDialog *HisClothDialog)
    {
        if (HisClothDialog->objectName().isEmpty())
            HisClothDialog->setObjectName(QString::fromUtf8("HisClothDialog"));
        HisClothDialog->resize(1047, 598);
        gridLayout = new QGridLayout(HisClothDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        widget_3 = new QWidget(HisClothDialog);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        gridLayout_2 = new QGridLayout(widget_3);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        widget_4 = new QWidget(widget_3);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        gridLayout_3 = new QGridLayout(widget_4);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label = new QLabel(widget_4);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        stratdateTimeEdit = new QDateTimeEdit(widget_4);
        stratdateTimeEdit->setObjectName(QString::fromUtf8("stratdateTimeEdit"));

        gridLayout_3->addWidget(stratdateTimeEdit, 0, 1, 1, 1);

        label_2 = new QLabel(widget_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 0, 2, 1, 1);

        enddateTimeEdit = new QDateTimeEdit(widget_4);
        enddateTimeEdit->setObjectName(QString::fromUtf8("enddateTimeEdit"));

        gridLayout_3->addWidget(enddateTimeEdit, 0, 3, 1, 1);

        label_3 = new QLabel(widget_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_3->addWidget(label_3, 0, 4, 1, 1);

        lineEdit = new QLineEdit(widget_4);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout_3->addWidget(lineEdit, 0, 5, 1, 1);

        timeRB = new QRadioButton(widget_4);
        timeRB->setObjectName(QString::fromUtf8("timeRB"));
        timeRB->setChecked(true);

        gridLayout_3->addWidget(timeRB, 0, 6, 1, 1);

        clothNoRB = new QRadioButton(widget_4);
        clothNoRB->setObjectName(QString::fromUtf8("clothNoRB"));

        gridLayout_3->addWidget(clothNoRB, 0, 7, 1, 1);

        selectPB = new QPushButton(widget_4);
        selectPB->setObjectName(QString::fromUtf8("selectPB"));

        gridLayout_3->addWidget(selectPB, 0, 8, 1, 1);


        gridLayout_2->addWidget(widget_4, 0, 0, 1, 1);

        tableView = new QTableView(widget_3);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->horizontalHeader()->setStretchLastSection(true);

        gridLayout_2->addWidget(tableView, 1, 0, 1, 1);


        gridLayout->addWidget(widget_3, 0, 0, 1, 1);

        widget = new QWidget(HisClothDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(200, 0));
        widget->setMaximumSize(QSize(200, 16777215));
        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(10, 10, 181, 241));
        SelectHistoryPB = new QPushButton(widget_2);
        SelectHistoryPB->setObjectName(QString::fromUtf8("SelectHistoryPB"));
        SelectHistoryPB->setGeometry(QRect(10, 10, 161, 40));
        SelectHistoryPB->setMinimumSize(QSize(0, 40));
        SelectHistoryPB->setMaximumSize(QSize(16777215, 40));
        pushButton_2 = new QPushButton(widget_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setEnabled(false);
        pushButton_2->setGeometry(QRect(10, 60, 161, 40));
        pushButton_2->setMinimumSize(QSize(0, 40));
        pushButton_2->setMaximumSize(QSize(16777215, 40));
        pushButton_3 = new QPushButton(widget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(20, 530, 161, 40));
        pushButton_3->setMinimumSize(QSize(0, 40));
        pushButton_3->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(widget, 0, 1, 1, 1);


        retranslateUi(HisClothDialog);

        QMetaObject::connectSlotsByName(HisClothDialog);
    } // setupUi

    void retranslateUi(QDialog *HisClothDialog)
    {
        HisClothDialog->setWindowTitle(QApplication::translate("HisClothDialog", "Dialog", nullptr));
        label->setText(QApplication::translate("HisClothDialog", "\345\274\200\345\247\213\346\227\266\351\227\264\357\274\232", nullptr));
        label_2->setText(QApplication::translate("HisClothDialog", "\347\273\223\346\235\237\346\227\266\351\227\264", nullptr));
        label_3->setText(QApplication::translate("HisClothDialog", "\345\270\203\345\215\267\345\217\267\357\274\232", nullptr));
        timeRB->setText(QApplication::translate("HisClothDialog", "\346\214\211\346\227\266\351\227\264\346\237\245\350\257\242", nullptr));
        clothNoRB->setText(QApplication::translate("HisClothDialog", "\346\214\211\345\270\203\345\215\267\345\217\267\346\237\245\350\257\242", nullptr));
        selectPB->setText(QApplication::translate("HisClothDialog", "\346\237\245\350\257\242", nullptr));
        SelectHistoryPB->setText(QApplication::translate("HisClothDialog", "\351\200\211\346\213\251\345\270\203\345\215\267", nullptr));
        pushButton_2->setText(QApplication::translate("HisClothDialog", "\345\210\240\351\231\244\345\270\203\345\215\267", nullptr));
        pushButton_3->setText(QApplication::translate("HisClothDialog", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HisClothDialog: public Ui_HisClothDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISCLOTHDIALOG_H
