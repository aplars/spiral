#-------------------------------------------------
#
# Project created by QtCreator 2015-02-12T08:24:54
#
#-------------------------------------------------

QT       += core network gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = meshimporter
TEMPLATE = app
CONFIG += c++14

QMAKE_CXXFLAGS += -isystem $$(SPAR_ROOT)/thirdparty/boost/
INCLUDEPATH += "$(SPAR_ROOT)/code/"
INCLUDEPATH += "$$(SPAR_ROOT)/thirdparty/assimp/include/"
INCLUDEPATH += "$$(SPAR_ROOT)/thirdparty/boost/"
INCLUDEPATH += "$$(SPAR_ROOT)/thirdparty/glm/"

LIBS += "-L$$(SPAR_ROOT)/thirdparty/lib/"
LIBS += -lassimp -lzlibstatic -lboost_serialization -lboost_system

CONFIG(release, debug|release) {
    LIBS += "-L$$(SPAR_ROOT)/libs/lib-release" -lscene_models -lconfig
    spar_lib_path = "$$(SPAR_ROOT)/libs/lib-release"
}

CONFIG(debug, debug|release) {
    LIBS += "-L$$(SPAR_ROOT)/libs/lib-debug" -lscene_models -lconfig
    spar_lib_path = "$$(SPAR_ROOT)/libs/lib-debug"
}

PRE_TARGETDEPS += "$${spar_lib_path}/libscene_models.a"

SOURCES += main.cpp\
        mainwindow.cpp \
    assimptosamodels.cpp \
    geometries.cpp

HEADERS  += mainwindow.h \
    assimptosamodels.h \
    geometries.h

FORMS    += mainwindow.ui
