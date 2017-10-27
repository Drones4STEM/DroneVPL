/*******************************************************************
 * File:varnode.cpp
 * Author: Ryan Feng
 * Description: This file includes the realization of class 
 *        VarNode. VarNode represents variables read and written 
 *        by other actions.
******************************************************************/


#include "varnode.h"
#include "diagramwindow.h"
//#include "colordlg.h"

#include "rec.h"

VarNode::VarNode()
{
    box=new QComboBox;
    num=-1;
    for(int i=0;i<6;i++)
    {
        flags[i]=false;
        array[i]=new VardefNode;
        //array[i]->setFlag(ItemIsMovable,false);
    }

    identifier="VarNode";
    rank = 0;
    sethw();
}

VarNode::~VarNode()
{/*
    for(int i=0;i<6;i++)
        delete array[i];*/
    ;
}

/*******************************************************************
 * Function name: outlineRect()
 * Description: return a rect
 * Callee:
 * Inputs:
 * Outputs:QRectF
******************************************************************/
QPolygonF VarNode::outlineRect() const
{
    QPolygonF poly;
    poly<<QPointF(10,16)<<QPointF(20,0)<<QPointF(10,-16)
       <<QPointF(-10,-16)<<QPointF(-20,0)<<QPointF(-10,16);

    return poly;
}

QRectF VarNode::boundingRect() const
{
    return QRectF(-20,-20,40,40);
}

QPainterPath VarNode::shape()  const
{
    QPolygonF poly=outlineRect();
    QPainterPath path;
    path.addPolygon(poly);
    return path;
}

void VarNode::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(outlineColor());
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(backgroundColor());

    QPolygonF poly = outlineRect();
    painter->drawPolygon(poly);
    QRectF rect=boundingRect();

    painter->setPen(textColor());
    painter->drawText(rect, Qt::AlignCenter, text());
}

QVariant VarNode::itemChange(GraphicsItemChange change,
                    const QVariant &value)
{
    if (change & ItemPositionHasChanged)
    {
        //if(this->collidingItems().isEmpty()||(this->collidingItems().count()==1&&dynamic_cast<Rec *>(this->collidingItems().first())!=0) )
       //{
            int static a=0;
            for(a=0;a<6;a++)
            {
                qDebug()<<a;
                if(flags[a])
                {
                    qDebug()<<flags[a];
                    qDebug()<<array[a];
                    int i=a%3;
                    int j;
                    if(a==0||a==2)j=-17;
                    else if(a==3||a==5)j=17;
                    else if(a==1)j=-35;
                    else j=35;
                    array[a]->setPos(pos().x() + (1-i)*30,
                                     pos().y() + j);
                }
            }
        /*if(flags[0])
            array[0]->setPos(pos().x(),pos().y()+20);
        qDebug()<<flags[0];*/

       /*}
        else
        {
            qDebug()<<false;
            if(flags[0])
            {
                int i=0%3;
                int j=-17;
                setPos(array[0]->x()- (1-i)*30,array[0]->y()-j);
            }
        }*/
    }
    return QGraphicsItem::itemChange(change, value);
}

/*void VarNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem *> items = this->collidingItems();
    int itemsCount = items.count();
    qDebug()<<itemsCount;
    for(int i=0;i<items.count();i++)
    {
        if(dynamic_cast<IoSmallNode*>(items[i]))
            itemsCount--;
    }
    if(itemsCount>0)
    {
        for(int i=0;i<items.count();i++)
        {
            if(!dynamic_cast<IoSmallNode*>(items[i])&&!dynamic_cast<Link*>(items[i])
                    &&!dynamic_cast<Yuan*>(items[i])&&!dynamic_cast<triYuan*>(items[i])
                    &&!dynamic_cast<QGraphicsProxyWidget*>(items[i])&&!dynamic_cast<ComputeSmallNode*>(items[i]))
            {
                double dx = this->pos().x() - items[i]->pos().x();
                double dy = this->pos().y() - items[i]->pos().y();
                double a = items[i]->boundingRect().width()/items[i]->boundingRect().height();
                if((abs(dx)/a>abs(dy)&&dx<=0))    //放在左边
                    setPos(items[i]->pos().x() - items[i]->boundingRect().width()/2 - boundingRect().width()/2 - 40,
                           items[i]->pos().y());
                else if(abs(dy)>abs(dx)/a&&dy>=0) //放在下边
                    setPos(items[i]->pos().x(),
                           items[i]->pos().y() + items[i]->boundingRect().height()/2 + boundingRect().height()/2 + 40);
                else if(abs(dx)/a>abs(dy)&&dx>=0)  //放在右边
                    setPos(items[i]->pos().x() + items[i]->boundingRect().width()/2 + boundingRect().width()/2 + 40,
                           items[i]->pos().y());
                else                               //放在上边
                    setPos(items[i]->pos().x(),
                           items[i]->pos().y() - items[i]->boundingRect().height()/2 - boundingRect().height()/2 - 40);
                break;
            }
        }
    }
    QGraphicsItem::mouseReleaseEvent(event);
}*/





