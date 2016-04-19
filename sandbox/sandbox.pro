#-------------------------------------------------
#
# Project created by QtCreator 2015-01-23T22:24:06
#
#-------------------------------------------------

QT       += core network gui
QT       += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sandbox
TEMPLATE = app
CONFIG += c++11

QMAKE_CXXFLAGS += -isystem $$(SPAR_ROOT)/thirdparty/boost/
QMAKE_CXXFLAGS += -fopenmp


INCLUDEPATH += "$(SPAR_ROOT)/code"
INCLUDEPATH += "$$(SPAR_ROOT)/thirdparty/boost"

LIBS += "-L$$(SPAR_ROOT)/thirdparty/lib/"
CONFIG( debug, debug|release ) {
    LIBS += "-L$$(SPAR_ROOT)/libs/lib-debug"
    spar_lib_path = "$$(SPAR_ROOT)/libs/lib-debug"
}
else {
    LIBS += "-L$$(SPAR_ROOT)/libs/lib-release"
    spar_lib_path = "$$(SPAR_ROOT)/libs/lib-release"
}
message($${spar_lib_path})
LIBS += -fopenmp
LIBS += -lrenderer_engine -lscene_engine -lscene_models -lconfig -ladt
LIBS += -lboost_serialization -lboost_system
PRE_TARGETDEPS += "$${spar_lib_path}/librenderer_engine.a"
PRE_TARGETDEPS += "$${spar_lib_path}/libscene_engine.a"
PRE_TARGETDEPS += "$${spar_lib_path}/libscene_models.a"

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h

FORMS    += mainwindow.ui

CONFIG += mobility
CONFIG += c++11

MOBILITY = 

