/*******************************************************************
 * File:vardefnode.cpp
 * Author: Ryan Feng
 * Description: This file includes the realization of class 
 *        VardefNode. VardefNode is a definition of variables.
******************************************************************/

#include "vardefnode.h"
#include "diagramwindow.h"
#include "varnode.h"
#include "yuan.h"
#include "link.h"

VardefNode::VardefNode()
{
    //yuan=new Yuan;
    //yuan2=new Yuan;
    myInt=0;
    myDouble=0.0;
    myString="NULL";
    identifier="VardefNode";
}

QPolygonF VardefNode::outlineRect() const
{
    QPolygonF poly;
    poly<<QPointF(10,16)<<QPointF(20,0)<<QPointF(10,-16)
       <<QPointF(-10,-16)<<QPointF(-20,0)<<QPointF(-10,16);
    return poly;
}

QRectF VardefNode::boundingRect() const
{
    return QRectF(-20,-20,40,40);
}

QPainterPath VardefNode::shape()  const
{
    QPolygonF poly=outlineRect();
    QPainterPath path;
    path.addPolygon(poly);
    return path;
}

void VardefNode::paint(QPainter *painter,
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

void VardefNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QString text;
    text = QInputDialog::getText(event->widget(),
                           tr("Edit Text"), tr("Enter new text:"),
                           QLineEdit::Normal, varName);
    if (text.isEmpty())return;

    varName=text;
    setText(tr("%1").arg(varName));

    if(this->node)
    {
        QString text1 = this->node->text();
        int x = QString::compare(text1, "int", Qt::CaseInsensitive);
        int y = QString::compare(text1, "double", Qt::CaseInsensitive);
        int z = QString::compare(text1, "string", Qt::CaseInsensitive);

        if(x==0)
        {
            int a=QInputDialog::getInt(event->widget(),
                                       tr("Edit value"), tr("Enter new value:"),
                                       QLineEdit::Normal, myInt);
            myInt=a;
            setText(tr("%1=%2").arg(text).arg(myInt));
        }
        else if(y==0)
        {
            double a=QInputDialog::getDouble(event->widget(),
                                       tr("Edit value"), tr("Enter new value:"),
                                       QLineEdit::Normal, myDouble);
            myDouble=a;
            setText(tr("%1=%2").arg(text).arg(myDouble));
        }
        else if(z==0)
        {
            QString a=QInputDialog::getText(event->widget(),
                                       tr("Edit value"), tr("Enter new value:"),
                                       QLineEdit::Normal, myString);
            myString=a;
            setText(tr("%1=%2").arg(varName).arg(myString));
        }
    }

}

QVariant VardefNode::itemChange(GraphicsItemChange change,
                    const QVariant &value)
{
    if (change & ItemPositionHasChanged) {
        yuan2->setPos(pos().x(),
                     pos().y() - 16 - yuan2->outlineRect().height()/2);
        foreach (Link *link, yuan2->myLinks)
        {link->trackYuans();update();}
        yuan->setPos(pos().x(),
                     pos().y() + 16 + yuan->boundingRect().height()/2);
        foreach (Link *link, yuan->myLinks)
        {link->trackYuans();update();}
    }
    return QGraphicsItem::itemChange(change, value);
}

