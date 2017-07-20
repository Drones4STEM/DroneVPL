/******************************************************************
* File: main.cpp
 * Author: Ryan Feng
 * Description: This is the main entrance of DroneVPL
******************************************************************/
#include <QApplication>

#include "diagramwindow.h"
#include <QFile>
#include "ODocument.h"


DiagramWindow* view;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DiagramWindow view1;
    view = &view1;
    view1.show();
    view1.show();
    return app.exec();
}
