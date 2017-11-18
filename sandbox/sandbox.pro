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
CONFIG += c++14

QMAKE_CXXFLAGS += -isystem $$(SPAR_ROOT)/thirdparty/boost/
QMAKE_CXXFLAGS += -fopenmp


INCLUDEPATH += "$(SPAR_ROOT)/code"
INCLUDEPATH += "$$(SPAR_ROOT)/thirdparty/boost"
INCLUDEPATH += "$$(SPAR_ROOT)/thirdparty/glm/"
INCLUDEPATH += "$$(SPAR_ROOT)/thirdparty/json/src/"

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
    glwidget.cpp \
    globalsettingsform.cpp \
    models/globalsettingsmodel.cpp \
    addentitiesform.cpp \
    models/entities_tree_model/entityfactoryitemmodel.cpp \
    models/entities_tree_model/entityfactorygroupnode.cpp \
    models/entities_tree_model/entityfactoryleafnode.cpp \
    models/entities_tree_model/entityfactorymodel.cpp \
    models/entities_tree_model/entityfactorynode.cpp \
    models/entities_tree_model/entityfactoryrootnode.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    globalsettingsform.h \
    models/globalsettingsmodel.h \
    addentitiesform.h \
    models/entities_tree_model/entityfactoryitemmodel.h \
    models/entities_tree_model/entityfactorygroupnode.h \
    models/entities_tree_model/entityfactoryleafnode.h \
    models/entities_tree_model/entityfactorymodel.h \
    models/entities_tree_model/entityfactorynode.h \
    models/entities_tree_model/entityfactoryrootnode.h

FORMS    += mainwindow.ui \
    globalsettingsform.ui \
    addentitiesform.ui

CONFIG += mobility
CONFIG += c++11

MOBILITY = 

DISTFILES += \
    ../sa_config.conf \
    ../data/classes/trees/tree.json \
    ../data/classes/test/test.json

