/*******************************************************************
 * File:takeoffnode.cpp
 * Author:
 * Desciption:This is a cpp file for developers, including many
 *     circumstances you may encounter during development.
******************************************************************/

#include<QObject>

#include "takeoffnode.h"
#include "yuan.h"
#include "link.h"
#include "diagramwindow.h"


TakeoffNode::TakeoffNode()
{
    time=0;
    identifier="TakeoffNode";
}

void TakeoffNode::setTime(double t)
{
    time=t;
}

double TakeoffNode::myTime()
{
    return time;
}

void TakeoffNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    double t = QInputDialog::getDouble(event->widget(),
                           tr("Edit Text"), tr("Enter new text:"),
                           QLineEdit::Normal, time);
    time=t;

    setText(tr("take off\n %1 s").arg(t));
}

QDataStream &operator <<(QDataStream &out,const TakeoffNode &node)
{
    out<<node.controlsId<<node.identifier<<node.zValue()
      <<node.pos()<<node.time<<node.text()
      <<node.textColor()<<node.outlineColor()
      <<node.backgroundColor();
    return out;
}

QDataStream &operator >>(QDataStream &in,TakeoffNode &node)
{
    int controlsId;
    QString identifier;
    double z;
    QPointF position;
    int time;
    QString text;
    QColor textColor;
    QColor outlineColor;
    QColor backgroundColor;

    in>>controlsId>>identifier>>z>>position>>time
      >>text>>textColor>>outlineColor>>backgroundColor;
    node.controlsId=controlsId;
    node.identifier=identifier;
    node.setZValue(z);
    node.setPos(position);
    node.setTime(time);
    node.setText(text);
    node.setOutlineColor(outlineColor);
    node.setBackgroundColor(backgroundColor);
    return in;

}


// /////////////////////////////////////////////////////////////////////////////////////////////////////

LandonNode::LandonNode()
{
    time=0;

    identifier="LandonNode";
}

void LandonNode::setTime(double t)
{
    time=t;
}

double LandonNode::myTime()
{
    return time;
}

void LandonNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    double t = QInputDialog::getDouble(event->widget(),
                           tr("Edit Time"), tr("Enter new time:"),
                           QLineEdit::Normal, time);
    time=t;
    setText(tr("Land on\n %1 s").arg(t));
}

QVariant LandonNode::itemChange(GraphicsItemChange change,
                    const QVariant &value)
{
    if (change & ItemPositionHasChanged) {
        yuan2->setPos(pos().x(),
                     pos().y() - outlineRect().height()/2-yuan->boundingRect().height()/2);
        foreach (Link *link, yuan2->myLinks)
        {link->trackYuans();update();}
    }
    return QGraphicsItem::itemChange(change, value);
}

// /////////////////////////////////////////////////////////////////////////////////////////////////////

int TranslationNode::riseNodeNum=0;      int TranslationNode::fallNodeNum=0;
int TranslationNode::advanceNodeNum=0;   int TranslationNode::backNodeNum=0;
int TranslationNode::rightNodeNum=0;     int TranslationNode::leftNodeNum=0;
TranslationNode::TranslationNode()
{
    box=new QComboBox;
    speed=0;
    time=0;

    identifier="RiseNode";

    connect(box,SIGNAL(currentIndexChanged(int)),this,SLOT(setNewIdentifier()));
}

TranslationNode::~TranslationNode()
{
    delete  box;
}

void TranslationNode::setTime(double t)
{
    time=t;
}

double TranslationNode::myTime()
{
    return time;
}

void TranslationNode::setSpeed(double s)
{
    speed=s;
}

double TranslationNode::mySpeed()
{
    return speed;
}

void TranslationNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    double s = QInputDialog::getDouble(event->widget(),
                           tr("Edit Speed"), tr("Enter new speed:"),
                           QLineEdit::Normal, speed);
    double t = QInputDialog::getDouble(event->widget(),
                           tr("Edit Time"), tr("Enter new time:"),
                           QLineEdit::Normal, time);
    speed=s;time=t;
    setText(tr(" %1 m/s \n %2 s").arg(s).arg(t));
}

QVariant TranslationNode::itemChange(GraphicsItemChange change,
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
        item->setPos(QPointF(pos().x()-40,
                     (pos().y() - outlineRect().height()/2 - item->boundingRect().height())));
    }
    return QGraphicsItem::itemChange(change, value);
}

