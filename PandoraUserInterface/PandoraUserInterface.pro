QT       += core gui

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
    sources/ansiescapecodehandler.cpp \
    sources/configLoader.cpp \
    sources/consoleWidget.cpp \
    sources/customLineEdit.cpp \
    sources/executableRunner.cpp \
    sources/main.cpp \
    sources/mainWindow.cpp \
    sources/runningSimulationWindow.cpp \
    libraries/tinyxml/tinyxml.cpp \
    libraries/tinyxml/tinystr.cpp \
    libraries/tinyxml/tinyxmlerror.cpp \
    libraries/tinyxml/tinyxmlparser.cpp \
    sources/utils.cpp \
    sources/wheelEventFilter.cpp

HEADERS += \
    headers/ansiescapecodehandler.h \
    headers/configLoader.h \
    headers/consoleWidget.h \
    headers/customLineEdit.h \
    headers/executableRunner.h \
    headers/mainWindow.h \
    headers/runningSimulationWindow.h \
    headers/utils.h \
    headers/wheelEventFilter.h \
    libraries/tinyxml/tinyxml.h \
    libraries/tinyxml/tinystr.h

FORMS += \
    UI/mainWindow.ui \
    UI/runningSimulationWindow.ui



OBJECTS_DIR = bin/objects
MOC_DIR = bin/moc
Release:RCC_DIR = bin/rcc
UI_DIR = bin/ui



LIBS += -L/opt/hdf5-1.8.19/lib/ -lhdf5


INCLUDEPATH += headers
INCLUDEPATH += libraries/tinyxml
INCLUDEPATH += /usr/include/mpich/
INCLUDEPATH += /opt/hdf5-1.8.19/include/


QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
