
# Which string, hash, list to use? Set 0 to use std, 1 to use Qt.
DEFINES += USE_QT_DATA_STRUCTURES=0



TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
QT -= gui

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp \
    lib/indexedstring.cpp \
    lib/stringsdatabase.cpp

HEADERS += \
    lib/indexedstring.h \
    lib/stringsdatabase.h

