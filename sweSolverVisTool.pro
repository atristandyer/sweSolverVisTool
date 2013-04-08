#-------------------------------------------------
#
# Project created by QtCreator 2013-03-31T14:02:59
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = sweSolverVisTool
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    TerrainLayer.cpp \
    glew.c \
    GLShader.cpp \
    GLCamera.cpp \
    GLPanel.cpp \
    Dialog_Open.cpp \
    WaterLayer.cpp

HEADERS  += MainWindow.h \
    TerrainLayer.h \
    adcData.h \
    glew.h \
    GLData.h \
    GLShader.h \
    GLCamera.h \
    GLPanel.h \
    Dialog_Open.h \
    WaterLayer.h

FORMS    += MainWindow.ui \
    Dialog_Open.ui

OTHER_FILES += \
    colorVertexShader.vert \
    colorFragmentShader.frag \
    blackVertexShader.vert \
    blackFragmentShader.frag \
    colorVertexShader2.vert \
    colorFragmentShader2.frag
