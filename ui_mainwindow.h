/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QWidget *mapWidget;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *cBox_updataLimit;
    QVBoxLayout *rl;
    QWidget *showWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QWidget *menuWidget;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_5;
    QLabel *label;
    QLabel *label_2;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_6;
    QLineEdit *lineEdit_path;
    QPushButton *NewClothRollPB;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *checkBox_3;
    QLabel *label_5;
    QLineEdit *lineEdit;
    QPushButton *startVisionPB;
    QPushButton *closeVisionPB;
    QPushButton *exportPB;
    QPushButton *clothHistoryPB;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_2;
    QLabel *label_4;
    QLabel *label_3;
    QPushButton *pushButton;
    QLineEdit *meterExlineEdit;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_3;
    QSpacerItem *horizontalSpacer_4;
    QLabel *clothLengthLB;
    QSpacerItem *horizontalSpacer_2;
    QLabel *clothNoLB;
    QLabel *label_15;
    QLabel *label_12;
    QLabel *clothNGNumberLB;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_6;
    QLabel *totalLabel_2;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *totalLabel_3;
    QComboBox *comboBox_1;
    QHBoxLayout *horizontalLayout_5;
    QLabel *totalLabel_6;
    QComboBox *comboBox_4;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *btn_Unlock;
    QPushButton *btn_Lock;
    QLineEdit *lineEdit_password;
    QSpacerItem *verticalSpacer_3;
    QPushButton *exitPB;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1813, 987);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"/*\n"
"Ubuntu Style Sheet for QT Applications\n"
"Author: Jaime A. Quiroga P.\n"
"Company: GTRONICK\n"
"Last updated: 01/10/2021 (dd/mm/yyyy), 15:18.\n"
"Available at: https://github.com/GTRONICK/QSS/blob/master/Ubuntu.qss\n"
"*/\n"
"QMainWindow {\n"
"	background-color:#f0f0f0;\n"
"}\n"
"QCheckBox {\n"
"	padding:2px;\n"
"}\n"
"QCheckBox:hover {\n"
"	border:1px solid rgb(255,150,60);\n"
"	border-radius:4px;\n"
"	padding: 1px;\n"
"	background-color:qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(190, 90, 50, 50), stop:1 rgba(250, 130, 40, 50));\n"
"}\n"
"QCheckBox::indicator:checked {\n"
"	border:1px solid rgb(246, 134, 86);\n"
"	border-radius:4px;\n"
"  	background-color:rgb(246, 134, 86)\n"
"}\n"
"QCheckBox::indicator:unchecked {\n"
"	border-width:1px solid rgb(246, 134, 86);\n"
"	border-radius:4px;\n"
"  	background-color:rgb(255,255,255);\n"
"}\n"
"QColorDialog {\n"
"	background-color:#f0f0f0;\n"
"}\n"
"QComboBox {\n"
"	color:rgb(81,72,65);\n"
"	background: #ffffff;\n"
"}\n"
"QComboB"
                        "ox:editable {\n"
"	selection-color:rgb(81,72,65);\n"
"	selection-background-color: #ffffff;\n"
"}\n"
"QComboBox QAbstractItemView {\n"
"	selection-color: #ffffff;\n"
"	selection-background-color: rgb(246, 134, 86);\n"
"}\n"
"QComboBox:!editable:on, QComboBox::drop-down:editable:on {\n"
"	color:  #1e1d23;	\n"
"}\n"
"QDateTimeEdit, QDateEdit, QDoubleSpinBox, QFontComboBox {\n"
"	color:rgb(81,72,65);\n"
"	background-color: #ffffff;\n"
"}\n"
"\n"
"QDialog {\n"
"	background-color:#f0f0f0;\n"
"}\n"
"\n"
"QLineEdit {\n"
"	color:rgb(17,17,17);\n"
"}\n"
"QLineEdit {\n"
"	background-color:rgb(255,255,255);\n"
"	selection-background-color:rgb(236,116,64);\n"
"}\n"
"QMenuBar {\n"
"	color:rgb(223,219,210);\n"
"	background-color:rgb(65,64,59);\n"
"}\n"
"QMenuBar::item {\n"
"	padding-top:4px;\n"
"	padding-left:4px;\n"
"	padding-right:4px;\n"
"	color:rgb(223,219,210);\n"
"	background-color:rgb(65,64,59);\n"
"}\n"
"QMenuBar::item:selected {\n"
"	color:rgb(255,255,255);\n"
"	padding-top:2px;\n"
"	padding-left:2px;\n"
"	padding-"
                        "right:2px;\n"