void TranslationNode::setNewIdentifier()
{
    int index=box->currentIndex();
    switch (index) {
    case 0:
    {
        identifier="RiseNode";
        riseNodeNum++;
        controlsId=riseNodeNum;
        break;
    }
    case 1:
    {
        identifier="FallNode";
        fallNodeNum++;
        controlsId=fallNodeNum;
        break;
    }
    case 2:
    {
        identifier="AdvanceNode";
        advanceNodeNum++;
        controlsId=advanceNodeNum;
        break;
    }
    case 3:
    {
        identifier="BackNode";
        backNodeNum++;
        controlsId=backNodeNum;
        break;
    }
    case 4:
    {
        identifier="RightNode";
        rightNodeNum++;
        controlsId=rightNodeNum;
        break;
    }
    case 5:
    {
        identifier="LeftNode";
        leftNodeNum++;
        controlsId=leftNodeNum;
        break;
    }
    default:
        break;
    }
}

// ////////////////////////////////////////////////////////////////////////////////////////////////////

int SomeNode::turnLeftNodeNum=0;
int SomeNode::turnRightNodeNum=0;
int SomeNode::hangingNodeNum=0;
int SomeNode::delayNodeNum=0;
SomeNode::SomeNode()
{
    box=new QComboBox;
    speed=0;
    time=0;
    angel=0;

    connect(box,SIGNAL(currentIndexChanged(int)),this,SLOT(setNewText()));
    connect(box,SIGNAL(currentIndexChanged(int)),this,SLOT(setNewIdentifier()));

    identifier="TurnLeftNode";

}

SomeNode::~SomeNode()
{
    delete box;
}

void SomeNode::setTime(double t)
{
    time=t;
}

double SomeNode::myTime()
{
    return time;
}

void SomeNode::setSpeed(double s)
{
    speed=s;
}

double SomeNode::mySpeed()
{
    return speed;
}

void SomeNode::setAngel(double a)
{
    angel=a;
}

double SomeNode::myAngel()
{
    return angel;
}

void SomeNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    int index=box->currentIndex();
    switch (index) {
    case 0:
    case 1:
    {
        double a = QInputDialog::getDouble(event->widget(),
                               tr("Edit Angel"), tr("Enter new angel:"),
                               QLineEdit::Normal, angel);
        double t = QInputDialog::getDouble(event->widget(),
                               tr("Edit Time"), tr("Enter new time:"),
                               QLineEdit::Normal, time);
        angel=a;time=t;
        setText(tr(" %1  \n %2 s").arg(a).arg(t));
        break;
    }
    case 2:
    case 3:
    {
        double t = QInputDialog::getDouble(event->widget(),
                               tr("Edit Time"), tr("Enter new time:"),
                               QLineEdit::Normal, time);
        time=t;
        setText(tr(" %1 s").arg(t));
        break;
    }
    default:
        break;
    }
}

QVariant SomeNode::itemChange(GraphicsItemChange change,
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

        item->setPos(QPointF(pos().x()-40,
                     (pos().y() - outlineRect().height()/2 - item->boundingRect().height())));
    }
    return QGraphicsItem::itemChange(change, value);
}

void SomeNode::setNewText()
{
    int index=box->currentIndex();
    if(index==0||index==1)
    {
        setText(tr(" %1  \n %2 s").arg(angel).arg(time));
    }
    else
        setText(tr(" %1 s").arg(time));
}

void SomeNode::setNewIdentifier()
{
    int index=box->currentIndex();
    switch (index) {
    case 0:
    {
        identifier="TurnLeftNode";
        turnLeftNodeNum++;
        controlsId=turnLeftNodeNum;
        break;
    }
    case 1:
    {
        identifier="TurnRightNode";
        turnRightNodeNum++;
        controlsId=turnRightNodeNum;
        break;
    }
    case 2:
    {
        identifier="HangingNode";
        hangingNodeNum++;
        controlsId=hangingNodeNum;
        break;
    }
    case 3:
    {
        identifier="DelayNode";
        delayNodeNum++;
        controlsId=delayNodeNum;
        break;
    }
    default:
        break;
    }
}







