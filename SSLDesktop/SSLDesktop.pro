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
    SslApp/sslapp.cpp \
    SslBox/sslbox.cpp \
    SslApp/sslappmodel.cpp \
    SslBox/sslboxmodel.cpp

HEADERS  += \
    SslApp/sslapp.h \
    SslBox/sslbox.h \
    SslApp/sslappmodel.h \
    SslBox/sslboxmodel.h

FORMS    += mainwindow.ui \
    SslApp/sslapp.ui \
    SslBox/sslbox.ui
