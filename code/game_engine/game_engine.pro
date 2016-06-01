#-------------------------------------------------
#
# Project created by QtCreator 2016-05-31T12:56:20
#
#-------------------------------------------------

QT       -= core gui

TARGET = game_engine
TEMPLATE = lib
CONFIG += staticlib

SOURCES += game_engine.cpp \
    components/component.cpp \
    systems/system.cpp \
    entities/entity.cpp

HEADERS += game_engine.h \
    components/component.h \
    systems/system.h \
    entities/entity.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