VariableNode::VariableNode(QGraphicsItem *parent)
    :QGraphicsObject(parent)
{
    varnum=1;              //初始产生一个变量
    button0 = new QPushButton;
    button0->setFixedSize(10,10);

    int i=0;
    for(i=0;i<4;i++)
    {
        typeBox[i]=new QComboBox;
        typeBox[i]->setFixedSize(60,26);
        nameEdit[i]=new QLineEdit;
        nameEdit[i]->setFixedSize(40,26);
        valueEdit[i]=new QLineEdit;
        valueEdit[i]->setFixedSize(40,26);
        button[i] = new QPushButton;
        button[i]->setFixedSize(10,10);
    }

    setFlags(ItemIsMovable|ItemIsSelectable|ItemSendsGeometryChanges);

    connect(button0,SIGNAL(clicked()),this,SLOT(addVarType()));
    for(int i=0;i<4;i++)
        connect(button[i],SIGNAL(clicked()),this,SLOT(emitSignal()));
}


VariableNode::~VariableNode()
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

QRectF VariableNode::outlineRect() const
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


QRectF VariableNode::boundingRect() const
{
    return outlineRect();
}

QPainterPath VariableNode::shape() const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRoundRect(rect,roundness(rect.width()),
                      roundness(rect.height()));
    return path;
}

void VariableNode::paint(QPainter *painter,
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
    painter->drawText(rect.topLeft().x()+30,rect.topLeft().y()+15,str1);
    painter->drawText(rect.topLeft().x()+90,rect.topLeft().y()+15,str2);
    painter->drawText(rect.topLeft().x()+150,rect.topLeft().y()+15,str3);

    pen.setColor(QColor(151,151,151,255));
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->drawLine(rect.topLeft().x()+5,rect.topLeft().y()+29,
                      rect.topRight().x()-5,rect.topRight().y()+29);

}


int VariableNode::roundness(double size) const
{
    const int Diameter = 12;
    return 100 * Diameter / (abs(int(size))+1);
}

void VariableNode::addVarType()
{
    if(varnum==4)
        return;
    varnum++;

    QRectF rect = outlineRect();
    button0Item->setPos(-110,rect.topLeft().y() + 5);
    for(int i=0;i<4;i++)
    {
        typeItem[i]->setPos(-110,rect.topLeft().y() + 42 + 30*i);
        nameItem[i]->setPos(-30,rect.topLeft().y() + 42 +30*i);
        valueItem[i]->setPos(60,rect.topLeft().y() + 42 + 30*i);
        buttonItem[i]->setPos(100,rect.topLeft().y() + 47 + 30*i);
    }

    typeItem[varnum-1]->setVisible(true);
    nameItem[varnum-1]->setVisible(true);
    valueItem[varnum-1]->setVisible(true);
    buttonItem[varnum-1]->setVisible(true);

    update();
}

void VariableNode::emitSignal()
{
    int i;
    QPushButton *pushButton = dynamic_cast<QPushButton*>(sender());
    for(i = 0;i<4;i++)
    {
        if(pushButton == button[i])
            break;
    }
    qDebug()<<i<<"heihei";
    emit addVarSignal(this,typeBox[i]->currentText(),nameEdit[i]->text(),valueEdit[i]->text());
}



VarSmallNode::VarSmallNode(QGraphicsItem *parent)
    :NewNode(parent)
{
    varValue = 0;
    varName = "";
    varType = "int";
}

QRectF VarSmallNode::outlineRect() const
{
    /*QRectF rect(1,0,26,26);
    rect.translate(-rect.center());
    return rect;*/
    const int Padding = 8;
    QFontMetrics metrics = qApp->font();
    QRectF rect = metrics.boundingRect(varName);
    rect.adjust(-Padding,-Padding,Padding,Padding);
    rect.translate(-rect.center());
    return rect;
}

QRectF VarSmallNode::boundingRect() const
{
    return outlineRect();
}

QPainterPath VarSmallNode::shape() const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRoundRect(rect,roundness(rect.width()),
                      roundness(rect.height()));
    return path;
}

void VarSmallNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(outlineColor());
        if (option->state &QStyle::State_Selected) {
            pen.setStyle(Qt::DotLine);
            pen.setWidth(2);
        }
        painter->setPen(pen);
        painter->setBrush(backgroundColor());

        QRectF rect = outlineRect();
        painter->drawRoundRect(rect, roundness(rect.width()),
                               roundness(rect.height()));

        painter->setPen(textColor());
        painter->drawText(rect, Qt::AlignCenter, varName);
}

QVariant VarSmallNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change & ItemPositionHasChanged){
        yuan->setPos(pos().x() + outlineRect().width()/2 + yuan->boundingRect().width()/2,
                     pos().y());
        foreach (Link *link, yuan->myLinks)
        {link->trackYuans();update();}
        yuan2->setPos(pos().x() - outlineRect().width()/2 - yuan->boundingRect().width()/2,
                     pos().y());
        foreach (Link *link, yuan2->myLinks)
        {link->trackYuans();update();}
    }
    return QGraphicsItem::itemChange(change, value);
}


