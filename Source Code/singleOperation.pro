#-------------------------------------------------
#
# Project created by QtCreator 2015-05-29T01:39:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = singleOperation
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Executor.cpp \
    Loader.cpp \
    MemoryManager.cpp \
    NIXBPE.cpp \
    Registers.cpp \
    Specification.cpp

HEADERS  += mainwindow.h \
    Executor.h \
    Loader.h \
    MemoryManager.h \
    NIXBPE.h \
    Registers.h \
    Specification.h

FORMS    += mainwindow.ui

RESOURCES += \
    myresources.qrc

DISTFILES += \
    application.rc

RC_FILE += \
    application.rc
