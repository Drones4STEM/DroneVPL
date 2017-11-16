/*******************************************************************
 * File:computenode.cpp
 * Author: Ryan Feng
 * Description: This file includes the realization of class 
 *        ComputeNode. ComputeNode is a node which connect two varnodes
 *        (variables) and compute an outcome.
******************************************************************/

#include "node.h"
#include "yuan.h"
#include "link.h"
#include <QtGui>
#include "computenode.h"
#include "rec.h"

/*******************************************************************
 * Function name: ComputeNode()
 * Description: This is a constructor of ComputeNode class
 * Callee: triYuan::triYuan(), Yuan::Yuan(), Yuan::setInout()
 * Inputs:
 * Outputs:
******************************************************************/
ComputeNode::ComputeNode()
{
    //yuan->master = new WidgetWrap(this);
    //yuan2->master = new WidgetWrap(this);
    //yuan3->master = new WidgetWrap(this);
    yuan2 = new Yuan();
    yuan3 = new triYuan();
    yuan2->setInout(1);
    yuan3->setInout(0);
    box = new QComboBox;
    box->setFixedSize(44,28);
    lineEdit1 = new QLineEdit;
    lineEdit2 = new QLineEdit;
    lineEdit1->setFixedSize(36,20);
    lineEdit2->setFixedSize(36,20);

    identifier="Compute";
    rank = 0;

    connect(box,SIGNAL(currentIndexChanged(int)),this,SLOT(setNewIdentifier()));
    connect(lineEdit1,SIGNAL(textChanged(QString)),this,SIGNAL(dirty()));
    connect(lineEdit2,SIGNAL(textChanged(QString)),this,SIGNAL(dirty()));
}

/*******************************************************************
 * Function name: ~ComputeNode()
 * Description: This is a destructor of ComputeNode class
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
ComputeNode::~ComputeNode()
{
    delete yuan2;
    delete yuan3;
    //delete box;
    //delete lineEdit1;
    //delete lineEdit2;
}

/*******************************************************************
 * Function name: outlineRect()
 * Description: return a rect
 * Callee: QRectF::rect()
 * Inputs:
 * Outputs:QRectF
******************************************************************/
QRectF ComputeNode::outlineRect() const
{
    QRectF rect(0,0,162,36);
    rect.translate(-rect.center());
    return rect;
}

QRectF ComputeNode::boundingRect() const
{
    return outlineRect();
}

QPainterPath ComputeNode::shape() const
{
    QRectF rect = outlineRect();
    QPainterPath path;
    path.addRoundedRect(rect,roundness(rect.width()),
                        roundness(rect.height()));
    return path;
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
QVariant ComputeNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change & ItemPositionHasChanged){
            yuan->setPos(pos().x() - 70,
                         pos().y() + outlineRect().height()/2 + yuan->boundingRect().height()/2);
            foreach (Link *link, yuan->myLinks)
            {link->trackYuans();update();}

            yuan2->setPos(pos().x() - 70,
                         pos().y() - outlineRect().height()/2 - yuan2->boundingRect().height()/2);
            foreach (Link *link, yuan2->myLinks)
            {link->trackYuans();update();}
            yuan3->setPos(pos().x() + outlineRect().width()/2 + yuan3->boundingRect().width()/2,
                         pos().y());
            foreach (Link *link, yuan3->myLinks)
            {link->trackYuans();update();}
    }
    return QGraphicsItem::itemChange(change, value);
}

void ComputeNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    {
        QList<QGraphicsItem *> items = this->collidingItems();
        int itemsCount = items.count();
        for(int i=0;i<items.count();i++)
        {
            if(dynamic_cast<IoSmallNode*>(items[i]))
                itemsCount--;
        }
        if(itemsCount>0)
        {
            for(int i=0;i<items.count();i++)
            {
                if(!dynamic_cast<IoSmallNode*>(items[i])&&!dynamic_cast<Link*>(items[i])
                        &&!dynamic_cast<Yuan*>(items[i])&&!dynamic_cast<triYuan*>(items[i])
                        &&!dynamic_cast<QGraphicsProxyWidget*>(items[i])&&!dynamic_cast<ComputeSmallNode*>(items[i])
                        &&!dynamic_cast<Rec*>(items[i]))
                {
                    double dx = this->pos().x() - items[i]->pos().x();
                    double dy = this->pos().y() - items[i]->pos().y();
                    double a = items[i]->boundingRect().width()/items[i]->boundingRect().height();
                    if((abs(dx)/a>abs(dy)&&dx<=0))    //放在左边
                        setPos(items[i]->pos().x() - items[i]->boundingRect().width()/2 - boundingRect().width()/2,
                               items[i]->pos().y());
                    else if(abs(dy)>abs(dx)/a&&dy>=0) //放在下边
                        setPos(items[i]->pos().x(),
                               items[i]->pos().y() + items[i]->boundingRect().height()/2 + boundingRect().height()/2);
                    else if(abs(dx)/a>abs(dy)&&dx>=0)  //放在右边
                        setPos(items[i]->pos().x() + items[i]->boundingRect().width()/2 + boundingRect().width()/2,
                               items[i]->pos().y());
                    else                               //放在上边
                        setPos(items[i]->pos().x(),
                               items[i]->pos().y() - items[i]->boundingRect().height()/2 - boundingRect().height()/2);
                    break;
                }
            }
        }
        QGraphicsItem::mouseReleaseEvent(event);
    }
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
void ComputeNode::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option, QWidget *widget)
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
}

/*******************************************************************
 * Function name: setNewIdentifier()
 * Description: This function  changes the identifier and controlsId
 *     of the item, it also add the computenodenum.
 * Callee: QBox::currentIndex()
 * Inputs:
 * Outputs:
******************************************************************/
void ComputeNode::setNewIdentifier()
{//这个函数先不用了
//    int index=box->currentIndex();
//    switch (index) {
//    case 0:
//    {
//        identifier="Compute";
//        break;
//    }
//    case 1:
//    {
//        identifier="Compute";
//        break;
//    }
//    case 2:
//    {
//        identifier="Compute";
//        break;
//    }
//    case 3:
//    {
//        identifier="Compute";
//        break;
//    }
//    case 4:
//    {
//        identifier="Compute";
//        break;
//    }
//    case 5:
//    {
//        identifier="Compute";
//        break;
//    }
//    case 6:
//    {
//        identifier="Compute";
//        break;
//    }
//    default:
//        break;
//    }
}



//-------------------ComputeSmallNode-----------------------//
ComputeSmallNode::ComputeSmallNode()
{
    identifier="Compute";
    setFlag(ItemIsMovable,false);
}

QRectF ComputeSmallNode::outlineRect() const
{
    QRectF rect(0,0,40,40);
    rect.translate(-rect.center());
    return rect;
}

QRectF ComputeSmallNode::boundingRect() const
{
    return outlineRect();
}

QPainterPath ComputeSmallNode::shape() const
{
    QRectF rect = outlineRect();
    QPainterPath path;
    path.addRect(rect);
    return path;
}

void ComputeSmallNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(outlineColor());
    painter->setPen(pen);
    painter->setBrush(backgroundColor());

    QRectF rect = outlineRect();
    painter->drawRect(rect);

    painter->setPen(textColor());
    QString myText=text();
    painter->drawText(rect,Qt::AlignCenter,myText);
}

QVariant ComputeSmallNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change & ItemPositionHasChanged) {
        yuan->setPos(pos().x() + outlineRect().width()/2 + yuan->boundingRect().width()/2,
                     pos().y());
        foreach (Link *link, yuan->myLinks)
        {link->trackYuans();update();}
    }
    return QGraphicsItem::itemChange(change, value);
}



/* ///////////////sinNode///////////////////*/
sinNode::sinNode()
{
    yuan2 = new Yuan();
    yuan3 = new triYuan();
    yuan2->setInout(1);
    yuan3->setInout(0);
    box = new QComboBox;
    box->setFixedSize(62,28);
    lineEdit2 = new QLineEdit;
    lineEdit2->setFixedSize(36,20);

    identifier="Compute";
    rank = 0;

    connect(box,SIGNAL(currentIndexChanged(int)),this,SLOT(setNewIdentifier()));
    connect(lineEdit2,SIGNAL(textChanged(QString)),this,SIGNAL(dirty()));
}


