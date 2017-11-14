/*******************************************************************
 * File:varnode.cpp
 * Author: Ryan Feng
 * Description: This file includes the realization of class
 *        VarNode. VarNode represents variables read and written
 *        by other actions.
******************************************************************/


#include "varnode.h"
#include "diagramwindow.h"
#include "rec.h"

VarNode::VarNode(QGraphicsItem *parent)
    :QGraphicsObject(parent)
{
    varnum=1;              //初始产生一个变量
    button0 = new QPushButton;
    button0->setFixedSize(20,20);
    button0->setIcon(QIcon(":/images/icon/add.png"));

    int i=0;
    for(i=0;i<4;i++)
    {
        typeBox[i]=new QComboBox;
        typeBox[i]->setFixedSize(86,28);
        nameEdit[i]=new QLineEdit;
        nameEdit[i]->setFixedSize(46,25);
        valueEdit[i]=new QLineEdit;
        valueEdit[i]->setFixedSize(48,25);
        button[i] = new QPushButton;
        button[i]->setFixedSize(20,20);
        button[i]->setIcon(QIcon(":/images/icon/generate.png"));
    }

    setFlags(ItemIsMovable|ItemIsSelectable|ItemSendsGeometryChanges);

    connect(button0,SIGNAL(clicked()),this,SLOT(addVarType()));
    for(int i=0;i<4;i++)
        connect(button[i],SIGNAL(clicked()),this,SLOT(emitSignal()));
}


VarNode::~VarNode()
{
    /*delete button0;
    delete typeBox1;
    delete nameEdit1;
    delete valueEdit1;
    delete button1;*/
    delete button0;
    int i=0;
    for(i=0;i<4;i++)
    {
        delete typeBox[i];
        delete nameEdit[i];
        delete valueEdit[i];
        delete button[i];
    }
}

QRectF VarNode::outlineRect() const
{
    QRectF rect1(0,0,230,70);
    rect1.translate(-rect1.center());
    QRectF rect2(0,0,230,100);
    rect2.translate(-rect2.center());
    QRectF rect3(0,0,230,130);
    rect3.translate(-rect3.center());
    QRectF rect4(0,0,230,160);
    rect4.translate(-rect4.center());
    switch (varnum) {
    case 1:
        return rect1;
        break;
    case 2:
        return rect2;
        break;
    case 3:
        return rect3;
        break;
    case 4:
        return rect4;
    default:
        break;
    }
}


QRectF VarNode::boundingRect() const
{
//    this->high = outlineRect().height();
//    this->wide = outlineRect().width();
    return outlineRect();
}

QPainterPath VarNode::shape() const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRoundRect(rect,roundness(rect.width()),
                      roundness(rect.height()));
    return path;
}

void VarNode::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         QWidget *widget)
{
    QPen pen(Qt::darkBlue);
    if(option->state&QStyle::State_Selected)
    {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(QColor(230,230,230));

    QRectF rect = outlineRect();
    painter->drawRoundedRect(rect,roundness(rect.width()),
                             roundness(rect.height()));

    pen.setColor(Qt::black);
    painter->setPen(pen);
    QFont font("MicrosoftYaHei");
    font.setPixelSize(14);
    painter->setFont(font);
    QString str1("增加变量");
    QString str2("名称");
    QString str3("初始值");
    painter->drawText(rect.topLeft().x()+33,rect.topLeft().y()+15,str1);
    painter->drawText(rect.topLeft().x()+104,rect.topLeft().y()+15,str2);
    painter->drawText(rect.topLeft().x()+157,rect.topLeft().y()+15,str3);

    pen.setColor(QColor(151,151,151,255));
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->drawLine(rect.topLeft().x()+5,rect.topLeft().y()+29,
                      rect.topRight().x()-5,rect.topRight().y()+29);

}


int VarNode::roundness(double size) const
{
    const int Diameter = 12;
    return 100 * Diameter / (abs(int(size))+1);
}

void VarNode::addVarType()
{
    if(varnum==4)
        return;
    varnum++;

    QRectF rect = outlineRect();
    button0Item->setPos(-108,rect.topLeft().y() + 5);
    for(int i=0;i<4;i++)
    {
        typeItem[i]->setPos(-110,rect.topLeft().y() + 34 + 32*i);
        nameItem[i]->setPos(-15,rect.topLeft().y() + 34 +32*i);
        valueItem[i]->setPos(41,rect.topLeft().y() + 34 + 32*i);
        buttonItem[i]->setPos(92,rect.topLeft().y() + 34 + 32*i);
    }

    typeItem[varnum-1]->setVisible(true);
    nameItem[varnum-1]->setVisible(true);
    valueItem[varnum-1]->setVisible(true);
    buttonItem[varnum-1]->setVisible(true);

    update();
}

void VarNode::emitSignal()
{
    int i;
    QPushButton *pushButton = dynamic_cast<QPushButton*>(sender());
    for(i = 0;i<4;i++)
    {
        if(pushButton == button[i])
            break;
    }
    emit addVarSignal(this,typeBox[i]->currentText(),nameEdit[i]->text(),valueEdit[i]->text());
}






