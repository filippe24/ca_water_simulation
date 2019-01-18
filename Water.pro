
TEMPLATE = app
TARGET = Water

QT += gui opengl

CONFIG += c++11

INCLUDEPATH += .

# Input
HEADERS += glwidget.h mainwindow.h \
    trianglemesh.h \
    plyreader.h \
    watergrid.h
FORMS += mainwindow.ui
SOURCES += glwidget.cpp main.cpp mainwindow.cpp \
    trianglemesh.cpp \
    plyreader.cpp \
    watergrid.cpp

DISTFILES += \
    shaders/particle.frag \
    shaders/particle.vert \
    shaders/water.frag \
    shaders/water.vert

RESOURCES += \
    resources.qrc
