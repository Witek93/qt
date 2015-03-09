#-------------------------------------------------
#
# Project created by QtCreator 2015-03-09T14:33:36
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SSLDesktop
TEMPLATE = app


SOURCES += main.cpp\
    sslapp.cpp \
    sslbox.cpp

HEADERS  += \
    sslapp.h \
    sslbox.h

FORMS    += mainwindow.ui \
    sslapp.ui \
    sslbox.ui
