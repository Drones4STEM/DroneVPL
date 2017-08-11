/*******************************************************************
 * File:rec.cpp
 * Author: Ryan Feng
 * Description: This file includes the realization of class Rec.
 *        Rec acts as logic rectangle.
******************************************************************/

#include <QtGui>

#include "node.h"
#include "yuan.h"
#include "link.h"
#include "newnode.h"
#include "rec.h"
#include "diagramwindow.h"
#include "wqDragItem.h"

Rec::Rec()
{
    box=new QComboBox;

    setAcceptDrops(true);  //目前没什么用

    identifier="Rec";
    controlsId=0;

    mBoundingRect = QRectF(0,0,400,400);
    mBoundingRect.translate(-mBoundingRect.center());


    QRectF curRect = boundingRect().adjusted(0,10,0,-10);

    // Drags
    wqDragItem* drag1 = new wqDragItem(this);
    wqDragItem* drag2 = new wqDragItem(this);
    wqDragItem* drag3 = new wqDragItem(this);
    wqDragItem* drag4 = new wqDragItem(this);

    drag1->setPos(curRect.topLeft());
    drag2->setPos(curRect.topRight());
    drag3->setPos(curRect.bottomLeft());
    drag4->setPos(curRect.bottomRight());

    connect(drag1, SIGNAL(sig_childMoved()), this, SLOT(slot_changeRect()));
    connect(drag2, SIGNAL(sig_childMoved()), this, SLOT(slot_changeRect()));
    connect(drag3, SIGNAL(sig_childMoved()), this, SLOT(slot_changeRect()));
    connect(drag4, SIGNAL(sig_childMoved()), this, SLOT(slot_changeRect()));

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
 * Description: This function defines the outer bounds of the item
 *     as a rectangle; all painting must be restricted to inside an
 *     item's bounding rect. IoNode uses this to determine
 *     whether the item requires redrawing.
 * Callee:
 * Inputs:
 * Outputs:QRectF
******************************************************************/
QRectF Rec::boundingRect() const
{
    //const int Margin = 6;
    //return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
    return mBoundingRect;
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
QPainterPath Rec::shape() const
{
    QRectF rect = mBoundingRect;

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

    QRectF rect = mBoundingRect;
    painter->drawRoundRect(rect, roundness(rect.width()),
                           roundness(rect.height()));
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

void Rec::slot_changeRect()
{
    QRectF rect(0,0,0,0);
    QList<QGraphicsItem*> allItems = childItems();
    foreach(QGraphicsItem* item, allItems)
    {
        wqDragItem* drag = dynamic_cast<wqDragItem*>(item);
        if(drag)
        {
            QRectF r = drag->mapRectToScene(drag->boundingRect());
            rect = rect.united(r);
        }
    }

    prepareGeometryChange();
    mBoundingRect = mapRectFromScene(rect);
}


