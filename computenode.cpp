#include "node.h"
#include "yuan.h"
#include "link.h"
#include <QtGui>
#include "computenode.h"


int ComputeNode::ComputeNodeAddNum=0;  int ComputeNode::ComputeNodeSubNum=0;
int ComputeNode::ComputeNodeMulNum=0;  int ComputeNode::ComputeNodeDivNum=0;
int ComputeNode::ComputeNodeCosNum=0;  int ComputeNode::ComputeNodeSinNum=0;
int ComputeNode::ComputeNodeTanNum=0;  int ComputeNode::ComputeNodeLogNum=0;
int ComputeNode::ComputeNodeENum=0;    int ComputeNode::ComputeNodeEqualNum=0;
int ComputeNode::ComputeNodeMoreNum=0; int ComputeNode::ComputeNodeLessNum=0;

ComputeNode::ComputeNode()
{
    yuan = new triYuan(this);
    yuan2 = new Yuan(this);
    yuan3 = new Yuan(this);
    yuan2->setInout(1);
    yuan3->setInout(1);
    box = new QComboBox;

    identifier="ComputeNodeAdd";
}

ComputeNode::~ComputeNode()
{
    delete yuan2;
    delete yuan3;
    delete box;
}

QRectF ComputeNode::outlineRect() const
{
    QRectF rect(-20,-20,40,40);
    return rect;
}

QVariant ComputeNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change & ItemPositionHasChanged) {
        yuan->setPos(pos().x(),
                     pos().y() + outlineRect().height()/2 +yuan->boundingRect().height()/2);
        foreach (Link *link, yuan->myLinks)
        {link->trackYuans();update();}

        yuan2->setPos(pos().x() - outlineRect().width()/2 - yuan2->outlineRect().width()/2,
                     pos().y());
        foreach (Link *link, yuan2->myLinks)
        {link->trackYuans();update();}

        yuan3->setPos(pos().x() + outlineRect().width()/2 + yuan2->outlineRect().width()/2,
                     pos().y());
        foreach (Link *link, yuan3->myLinks)
        {link->trackYuans();update();}

        item->setPos(QPointF(pos().x()- item->boundingRect().width()/2,
                             pos().y() - outlineRect().height()/2 - item->boundingRect().height()/2));
    }
    return QGraphicsItem::itemChange(change, value);
}

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