"	border-top-width:2px;\n"
"	border-left-width:2px;\n"
"	border-right-width:2px;\n"
"	border-top-right-radius:4px;\n"
"	border-top-left-radius:4px;\n"
"	border-style:solid;\n"
"	background-color:rgb(65,64,59);\n"
"	border-top-color: rgb(47,47,44);\n"
"	border-right-color: qlineargradient(spread:pad, x1:0, y1:1, x2:1, y2:0, stop:0 rgba(90, 87, 78, 255), stop:1 rgba(47,47,44, 255));\n"
"	border-left-color:  qlineargradient(spread:pad, x1:1, y1:0, x2:0, y2:0, stop:0 rgba(90, 87, 78, 255), stop:1 rgba(47,47,44, 255));\n"
"}\n"
"QMenu {\n"
"	color:rgb(223,219,210);\n"
"	background-color:rgb(65,64,59);\n"
"}\n"
"QMenu::item {\n"
"	color:rgb(223,219,210);\n"
"	padding:4px 10px 4px 20px;\n"
"}\n"
"QMenu::item:selected {\n"
"	color:rgb(255,255,255);\n"
"	background-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(225, 108, 54, 255), stop:1 rgba(246, 134, 86, 255));\n"
"	border-style:solid;\n"
"	border-width:3px;\n"
"	padding:4px 7px 4px 17px;\n"
"	border-bottom-color:qlineargradi"
                        "ent(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(175,85,48,255), stop:1 rgba(236,114,67, 255));\n"
"	border-top-color:qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(253,156,113,255), stop:1 rgba(205,90,46, 255));\n"
"	border-right-color:qlineargradient(spread:pad, x1:0, y1:0.5, x2:1, y2:0.5, stop:0 rgba(253,156,113,255), stop:1 rgba(205,90,46, 255));\n"
"	border-left-color:qlineargradient(spread:pad, x1:1, y1:0.5, x2:0, y2:0.5, stop:0 rgba(253,156,113,255), stop:1 rgba(205,90,46, 255));\n"
"}\n"
"QPlainTextEdit {\n"
"	border: 1px solid transparent;\n"
"	color:rgb(17,17,17);\n"
"	selection-background-color:rgb(236,116,64);\n"
"    background-color: #FFFFFF;\n"
"}\n"
"QProgressBar {\n"
"	text-align: center;\n"
"	color: rgb(0, 0, 0);\n"
"	border: 1px inset rgb(150,150,150); \n"
"	border-radius: 10px;\n"
"	background-color:rgb(221,221,219);\n"
"}\n"
"QProgressBar::chunk:horizontal {\n"
"	background-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(225, 108, 54, "
                        "255), stop:1 rgba(246, 134, 86, 255));\n"
"	border:1px solid;\n"
"	border-radius:8px;\n"
"	border-bottom-color:qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(175,85,48,255), stop:1 rgba(236,114,67, 255));\n"
"	border-top-color:qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(253,156,113,255), stop:1 rgba(205,90,46, 255));\n"
"	border-right-color:qlineargradient(spread:pad, x1:0, y1:0.5, x2:1, y2:0.5, stop:0 rgba(253,156,113,255), stop:1 rgba(205,90,46, 255));\n"
"	border-left-color:qlineargradient(spread:pad, x1:1, y1:0.5, x2:0, y2:0.5, stop:0 rgba(253,156,113,255), stop:1 rgba(205,90,46, 255));\n"
"}\n"
"QPushButton{\n"
"	color:rgb(17,17,17);\n"
"	border-width: 1px;\n"
"	border-radius: 6px;\n"
"	border-bottom-color: rgb(150,150,150);\n"
"	border-right-color: rgb(165,165,165);\n"
"	border-left-color: rgb(165,165,165);\n"
"	border-top-color: rgb(180,180,180);\n"
"	border-style: solid;\n"
"	padding: 4px;\n"
"	background-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0"
                        ".5, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(255, 255, 255, 255));\n"
"}\n"
"QPushButton:hover{\n"
"	color:rgb(255,150,60);\n"
"	border-width: 1px;\n"
"	border-radius:6px;\n"
"	border-top-color: rgb(255,150,60);\n"
"	border-right-color: qlineargradient(spread:pad, x1:0, y1:1, x2:1, y2:0, stop:0 rgba(200, 70, 20, 255), stop:1 rgba(255,150,60, 255));\n"
"	border-left-color:  qlineargradient(spread:pad, x1:1, y1:0, x2:0, y2:0, stop:0 rgba(200, 70, 20, 255), stop:1 rgba(255,150,60, 255));\n"
"	border-bottom-color: rgb(200,70,20);\n"
"	border-style: solid;\n"
"	padding: 2px;\n"
"	background-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(255, 255, 255, 255));\n"
"}\n"
"QPushButton:default{\n"
"	color:rgb(17,17,17);\n"
"	border-width: 1px;\n"
"	border-radius:6px;\n"
"	border-top-color: rgb(255,150,60);\n"
"	border-right-color: qlineargradient(spread:pad, x1:0, y1:1, x2:1, y2:0, stop:0 rgba(200, 70, 20, 255), stop:1 rgba(255,150,60, 255));\n"
"	bor"
                        "der-left-color:  qlineargradient(spread:pad, x1:1, y1:0, x2:0, y2:0, stop:0 rgba(200, 70, 20, 255), stop:1 rgba(255,150,60, 255));\n"
