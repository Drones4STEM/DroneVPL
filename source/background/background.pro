#-------------------------------------------------
#
# Project created by QtCreator 2017-03-12T10:49:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = background
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


SOURCES += main.cpp\
        widget.cpp \
    widgetmap.cpp \
    logic_instrument.cpp \
    map_instrument.cpp \
    digraph.cpp \
    format.cpp\
    ../diagram1/combobox.cpp \
    ../diagram1/diagramwindow.cpp \
    ../diagram1/judgerec.cpp \
    ../diagram1/link.cpp \
    ../diagram1/main.cpp \
    ../diagram1/newnode.cpp \
    ../diagram1/node.cpp \
    ../diagram1/propertiesdialog.cpp \
    ../diagram1/rec.cpp \
    ../diagram1/takeoffnode.cpp \
    ../diagram1/vardefnode.cpp \
    ../diagram1/varnode.cpp \
    ../diagram1/yuan.cpp

HEADERS  += widget.h \
    widgetmap.h \
    logic_instrument.h \
    map_instrument.h \
    format.h \
    digraph.h\
    ../diagram1/combobox.h \
    ../diagram1/diagramwindow.h \
    ../diagram1/judgerec.h \
    ../diagram1/link.h \
    ../diagram1/newnode.h \
    ../diagram1/node.h \
    ../diagram1/propertiesdialog.h \
    ../diagram1/rec.h \
    ../diagram1/recwidget.h \
    ../diagram1/takeoffnode.h \
    ../diagram1/vardefnode.h \
    ../diagram1/varnode.h \
    ../diagram1/yuan.h

FORMS    += widget.ui
