/********************************************************
 * File: widget.cpp
 * Author: Gao Jiefeng
 * Description:When the real widget code hadn't been
 *    finished, I have to replace that with something.
 *    This is a formally widget, only used to complet
 *    the grammar highlight, and has no real usage.
*********************************************************/

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}
