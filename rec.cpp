/*******************************************************************
 * File:rec.cpp
 * Author:
 * Desciption:This is a cpp file for developers, including many
 *     circumstances you may encounter during development.
******************************************************************/

#include <QtGui>

#include "node.h"
#include "yuan.h"
#include "link.h"
#include "newnode.h"
#include "rec.h"
#include "diagramwindow.h"

Rec::Rec()
{
    box=new QComboBox;

    setAcceptDrops(true);  //目前没什么用

    identifier="Rec";
    controlsId=0;

    connect(box,SIGNAL(currentIndexChanged(int)),this,SLOT(showYuan()));
}

Rec::~Rec()
{
    delete box;
    //delete item;
}

/*******************************************************************
 * Function name: outlineRect()
 * Description: return a rect
 * Callee:
 * Inputs:
 * Outputs:QRectF
******************************************************************/
QRectF Rec::outlineRect() const
{
    QRectF rect(0,0,400,400);
    rect.translate(-rect.center());
    return rect;
}

/*******************************************************************
 * Function name: boundingRect()
 * Description: You can see the details of this function in
 *     ionode.cpp
******************************************************************/
QRectF Rec::boundingRect() const
{
    const int Margin = 6;
    return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

/*******************************************************************
 * Function name: shape()
 * Description: You can see the details of this function in
 *     ionode.cpp
******************************************************************/
QPainterPath Rec::shape() const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRoundRect(rect, roundness(rect.width()),
                      roundness(rect.height()));
    return path;
}

/*******************************************************************
 * Function name: paint()
 * Description: You can see the details of this function in
 *     computenode.cpp
******************************************************************/
void Rec::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget */)
{
    QPen pen(outlineColor());
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(backgroundColor());

    QRectF rect = outlineRect();
    painter->drawRoundRect(rect, roundness(rect.width()),
                           roundness(rect.height()));
}

/*******************************************************************
 * Function name: itemChange()
 * Description: You can see the details of this function in
 *     computenode.cpp
******************************************************************/
QVariant Rec::itemChange(GraphicsItemChange change,
                    const QVariant &value)
{
    if (change & ItemPositionHasChanged) {
        yuan2->setPos(pos().x() - outlineRect().width()/2 + item->boundingRect().width()/2,
                     pos().y() - outlineRect().height()/2 + item->boundingRect().height()*1.5);
        foreach (Link *link, yuan->myLinks)
        {link->trackYuans();update();}

        item->setPos(QPointF(pos().x()-outlineRect().width()/2,
                     (pos().y() - outlineRect().height()/2)));
    }
    return QGraphicsItem::itemChange(change, value);
}

/*******************************************************************
 * Function name: showYuan()
 * Description: This function show Yuan according to the logic
 *     function you choose. If you choose "if" or "while", Yuan will
 *     be shown, but if you choose "else", Yuan will not be shown.
******************************************************************/
void Rec::showYuan()
{
    int i=box->currentIndex();
    switch (i) {
    case 0:
    case 2:
    {
        yuan2->setVisible(true);
        break;
    }
    case 1:
    {
        yuan2->setVisible(false);
        break;
    }
    default:
        break;
    }
}


