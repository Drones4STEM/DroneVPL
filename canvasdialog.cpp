/*******************************************************************
 * File:canvasdialog.cpp
 * Author: lxn
 * Description: This file includes the realization of class
 *        CanvasDialog.
******************************************************************/
#include <QtGui>
#include <QGraphicsView>
#include "canvasdialog.h"

CanvasDialog::CanvasDialog(QGraphicsView *aview,QWidget *parent):QDialog(parent)
{
    setupUi(this);
    view=aview;
    wspinBox->setValue(aview->width());
    hspinBox->setValue(aview->height());
    connect(buttonBox,SIGNAL(accepted()),this,SLOT(on_buttonBox_accepted()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
void CanvasDialog::on_buttonBox_accepted()
{
    if(comboBox->currentText()=="px")
    {
     view->setSceneRect(0,0,wspinBox->value(),hspinBox->value());
    }
    else if(comboBox->currentText()=="mm")
    {
    view->setSceneRect(0,0,((wspinBox->value())*1920/25.4),((hspinBox->value())*1080/25.4));
    }
}

