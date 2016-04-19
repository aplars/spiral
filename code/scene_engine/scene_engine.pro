#-------------------------------------------------
#
# Project created by QtCreator 2015-02-13T08:58:51
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

TARGET = scene_engine
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11

QMAKE_CXXFLAGS += -isystem $$(BOOST_ROOT)/
QMAKE_CXXFLAGS += -fopenmp

INCLUDEPATH += "$$(SPAR_ROOT)/code/"
INCLUDEPATH += "$$(SPAR_ROOT)/thirdparty/boost"

SOURCES += meshrenderable.cpp \
    scene.cpp \
    fpscamera.cpp \
    scenestate.cpp \
    directionallight.cpp \
    cuberenderable.cpp \
    debugentity.cpp \
    backgroundworker.cpp \
    streamedmeshentity.cpp

HEADERS += meshrenderable.h \
    scene.h \
    fpscamera.h \
    scenestate.h \
    directionallight.h \
    cuberenderable.h \
    debugentity.h \
    datastorage.h \
    backgroundworker.h \
    streamedmeshentity.h \
    imagecache.h

CONFIG( debug, debug|release ) {
    DESTDIR = $$(SPAR_ROOT)/libs/lib-debug
}
else {
    DESTDIR = $$(SPAR_ROOT)/libs/lib-release
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    ../../data/shaders/ubershader.vsh \
    ../../data/shaders/ubershader.fsh \
    ../../data/shaders/debugshader.fsh \
    ../../data/shaders/debugshader.vsh