"	border-bottom-color: rgb(200,70,20);\n"
"	border-style: solid;\n"
"	padding: 2px;\n"
"	background-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(255, 255, 255, 255));\n"
"}\n"
"QPushButton:pressed{\n"
"	color:rgb(17,17,17);\n"
"	border-width: 1px;\n"
"	border-radius: 6px;\n"
"	border-width: 1px;\n"
"	border-top-color: rgba(255,150,60,200);\n"
"	border-right-color: qlineargradient(spread:pad, x1:0, y1:1, x2:1, y2:0, stop:0 rgba(200, 70, 20, 255), stop:1 rgba(255,150,60, 200));\n"
"	border-left-color:  qlineargradient(spread:pad, x1:1, y1:0, x2:0, y2:0, stop:0 rgba(200, 70, 20, 255), stop:1 rgba(255,150,60, 200));\n"
"	border-bottom-color: rgba(200,70,20,200);\n"
"	border-style: solid;\n"
"	padding: 2px;\n"
"	background-color: qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:1, stop:0 rgba(220, 220, 220, 255), stop"
                        ":1 rgba(255, 255, 255, 255));\n"
"}\n"
"QPushButton:disabled{\n"
"	color:rgb(174,167,159);\n"
"	border-width: 1px;\n"
"	border-radius: 6px;\n"
"	background-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(200, 200, 200, 255), stop:1 rgba(230, 230, 230, 255));\n"
"}\n"
"QRadioButton {\n"
"	padding: 1px;\n"
"}\n"
"QRadioButton::indicator:checked {\n"
"	height: 10px;\n"
"	width: 10px;\n"
"	border-style:solid;\n"
"	border-radius:5px;\n"
"	border-width: 1px;\n"
"	border-color: rgba(246, 134, 86, 255);\n"
"	color: #a9b7c6;\n"
"	background-color:rgba(246, 134, 86, 255);\n"
"}\n"
"QRadioButton::indicator:!checked {\n"
"	height: 10px;\n"
"	width: 10px;\n"
"	border-style:solid;\n"
"	border-radius:5px;\n"
"	border-width: 1px;\n"
"	border-color: rgb(246, 134, 86);\n"
"	color: #a9b7c6;\n"
"	background-color: transparent;\n"
"}\n"
"QScrollArea {\n"
"	color: white;\n"
"	background-color:#f0f0f0;\n"
"}\n"
"QSlider::groove {\n"
"	border-style: solid;\n"
"	border-width: 1px;\n"
"	border-color: rgb(207,"
                        "207,207);\n"
"}\n"
"QSlider::groove:horizontal {\n"
"	height: 5px;\n"
"	background: rgb(246, 134, 86);\n"
"}\n"
"QSlider::groove:vertical {\n"
"	width: 5px;\n"
"	background: rgb(246, 134, 86);\n"
"}\n"
"QSlider::handle:horizontal {\n"
"	background: rgb(253,253,253);\n"
"	border-style: solid;\n"
"	border-width: 1px;\n"
"	border-color: rgb(207,207,207);\n"
"	width: 12px;\n"
"	margin: -5px 0;\n"
"	border-radius: 7px;\n"
"}\n"
"QSlider::handle:vertical {\n"
"	background: rgb(253,253,253);\n"
"	border-style: solid;\n"
"	border-width: 1px;\n"
"	border-color: rgb(207,207,207);\n"
"	height: 12px;\n"
"	margin: 0 -5px;\n"
"	border-radius: 7px;\n"
"}\n"
"QSlider::add-page:horizontal, QSlider::add-page:vertical {\n"
" 	background: white;\n"
"}\n"
"QSlider::sub-page:horizontal, QSlider::sub-page:vertical {\n"
"	background: rgb(246, 134, 86);\n"
"}\n"
"QStatusBar, QSpinBox {\n"
"	color:rgb(81,72,65);\n"
"}\n"
"QSpinBox {\n"
"	background-color: #ffffff;\n"
"}\n"
"QScrollBar:horizontal {\n"
"	max-height: 20px;\n"
"	border: 1p"
                        "x transparent;\n"
"	margin: 0px 20px 0px 20px;\n"
"}\n"
"QScrollBar::handle:horizontal {\n"
"	background: rgb(253,253,253);\n"
"	border: 1px solid rgb(207,207,207);\n"
"	border-radius: 7px;\n"
"	min-width: 25px;\n"
"}\n"
"QScrollBar::handle:horizontal:hover {\n"
"	background: rgb(253,253,253);\n"
"	border: 1px solid rgb(255,150,60);\n"
"	border-radius: 7px;\n"
"	min-width: 25px;\n"
"}\n"
"QScrollBar::add-line:horizontal {\n"
"  	border: 1px solid rgb(207,207,207);\n"
"  	border-top-right-radius: 7px;\n"
"  	border-top-left-radius: 7px;\n"
"  	border-bottom-right-radius: 7px;\n"
"  	background: rgb(255, 255, 255);\n"
"  	width: 20px;\n"
"  	subcontrol-position: right;\n"
"  	subcontrol-origin: margin;\n"
"}\n"
"QScrollBar::add-line:horizontal:hover {\n"
"  	border: 1px solid rgb(255,150,60);\n"
"  	border-top-right-radius: 7px;\n"
"  	border-top-left-radius: 7px;\n"
"  	border-bottom-right-radius: 7px;\n"
"  	background: rgb(255, 255, 255);\n"
"  	width: 20px;\n"
"  	subcontrol-position: right;\n"
"  	subcontro"
                        "l-origin: margin;\n"
