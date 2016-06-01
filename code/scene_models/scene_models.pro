#-------------------------------------------------
#
# Project created by QtCreator 2015-02-12T11:34:23
#
#-------------------------------------------------

QT       += core gui

TARGET = scene_models
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14

QMAKE_CXXFLAGS += -isystem $$(SPAR_ROOT)/thirdparty/boost/

INCLUDEPATH += "$$(SPAR_ROOT)/code"
INCLUDEPATH += "$$(SPAR_ROOT)/thirdparty/boost"

warning ($$(SPAR_ROOT))

SOURCES += \
    materialmodel.cpp \
    transformationnodemodel.cpp \
    submeshmodel.cpp \
    meshmodel.cpp \
    aabbmodel.cpp \
    nodemodel.cpp \
    groupnodemodel.cpp \
    meshnodemodel.cpp \
    visitor.cpp \
    visitorapplytransformations.cpp \
    visitorgetmeshnodes.cpp \
    visitorgetmeshnodesunderspecifictransformation.cpp \
    animationchannelmodel.cpp \
    animationmodel.cpp \
    visitorapplyanimation.cpp \
    visitorgettransformationnode.cpp \
    jointmodel.cpp \
    skeletonmodel.cpp

HEADERS += \
    materialmodel.h \
    transformationnodemodel.h \
    serialization.h \
    submeshmodel.h \
    meshmodel.h \
    aabbmodel.h \
    nodemodel.h \
    groupnodemodel.h \
    meshnodemodel.h \
    visitor.h \
    visitorapplytransformations.h \
    visitorgetmeshnodes.h \
    visitorgetmeshnodesunderspecifictransformation.h \
    animationchannelmodel.h \
    animationmodel.h \
    visitorapplyanimation.h \
    visitorgettransformationnode.h \
    jointmodel.h \
    skeletonmodel.h

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
