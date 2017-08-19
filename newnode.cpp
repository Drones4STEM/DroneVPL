/*******************************************************************
 * File:newnode.cpp
 * Author: Ryan Feng
 * Description: This file includes the realization of class NewNode.
 *        NewNode is the parent class of IoNode, IoSmallNode, Rec,
 *        LandonNode, TranslationNode, SomeNode, VardefNode.
******************************************************************/

#include <QtGui>

#include "link.h"
#include "newnode.h"
#include "yuan.h"

/*******************************************************************
 * Function name: NewNode()
 * Description: This is a constructor of NewNode class.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
NewNode::NewNode()
{
    myTextColor = Qt::darkGreen;
    myOutlineColor = Qt::darkBlue;
    myBackgroundColor = Qt::white;

    //QGraphicsItem*p=dynamic_cast<QGraphicsItem*>(this);
    yuan=new triYuan();
    yuan2=new Yuan();
    yuan2->setInout(1);

    setFlags(ItemIsMovable | ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);

    identifier="NewNode";
    controlsId=0;

}

/*******************************************************************
 * Function name: ~NewNode()
 * Description: This is a destructor of NewNode class
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
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
    sethw();
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
/*
bool NewNode::set_controlsId(int id)
{
    if(controlsId = id) return true;
    else return false;
}
bool NewNode::set_identifier(QString idtf)
{
    identifier = idtf;
    if(identifier != "") return true;
    else return false;
}
bool NewNode::set_name()
{
    QString id = QString::number(controlsId,10);
    this->name = identifier + id;
    if(name!="") return true;
    else return false;
}
bool NewNode::set_master(WidgetWrap* w)
{
    yuan->master = w;
    yuan2->master = w;
    return true;
}*/
/*******************************************************************
 * Function name: boundingRect()
 * Description: This function defines the outer bounds of the item
 *     as a rectangle; all painting must be restricted to inside an
 *     item's bounding rect. This function overwrites
 *     IoNode::boundingRect()
 * Callee:
 * Inputs:
 * Outputs:QRectF
******************************************************************/
QRectF NewNode::boundingRect() const
{
    const int Margin = 4;
    return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

/*******************************************************************
 * Function name: shape()
 * Description: This function returns the shape of this item as a
 *     QPainterPath in local coordinates. The shape is used for many
 *     things, including collision detection, hit tests, and for the
 *     QGraphicsScene::items() functions.
 * Callee:
 * Inputs:
 * Outputs:QRectF
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
 * Description: This function  paints the contents of an item in
 *     local coordinates.
 * Callee: QPen::pen(), QPainter::setPen(), QPainter::setBrush()
 *         QPainter::drowRoundRect(), QPainter::drawText()
 * Inputs: QPainter paint
 *         QStyleOptionGraphicsItem *option - provides style options
 *             for the item, such as its state, exposed area and
 *             its level-of-detail hints.
 *         QWidget *widget - The widget argument is optional. If
 *             provided, it points to the widget that is being painted
 *             on; otherwise, it is 0. For cached painting, widget is
 *             always 0.
 * Outputs:
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
 * Description: This function receives mouse doubleclick events for
 *     this item.
 * Callee:
 * Inputs: QGraphicsSceneMouseEvent *event
 * Outputs:
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
 * Description: This function is to notify custom items that some
 *     part of the item's state changes.
 * Callee: Yuan::setPos(), Link::trackYuans(), setPos()
 * Inputs: GraphicsItemChange change - the parameter of the item
 *             that is changing
 *         QVariant &value - new value
 * Outputs:
******************************************************************/
QVariant NewNode::itemChange(GraphicsItemChange change,
                    const QVariant &value)
{   
    if (change & ItemPositionHasChanged)   {    
        yuan->setPos(pos().x(),
                     pos().y() + outlineRect().height()/2 + yuan->boundingRect().height()/2);
        foreach (Link *link, yuan->myLinks)
        {link->trackYuans();update();}
        yuan2->setPos(pos().x() - outlineRect().width()/2 - yuan2->outlineRect().width()/2,
                     pos().y());
        foreach (Link *link, yuan2->myLinks)
        {link->trackYuans();update();}     }
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

void NewNode::sethw()
{
    QRectF rect = outlineRect();
    high = rect.height();
    wide = rect.width();
}

void NewNode::setxy(QPointF point)
{
    lx = point.x();
    ly = point.y();

}
