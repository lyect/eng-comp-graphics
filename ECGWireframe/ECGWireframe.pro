QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/common/editorstate.cpp \
    src/common/renderparameters.cpp \
    src/ecgwireframe.cpp \
    src/main.cpp \
    src/widgets/editor/controlpointeditorwidget.cpp \
    src/widgets/editor/renderparameterswidget.cpp \
    src/widgets/editor/wireframeeditor.cpp \
    src/widgets/mainwindow/mainwindow.cpp \
    src/widgets/view/graphics/pointwrapper.cpp \
    src/widgets/view/graphics/controlpointitem.cpp \
    src/widgets/view/graphics/splineitem.cpp \
    src/widgets/view/graphics/tickarrowitem.cpp \
    src/widgets/view/wireframeeditorscene.cpp \
    src/widgets/view/wireframeeditorview.cpp

HEADERS += \
    src/common/constants.h \
    src/common/editorstate.h \
    src/common/renderparameters.h \
    src/ecgwireframe.h \
    src/widgets/editor/controlpointeditorwidget.h \
    src/widgets/editor/renderparameterswidget.h \
    src/widgets/editor/wireframeeditor.h \
    src/widgets/mainwindow/mainwindow.h \
    src/widgets/view/graphics/pointwrapper.h \
    src/widgets/view/graphics/controlpointitem.h \
    src/widgets/view/graphics/splineitem.h \
    src/widgets/view/graphics/tickarrowitem.h \
    src/widgets/view/wireframeeditorscene.h \
    src/widgets/view/wireframeeditorview.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc
