/*******************************************************************
 * File:VarInstancenode.cpp
 * Author: Ryan Feng
 * Description: This file includes the realization of class
 *        VarInstanceNode. VarInstanceNode is a definition of variables.
******************************************************************/

#include "varinstancenode.h"
#include "diagramwindow.h"
#include "varnode.h"
#include "yuan.h"
#include "link.h"
#include "rec.h"

VarInstanceNode::VarInstanceNode(QGraphicsItem *parent)
    :NewNode(parent)
{
    varValue = 0;
    varName = "";
    varType = "int";
}

QRectF VarInstanceNode::outlineRect() const
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

QRectF VarInstanceNode::boundingRect() const
{
//    this->high = outlineRect().height();
//    this->wide = outlineRect().width();
    return outlineRect();
}

QPainterPath VarInstanceNode::shape() const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRoundRect(rect,roundness(rect.width()),
                      roundness(rect.height()));
    return path;
}

void VarInstanceNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

QVariant VarInstanceNode::itemChange(GraphicsItemChange change, const QVariant &value)
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
