#-------------------------------------------------
#
# Created manually 2015 :)
#
#-------------------------------------------------

QT       -= core 

TARGET = math
TEMPLATE = lib
CONFIG += staticlib


INCLUDEPATH += $$(BOOST_ROOT)/


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
    intersectiontests.h

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