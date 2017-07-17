#include "odescription.h"
#include "ui_odescription.h"
#include <QLayout>


ODescription::ODescription(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ODescription)
{
    ui->setupUi(this);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(ui->textBrowser);
    setLayout(layout);
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
