#include "widget.h"
#include <QApplication>
#include <Qdebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    qDebug("hello");



    return a.exec();
}
