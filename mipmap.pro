TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


LIBS += -lGL -lSDL -lSDL_image -lGLU
SOURCES += main.cpp\
        camera.cpp \
    vector3d.cpp

HEADERS  += camera.h \
    vector3d.h

DISTFILES +=

RESOURCES += \
    res.qrc

