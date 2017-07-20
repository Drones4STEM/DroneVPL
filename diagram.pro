TEMPLATE      = app
HEADERS       = diagramwindow.h \
                link.h \
                node.h \
                propertiesdialog.h \
    yuan.h \
    newnode.h \
    rec.h \
    takeoffnode.h \
    varnode.h \
    vardefnode.h \
    computenode.h \
    ionode.h \
    itemtypes.h \
    aqp/alt_key.hpp \
    aqp/aqp.hpp \
    aqp/kuhn_munkres.hpp \
    scene.h \
    widgetcondition.h
SOURCES       = diagramwindow.cpp \
                link.cpp \
                main.cpp \
                node.cpp \
                propertiesdialog.cpp \
    yuan.cpp \
    newnode.cpp \
    rec.cpp \
    takeoffnode.cpp \
    varnode.cpp \
    vardefnode.cpp \
    computenode.cpp \
    ionode.cpp \
    aqp/alt_key.cpp \
    aqp/aqp.cpp \
    aqp/kuhn_munkres.cpp \
    scene.cpp \
    widgetcondition.cpp
FORMS         = propertiesdialog.ui
RESOURCES     = resources.qrc \
    aqp/aqp.qrc

SUBDIRS += \
    aqp/aqp.pro

DISTFILES += \
    aqp/aqp.pro.user \
    aqp/mime.types
