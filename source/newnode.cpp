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
NewNode::NewNode(QGraphicsItem *parent)
    :QGraphicsObject(parent)
{
    myTextColor = Qt::darkGreen;
    myOutlineColor = Qt::darkBlue;
    myBackgroundColor = QColor(230,230,230);

    //QGraphicsItem*p=dynamic_cast<QGraphicsItem*>(this);
    yuan=new triYuan();
    yuan2=new Yuan();
    yuan2->setInout(1);

    setFlags(ItemIsMovable | ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setFlag(ItemIsFocusable);

    identifier="NewNode";
    controlsId=0;

    connect(this,SIGNAL(xChanged()),this,SLOT(emitSignal()));
    connect(this,SIGNAL(yChanged()),this,SLOT(emitSignal()));
    connect(this,SIGNAL(positionChanged(QPoint)),this,SLOT(setPosition()));
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
    emit dirty();
    update();
    sethw();
}

QString NewNode::text() const
{
    return myText;
}

void NewNode::setTextColor(const QColor &color)
{
    if(isSelected()&&color!=textColor())
    {
        myTextColor = color;
        emit dirty();
        update();
    }
}

QColor NewNode::textColor() const
{
    return myTextColor;
}

void NewNode::setOutlineColor(const QColor &color)
{
    if(isSelected()&&color!=outlineColor())
    {
        myOutlineColor = color;
        emit dirty();
        update();
    }
}

QColor NewNode::outlineColor() const
{
    return myOutlineColor;
}

void NewNode::setBackgroundColor(const QColor &color)
{
    if(isSelected()&&color!=backgroundColor())
    {
        myBackgroundColor = color;
        emit dirty();
        update();
    }
}

QColor NewNode::backgroundColor() const
{
    return myBackgroundColor;
}

void NewNode::setPosition(QPoint pos)   //根据lineEdit的改变移动node
{
    if(isSelected()&&pos!=position())
    {
        myPosition = pos;
        setPos(pos);
        emit dirty();
        update();
    }

}

QPoint NewNode::position()const
{
    return myPosition;
}

void NewNode::setPosition()           //在控件移动时，改变myPosition变量
{
    myPosition = pos().toPoint();
    emit dirty();
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
    ;
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
    return 100 * Diameter / (abs(int(size))+1);
}

Yuan* NewNode::myYuan()const
{
    return yuan;
}

void NewNode::emitSignal()
{
    emit positionChanged(pos().toPoint());
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
