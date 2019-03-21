#-------------------------------------------------
#
# Project created by QtCreator 2018-11-07T20:39:35
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LAB_03_02_Notebook
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tablemodel.cpp \
    TempHashFunction.cpp \
    UserDataDialog.cpp

HEADERS  += mainwindow.h \
    HashMultiTable.h \
    HashTable.h \
    tablemodel.h \
    TempHashFunction.h \
    UserStruct.h \
    UserDataDialog.h
