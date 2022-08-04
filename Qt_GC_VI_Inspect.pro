QT       += core gui axcontainer serialport network charts concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BaseItem.cpp \
    ChartView.cpp \
    ControlItem.cpp \
    QGraphicsScenes.cpp \
    QGraphicsViews.cpp \
    arithmetic.cpp \
    arithmeticqthread.cpp \
    callout.cpp \
    circlequeue.cpp \
    dalsa_camera.cpp \
    datastorage.cpp \
    excelhelper.cpp \
    hconaction.cpp \
    hintdialog.cpp \
    hisclothdialog.cpp \
    ikapcamera.cpp \
    lightsource.cpp \
    main.cpp \
    mainwindow.cpp \
    newclothrolldialog.cpp \
    plc.cpp \
    showimagebigdialog.cpp \
    showimagelabel.cpp \
    socketclient.cpp \
    socketqthread.cpp \
    socketserver.cpp

HEADERS += \
    BaseItem.h \
    ChartView.h \
    ControlItem.h \
    QGraphicsScenes.h \
    QGraphicsViews.h \
    arithmetic.h \
    arithmeticqthread.h \
    callout.h \
    circlequeue.h \
    dalsa_camera.h \
    datastorage.h \
    excelhelper.h \
    hconaction.h \
    hintdialog.h \
    hisclothdialog.h \
    ikapcamera.h \
    lightsource.h \
    mainwindow.h \
    models.h \
    newclothrolldialog.h \
    plc.h \
    showimagebigdialog.h \
    showimagelabel.h \
    socketclient.h \
    socketqthread.h \
    socketserver.h \
    ui_imageshowform.h

FORMS += \
    hintdialog.ui \
    hisclothdialog.ui \
    mainwindow.ui \
    newclothrolldialog.ui \
    showimagebigdialog.ui

RC_ICONS = logoNew.ico

INCLUDEPATH += 'C:/Program Files/Teledyne DALSA/Sapera/Include'
INCLUDEPATH += 'C:/Program Files/Teledyne DALSA/Sapera/Classes/Basic'

LIBS += -L'C:/Program Files/Teledyne DALSA/Sapera/Lib/Win64/' -lSapClassBasic

INCLUDEPATH += 'C:/Program Files/Teledyne DALSA/Sapera/Lib/Win64'
DEPENDPATH += 'C:/Program Files/Teledyne DALSA/Sapera/Lib/Win64'

##Add Opencv4.5.1 64bit
#INCLUDEPATH += \
#                C:/Opencv4.5.1/opencv/build/include \
#                C:/Opencv4.5.1/opencv/build/include/opencv2

#LIBS += \
#        C:/Opencv4.5.1/opencv/build/x64/vc15/lib/opencv_world451.lib
##        C:/Opencv4.5.1/opencv/build/x64/vc15/lib/opencv_world451d.lib

#Add Ikap
INCLUDEPATH += \
                C:/TTTTTT/Include/IKap

LIBS += \
C:/TTTTTT/lib/IKap/IKapC.lib

#Add Opencv4.5.1 64bit
INCLUDEPATH += \
#                C:/Opencv4.5.1/opencv/build/include \
#                C:/Opencv4.5.1/opencv/build/include/opencv2
                C:/TTTTTT/Include/opencv \
                C:/TTTTTT/Include/opencv/opencv2
win32:CONFIG(debug, debug|release): {
LIBS += C:/TTTTTT/lib/opencv/opencv_world451d.lib
}
else:win32:CONFIG(release, debug|release): {
LIBS += C:/TTTTTT/lib/opencv/opencv_world451.lib
}

#Add halcon
INCLUDEPATH += \
      C:/TTTTTT/Include/halcon19X64 \
      C:/TTTTTT/Include/halcon19X64/halconcpp

LIBS += \
      C:/TTTTTT/lib/halcon19X64/halcon.lib \
      C:/TTTTTT/lib/halcon19X64/halconcpp.lib

DEPENDPATH += \
      C:/TTTTTT/Include/halcon19X64 \
      C:/TTTTTT/Include/halcon19X64/halconcpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    logo.qrc

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
LIBS += -ldbghelp
