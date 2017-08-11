/*******************************************************************
 * File: main.cpp
 * Author: Ryan Feng
 * Description: This is the main entrance of DroneVPL.
******************************************************************/
#include <QApplication>

#include "diagramwindow.h"
#include "widgetmap.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(app.translate("main", "Page Designer"));
    app.setOrganizationName("Qtrac Ltd.");
    app.setOrganizationDomain("qtrac.eu");
    app.setWindowIcon(QIcon(":/images/fileopen.png"));

    DiagramWindow *mainWin=new DiagramWindow;
    mainWin->show();
    return app.exec();
}
