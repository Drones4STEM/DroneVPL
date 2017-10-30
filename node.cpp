/*******************************************************************
 * File:node.cpp
 * Author: Ryan Feng
 * Description: This file includes the realization of class Node.
 *        Node is the parent class of ComputeNode, TakeoffNode, 
 *        VarNode.
******************************************************************/

#include <QtGui>

#include "link.h"
#include "node.h"
#include "yuan.h"
#include "rec.h"
#include <math.h>

/*******************************************************************
 * Function name: Node()
 * Description: This is a constructor of Node class.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
Node::Node(QGraphicsItem *parent)
    :QGraphicsObject(parent)
{
    myTextColor = Qt::darkGreen;
    myOutlineColor = Qt::darkBlue;
    myBackgroundColor = QColor(230,230,230);

    //QGraphicsItem* p=dynamic_cast<QGraphicsItem*>(this);
    //yuan=new triYuan(p);
    yuan=new triYuan();

    setFlags(ItemIsMovable | ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);

    controlsId=0;
    identifier="Node";

    connect(this,SIGNAL(xChanged()),this,SLOT(emitSignal()));
    connect(this,SIGNAL(yChanged()),this,SLOT(emitSignal()));
    connect(this,SIGNAL(positionChanged(QPoint)),this,SLOT(setPosition()));

}

/*******************************************************************
 * Function name: ~Node()
 * Description: This is a destructor of Node class
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
Node::~Node()
{
    delete yuan;
}

void Node::setText(const QString &text)
{
    prepareGeometryChange();
    myText = text;
    update();
    emit dirty();
    sethw();
}

QString Node::text() const
{
    return myText;
}

void Node::setTextColor(const QColor &color)
{
    if(isSelected()&&color!=textColor())
    {
        myTextColor = color;
        emit dirty();
        update();
    }
}

QColor Node::textColor() const
{
    return myTextColor;
}

void Node::setOutlineColor(const QColor &color)
{
    if(isSelected()&&color!=outlineColor())
    {
        myOutlineColor = color;
        emit dirty();
        update();
    }
}

QColor Node::outlineColor() const
{
    return myOutlineColor;
}

void Node::setBackgroundColor(const QColor &color)
{
    if(isSelected()&&color!=backgroundColor())
    {
        myBackgroundColor = color;
        emit dirty();
        update();
    }
}

QColor Node::backgroundColor() const
{
    return myBackgroundColor;
}

void Node::setPosition(QPoint pos)   //根据lineEdit的改变移动node
{
    if(isSelected()&&pos!=position())
    {
        myPosition = pos;
        setPos(pos);
        emit dirty();
        update();
    }
}

QPoint Node::position()const
{
    return myPosition;
}

void Node::setPosition()           //在控件移动时，改变myPosition变量
{
    myPosition = pos().toPoint();
    emit dirty();
}

/*
bool Node::set_controlsId(int id)
{
    if(controlsId = id) return true;
    else return false;
}
bool Node::set_identifier(QString idtf)
{
    identifier = idtf;
    if(identifier != "") return true;
    else return false;
}
bool Node::set_name()
{
    QString id = QString::number(controlsId,10);
    name = identifier + id;
    yuan->master->name = name;
    if(name!="") return true;
    else return false;
}
bool Node::set_master()
{
    ;//yuan->master = wrap;
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
QRectF Node::boundingRect() const
{
    const int Margin = 6;
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
QPainterPath Node::shape() const
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
void Node::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget */)
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
    painter->drawText(rect, Qt::AlignCenter, myText);
    /*QString text1 = QString(tr("takeoff"));
    painter->drawText(-30,5,text1);
    painter->drawText(0,5,text1);*/
}

/*******************************************************************
 * Function name: mouseDoubleClickEvent()
 * Description: This function receives mouse doubleclick events for
 *     this item.
 * Callee:
 * Inputs: QGraphicsSceneMouseEvent *event
 * Outputs:
******************************************************************/
void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
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
QVariant Node::itemChange(GraphicsItemChange change,
                    const QVariant &value)
{

    if (change & ItemPositionHasChanged){
        //if(this->collidingItems().isEmpty()||(this->collidingItems().count()==1&&dynamic_cast<Rec *>(this->collidingItems().first())!=0) )
       //{
            yuan->setPos(pos().x(),
                         pos().y()+ outlineRect().height()/2 +yuan->boundingRect().height()/2);
            foreach (Link *link, yuan->myLinks)
            {link->trackYuans();update();}
       //}
        /*else{
            setPos(yuan->pos().x(),
                          yuan->pos().y()-outlineRect().height()/2 -yuan->boundingRect().height()/2);*/
        //}
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
QRectF Node::outlineRect() const
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
int Node::roundness(double size) const
{
    const int Diameter = 12;
    return 100 * Diameter / (abs(int(size))+1);
}

triYuan* Node::myYuan()const
{
    return yuan;
}


void Node::emitSignal()
{
    emit positionChanged(pos().toPoint());
}

void Node::sethw()
{
    QRectF rect = outlineRect();
    high = rect.height();
    wide = rect.width();
}

void Node::setxy(QPointF point)
{
    lx = point.x();
    ly = point.y();
}
