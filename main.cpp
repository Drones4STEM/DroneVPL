/*******************************************************************
 * File:main.cpp
 * Author:
 * Desciption:This is a cpp file for developers, including many
 *     circumstances you may encounter during development.
******************************************************************/
#include <QApplication>

#include "diagramwindow.h"
#include <QFile>
#include "ODocument.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DiagramWindow *view;
    view = new DiagramWindow;
    view->show();



    return app.exec();
}
