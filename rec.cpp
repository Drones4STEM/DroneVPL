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
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>

Rec::Rec()
{
    //box=new QComboBox;

    setAcceptDrops(true);  //目前没什么用

    setLogicFlag(1);

    yuan3 = new Yuan();//线上圆
    yuan4 = new triYuan();//线下圆
    yuan5 = new Yuan();//底部圆
    yuan6 = new Yuan();//else 两侧圆
    yuan7 = new triYuan;//if两侧圆
    yuan3->setInout(1);
    yuan4->setInout(0);
    yuan5->setInout(1);
    yuan6->setInout(1);
    yuan7->setInout(0);

    identifier="Rec";
    controlsId=0;
    rank = 0;
    flink.clear();
    tlink.clear();

    mBoundingRect = QRectF(0,0,400,400);
    mBoundingRect.translate(-mBoundingRect.center());
    setLogichw();

    /*QRectF curRect = boundingRect().adjusted(0,10,0,-10);

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
    connect(drag4, SIGNAL(sig_childMoved()), this, SLOT(slot_changeRect()));*/
}

Rec::~Rec()
{
    delete yuan3;
    delete yuan4;
    delete yuan5;
    delete yuan6;
    delete yuan7;
    //delete box;
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
    QPen pen(Qt::white);
    pen.setWidth(2);
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(Qt::transparent);

    QRectF rect = mBoundingRect;
    painter->drawRoundRect(rect, roundness(rect.width()),
                           roundness(rect.height()));

    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->drawLine(boundingRect().topLeft().x() + 50,
                      boundingRect().topLeft().y() + 60,
                      boundingRect().topRight().x(),
                      boundingRect().topRight().y()+60);

    QImage img1 = QImage(":/images/icon/if.png");
    QImage img2 = QImage(":/images/icon/else.png");
    QImage img3 = QImage(":/images/icon/while.png");
    switch (logicFlag) {
    case 1:
        painter->drawImage(boundingRect().topLeft().x() + 15,
                           boundingRect().topLeft().y() + 15,img1);
        break;
    case 2:
        painter->drawImage(boundingRect().topLeft().x() + 15,
                           boundingRect().topLeft().y() + 15,img2);
        break;
    case 3:
        painter->drawImage(boundingRect().topLeft().x() + 15,
                           boundingRect().topLeft().y() + 15,img3);
        break;
    default:
        break;
    }
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
        /*yuan2->setPos(scenePos().x() - mBoundingRect.width()/2 ,
                      scenePos().y() - mBoundingRect.height()/2 );
        yuan->setPos(QPointF(pos().x(), pos().y() + mBoundingRect.height()*0.5));*/
        yuan->setPos(QPointF(scenePos().x() - mBoundingRect.width()/2 + 67,
                                  scenePos().y() + mBoundingRect.height()/2 + yuan->boundingRect().height()/2));
        yuan2->setPos(QPointF(scenePos().x() - mBoundingRect.width()/2 + 67,
                                  scenePos().y() - mBoundingRect.height()/2 - yuan2->boundingRect().height()/2));
        yuan3->setPos(QPointF(scenePos().x() - mBoundingRect.width()/2 + 67,
                                  scenePos().y() - mBoundingRect.height()/2 + 51));
        yuan4->setPos(QPointF(scenePos().x() - mBoundingRect.width()/2 + 67,
                                  scenePos().y() - mBoundingRect.height()/2 + 66));
        yuan5->setPos(QPointF(scenePos().x() - mBoundingRect.width()/2 + 67,
                                  scenePos().y() + mBoundingRect.height()/2 - yuan5->boundingRect().height()/2));
        yuan6->setPos(QPointF(scenePos().x() - mBoundingRect.width()/2 - yuan6->boundingRect().width()/2,
                                  scenePos().y() - mBoundingRect.height()/2 + 20));
        yuan7->setPos(QPointF(scenePos().x() + mBoundingRect.width()/2 + yuan6->boundingRect().width()/2,
                                  scenePos().y() - mBoundingRect.height()/2 + 20));
        foreach (Link *link, yuan->myLinks)
        {link->trackYuans();update();}
        foreach (Link *link, yuan2->myLinks)
        {link->trackYuans();update();}
        foreach (Link *link, yuan3->myLinks)
        {link->trackYuans();update();}
        foreach (Link *link, yuan4->myLinks)
        {link->trackYuans();update();}
        foreach (Link *link, yuan5->myLinks)
        {link->trackYuans();update();}
        foreach (Link *link, yuan6->myLinks)
        {link->trackYuans();update();}
        foreach (Link *link, yuan7->myLinks)
        {link->trackYuans();update();}
    }
    return QGraphicsItem::itemChange(change, value);
}


/*void Rec::slot_changeRect()
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
}*/


void Rec::setLogichw()
{
    high = mBoundingRect.height();
    wide = mBoundingRect.width();
}

/*void Rec::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() & Qt::ShiftModifier ||
       event->modifiers() & Qt::ControlModifier)
    {
        bool move = event->modifiers() & Qt::ControlModifier;
        bool changed = true;
        double dx1 = 0.0;
        double dy1 = 0.0;
        double dx2 = 0.0;
        double dy2 = 0.0;
        switch (event->key()) {
        case Qt::Key_Left:
            if(move)
                dx1 = -1.0;
            dx2 = -1.0;
            break;
        case Qt::Key_Right:
            if(move)
                dx1 = 1.0;
            dx2 =1.0;
            break;
        case Qt::Key_Up:
            if(move)
                dy1 = -1.0;
            dy2 = 1.0;
            break;
        case Qt::Key_Down:
            if(move)
                dy1 = 1.0;
            dy2 = 1.0;
            break;
        default:
            changed = false;
        }
        if(changed)
        {
            prepareGeometryChange();
            mBoundingRect.adjusted(dx1,dy1,dx2,dy2);
            scene()->update();
            emit dirty();
            return;
        }
    }
    QGraphicsItem::keyPressEvent(event);
}*/

void Rec::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->modifiers() & Qt::ShiftModifier)
    {
        m_resizing = true;
        setCursor(Qt::SizeAllCursor);
    }
    else
        QGraphicsItem::mousePressEvent(event);
}

void Rec::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_resizing)
    {
        QRectF rectangle(mBoundingRect);
        double dx = rectangle.width();
        double dy = rectangle.height();
        //if(dx<=10||dy<=10)
        //    QGraphicsItem::mouseMoveEvent(event);
        //else
        //{
            if(event->pos().x()<rectangle.x())
                rectangle.setBottomLeft(event->pos());
            else
                rectangle.setBottomRight(event->pos());
            mBoundingRect=rectangle;
            mBoundingRect.translate(-mBoundingRect.center());
            scene()->update();
        high = rectangle.height();
        wide = rectangle.width();
        //}
    }
    else
        QGraphicsItem::mouseMoveEvent(event);
}

void Rec::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_resizing)
    {
        m_resizing = false;
        setCursor(Qt::ArrowCursor);
        emit dirty();
    }
    else
        QGraphicsItem::mouseReleaseEvent(event);
}



