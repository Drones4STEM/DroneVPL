/*******************************************************************
 * File:ionode.cpp
 * Author: Ryan Feng
 * Description: This file includes the realization of class 
 *        IoNode. IoNode is a node which represents I/O operations.
******************************************************************/

#include "ionode.h"
#include "yuan.h"
#include "link.h"
#include "rec.h"

/*******************************************************************
 * Function name: IoNode()
 * Description: This is a constructor of IoNode class.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
IoNode::IoNode()
{
    box=new QComboBox;
    node1=new IoSmallNode;
    node2=new IoSmallNode;
    node3=new IoSmallNode;

    identifier="IoNode";
}

/*******************************************************************
 * Function name: ~IoNode()
 * Description: This is a destructor of IoNode class
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
IoNode::~IoNode()
{
    delete box;
    delete node1;
    delete node2;
    delete node3;
}

/*******************************************************************
 * Function name: outlineRect()
 * Description: return a rect
 * Callee:
 * Inputs:
 * Outputs:QRectF
******************************************************************/
QRectF IoNode::outlineRect() const
{
    QRectF rect(0,0,60,60);
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
QRectF IoNode::boundingRect() const
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
QPainterPath IoNode::shape() const
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
void IoNode::paint(QPainter *painter,
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

    painter->setPen(textColor());
    QString myText = text();
    painter->drawText(rect, Qt::AlignCenter, myText);
}

/*******************************************************************
 * Function name: itemChange()
 * Description: You can see the details of this function in
 *     computenode.cpp
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
QVariant IoNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change & ItemPositionHasChanged){
        if(this->collidingItems().isEmpty()||(this->collidingItems().count()==1&&dynamic_cast<Rec *>(this->collidingItems().first())!=0) )
       {
            yuan->setPos(pos().x(),
                         pos().y() + outlineRect().height()/2 + yuan->boundingRect().height()/2);
            foreach (Link *link, yuan->myLinks)
            {link->trackYuans();update();}

            yuan2->setPos(pos().x() - outlineRect().width()/2 - yuan2->outlineRect().width()/2,
                         pos().y());
            foreach (Link *link, yuan2->myLinks)
            {link->trackYuans();update();}

            item->setPos(pos().x() - outlineRect().width()/2,
                         pos().y() - outlineRect().height()/2 - item->boundingRect().height());

            node2->setPos(pos().x() + outlineRect().width()/2 + node2->outlineRect().width()/2,
                          pos().y());
            node1->setPos(pos().x() + outlineRect().width()/2 + node1->outlineRect().width()/2,
                          node2->pos().y() - node2->outlineRect().height());
            node3->setPos(pos().x() + outlineRect().width()/2 + node3->outlineRect().width()/2,
                          node2->pos().y() + node2->outlineRect().height());
       }
        else{
            setPos(node2->x()- outlineRect().width()/2 -node2->outlineRect().width()/2,
                   node2->y());
        }}
    return QGraphicsItem::itemChange(change, value);
}
////////////////////////////////////////////////////////////////////////////
/*******************************************************************
 * Function name: BatteryNode()
 * Description: This is a constructor of IoNode class.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
BatteryNode::BatteryNode()
{
    box=new QComboBox;
    box->addItem(tr("Battery"));
    box->addItem(tr("voltage"));
    box->addItem(tr("current"));
    box->addItem(tr("level"));
    node1=new IoSmallNode;
    node2=new IoSmallNode;
    node3=new IoSmallNode;

    identifier="Battery";
}

/*******************************************************************
 * Function name: ~BatteryNode()
 * Description: This is a destructor of IoNode class
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
BatteryNode::~BatteryNode()
{
    delete box;
    delete node1;
    delete node2;
    delete node3;
}

/*******************************************************************
 * Function name: outlineRect()
 * Description: return a rect
 * Callee:
 * Inputs:
 * Outputs:QRectF
******************************************************************/
QRectF BatteryNode::outlineRect() const
{
    QRectF rect(0,0,60,60);
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
QRectF BatteryNode::boundingRect() const
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
QPainterPath BatteryNode::shape() const
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
void BatteryNode::paint(QPainter *painter,
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

    painter->setPen(textColor());
    QString myText = text();
    painter->drawText(rect, Qt::AlignCenter, myText);
}

/*******************************************************************
 * Function name: itemChange()
 * Description: You can see the details of this function in
 *     computenode.cpp
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
QVariant BatteryNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change & ItemPositionHasChanged){
        if(this->collidingItems().isEmpty()||(this->collidingItems().count()==1&&dynamic_cast<Rec *>(this->collidingItems().first())!=0) )
       {
            yuan->setPos(pos().x(),
                         pos().y() + outlineRect().height()/2 + yuan->boundingRect().height()/2);
            foreach (Link *link, yuan->myLinks)
            {link->trackYuans();update();}

            yuan2->setPos(pos().x() - outlineRect().width()/2 - yuan2->outlineRect().width()/2,
                         pos().y());
            foreach (Link *link, yuan2->myLinks)
            {link->trackYuans();update();}

//            item->setPos(pos().x() - outlineRect().width()/2,
//                         pos().y() - outlineRect().height()/2 - item->boundingRect().height());

            node2->setPos(pos().x() + outlineRect().width()/2 + node2->outlineRect().width()/2,
                          pos().y());
            node1->setPos(pos().x() + outlineRect().width()/2 + node1->outlineRect().width()/2,
                          node2->pos().y() - node2->outlineRect().height());
            node3->setPos(pos().x() + outlineRect().width()/2 + node3->outlineRect().width()/2,
                          node2->pos().y() + node2->outlineRect().height());
       }
        else{
            setPos(node2->x()- outlineRect().width()/2 -node2->outlineRect().width()/2,
                   node2->y());
        }}
    return QGraphicsItem::itemChange(change, value);
}
///////////////////////////////////////////////////////////////////////////
/*******************************************************************
 * Function name: GimbalNode()
 * Description: This is a constructor of IoNode class.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
GimbalNode::GimbalNode()
{
    box=new QComboBox;
    box->addItem(tr("Gimbal"));
    box->addItem(tr("pitch"));
    box->addItem(tr("roll"));
    box->addItem(tr("yaw"));
    node1=new IoSmallNode;
    node2=new IoSmallNode;
    node3=new IoSmallNode;

    identifier="Gimbal";
}

/*******************************************************************
 * Function name: ~BatteryNode()
 * Description: This is a destructor of IoNode class
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
GimbalNode::~GimbalNode()
{
    delete box;
    delete node1;
    delete node2;
    delete node3;
}

/*******************************************************************
 * Function name: outlineRect()
 * Description: return a rect
 * Callee:
 * Inputs:
 * Outputs:QRectF
******************************************************************/
QRectF GimbalNode::outlineRect() const
{
    QRectF rect(0,0,60,60);
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
QRectF GimbalNode::boundingRect() const
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
QPainterPath GimbalNode::shape() const
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
void GimbalNode::paint(QPainter *painter,
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

    painter->setPen(textColor());
    QString myText = text();
    painter->drawText(rect, Qt::AlignCenter, myText);
}

/*******************************************************************
 * Function name: itemChange()
 * Description: You can see the details of this function in
 *     computenode.cpp
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
QVariant GimbalNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change & ItemPositionHasChanged){
        if(this->collidingItems().isEmpty()||(this->collidingItems().count()==1&&dynamic_cast<Rec *>(this->collidingItems().first())!=0) )
       {
            yuan->setPos(pos().x(),
                         pos().y() + outlineRect().height()/2 + yuan->boundingRect().height()/2);
            foreach (Link *link, yuan->myLinks)
            {link->trackYuans();update();}

            yuan2->setPos(pos().x() - outlineRect().width()/2 - yuan2->outlineRect().width()/2,
                         pos().y());
            foreach (Link *link, yuan2->myLinks)
            {link->trackYuans();update();}

//            item->setPos(pos().x() - outlineRect().width()/2,
//                         pos().y() - outlineRect().height()/2 - item->boundingRect().height());

            node2->setPos(pos().x() + outlineRect().width()/2 + node2->outlineRect().width()/2,
                          pos().y());
            node1->setPos(pos().x() + outlineRect().width()/2 + node1->outlineRect().width()/2,
                          node2->pos().y() - node2->outlineRect().height());
            node3->setPos(pos().x() + outlineRect().width()/2 + node3->outlineRect().width()/2,
                          node2->pos().y() + node2->outlineRect().height());
       }
        else{
            setPos(node2->x()- outlineRect().width()/2 -node2->outlineRect().width()/2,
                   node2->y());
        }}
    return QGraphicsItem::itemChange(change, value);
}

// ////////////////////////////////////////////////////////////////////////

/*******************************************************************
 * Function name: IoSmallNode()
 * Description: This is a constructor of IoNode class.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
IoSmallNode::IoSmallNode()
{
    setFlag(ItemIsMovable,false);
    identifier="IoSmallNode";
}

void IoSmallNode::setIoType(QString &type)
{
    myIoType = type;
}
QString IoSmallNode::ioType()
{
    return myIoType;
}
void IoSmallNode::setVar(QString &var)
{
    myVar=var;
}
QString IoSmallNode::var()
{
    return myVar;
}

/*******************************************************************
 * Function name: mouseDoubleClickEvent()
 * Description: This function receives mouse doubleclick events for
 *     this item.
 * Callee:
 * Inputs: QGraphicsSceneMouseEvent *event
 * Outputs:
******************************************************************/
void IoSmallNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QString type = QInputDialog::getText(event->widget(),
                           tr("Edit Type"), tr("Enter new Type:"),
                           QLineEdit::Normal, myIoType);
    if (!type.isEmpty())
        myIoType=type;

    QString v=var();
    v = QInputDialog::getText(event->widget(),
                           tr("Edit Text"), tr("Enter new text:"),
                           QLineEdit::Normal, v);
    if (!v.isEmpty())
        myVar=v;

    setText(tr("%1 %2").arg(myIoType).arg(v));
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
QVariant IoSmallNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change & ItemPositionHasChanged) {
        yuan->setPos(pos().x() + outlineRect().width()/2 + yuan->boundingRect().width()/2,
                     pos().y());
        foreach (Link *link, yuan->myLinks)
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
QRectF IoSmallNode::outlineRect() const
{
    QRectF rect(0,0,60,20);
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
QRectF IoSmallNode::boundingRect() const
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
QPainterPath IoSmallNode::shape() const
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
void IoSmallNode::paint(QPainter *painter,
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

    painter->setPen(textColor());
    QString myText = text();
    painter->drawText(rect, Qt::AlignCenter, myText);
}
/////////////////////////////////////////////////////////////
/*******************************************************************
 * Function name: AttitudeNode()
 * Description: This is a constructor of IoNode class.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
AttitudeNode::AttitudeNode()
{
    box=new QComboBox;
    box->addItem(tr("Attitude"));
    box->addItem(tr("roll"));
    box->addItem(tr("pitch"));
    box->addItem(tr("yaw"));
    node1=new IoSmallNode;
    node2=new IoSmallNode;
    node3=new IoSmallNode;

    identifier="Attitude";
}

/*******************************************************************
 * Function name: ~AttitudeNode()
 * Description: This is a destructor of IoNode class
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
AttitudeNode::~AttitudeNode()
{
    delete box;
    delete node1;
    delete node2;
    delete node3;
}

/*******************************************************************
 * Function name: outlineRect()
 * Description: return a rect
 * Callee:
 * Inputs:
 * Outputs:QRectF
******************************************************************/
QRectF AttitudeNode::outlineRect() const
{
    QRectF rect(0,0,60,60);
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
QRectF AttitudeNode::boundingRect() const
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
QPainterPath AttitudeNode::shape() const
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
void AttitudeNode::paint(QPainter *painter,
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

    painter->setPen(textColor());
    QString myText = text();
    painter->drawText(rect, Qt::AlignCenter, myText);
}

/*******************************************************************
 * Function name: itemChange()
 * Description: You can see the details of this function in
 *     computenode.cpp
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
QVariant AttitudeNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change & ItemPositionHasChanged){
        if(this->collidingItems().isEmpty()||(this->collidingItems().count()==1&&dynamic_cast<Rec *>(this->collidingItems().first())!=0) )
       {
            yuan->setPos(pos().x(),
                         pos().y() + outlineRect().height()/2 + yuan->boundingRect().height()/2);
            foreach (Link *link, yuan->myLinks)
            {link->trackYuans();update();}

            yuan2->setPos(pos().x() - outlineRect().width()/2 - yuan2->outlineRect().width()/2,
                         pos().y());
            foreach (Link *link, yuan2->myLinks)
            {link->trackYuans();update();}

//            item->setPos(pos().x() - outlineRect().width()/2,
//                         pos().y() - outlineRect().height()/2 - item->boundingRect().height());

            node2->setPos(pos().x() + outlineRect().width()/2 + node2->outlineRect().width()/2,
                          pos().y());
            node1->setPos(pos().x() + outlineRect().width()/2 + node1->outlineRect().width()/2,
                          node2->pos().y() - node2->outlineRect().height());
            node3->setPos(pos().x() + outlineRect().width()/2 + node3->outlineRect().width()/2,
                          node2->pos().y() + node2->outlineRect().height());
       }
        else{
            setPos(node2->x()- outlineRect().width()/2 -node2->outlineRect().width()/2,
                   node2->y());
        }}
    return QGraphicsItem::itemChange(change, value);
}

/////////////////////////////////////////////////////////////
/*******************************************************************
 * Function name: ChannelNode()
 * Description: This is a constructor of IoNode class.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
ChannelNode::ChannelNode()
{
    box=new QComboBox;
    box->addItem(tr("Channel"));
    box->addItem(tr("one"));
    box->addItem(tr("two"));
    box->addItem(tr("three"));
    box->addItem(tr("four"));
    box->addItem(tr("five"));
    box->addItem(tr("six"));
    box->addItem(tr("seven"));
    box->addItem(tr("eight"));
    box->addItem(tr("nine"));
    node1=new IoSmallNode;
    node2=new IoSmallNode;
    node3=new IoSmallNode;
    node4=new IoSmallNode;
    node5=new IoSmallNode;
    node6=new IoSmallNode;
    node7=new IoSmallNode;
    node8=new IoSmallNode;
    node9=new IoSmallNode;

    identifier="Channel";
}

/*******************************************************************
 * Function name: ~ChannelNode()
 * Description: This is a destructor of IoNode class
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
ChannelNode::~ChannelNode()
{
    delete box;
    delete node1;
    delete node2;
    delete node3;
    delete node4;
    delete node5;
    delete node6;
    delete node7;
    delete node8;
    delete node9;
}

/*******************************************************************
 * Function name: outlineRect()
 * Description: return a rect
 * Callee:
 * Inputs:
 * Outputs:QRectF
******************************************************************/
QRectF ChannelNode::outlineRect() const
{
    QRectF rect(0,0,60,180);
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
QRectF ChannelNode::boundingRect() const
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
QPainterPath ChannelNode::shape() const
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
void ChannelNode::paint(QPainter *painter,
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

    painter->setPen(textColor());
    QString myText = text();
    painter->drawText(rect, Qt::AlignCenter, myText);
}

/*******************************************************************
 * Function name: itemChange()
 * Description: You can see the details of this function in
 *     computenode.cpp
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
QVariant ChannelNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change & ItemPositionHasChanged){
        if(this->collidingItems().isEmpty()||(this->collidingItems().count()==1&&dynamic_cast<Rec *>(this->collidingItems().first())!=0) )
        {qDebug()<<"........if";
            yuan->setPos(pos().x(),
                         pos().y() + outlineRect().height()/2);
            foreach (Link *link, yuan->myLinks)
            {link->trackYuans();update();}

            yuan2->setPos(pos().x() - outlineRect().width()/2 - yuan2->outlineRect().width()/2,
                         pos().y());
            foreach (Link *link, yuan2->myLinks)
            {link->trackYuans();update();}

            node1->setPos(pos().x() + outlineRect().width()/2 + node1->outlineRect().width()/2,
                          pos().y() - outlineRect().height()/2 + node1->outlineRect().height()/2);
            node2->setPos(pos().x() + outlineRect().width()/2 + node1->outlineRect().width()/2,
                          node1->pos().y() + node1->outlineRect().height());
            node3->setPos(pos().x() + outlineRect().width()/2 + node3->outlineRect().width()/2,
                          node1->pos().y() + 2*node1->outlineRect().height());
            node4->setPos(pos().x() + outlineRect().width()/2 + node3->outlineRect().width()/2,
                          node1->pos().y() + 3*node1->outlineRect().height());
            node5->setPos(pos().x() + outlineRect().width()/2 + node1->outlineRect().width()/2,
                          node1->pos().y() + 4*node1->outlineRect().height());
            node6->setPos(pos().x() + outlineRect().width()/2 + node3->outlineRect().width()/2,
                          node1->pos().y() + 5*node1->outlineRect().height());
            node7->setPos(pos().x() + outlineRect().width()/2 + node3->outlineRect().width()/2,
                          node1->pos().y() + 6*node1->outlineRect().height());
            node8->setPos(pos().x() + outlineRect().width()/2 + node1->outlineRect().width()/2,
                          node1->pos().y() + 7*node1->outlineRect().height());
            node9->setPos(pos().x() + outlineRect().width()/2 + node3->outlineRect().width()/2,
                          node1->pos().y() + 8*node1->outlineRect().height());
        }else{
            qDebug()<<"........else";
            //setPos(node1->x()- outlineRect().width()/2 -node1->outlineRect().width()/2,
            //       node1->y());
            setPos(yuan2->pos().x() + yuan2->outlineRect().width()/2 + outlineRect().width()/2,
                   yuan2->pos().y());
        }
    }
    return QGraphicsItem::itemChange(change, value);
}


/////////////////////////////////////////////////////////////
/*******************************************************************
 * Function name: RangeFinderNode()
 * Description: This is a constructor of IoNode class.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
RangeFinderNode::RangeFinderNode()
{
    box=new QComboBox;
    box->addItem(tr("Range Finder"));
    box->addItem(tr("distance"));
    node2=new IoSmallNode;

    identifier="RangeFinder";
}

/*******************************************************************
 * Function name: ~RangeFinderNode()
 * Description: This is a destructor of IoNode class
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
RangeFinderNode::~RangeFinderNode()
{
    delete box;
    delete node2;
}

/*******************************************************************
 * Function name: outlineRect()
 * Description: return a rect
 * Callee:
 * Inputs:
 * Outputs:QRectF
******************************************************************/
QRectF RangeFinderNode::outlineRect() const
{
    QRectF rect(0,0,60,60);
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
QRectF RangeFinderNode::boundingRect() const
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
QPainterPath RangeFinderNode::shape() const
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
void RangeFinderNode::paint(QPainter *painter,
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

    painter->setPen(textColor());
    QString myText = text();
    painter->drawText(rect, Qt::AlignCenter, myText);
}

/*******************************************************************
 * Function name: itemChange()
 * Description: You can see the details of this function in
 *     computenode.cpp
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
QVariant RangeFinderNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change & ItemPositionHasChanged){
        if(this->collidingItems().isEmpty()||(this->collidingItems().count()==1&&dynamic_cast<Rec *>(this->collidingItems().first())!=0) )
       {
            yuan->setPos(pos().x(),
                         pos().y() + outlineRect().height()/2 + yuan->boundingRect().height()/2);
            foreach (Link *link, yuan->myLinks)
            {link->trackYuans();update();}

            yuan2->setPos(pos().x() - outlineRect().width()/2 - yuan2->outlineRect().width()/2,
                         pos().y());
            foreach (Link *link, yuan2->myLinks)
            {link->trackYuans();update();}

//            item->setPos(pos().x() - outlineRect().width()/2,
//                         pos().y() - outlineRect().height()/2 - item->boundingRect().height());

            node2->setPos(pos().x() + outlineRect().width()/2 + node2->outlineRect().width()/2,
                          pos().y());
       }
        else{
            setPos(node2->x()- outlineRect().width()/2 -node2->outlineRect().width()/2,
                   node2->y());
        }}
    return QGraphicsItem::itemChange(change, value);
}
