#-------------------------------------------------
#
# Project created by QtCreator 2016-06-03T10:37:16
#
#-------------------------------------------------

QT       -= core gui

TARGET = geodesy
TEMPLATE = lib
CONFIG += staticlib

SOURCES += geodesy.cpp \
    laloposition.cpp

HEADERS += geodesy.h \
    laloposition.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
