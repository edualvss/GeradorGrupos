#-------------------------------------------------
#
# Project created by QtCreator 2017-08-30T20:14:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += static

TARGET = GeradorGrupos
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    recursos.qrc

# Win32 specific settings
win32 {
    QMAKE_TARGET_COMPANY = "CAU - Univali"
    QMAKE_TARGET_PRODUCT = GGCAU
    QMAKE_TARGET_DESCRIPTION = "Gerador de grupos do CAU"
    QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2017 Eduardo Alves da Silva"
    RC_ICONS = app.ico
}