"}\n"
"QScrollBar::add-line:horizontal:pressed {\n"
"  	border: 1px solid grey;\n"
"  	border-top-left-radius: 7px;\n"
"  	border-top-right-radius: 7px;\n"
"  	border-bottom-right-radius: 7px;\n"
"  	background: rgb(231,231,231);\n"
"  	width: 20px;\n"
"  	subcontrol-position: right;\n"
"  	subcontrol-origin: margin;\n"
"}\n"
"QScrollBar::sub-line:horizontal {\n"
"  	border: 1px solid rgb(207,207,207);\n"
"  	border-top-right-radius: 7px;\n"
"  	border-top-left-radius: 7px;\n"
"  	border-bottom-left-radius: 7px;\n"
"  	background: rgb(255, 255, 255);\n"
"  	width: 20px;\n"
"  	subcontrol-position: left;\n"
"  	subcontrol-origin: margin;\n"
"}\n"
"QScrollBar::sub-line:horizontal:hover {\n"
"  	border: 1px solid rgb(255,150,60);\n"
"  	border-top-right-radius: 7px;\n"
"  	border-top-left-radius: 7px;\n"
"  	border-bottom-left-radius: 7px;\n"
"  	background: rgb(255, 255, 255);\n"
"  	width: 20px;\n"
"  	subcontrol-position: left;\n"
"  	subcontrol-origin: margin;\n"
"}\n"
"QScrollBar::sub-li"
                        "ne:horizontal:pressed {\n"
"  	border: 1px solid grey;\n"
"  	border-top-right-radius: 7px;\n"
"  	border-top-left-radius: 7px;\n"
"  	border-bottom-left-radius: 7px;\n"
"  	background: rgb(231,231,231);\n"
"  	width: 20px;\n"
"  	subcontrol-position: left;\n"
"  	subcontrol-origin: margin;\n"
"}\n"
"QScrollBar::left-arrow:horizontal {\n"
"  	border: 1px transparent grey;\n"
"  	border-top-left-radius: 3px;\n"
"  	border-bottom-left-radius: 3px;\n"
"  	width: 6px;\n"
"  	height: 6px;\n"
"  	background: rgb(230,230,230);\n"
"}\n"
"QScrollBar::right-arrow:horizontal {\n"
"	border: 1px transparent grey;\n"
"	border-top-right-radius: 3px;\n"
"	border-bottom-right-radius: 3px;\n"
"  	width: 6px;\n"
"  	height: 6px;\n"
" 	background: rgb(230,230,230);\n"
"}\n"
"QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {\n"
" 	background: none;\n"
"} \n"
"QScrollBar:vertical {\n"
"	max-width: 20px;\n"
"	border: 1px transparent grey;\n"
"	margin: 20px 0px 20px 0px;\n"
"}\n"
"QScrollBar::add-line:vertical {\n"
""
                        "	border: 1px solid;\n"
"	border-color: rgb(207,207,207);\n"
"	border-bottom-right-radius: 7px;\n"
"	border-bottom-left-radius: 7px;\n"
"	border-top-left-radius: 7px;\n"
"	background: rgb(255, 255, 255);\n"
"  	height: 20px;\n"
"  	subcontrol-position: bottom;\n"
"  	subcontrol-origin: margin;\n"
"}\n"
"QScrollBar::add-line:vertical:hover {\n"
"  	border: 1px solid;\n"
"  	border-color: rgb(255,150,60);\n"
"  	border-bottom-right-radius: 7px;\n"
"  	border-bottom-left-radius: 7px;\n"
"  	border-top-left-radius: 7px;\n"
"  	background: rgb(255, 255, 255);\n"
"  	height: 20px;\n"
"  	subcontrol-position: bottom;\n"
"  	subcontrol-origin: margin;\n"
"}\n"
"QScrollBar::add-line:vertical:pressed {\n"
"  	border: 1px solid grey;\n"
"  	border-bottom-left-radius: 7px;\n"
"  	border-bottom-right-radius: 7px;\n"
"  	border-top-left-radius: 7px;\n"
"  	background: rgb(231,231,231);\n"
"  	height: 20px;\n"
"  	subcontrol-position: bottom;\n"
"  	subcontrol-origin: margin;\n"
"}\n"
"QScrollBar::sub-line:vertical {\n"
"  	b"
                        "order: 1px solid rgb(207,207,207);\n"
