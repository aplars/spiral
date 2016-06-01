#-------------------------------------------------
#
# Created manually 2015 :)
#
#-------------------------------------------------

QT       += core
QT       += network
TARGET = config
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14

QMAKE_CXXFLAGS += -isystem $$(SPAR_ROOT)/thirdparty/boost/
INCLUDEPATH += "$$(SPAR_ROOT)/thirdparty/boost/"


HEADERS += lexer.h \ 
    directoryhelpers.h \
    parser.h \
    token.h \
    config.h

SOURCES += \
    lexer.cpp \
    directoryhelpers.cpp \
    parser.cpp \
    token.cpp \
    config.cpp


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

