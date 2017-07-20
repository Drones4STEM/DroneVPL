#include "odocument.h"
#include "ui_odocument.h"
#include <QLayout>

oDocument::oDocument(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::oDocument)
{
    ui->setupUi(this);
    QHBoxLayout *layout = new QHBoxLayout;
    setGeometry(720,400,1400,800);
    layout->addWidget(ui->textBrowser);
    setLayout(layout);
}

oDocument::~oDocument()
{
    delete ui;
}