"  	border-top-right-radius: 7px;\n"
"  	border-top-left-radius: 7px;\n"
"  	border-bottom-left-radius: 7px;\n"
"  	background: rgb(255, 255, 255);\n"
"  	height: 20px;\n"
"  	subcontrol-position: top;\n"
"  	subcontrol-origin: margin;\n"
"}\n"
"QScrollBar::sub-line:vertical:hover {\n"
"  	border: 1px solid rgb(255,150,60);\n"
"  	border-top-right-radius: 7px;\n"
"  	border-top-left-radius: 7px;\n"
"  	border-bottom-left-radius: 7px;\n"
"	background: rgb(255, 255, 255);\n"
"  	height: 20px;\n"
"  	subcontrol-position: top;\n"
"  	subcontrol-origin: margin;\n"
"}\n"
"QScrollBar::sub-line:vertical:pressed {\n"
"  	border: 1px solid grey;\n"
"  	border-top-left-radius: 7px;\n"
"  	border-top-right-radius: 7px;\n"
"  	background: rgb(231,231,231);\n"
" 	height: 20px;\n"
"  	subcontrol-position: top;\n"
"  	subcontrol-origin: margin;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"	background: rgb(253,253,253);\n"
"	border: 1px solid rgb(207,207,207);\n"
"	border-radius: 7px;\n"
"	"
                        "min-height: 25px;\n"
"}\n"
"QScrollBar::handle:vertical:hover {\n"
"	background: rgb(253,253,253);\n"
"	border: 1px solid rgb(255,150,60);\n"
"	border-radius: 7px;\n"
"	min-height: 25px;\n"
"}\n"
"QScrollBar::up-arrow:vertical {\n"
"	border: 1px transparent grey;\n"
"  	border-top-left-radius: 3px;\n"
"	border-top-right-radius: 3px;\n"
"  	width: 6px;\n"
"  	height: 6px;\n"
"  	background: rgb(230,230,230);\n"
"}\n"
"QScrollBar::down-arrow:vertical {\n"
"  	border: 1px transparent grey;\n"
"  	border-bottom-left-radius: 3px;\n"
"  	border-bottom-right-radius: 3px;\n"
"  	width: 6px;\n"
"  	height: 6px;\n"
"  	background: rgb(230,230,230);\n"
"}\n"
"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n"
"  	background: none;\n"
"}\n"
"QTabWidget {\n"
"	color:rgb(0,0,0);\n"
"	background-color:rgb(247,246,246);\n"
"}\n"
"QTabWidget::pane {\n"
"	border-color: rgb(180,180,180);\n"
"	background-color:rgb(247,246,246);\n"
"	border-style: solid;\n"
"	border-width: 1px;\n"
"  	border-radius: 6px;\n"
"}\n"
"Q"
                        "TabBar::tab {\n"
"	padding-left:4px;\n"
"	padding-right:4px;\n"
"	padding-bottom:2px;\n"
"	padding-top:2px;\n"
"	color:rgb(81,72,65);\n"
"  	background-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(221,218,217,255), stop:1 rgba(240,239,238,255));\n"
"	border-style: solid;\n"
"	border-width: 1px;\n"
"  	border-top-right-radius:4px;\n"
"	border-top-left-radius:4px;\n"
"	border-top-color: rgb(180,180,180);\n"
"	border-left-color: rgb(180,180,180);\n"
"	border-right-color: rgb(180,180,180);\n"
"	border-bottom-color: transparent;\n"
"}\n"
"QTabBar::tab:selected, QTabBar::tab:last:selected, QTabBar::tab:hover {\n"
"  	background-color:rgb(247,246,246);\n"
"  	margin-left: 0px;\n"
"  	margin-right: 1px;\n"
"}\n"
"QTabBar::tab:!selected {\n"
"	margin-top: 1px;\n"
"	margin-right: 1px;\n"
"}\n"
"QTextEdit {\n"
"	border-width: 1px;\n"
"	border-style: solid;\n"
"	border-color:transparent;\n"
"	color:rgb(17,17,17);\n"
"	selection-background-color:rgb(236,116,64);\n"
"}\n"
"QTimeEdit, QToolBox, "
                        "QToolBox::tab, QToolBox::tab:selected {\n"
