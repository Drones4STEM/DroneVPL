#include "odescription.h"
#include "ui_odescription.h"

ODescription::ODescription(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ODescription)
{
    ui->setupUi(this);
}

ODescription::~ODescription()
{
    delete ui;
}

void ODescription::on_actionNew_Window_triggered()
{
    aDescription = new QDialog(this);
    aDescription->setModal(false);
    aDescription->show();
}
