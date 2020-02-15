#-------------------------------------------------
#
# Project created by QtCreator 2020-02-12T17:05:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FolderMonitor
TEMPLATE = app


SOURCES += main.cpp\
        FolderMonitorView.cpp \
    FolderMonitorModel.cpp

HEADERS  += FolderMonitorView.h \
    FolderInforWorkerPrivate.h \
    FolderMonitorModel.h