sinNode::~sinNode()
{
    delete yuan2;
    delete yuan3;
}

QRectF sinNode::outlineRect() const
{
    QRectF rect(0,0,160,36);
    rect.translate(-rect.center());
    return rect;
}

QRectF sinNode::boundingRect() const
{
    return outlineRect();
}

QPainterPath sinNode::shape() const
{
    QRectF rect = outlineRect();
    QPainterPath path;
    path.addRoundedRect(rect,roundness(rect.width()),
                        roundness(rect.height()));
    return path;
}

QVariant sinNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change & ItemPositionHasChanged){
            yuan->setPos(pos().x() - 65,
                         pos().y() + outlineRect().height()/2 + yuan->boundingRect().height()/2 + 3);
            foreach (Link *link, yuan->myLinks)
            {link->trackYuans();update();}

            yuan2->setPos(pos().x() - 65,
                         pos().y() - outlineRect().height()/2 - yuan2->boundingRect().height()/2 - 3);
            foreach (Link *link, yuan2->myLinks)
            {link->trackYuans();update();}
            yuan3->setPos(pos().x() + outlineRect().width()/2 + yuan3->boundingRect().width()/2 + 3,
                         pos().y());
            foreach (Link *link, yuan3->myLinks)
            {link->trackYuans();update();}
    }
    return QGraphicsItem::itemChange(change, value);
}

void sinNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    {
        QList<QGraphicsItem *> items = this->collidingItems();
        int itemsCount = items.count();
        for(int i=0;i<items.count();i++)
        {
            if(dynamic_cast<IoSmallNode*>(items[i]))
                itemsCount--;
        }
        if(itemsCount>0)
        {
            for(int i=0;i<items.count();i++)
            {
                if(!dynamic_cast<IoSmallNode*>(items[i])&&!dynamic_cast<Link*>(items[i])
                        &&!dynamic_cast<Yuan*>(items[i])&&!dynamic_cast<triYuan*>(items[i])
                        &&!dynamic_cast<QGraphicsProxyWidget*>(items[i])&&!dynamic_cast<ComputeSmallNode*>(items[i])
                        &&!dynamic_cast<Rec*>(items[i]))
                {
                    double dx = this->pos().x() - items[i]->pos().x();
                    double dy = this->pos().y() - items[i]->pos().y();
                    double a = items[i]->boundingRect().width()/items[i]->boundingRect().height();
                    if((abs(dx)/a>abs(dy)&&dx<=0))    //放在左边
                        setPos(items[i]->pos().x() - items[i]->boundingRect().width()/2 - boundingRect().width()/2,
                               items[i]->pos().y());
                    else if(abs(dy)>abs(dx)/a&&dy>=0) //放在下边
                        setPos(items[i]->pos().x(),
                               items[i]->pos().y() + items[i]->boundingRect().height()/2 + boundingRect().height()/2);
                    else if(abs(dx)/a>abs(dy)&&dx>=0)  //放在右边
                        setPos(items[i]->pos().x() + items[i]->boundingRect().width()/2 + boundingRect().width()/2,
                               items[i]->pos().y());
                    else                               //放在上边
                        setPos(items[i]->pos().x(),
                               items[i]->pos().y() - items[i]->boundingRect().height()/2 - boundingRect().height()/2);
                    break;
                }
            }
        }
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

void sinNode::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option, QWidget *widget)
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

    QImage img = QImage(":/images/icon/赋值1.png");
    painter->drawImage(50,-15,img);
}

void sinNode::setNewIdentifier()
{//尽管这个函数之后可能有用，但现在不是必需的
//    int index=box->currentIndex();
//    switch (index) {
//    case 0:
//    {
//        identifier="ComputeNodeSin";
//        break;
//    }
//    case 1:
//    {
//        identifier="ComputeNodeCos";
//        break;
//    }
//    case 2:
//    {
//        identifier="ComputeNodeTan";
//        break;
//    }
//    default:
//        break;
//    }
}


