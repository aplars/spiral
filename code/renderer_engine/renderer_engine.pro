#-------------------------------------------------
#
# Project created by QtCreator 2015-01-23T22:15:39
#
#-------------------------------------------------

QT       += opengl

QT       -= gui

TARGET = renderer_engine
TEMPLATE = lib

CONFIG += staticlib
CONFIG += c++14

QMAKE_CXXFLAGS += -isystem $$(SPAR_ROOT)/thirdparty/boost/
INCLUDEPATH += "$$(SPAR_ROOT)/code/"
#INCLUDEPATH += "$$(SPAR_ROOT)/thirdparty/boost"
INCLUDEPATH += "$$(SPAR_ROOT)/thirdparty/glm/"

SOURCES += \
    renderer.cpp \
    widget.cpp \
    vertexbuffer.cpp \
    vertexdescription.cpp \
    rendercontext.cpp \
    renderdevice.cpp \
    vertexarray.cpp \
    shaderprogram.cpp \
    indexbuffer.cpp \
    texture.cpp \
    image.cpp \
    framebuffer.cpp

HEADERS += \
    widget.h \
    renderer.h \
    vertexbuffer.h \
    drawdata.h \
    vertexarray.h \
    vertexdescription.h \
    rendercontext.h \
    renderdevice.h \
    shaderprogram.h \
    indexbuffer.h \
    texture.h \
    image.h \
    framebuffer.h \
    shaderuniforms.h

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
    ../../data/shaders/ubershader.fsh \
    ../../data/shaders/ubershader.vsh
