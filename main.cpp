/*******************************************************************
 * File: main.cpp
 * Author: Ryan Feng
 * Description: This is the main entrance of DroneVPL.
******************************************************************/
#include <QApplication>

#include "diagramwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    DiagramWindow view;
    view.show();
    return app.exec();
}