/* ///////////////logNode///////////////////*/
logNode::logNode()
{
    yuan2 = new Yuan();
    yuan3 = new triYuan();
    yuan2->setInout(1);
    yuan3->setInout(0);
    lineEdit1 = new QLineEdit;
    lineEdit1->setFixedSize(14,12);
    lineEdit2 = new QLineEdit;
    lineEdit2->setFixedSize(36,20);

    identifier="ComputeNodeLog";
    rank = 0;

    connect(lineEdit1,SIGNAL(textChanged(QString)),this,SIGNAL(dirty()));
    connect(lineEdit2,SIGNAL(textChanged(QString)),this,SIGNAL(dirty()));
}


logNode::~logNode()
{
    delete yuan2;
    delete yuan3;
}

QRectF logNode::outlineRect() const
{
    QRectF rect(0,0,146,36);
    rect.translate(-rect.center());
    return rect;
}

QRectF logNode::boundingRect() const
{
    return outlineRect();
}

QPainterPath logNode::shape() const
{
    QRectF rect = outlineRect();
    QPainterPath path;
    path.addRoundedRect(rect,roundness(rect.width()),
                        roundness(rect.height()));
    return path;
}

QVariant logNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change & ItemPositionHasChanged){
            yuan->setPos(pos().x() - 60,
                         pos().y() + outlineRect().height()/2 + yuan->boundingRect().height()/2 + 3);
            foreach (Link *link, yuan->myLinks)
            {link->trackYuans();update();}

            yuan2->setPos(pos().x() - 60,
                         pos().y() - outlineRect().height()/2 - yuan2->boundingRect().height()/2 - 3);
            foreach (Link *link, yuan2->myLinks)
            {link->trackYuans();update();}
            yuan3->setPos(pos().x() + outlineRect().width()/2 + yuan3->boundingRect().width()/2 + 3,
                         pos().y());
            foreach (Link *link, yuan3->myLinks)
            {link->trackYuans();update();}
    }
    return QGraphicsItem::itemChange(change, value);
}

void logNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    {
        QList<QGraphicsItem *> items = this->collidingItems();
        int itemsCount = items.count();
        for(int i=0;i<items.count();i++)
        {
            if(dynamic_cast<IoSmallNode*>(items[i]))
                itemsCount--;
        }
        if(itemsCount>0)
        {
            for(int i=0;i<items.count();i++)
            {
                if(!dynamic_cast<IoSmallNode*>(items[i])&&!dynamic_cast<Link*>(items[i])
                        &&!dynamic_cast<Yuan*>(items[i])&&!dynamic_cast<triYuan*>(items[i])
                        &&!dynamic_cast<QGraphicsProxyWidget*>(items[i])&&!dynamic_cast<ComputeSmallNode*>(items[i])
                        &&!dynamic_cast<Rec*>(items[i]))
                {
                    double dx = this->pos().x() - items[i]->pos().x();
                    double dy = this->pos().y() - items[i]->pos().y();
                    double a = items[i]->boundingRect().width()/items[i]->boundingRect().height();
                    if((abs(dx)/a>abs(dy)&&dx<=0))    //放在左边
                        setPos(items[i]->pos().x() - items[i]->boundingRect().width()/2 - boundingRect().width()/2,
                               items[i]->pos().y());
                    else if(abs(dy)>abs(dx)/a&&dy>=0) //放在下边
                        setPos(items[i]->pos().x(),
                               items[i]->pos().y() + items[i]->boundingRect().height()/2 + boundingRect().height()/2);
                    else if(abs(dx)/a>abs(dy)&&dx>=0)  //放在右边
                        setPos(items[i]->pos().x() + items[i]->boundingRect().width()/2 + boundingRect().width()/2,
                               items[i]->pos().y());
                    else                               //放在上边
                        setPos(items[i]->pos().x(),
                               items[i]->pos().y() - items[i]->boundingRect().height()/2 - boundingRect().height()/2);
                    break;
                }
            }
        }
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

void logNode::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option, QWidget *widget)
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

    QImage img = QImage(":/images/icon/赋值1.png");
    painter->drawImage(45,-13,img);

    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    QFont font("MicrosoftYaHei");
    font.setPixelSize(18);
    painter->setFont(font);
    QString str("log");
    painter->drawText(-62,7,str);
}


