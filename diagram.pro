INCLUDEPATH += ../DroneVPL/background/

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
    canvasdialog.h \
    odescription.h \
    odocument.h \
    scene.h \
    widgetcondition.h \
    view.h \
    widgetwrap.h \
    background/map_instrument.h \
    background/widgetmap.h \
    background/format.h \
    wqdragitem.h \

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
    canvasdialog.cpp \
    odescription.cpp \
    odocument.cpp \
    scene.cpp \
    widgetcondition.cpp \
    widgetwrap.cpp \
    background/map_instrument.cpp \
    background/widgetmap.cpp \
    background/format.cpp \
    wqdragitem.cpp \

FORMS         = propertiesdialog.ui \
    awindow.ui \
    canvasdialog.ui \
    odescription.ui \
    odocument.ui
RESOURCES     = resources.qrc \
    aqp/aqp.qrc

SUBDIRS += \
    aqp/aqp.pro

DISTFILES += \
    aqp/aqp.pro.user \
    aqp/mime.types \
    document.html
