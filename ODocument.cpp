#include "ODocument.h"
#include "ui_awindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>


aWindow::aWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aWindow)
{
    ui->setupUi(this);
    QFile aFile("Widget_Manual2.1.txt");
    if(!aFile.open(QFile::ReadOnly | QFile::Text))
       //QDebug() << "Can not open";
    QTextStream WidM(&aFile);
 //   ui->textBrowser->append(WidM.readline());


}

aWindow::~aWindow()
{
    delete ui;
}

void aWindow::on_actionNew_Window_triggered()
{
    aDialog = new QDialog(this);
    aDialog->setModal(false);
    aDialog->show();
}
