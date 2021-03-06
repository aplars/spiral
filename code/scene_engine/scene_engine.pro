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
CONFIG += c++14

QMAKE_CXXFLAGS += -isystem $$(BOOST_ROOT)/
QMAKE_CXXFLAGS += -fopenmp

INCLUDEPATH += "$$(SPAR_ROOT)/code/"
INCLUDEPATH += "$$(SPAR_ROOT)/thirdparty/boost"
INCLUDEPATH += "$$(SPAR_ROOT)/thirdparty/glm/"

SOURCES += meshrenderable.cpp \
    scene.cpp \
    fpscamera.cpp \
    directionallight.cpp \
    cuberenderable.cpp \
    debugentity.cpp \
    converters.cpp \
    shadows/shadowmapping.cpp \
    sky/sky.cpp \
    terrain/simple/terrain.cpp \
    terrain/plane/plane.cpp \
    sky/lightshafts.cpp \
    rectanglerenderable.cpp \
    grass/grass.cpp \
    sky/sunposition.cpp \
    grass/oneplant.cpp \
    weather/weather.cpp \
    model.cpp

HEADERS += meshrenderable.h \
    scene.h \
    fpscamera.h \
    directionallight.h \
    cuberenderable.h \
    debugentity.h \
    datastorage.h \
    imagecache.h \
    shadercache.h \
    texturecache.h \
    converters.h \
    shadows/shadowmapping.h \
    sky/sky.h \
    terrain/simple/terrain.h \
    terrain/plane/plane.h \
    renderpass.h \
    sky/lightshafts.h \
    rectanglerenderable.h \
    grass/grass.h \
    sky/sunposition.h \
    grass/oneplant.h \
    weather/weather.h \
    model.h

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
    ../../data/shaders/debugshader.vsh \
    ../../data/shaders/ubershadowshader.fsh \
    ../../data/shaders/ubershadowshader.vsh \
    ../../data/shaders/skyshader.fsh \
    ../../data/shaders/skyshader.vsh \
    ../../data/shaders/blackshader.fsh \
    ../../data/shaders/blackshader.vsh \
    ../../data/shaders/skyshaderblackandwhite.fsh \
    ../../data/shaders/skyshaderblackandwhite.vsh \
    ../../data/shaders/lightshafts.fsh \
    ../../data/shaders/lightshafts.fsh \
    ../../data/shaders/grassshader.vsh \
    ../../data/shaders/grassshader.fsh \
    ../../data/shaders/grassblackshader.fsh \
    ../../data/textures/grasspack.png
