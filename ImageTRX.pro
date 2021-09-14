#-------------------------------------------------
#
# Project created by QtCreator 2021-09-11T17:49:02
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageTRX
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        openglwidget.cpp \
        server.cpp \
        client.cpp

HEADERS  += mainwindow.h \
        openglwidget.h \
        server.h \
        client.h

FORMS    += mainwindow.ui
