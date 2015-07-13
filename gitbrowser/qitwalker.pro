#-------------------------------------------------
#
# Project created by QtCreator 2014-10-24T14:46:55
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gitwalker
TEMPLATE = app

SOURCES += main.cpp\
    versionbrowserdialog.cpp \
    commitinfo.cpp \
    filesbrowser.cpp \
    syncdialog.cpp \
    gitmanager.cpp \
    texteditor.cpp \
    passwordprovider.cpp

HEADERS  += \
    versionbrowserdialog.h \
    commitinfo.h \
    filesbrowser.h \
    syncdialog.h \
    gitmanager.h \
    texteditor.h \
    passwordprovider.h

FORMS    += \
    versionbrowserdialog.ui \
    filesbrowser.ui \
    syncdialog.ui \
    texteditor.ui

OTHER_FILES +=

RESOURCES += \
    resources.qrc
