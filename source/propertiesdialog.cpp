/*******************************************************************
 * File:propertiesdialog.cpp
 * Author: Ryan Feng
 * Description: This file includes the realization of class 
 *        PropertiesDialog. PropertiesDialog is the dialog opened 
 *        after double click on widgets. (Double clicking is 
 *        currently disabled)
******************************************************************/
#include <QtGui>

#include "node.h"
#include "newnode.h"
#include "propertiesdialog.h"

PropertiesDialog::PropertiesDialog(Node *node, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    this->node = node;
    this->newnode=0;
    xSpinBox->setValue(int(node->x()));
    ySpinBox->setValue(int(node->y()));
    textLineEdit->setText(node->text());
    textColor = node->textColor();
    outlineColor = node->outlineColor();
    backgroundColor = node->backgroundColor();

    updateColorLabel(outlineColorLabel, outlineColor);
    updateColorLabel(backgroundColorLabel, backgroundColor);
    updateColorLabel(textColorLabel, textColor);
}

PropertiesDialog::PropertiesDialog(NewNode *newnode, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    this->newnode = newnode;
    this->node=0;
    xSpinBox->setValue(int(newnode->x()));
    ySpinBox->setValue(int(newnode->y()));
    textLineEdit->setText(newnode->text());
    textColor = newnode->textColor();
    outlineColor = newnode->outlineColor();
    backgroundColor = newnode->backgroundColor();

    updateColorLabel(outlineColorLabel, outlineColor);
    updateColorLabel(backgroundColorLabel, backgroundColor);
    updateColorLabel(textColorLabel, textColor);
}

void PropertiesDialog::on_buttonBox_accepted()
{
    if(node){
        node->setPos(xSpinBox->value(), ySpinBox->value());
        node->setText(textLineEdit->text());
        node->setOutlineColor(outlineColor);
        node->setBackgroundColor(backgroundColor);
        node->setTextColor(textColor);
        node->update();
        QDialog::accept();
    }
    else
    {
        newnode->setPos(xSpinBox->value(), ySpinBox->value());
        newnode->setText(textLineEdit->text());
        newnode->setOutlineColor(outlineColor);
        newnode->setBackgroundColor(backgroundColor);
        newnode->setTextColor(textColor);
        newnode->update();
        QDialog::accept();
    }
}

void PropertiesDialog::on_textColorButton_clicked()
{
    chooseColor(textColorLabel, &textColor);
}

void PropertiesDialog::on_outlineColorButton_clicked()
{
    chooseColor(outlineColorLabel, &outlineColor);
}

void PropertiesDialog::on_backgroundColorButton_clicked()
{
    chooseColor(backgroundColorLabel, &backgroundColor);
}

void PropertiesDialog::updateColorLabel(QLabel *label,
                                        const QColor &color)
{
    QPixmap pixmap(16, 16);
    pixmap.fill(color);
    label->setPixmap(pixmap);
}

void PropertiesDialog::chooseColor(QLabel *label, QColor *color)
{
    QColor newColor = QColorDialog::getColor(*color, this);
    if (newColor.isValid()) {
        *color = newColor;
        updateColorLabel(label, *color);
    }
}