/* ////////eNode////////////////*/
eNode::eNode()
{
    yuan2 = new Yuan();
    yuan3 = new triYuan();
    yuan2->setInout(1);
    yuan3->setInout(0);
    lineEdit2 = new QLineEdit;
    lineEdit2->setFixedSize(15,12);

    identifier="ComputeNodeE";
    rank = 0;

    connect(lineEdit2,SIGNAL(textChanged(QString)),this,SIGNAL(dirty()));
}


eNode::~eNode()
{
    delete yuan2;
    delete yuan3;
}

QRectF eNode::outlineRect() const
{
    QRectF rect(0,0,88,36);
    rect.translate(-rect.center());
    return rect;
}

QRectF eNode::boundingRect() const
{
    return outlineRect();
}

QPainterPath eNode::shape() const
{
    QRectF rect = outlineRect();
    QPainterPath path;
    path.addRoundedRect(rect,roundness(rect.width()),
                        roundness(rect.height()));
    return path;
}

QVariant eNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change & ItemPositionHasChanged){
            yuan->setPos(pos().x() - 40,
                         pos().y() + outlineRect().height()/2 + yuan->boundingRect().height()/2 + 3);
            foreach (Link *link, yuan->myLinks)
            {link->trackYuans();update();}

            yuan2->setPos(pos().x() - 40,
                         pos().y() - outlineRect().height()/2 - yuan2->boundingRect().height()/2 - 3);
            foreach (Link *link, yuan2->myLinks)
            {link->trackYuans();update();}
            yuan3->setPos(pos().x() + outlineRect().width()/2 + yuan3->boundingRect().width()/2 + 3,
                         pos().y());
            foreach (Link *link, yuan3->myLinks)
            {link->trackYuans();update();}
    }
    return QGraphicsItem::itemChange(change, value);
}

void eNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    {
        QList<QGraphicsItem *> items = this->collidingItems();
        int itemsCount = items.count();
        for(int i=0;i<items.count();i++)
        {
            if(dynamic_cast<IoSmallNode*>(items[i]))
                itemsCount--;
        }
        if(itemsCount>0)
        {
            for(int i=0;i<items.count();i++)
            {
                if(!dynamic_cast<IoSmallNode*>(items[i])&&!dynamic_cast<Link*>(items[i])
                        &&!dynamic_cast<Yuan*>(items[i])&&!dynamic_cast<triYuan*>(items[i])
                        &&!dynamic_cast<QGraphicsProxyWidget*>(items[i])&&!dynamic_cast<ComputeSmallNode*>(items[i])
                        &&!dynamic_cast<Rec*>(items[i]))
                {
                    double dx = this->pos().x() - items[i]->pos().x();
                    double dy = this->pos().y() - items[i]->pos().y();
                    double a = items[i]->boundingRect().width()/items[i]->boundingRect().height();
                    if((abs(dx)/a>abs(dy)&&dx<=0))    //放在左边
                        setPos(items[i]->pos().x() - items[i]->boundingRect().width()/2 - boundingRect().width()/2,
                               items[i]->pos().y());
                    else if(abs(dy)>abs(dx)/a&&dy>=0) //放在下边
                        setPos(items[i]->pos().x(),
                               items[i]->pos().y() + items[i]->boundingRect().height()/2 + boundingRect().height()/2);
                    else if(abs(dx)/a>abs(dy)&&dx>=0)  //放在右边
                        setPos(items[i]->pos().x() + items[i]->boundingRect().width()/2 + boundingRect().width()/2,
                               items[i]->pos().y());
                    else                               //放在上边
                        setPos(items[i]->pos().x(),
                               items[i]->pos().y() - items[i]->boundingRect().height()/2 - boundingRect().height()/2);
                    break;
                }
            }
        }
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

void eNode::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option, QWidget *widget)
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

    QImage img = QImage(":/images/icon/赋值1.png");
    painter->drawImage(15,-13,img);

    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    QFont font("MicrosoftYaHei");
    font.setPixelSize(18);
    painter->setFont(font);
    QString str("e");
    painter->drawText(-33,7,str);
}
