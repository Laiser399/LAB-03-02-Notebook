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
    common/TempHashFunction.cpp \
    UserDataDialog.cpp \
    SearchDataDialog.cpp

HEADERS  += \
    mainwindow.h \
    tablemodel.h \
    \
    common/TempHashFunction.h \
    common/HashMultiTable.h \
    common/HashTable.h \
    common/UserStruct.h \
    common/searchstruct.h \
    UserDataDialog.h \
    SearchDataDialog.h


