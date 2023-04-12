QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    borderedgraphicsview.cpp \
    dialogs/base/colorparameter.cpp \
    dialogs/base/numericparameter.cpp \
    dialogs/base/numericslider.cpp \
    dialogs/base/parameterdialog.cpp \
    dialogs/base/switchparameter.cpp \
    dialogs/blurfilterdialog.cpp \
    dialogs/floydditheringdialog.cpp \
    dialogs/gammacorrectiondialog.cpp \
    dialogs/imagerotationdialog.cpp \
    dialogs/orderedditheringdialog.cpp \
    dialogs/robertsfilterdialog.cpp \
    dialogs/showfitteddialog.cpp \
    dialogs/sobelfilterdialog.cpp \
    displayarea.cpp \
    ecgfilter.cpp \
    imageprocessor.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    borderedgraphicsview.h \
    constants.h \
    dialogs/base/colorparameter.h \
    dialogs/base/numericparameter.h \
    dialogs/base/numericslider.h \
    dialogs/base/parameterdialog.h \
    dialogs/base/switchparameter.h \
    dialogs/blurfilterdialog.h \
    dialogs/floydditheringdialog.h \
    dialogs/gammacorrectiondialog.h \
    dialogs/imagerotationdialog.h \
    dialogs/orderedditheringdialog.h \
    dialogs/robertsfilterdialog.h \
    dialogs/showfitteddialog.h \
    dialogs/sobelfilterdialog.h \
    displayarea.h \
    ecgfilter.h \
    imageprocessor.h \
    mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

#####################################
# ADD YOUR PATH TO INSTALLED OPENCV #
#####################################
# Here, you need to set paths to opencv shared libraries
# Program was developed on Ubuntu, so default unix paths have been already set
win32:CONFIG(release, debug|release):
else:win32:CONFIG(debug, debug|release):
else:unix: LIBS += -L/usr/local/lib/ -lopencv_imgproc -lopencv_core

#####################################
# ADD YOUR PATH TO INSTALLED OPENCV #
#####################################
# Here, you need to set path to directory with opencv headers
# Program was developed on Ubuntu, so default unix paths have been already set
INCLUDEPATH += /usr/local/include/opencv4
DEPENDPATH += /usr/local/include/opencv4