"	color:rgb(81,72,65);\n"
"	background-color: #ffffff;\n"
"}\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"QComboBox, QFontComboBox {\n"
"    border-width: 2px;\n"
"    border-radius: 8px;\n"
"    border-style: solid;\n"
"    border-top-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 #c1c9cf, stop:1 #d2d8dd);\n"
"    border-right-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #c1c9cf, stop:1 #d2d8dd);\n"
"    border-bottom-color: qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:1, stop:0 #c1c9cf, stop:1 #d2d8dd);\n"
"    border-left-color: qlineargradient(spread:pad, x1:1, y1:0, x2:0, y2:0, stop:0 #c1c9cf, stop:1 #d2d8dd);\n"
"    background-color: #f4f4f4;\n"
"    color: #272727;\n"
"    padding-left: 5px;\n"
"}\n"
"QComboBox:editable, QComboBox:!editable, QComboBox::drop-down:editable, QComboBox:!editable:on, QComboBox::drop-down:editable:on {\n"
"    background: #"
                        "ffffff;\n"
"}\n"
"QComboBox::drop-down {\n"
"    subcontrol-origin: padding;\n"
"    subcontrol-position: top right;\n"
"    width: 15px;\n"
"    color: #272727;\n"
"    border-left-width: 1px;\n"
"    border-left-color: darkgray;\n"
"    border-left-style: solid;\n"
"    border-top-right-radius: 3px;\n"
"    border-bottom-right-radius: 3px;\n"
"}\n"
"QComboBox::down-arrow {\n"
"    image: url(/usr/share/icons/Adwaita/16x16/actions/go-down-symbolic.symbolic.png); /*Adawaita icon thene*/\n"
"}\n"
"\n"
"QComboBox::down-arrow:on {\n"
"    top: 1px;\n"
"    left: 1px;\n"
"}\n"
"QComboBox QAbstractItemView {\n"
"    border: 1px solid darkgray;\n"
"    border-radius: 8px;\n"
"    selection-background-color: #dadada;\n"
"    selection-color: #272727;\n"
"    color: #272727;\n"
"    background: white;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        mapWidget = new QWidget(centralwidget);
        mapWidget->setObjectName(QString::fromUtf8("mapWidget"));
        mapWidget->setMinimumSize(QSize(360, 0));
        mapWidget->setMaximumSize(QSize(350, 16777215));
        verticalLayout_2 = new QVBoxLayout(mapWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(mapWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMaximumSize(QSize(16777215, 16777215));
        groupBox->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        cBox_updataLimit = new QCheckBox(groupBox);
        cBox_updataLimit->setObjectName(QString::fromUtf8("cBox_updataLimit"));
        cBox_updataLimit->setLayoutDirection(Qt::LeftToRight);
        cBox_updataLimit->setStyleSheet(QString::fromUtf8("padding-left:100px\n"
""));

        verticalLayout_4->addWidget(cBox_updataLimit);

        rl = new QVBoxLayout();
        rl->setObjectName(QString::fromUtf8("rl"));

        verticalLayout_4->addLayout(rl);


        verticalLayout_2->addWidget(groupBox);


        horizontalLayout->addWidget(mapWidget);

        showWidget = new QWidget(centralwidget);
        showWidget->setObjectName(QString::fromUtf8("showWidget"));
        verticalLayout = new QVBoxLayout(showWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_2 = new QGroupBox(showWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        scrollArea = new QScrollArea(groupBox_2);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1133, 873));
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);


        verticalLayout->addWidget(groupBox_2);


        horizontalLayout->addWidget(showWidget);

        menuWidget = new QWidget(centralwidget);
        menuWidget->setObjectName(QString::fromUtf8("menuWidget"));
        menuWidget->setMinimumSize(QSize(200, 0));
        menuWidget->setMaximumSize(QSize(250, 16777215));
        verticalLayout_3 = new QVBoxLayout(menuWidget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        widget_5 = new QWidget(menuWidget);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        widget_5->setMaximumSize(QSize(16777215, 150));
        label = new QLabel(widget_5);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 201, 51));
        label->setPixmap(QPixmap(QString::fromUtf8(":/logo/logo.PNG")));
        label->setScaledContents(true);
        label_2 = new QLabel(widget_5);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 80, 191, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        label_2->setFont(font);

        verticalLayout_3->addWidget(widget_5);

        groupBox_3 = new QGroupBox(menuWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_6 = new QVBoxLayout(groupBox_3);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        lineEdit_path = new QLineEdit(groupBox_3);
        lineEdit_path->setObjectName(QString::fromUtf8("lineEdit_path"));

        verticalLayout_6->addWidget(lineEdit_path);

        NewClothRollPB = new QPushButton(groupBox_3);
        NewClothRollPB->setObjectName(QString::fromUtf8("NewClothRollPB"));
        NewClothRollPB->setMinimumSize(QSize(0, 50));
        NewClothRollPB->setSizeIncrement(QSize(20, 0));
        NewClothRollPB->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_6->addWidget(NewClothRollPB);

        checkBox = new QCheckBox(groupBox_3);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        verticalLayout_6->addWidget(checkBox);

        checkBox_2 = new QCheckBox(groupBox_3);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

        verticalLayout_6->addWidget(checkBox_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        checkBox_3 = new QCheckBox(groupBox_3);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
        checkBox_3->setChecked(true);

        horizontalLayout_3->addWidget(checkBox_3);

        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_3->addWidget(label_5);

        lineEdit = new QLineEdit(groupBox_3);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout_3->addWidget(lineEdit);


        verticalLayout_6->addLayout(horizontalLayout_3);

        startVisionPB = new QPushButton(groupBox_3);
        startVisionPB->setObjectName(QString::fromUtf8("startVisionPB"));
        startVisionPB->setEnabled(true);
        startVisionPB->setMinimumSize(QSize(0, 50));

        verticalLayout_6->addWidget(startVisionPB);

        closeVisionPB = new QPushButton(groupBox_3);
        closeVisionPB->setObjectName(QString::fromUtf8("closeVisionPB"));
        closeVisionPB->setEnabled(true);
        closeVisionPB->setMinimumSize(QSize(0, 50));

        verticalLayout_6->addWidget(closeVisionPB);

        exportPB = new QPushButton(groupBox_3);
        exportPB->setObjectName(QString::fromUtf8("exportPB"));
        exportPB->setEnabled(true);
        exportPB->setMinimumSize(QSize(0, 50));

        verticalLayout_6->addWidget(exportPB);

        clothHistoryPB = new QPushButton(groupBox_3);
        clothHistoryPB->setObjectName(QString::fromUtf8("clothHistoryPB"));
        clothHistoryPB->setEnabled(true);
        clothHistoryPB->setMinimumSize(QSize(0, 50));

        verticalLayout_6->addWidget(clothHistoryPB);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_6->addItem(verticalSpacer);

        groupBox_4 = new QGroupBox(groupBox_3);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_2 = new QGridLayout(groupBox_4);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_4 = new QLabel(groupBox_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 0, 2, 1, 1);

        label_3 = new QLabel(groupBox_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        pushButton = new QPushButton(groupBox_4);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout_2->addWidget(pushButton, 1, 1, 1, 1);

        meterExlineEdit = new QLineEdit(groupBox_4);
        meterExlineEdit->setObjectName(QString::fromUtf8("meterExlineEdit"));
        meterExlineEdit->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(meterExlineEdit, 0, 1, 1, 1);


        verticalLayout_6->addWidget(groupBox_4);

        groupBox_5 = new QGroupBox(groupBox_3);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_3 = new QGridLayout(groupBox_5);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalSpacer_4 = new QSpacerItem(45, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_4, 1, 3, 1, 1);

        clothLengthLB = new QLabel(groupBox_5);
        clothLengthLB->setObjectName(QString::fromUtf8("clothLengthLB"));

        gridLayout_3->addWidget(clothLengthLB, 1, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(45, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 0, 3, 1, 1);

        clothNoLB = new QLabel(groupBox_5);
        clothNoLB->setObjectName(QString::fromUtf8("clothNoLB"));

        gridLayout_3->addWidget(clothNoLB, 0, 2, 1, 1);

        label_15 = new QLabel(groupBox_5);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_3->addWidget(label_15, 2, 1, 1, 1);

        label_12 = new QLabel(groupBox_5);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_3->addWidget(label_12, 1, 1, 1, 1);

        clothNGNumberLB = new QLabel(groupBox_5);
        clothNGNumberLB->setObjectName(QString::fromUtf8("clothNGNumberLB"));

        gridLayout_3->addWidget(clothNGNumberLB, 2, 2, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(45, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_5, 2, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(45, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 0, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(45, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_6, 2, 3, 1, 1);

        totalLabel_2 = new QLabel(groupBox_5);
        totalLabel_2->setObjectName(QString::fromUtf8("totalLabel_2"));

        gridLayout_3->addWidget(totalLabel_2, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(45, 13, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_3, 1, 0, 1, 1);


        verticalLayout_6->addWidget(groupBox_5);

        groupBox_6 = new QGroupBox(groupBox_3);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        verticalLayout_5 = new QVBoxLayout(groupBox_6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        totalLabel_3 = new QLabel(groupBox_6);
        totalLabel_3->setObjectName(QString::fromUtf8("totalLabel_3"));

        horizontalLayout_2->addWidget(totalLabel_3);

        comboBox_1 = new QComboBox(groupBox_6);
        comboBox_1->addItem(QString());
        comboBox_1->addItem(QString());
        comboBox_1->addItem(QString());
        comboBox_1->addItem(QString());
        comboBox_1->addItem(QString());
        comboBox_1->setObjectName(QString::fromUtf8("comboBox_1"));
        comboBox_1->setEnabled(false);

        horizontalLayout_2->addWidget(comboBox_1);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 3);

        verticalLayout_5->addLayout(horizontalLayout_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        totalLabel_6 = new QLabel(groupBox_6);
        totalLabel_6->setObjectName(QString::fromUtf8("totalLabel_6"));

        horizontalLayout_5->addWidget(totalLabel_6);

        comboBox_4 = new QComboBox(groupBox_6);
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->setObjectName(QString::fromUtf8("comboBox_4"));
        comboBox_4->setEnabled(false);

        horizontalLayout_5->addWidget(comboBox_4);

        horizontalLayout_5->setStretch(0, 1);
        horizontalLayout_5->setStretch(1, 3);

        verticalLayout_5->addLayout(horizontalLayout_5);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        btn_Unlock = new QPushButton(groupBox_6);
        btn_Unlock->setObjectName(QString::fromUtf8("btn_Unlock"));

        horizontalLayout_7->addWidget(btn_Unlock);

        btn_Lock = new QPushButton(groupBox_6);
        btn_Lock->setObjectName(QString::fromUtf8("btn_Lock"));

        horizontalLayout_7->addWidget(btn_Lock);


        verticalLayout_5->addLayout(horizontalLayout_7);

        lineEdit_password = new QLineEdit(groupBox_6);
        lineEdit_password->setObjectName(QString::fromUtf8("lineEdit_password"));
        lineEdit_password->setEchoMode(QLineEdit::Password);
        lineEdit_password->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(lineEdit_password);


        verticalLayout_6->addWidget(groupBox_6);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_6->addItem(verticalSpacer_3);

        exitPB = new QPushButton(groupBox_3);
        exitPB->setObjectName(QString::fromUtf8("exitPB"));
        exitPB->setMinimumSize(QSize(0, 50));

        verticalLayout_6->addWidget(exitPB);


        verticalLayout_3->addWidget(groupBox_3);


        horizontalLayout->addWidget(menuWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1813, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(exitPB, SIGNAL(clicked(bool)), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "\345\270\203\345\215\267\344\277\241\346\201\257  (\346\273\232\350\275\256\347\277\273\351\241\265\357\274\214Ctrl+\346\273\232\350\275\256\347\274\251\346\224\276\357\274\214\345\217\263\351\224\256\345\244\215\345\216\237)", nullptr));
        cBox_updataLimit->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\350\207\252\345\212\250\346\233\264\346\226\260\347\225\214\351\231\220", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\345\233\276\347\211\207\345\261\225\347\244\272\357\274\232", nullptr));
        label->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "\346\266\202\350\203\266\351\223\234\347\256\224\346\243\200\346\265\213\350\275\257\344\273\266", nullptr));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\346\216\247\345\210\266\345\215\225\345\205\203\357\274\232", nullptr));
        NewClothRollPB->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\345\233\276\347\211\207\346\226\207\344\273\266\345\244\271", nullptr));
        checkBox->setText(QApplication::translate("MainWindow", "\351\252\214\350\257\201\346\243\200\345\207\272", nullptr));
        checkBox_2->setText(QApplication::translate("MainWindow", "\351\252\214\350\257\201\345\210\206\347\261\273", nullptr));
        checkBox_3->setText(QApplication::translate("MainWindow", "\351\252\214\350\257\201\345\220\216\345\244\204\347\220\206", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "   \347\261\273\345\210\253\357\274\232", nullptr));
        lineEdit->setText(QApplication::translate("MainWindow", "0", nullptr));
        startVisionPB->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\346\243\200\346\265\213", nullptr));
        closeVisionPB->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\346\243\200\346\265\213", nullptr));
        exportPB->setText(QApplication::translate("MainWindow", "\345\257\274\345\207\272\346\212\245\350\241\250", nullptr));
        clothHistoryPB->setText(QApplication::translate("MainWindow", "\345\216\206\345\217\262\345\270\203\345\215\267", nullptr));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "\346\243\200\346\265\213\350\256\276\347\275\256\357\274\232", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "\347\261\263", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "\347\261\263\351\225\277\350\241\245\345\201\277\357\274\232", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        meterExlineEdit->setText(QApplication::translate("MainWindow", "0", nullptr));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "\346\243\200\346\265\213\346\230\276\347\244\272\357\274\232", nullptr));
        clothLengthLB->setText(QApplication::translate("MainWindow", "N", nullptr));
        clothNoLB->setText(QApplication::translate("MainWindow", "N", nullptr));
        label_15->setText(QApplication::translate("MainWindow", "\346\243\200\346\265\213\346\225\260\351\207\217\357\274\232", nullptr));
        label_12->setText(QApplication::translate("MainWindow", "\345\270\203\345\215\267\351\225\277\345\272\246\357\274\232", nullptr));
        clothNGNumberLB->setText(QApplication::translate("MainWindow", "N", nullptr));
        totalLabel_2->setText(QApplication::translate("MainWindow", "\345\270\203\345\215\267\347\274\226\345\217\267\357\274\232", nullptr));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "\347\226\265\347\202\271\347\233\264\345\276\204\351\200\211\346\213\251", nullptr));
        totalLabel_3->setText(QApplication::translate("MainWindow", "\347\274\272\350\203\266\357\274\232", nullptr));
        comboBox_1->setItemText(0, QApplication::translate("MainWindow", "1mm", nullptr));
        comboBox_1->setItemText(1, QApplication::translate("MainWindow", "2mm", nullptr));
        comboBox_1->setItemText(2, QApplication::translate("MainWindow", "3mm", nullptr));
        comboBox_1->setItemText(3, QApplication::translate("MainWindow", "4mm", nullptr));
        comboBox_1->setItemText(4, QApplication::translate("MainWindow", "5mm", nullptr));

        totalLabel_6->setText(QApplication::translate("MainWindow", "\351\273\221\347\202\271\357\274\232", nullptr));
        comboBox_4->setItemText(0, QApplication::translate("MainWindow", "0.5mm", nullptr));
        comboBox_4->setItemText(1, QApplication::translate("MainWindow", "1mm", nullptr));
        comboBox_4->setItemText(2, QApplication::translate("MainWindow", "1.5mm", nullptr));
        comboBox_4->setItemText(3, QApplication::translate("MainWindow", "2mm", nullptr));
        comboBox_4->setItemText(4, QApplication::translate("MainWindow", "2.5mm", nullptr));

        btn_Unlock->setText(QApplication::translate("MainWindow", "\350\247\243\351\224\201", nullptr));
        btn_Lock->setText(QApplication::translate("MainWindow", "\345\212\240\351\224\201", nullptr));
        lineEdit_password->setText(QString());
        exitPB->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
