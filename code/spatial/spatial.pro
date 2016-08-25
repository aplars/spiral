#-------------------------------------------------
#
# Project created by QtCreator 2016-08-09T06:38:49
#
#-------------------------------------------------

QT       -= core gui

TARGET = spatial
TEMPLATE = lib
CONFIG += staticlib

SOURCES += kdtree.cpp

HEADERS += kdtree.h

INCLUDEPATH += "$$(SPAR_ROOT)/code/"

unix {
    target.path = /usr/lib
    INSTALLS += target
}
