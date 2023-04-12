QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/conf/settings.cpp \
    src/dialogs/dialogparameter.cpp \
    src/dialogs/linedialog.cpp \
    src/dialogs/polygondialog.cpp \
    src/dialogs/resizedialog.cpp \
    src/dialogs/stardialog.cpp \
    src/dialogs/tooldialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/paintarea.cpp \
    src/span.cpp

HEADERS += \
    src/conf/settings.h \
    src/dialogs/dialogparameter.h \
    src/dialogs/linedialog.h \
    src/dialogs/polygondialog.h \
    src/dialogs/resizedialog.h \
    src/dialogs/stardialog.h \
    src/dialogs/tooldialog.h \
    src/mainwindow.h \
    src/paintarea.h \
    src/span.h

TRANSLATIONS += \
    lang/ECGPaint_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc

