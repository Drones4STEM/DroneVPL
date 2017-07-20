/*******************************************************************
 * File:yuan.cpp
 * Author: Ryan Feng
 * Description: This file includes the realization of class 
 *        Yuan and triYuan. They are "in"/"out" nodes in DroneVPL
******************************************************************/


#include <QtGui>

#include "link.h"
#include "yuan.h"
#include "node.h"

Yuan::Yuan(QGraphicsItem *parent)
{
    node=parent;
    myOutlineColor = Qt::darkBlue;
    myBackgroundColor = Qt::red;

    setFlags(ItemIsSelectable);
    m_mode=0;
}

Yuan::~Yuan()
{
    foreach (Link *link, myLinks)
        delete link;
}

void Yuan::setOutlineColor(const QColor &color)
{
    myOutlineColor = color;
    update();
}

QColor Yuan::outlineColor() const
{
    return myOutlineColor;
}

void Yuan::setBackgroundColor(const QColor &color)
{
    myBackgroundColor = color;
    update();
}

QColor Yuan::backgroundColor() const
{
    return myBackgroundColor;
}

void Yuan::addLink(Link *link)
{
    myLinks.insert(link);
}

void Yuan::removeLink(Link *link)
{
    myLinks.remove(link);
}

QRectF Yuan::boundingRect() const
{
    const int Margin=1;
    return outlineRect().adjusted(-Margin,-Margin,Margin,Margin);
}

QPainterPath Yuan::shape() const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRoundRect(rect, roundness(rect.width()),
                      roundness(rect.height()));
    return path;
}

void Yuan::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget */)
{
    QPen pen(myOutlineColor);
    if (option->state &QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(myBackgroundColor);

    QRectF rect = outlineRect();
    painter->drawRoundRect(rect, 99,
                           99);

    if(this->collidingItems().count()>=1&&dynamic_cast<specialYuan *>(this->collidingItems().last())!=0)
     {
        painter->setBrush(Qt::yellow);
        painter->drawRoundRect(rect);
     }

}

/*******************************************************************
 * Function name: outlineRect()
 * Description: return a rect
 * Callee:
 * Inputs:
 * Outputs:QRectF
******************************************************************/
QRectF Yuan::outlineRect() const
{
    QRectF rect(0,0,15,15);
    rect.translate(-rect.center());
    return rect;
}

int Yuan::roundness(double size) const
{
    const int Diameter = 1;
    return 100 * Diameter / int(size);
}

QGraphicsItem* Yuan::myNode() const
{
    return node;
}

/*******************************************************************
 * Function name: setInout()
 * Description: This function set the inout mode of a Yuan.
 * Callee:
 * Inputs: int mode - 1:in ; 0:out
 * Outputs:
******************************************************************/
void Yuan::setInout(int mode)
{
    m_mode=mode;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////////

triYuan::triYuan(QGraphicsItem *parent)
{
    node=parent;
    setOutlineColor(Qt::darkBlue);
    setBackgroundColor(Qt::red);

    setFlags(ItemIsSelectable);
    setInout(0);
 // connect(this,SIGNAL(signal()),this->parent(),SLOT(set_new_line()));
}

QPolygonF triYuan::outlineRect() const
{
    QPolygonF poly;
    poly<<QPointF(-10,-6)<<QPointF(10,-6)<<QPointF(0,11);
    return poly;
}

QRectF triYuan::boundingRect() const
{
    return QRectF(-10,-10,20,20);
}

QPainterPath triYuan::shape()  const
{
    QPolygonF poly=outlineRect();
    QPainterPath path;
    path.addPolygon(poly);
    return path;
}

void triYuan::paint(QPainter *painter,
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

}

specialYuan::specialYuan(QGraphicsItem *parent)
{
    node=parent;
    setOutlineColor(Qt::white);
    setBackgroundColor(Qt::white);

    setInout(0);
}

QRectF specialYuan::outlineRect() const
{
    QRectF rect(0,0,50,50);
    rect.translate(-rect.center());
    return rect;
}

QRectF specialYuan::boundingRect() const
{
    const int Margin=1;
    return outlineRect().adjusted(-Margin,-Margin,Margin,Margin);}

QPainterPath specialYuan::shape()  const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRoundRect(rect, roundness(rect.width()),
                      roundness(rect.height()));
    return path;
}

void specialYuan::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(myOutlineColor);
    if (option->state &QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(myBackgroundColor);

    QRectF rect = outlineRect();
    painter->drawRoundRect(rect, 99,
                           99);
}
