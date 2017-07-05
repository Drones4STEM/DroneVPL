/*******************************************************************
 * File:newnode.cpp
 * Author:
 * Desciption:This is a cpp file for developers, including many
 *     circumstances you may encounter during development.
******************************************************************/

#include <QtGui>

#include "link.h"
#include "newnode.h"
#include "yuan.h"

NewNode::NewNode()
{
    myTextColor = Qt::darkGreen;
    myOutlineColor = Qt::darkBlue;
    myBackgroundColor = Qt::white;

    QGraphicsItem*p=dynamic_cast<QGraphicsItem*>(this);
    yuan=new triYuan(p);
    yuan2=new Yuan(p);
    yuan2->setInout(1);

    setFlags(ItemIsMovable | ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);

    identifier="NewNode";
    controlsId=0;
}

NewNode::~NewNode()
{
    delete yuan;
    delete yuan2;
}

void NewNode::setText(const QString &text)
{
    prepareGeometryChange();
    myText = text;
    update();
}

QString NewNode::text() const
{
    return myText;
}

void NewNode::setTextColor(const QColor &color)
{
    myTextColor = color;
    update();
}

QColor NewNode::textColor() const
{
    return myTextColor;
}

void NewNode::setOutlineColor(const QColor &color)
{
    myOutlineColor = color;
    update();
}

QColor NewNode::outlineColor() const
{
    return myOutlineColor;
}

void NewNode::setBackgroundColor(const QColor &color)
{
    myBackgroundColor = color;
    update();
}

QColor NewNode::backgroundColor() const
{
    return myBackgroundColor;
}

/*******************************************************************
 * Function name: boundingRect()
 * Description: You can see the details of this function in
 *     ionode.cpp
******************************************************************/
QRectF NewNode::boundingRect() const
{
    const int Margin = 4;
    return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

/*******************************************************************
 * Function name: shape()
 * Description: You can see the details of this function in
 *     ionode.cpp
******************************************************************/
QPainterPath NewNode::shape() const
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
void NewNode::paint(QPainter *painter,
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
    painter->drawRoundRect(rect, roundness(rect.width()),
                           roundness(rect.height()));

    painter->setPen(myTextColor);
    painter->drawText(rect, Qt::AlignCenter, myText);
}

/*******************************************************************
 * Function name: mouseDoubleClickEvent()
 * Description: You can see the details of this function in
 *     ionode.cpp
******************************************************************/
void NewNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QString text = QInputDialog::getText(event->widget(),
                           tr("Edit Text"), tr("Enter new text:"),
                           QLineEdit::Normal, myText);
    if (!text.isEmpty())
        setText(text);
}

/*******************************************************************
 * Function name: itemChange()
 * Description: You can see the details of this function in
 *     computenode.cpp
******************************************************************/
QVariant NewNode::itemChange(GraphicsItemChange change,
                    const QVariant &value)
{
    if (change & ItemPositionHasChanged) {
        yuan->setPos(pos().x(),
                     pos().y() + outlineRect().height()/2 + yuan->boundingRect().height()/2);
        foreach (Link *link, yuan->myLinks)
        {link->trackYuans();update();}
        yuan2->setPos(pos().x() - outlineRect().width()/2 - yuan2->outlineRect().width()/2,
                     pos().y());
        foreach (Link *link, yuan2->myLinks)
        {link->trackYuans();update();}

    }
    return QGraphicsItem::itemChange(change, value);
}

/*******************************************************************
 * Function name: outlineRect()
 * Description: return a rect
 * Callee:
 * Inputs:
 * Outputs:QRectF
******************************************************************/
QRectF NewNode::outlineRect() const
{
    const int Padding = 8;
    QFontMetricsF metrics = qApp->font();
    QRectF rect = metrics.boundingRect(myText);
    rect.adjust(-Padding, -Padding, +Padding, +Padding);
    rect.translate(-rect.center());
    return rect;
}

/*******************************************************************
 * Function name: roundness()
 * Description: return a fit roundness according to the width and
 *     height of a rectangle.
 * Callee:
 * Inputs: double size - size of a rectangle
 * Outputs:int roundness
******************************************************************/
int NewNode::roundness(double size) const
{
    const int Diameter = 12;
    return 100 * Diameter / int(size);
}

Yuan* NewNode::myYuan()const
{
    return yuan;
}
