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
    num=0;
    for(int i=0;i<6;i++)
    {
        flags[i]=false;
        array[i]=new VardefNode;
        array[i]->setFlag(ItemIsMovable,false);
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
{ if (change & ItemPositionHasChanged){
        if(this->collidingItems().isEmpty()||(this->collidingItems().count()==1&&dynamic_cast<Rec *>(this->collidingItems().first())!=0) )
       {
            int a=0;
            for(;a<6;a++)
            {
                if(flags[a])
                {
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
       }
        else{
            if(flags[0])
            {
                int i=0%3;
                int j=-17;
                setPos(array[0]->x()- (1-i)*30,array[0]->y()-j);
            }
        }}
    return QGraphicsItem::itemChange(change, value);
}
