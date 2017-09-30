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


int ComputeNode::ComputeNodeAddNum=0;  int ComputeNode::ComputeNodeSubNum=0;
int ComputeNode::ComputeNodeMulNum=0;  int ComputeNode::ComputeNodeDivNum=0;
int ComputeNode::ComputeNodeCosNum=0;  int ComputeNode::ComputeNodeSinNum=0;
int ComputeNode::ComputeNodeTanNum=0;  int ComputeNode::ComputeNodeLogNum=0;
int ComputeNode::ComputeNodeENum=0;    int ComputeNode::ComputeNodeEqualNum=0;
int ComputeNode::ComputeNodeMoreNum=0; int ComputeNode::ComputeNodeLessNum=0;

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
    //yuan = new triYuan();
    yuan2 = new Yuan();
    yuan3 = new Yuan();
    yuan2->setInout(1);
    yuan3->setInout(1);
    box = new QComboBox;
    rect1 = new ComputeSmallNode;
    rect2 = new ComputeSmallNode;

    rect1text = "";
    rect1text = "";

    identifier="Compute";
    rank = 0;
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
    delete box;
    delete rect1;
    delete rect2;
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
    QRectF rect(-20,-20,40,40);
    return rect;
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
        if(this->collidingItems().isEmpty()||(this->collidingItems().count()==1&&dynamic_cast<Rec *>(this->collidingItems().first())!=0) )
       {
            yuan->setPos(pos().x(),
                         pos().y() + outlineRect().height()/2 +yuan->boundingRect().height()/2);
            foreach (Link *link, yuan->myLinks)
            {link->trackYuans();update();}

            yuan2->setPos(pos().x() - outlineRect().width()/2 - rect1->boundingRect().width()
                          - yuan2->outlineRect().width()/2,
                         pos().y());
            foreach (Link *link, yuan2->myLinks)
            {link->trackYuans();update();}

            yuan3->setPos(pos().x() + outlineRect().width()/2 + rect2->boundingRect().width()
                          + yuan3->outlineRect().width()/2,
                         pos().y());
            foreach (Link *link, yuan3->myLinks)
            {link->trackYuans();update();}

            item->setPos(QPointF(pos().x()- item->boundingRect().width()/2,
                                 pos().y() - outlineRect().height()/2 - item->boundingRect().height()));

            rect1->setPos(pos().x() - outlineRect().width(),
                          pos().y());
            rect2->setPos(pos().x() + outlineRect().width(),
                          pos().y());

       }
        else{
            setPos(yuan->x(),
                   yuan->y() - outlineRect().height()/2 -yuan->boundingRect().height()/2);
        }}
    return QGraphicsItem::itemChange(change, value);
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

    painter->setPen(textColor());
    QString myText=text();
    painter->drawText(rect, Qt::AlignCenter, myText);
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
{
    int index=box->currentIndex();
    switch (index) {
    case 0:
    {
        identifier="ComputeNodeAdd";
        ComputeNodeAddNum++;
        controlsId=ComputeNodeAddNum;
        break;
    }
    case 1:
    {
        identifier="ComputeNodeSub";
        ComputeNodeSubNum++;
        controlsId=ComputeNodeSubNum;
        break;
    }
    case 2:
    {
        identifier="ComputeNodeMul";
        ComputeNodeMulNum++;
        controlsId=ComputeNodeMulNum;
        break;
    }
    case 3:
    {
        identifier="ComputeNodeDiv";
        ComputeNodeDivNum++;
        controlsId=ComputeNodeDivNum;
        break;
    }
    case 4:
    {
        identifier="ComputeNodeCos";
        ComputeNodeCosNum++;
        controlsId=ComputeNodeCosNum;
        break;
    }
    case 5:
    {
        identifier="ComputeNodeSin";
        ComputeNodeSinNum++;
        controlsId=ComputeNodeSinNum;
        break;
    }
    case 6:
    {
        identifier="ComputeNodeTan";
        ComputeNodeTanNum++;
        controlsId=ComputeNodeTanNum;
        break;
    }
    case 7:
    {
        identifier="ComputeNodeLog";
        ComputeNodeLogNum++;
        controlsId=ComputeNodeLogNum;
        break;
    }
    case 8:
    {
        identifier="ComputeNodeE";
        ComputeNodeENum++;
        controlsId=ComputeNodeENum;
        break;
    }
    case 9:
    {
        identifier="ComputeNodeEqual";
        ComputeNodeEqualNum++;
        controlsId=ComputeNodeEqualNum;
        break;
    }
    case 10:
    {
        identifier="ComputeNodeMore";
        ComputeNodeMoreNum++;
        controlsId=ComputeNodeMoreNum;
        break;
    }
    case 11:
    {
        identifier="ComputeNodeLess";
        ComputeNodeLessNum++;
        controlsId=ComputeNodeLessNum;
        break;
    }
    default:
        break;
    }
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





