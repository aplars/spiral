#-------------------------------------------------
#
# Created manually 2015 :)
#
#-------------------------------------------------

QT       += core

TARGET = adt
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11

QMAKE_CXXFLAGS += -isystem $$(SPAR_ROOT)/thirdparty/boost/
INCLUDEPATH += "$$(SPAR_ROOT)/thirdparty/boost/"


HEADERS += lru.h \ 
    stringid.h \
    subject.h


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

