#-------------------------------------------------
#
# Created manually 2015 :)
#
#-------------------------------------------------

QT       -= core 

TARGET = math
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14

INCLUDEPATH += $$(BOOST_ROOT)/
INCLUDEPATH += "$$(SPAR_ROOT)/thirdparty/glm/"


HEADERS += PlaneT.h \ 
    FrustumT.h \
    RectangleT.h \
    ScalarT.h \
    intersectiontests.h \
    vec3ext.h \
    mat4ext.h \
    sphereT.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

SOURCES += \
    PlaneT.inl \
