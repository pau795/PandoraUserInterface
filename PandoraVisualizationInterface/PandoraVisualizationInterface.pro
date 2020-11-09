QT       += core gui
QT       += opengl

INCLUDEPATH +=  glm

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    sources/agentSettingsDialog.cpp \
    sources/customGLWidget.cpp \
    sources/grid.cpp \
    sources/main.cpp \
    sources/mainWindow.cpp \
    sources/shape.cpp \
    sources/tile.cpp \
    sources/raster.cpp \
    sources/dynamicRaster.cpp \
    sources/simulationAgent.cpp \
    sources/simulationWorld.cpp \

HEADERS += \
    headers/agentSettingsDialog.h \
    headers/customGLWidget.h \
    headers/grid.h \
    headers/mainWindow.h \
    headers/shape.h \
    headers/tile.h \
    headers/raster.h \
    headers/dynamicRaster.h \
    headers/simulationAgent.h \
    headers/simulationWorld.h \

FORMS += \
    UI/agentSettingsDialog.ui \
    UI/mainWindow.ui


INCLUDEPATH += headers




OBJECTS_DIR = bin/objects
MOC_DIR = bin/moc
Release:RCC_DIR = bin/rcc
UI_DIR = bin/ui

unix:!macx: LIBS += -L$$PWD/libraries/HDF5/lib/ -lhdf5

INCLUDEPATH += $$PWD/libraries/HDF5/include
DEPENDPATH += $$PWD/libraries/HDF5/include

unix:!macx: PRE_TARGETDEPS += $$PWD/libraries/HDF5/lib/libhdf5.a
