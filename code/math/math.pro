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
    Matrix33T.h \
    Matrix44T.h \
    QuaternionT.h \
    RectangleT.h \
    ScalarT.h \
    Vector2T.h \
    Vector3T.h \
    Vector4T.h \
    intersectiontests.h \
    sphere.h \
    vec3ext.h \
    mat4ext.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

SOURCES += \
    Matrix33T.inl \
    Matrix44T.inl \
    PlaneT.inl \
    Vector2T.inl \
    Vector3T.inl \
    Vector4T.inl
