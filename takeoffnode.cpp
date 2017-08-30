/*******************************************************************
 * File:takeoffnode.cpp
 * Author: Ryan Feng
 * Description: This file includes the realization of class 
 *        TakeoffNode, LandNode, TranslationNode and SomeNode.
******************************************************************/

#include<QObject>

#include "takeoffnode.h"
#include "yuan.h"
#include "link.h"
#include "diagramwindow.h"
#include "rec.h"


TakeOffNode::TakeOffNode()
{
    identifier="TakeOff";
    time=0;
    rank = 0;
    altitude = 0;
}

void TakeOffNode::setTime(double t)
{
    time=t;
}

void TakeOffNode::setAltitude(double a)
{
    altitude=a;
}

double TakeOffNode::myAltitude()
{
    return altitude;
}

double TakeOffNode::myTime()
{
    return time;
}

void TakeOffNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    double t = QInputDialog::getDouble(event->widget(),
                           tr("Edit Text"), tr("Enter new text:"),
                           QLineEdit::Normal, altitude);
    altitude = t;
    setText(tr("take off\n %1 m").arg(t));

}

QDataStream &operator <<(QDataStream &out,const TakeOffNode &node)
{
    out<<node.controlsId<<node.identifier<<node.zValue()
      <<node.pos()<<node.time<<node.text()
      <<node.textColor()<<node.outlineColor()
      <<node.backgroundColor();
    return out;
}

QDataStream &operator >>(QDataStream &in,TakeOffNode &node)
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

LandNode::LandNode()
{
    identifier="Land";
    rank = 0;
    time=0;
}

void LandNode::setTime(double t)
{
    time=t;
}

double LandNode::myTime()
{
    return time;
}

void LandNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    double t = QInputDialog::getDouble(event->widget(),
                           tr("Edit Time"), tr("Enter new time:"),
                           QLineEdit::Normal, time);
    time=t;
    setText(tr("Land on\n %1 s").arg(t));
}

QVariant LandNode::itemChange(GraphicsItemChange change,
                    const QVariant &value)
{if (change & ItemPositionHasChanged){
         if(this->collidingItems().isEmpty()||(this->collidingItems().count()==1&&dynamic_cast<Rec *>(this->collidingItems().first())!=0) )
       {
            yuan2->setPos(pos().x(),
                         pos().y() - outlineRect().height()/2-yuan->boundingRect().height()/2);
            foreach (Link *link, yuan2->myLinks)
            {link->trackYuans();update();}
            update();
       }
        else{
            setPos(yuan2->pos().x(),
                           yuan2->pos().y()+outlineRect().height()/2 +yuan->boundingRect().height()/2);
        }}
    return QGraphicsItem::itemChange(change, value);
    /*if (change & ItemPositionHasChanged) {
        yuan2->setPos(pos().x(),
                     pos().y() - outlineRect().height()/2-yuan->boundingRect().height()/2);
        foreach (Link *link, yuan2->myLinks)
        {link->trackYuans();update();}
    }
    return QGraphicsItem::itemChange(change, value);*/
}

// /////////////////////////////////////////////////////////////////////////////////////////////////////

int GoNode::riseNodeNum=0;      int GoNode::fallNodeNum=0;
int GoNode::advanceNodeNum=0;   int GoNode::backNodeNum=0;
int GoNode::rightNodeNum=0;     int GoNode::leftNodeNum=0;
GoNode::GoNode()
{
    box=new QComboBox;
    time=0;
    groundspeed = 0;

    identifier="Go";
    rank = 0;
    connect(box,SIGNAL(currentIndexChanged(int)),this,SLOT(setNewIdentifier()));
}

GoNode::~GoNode()
{
    delete  box;
}

void GoNode::setTime(double t)
{
    time=t;
}

double GoNode::myTime()
{
    return time;
}

void GoNode::setGroundSpeed(double s)
{
    groundspeed=s;
}

double GoNode::myGroundSpeed()
{
    return groundspeed;
}

void GoNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    double gs = QInputDialog::getDouble(event->widget(),
                           tr("Edit GroudSpeed"), tr("Enter new GroundSpeed:"),
                           QLineEdit::Normal, groundspeed);
    //double t = QInputDialog::getDouble(event->widget(),
                          // tr("Edit Time"), tr("Enter new time:"),
                          // QLineEdit::Normal, time);
    groundspeed=gs;
    setText(tr(" %1 m/s").arg(gs));
}

QVariant GoNode::itemChange(GraphicsItemChange change,
                    const QVariant &value)
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
                item->setPos(QPointF(pos().x()-40,
                             (pos().y() - outlineRect().height()/2 - item->boundingRect().height())));

           }
            else{
                setPos(yuan2->pos().x()+ outlineRect().width()/2 + yuan2->outlineRect().width()/2,
                               yuan2->pos().y());
            }}
        return QGraphicsItem::itemChange(change, value);

}

void GoNode::setDirection()
{
    int index=box->currentIndex();
    switch (index) {
    case 0:
    {
        direction="GoUp";
        riseNodeNum++;
        //controlsId=riseNodeNum;
        break;
    }
    case 1:
    {
        direction="GoDown";
        fallNodeNum++;
        //controlsId=fallNodeNum;
        break;
    }
    case 2:
    {
        direction="Forward";
        advanceNodeNum++;
        //controlsId=advanceNodeNum;
        break;
    }
    case 3:
    {
        direction="Backward";
        backNodeNum++;
        //controlsId=backNodeNum;
        break;
    }
    case 4:
    {
        direction="GoRight";
        rightNodeNum++;
        //controlsId=rightNodeNum;
        break;
    }
    case 5:
    {
        direction="GoLeft";
        leftNodeNum++;
        //controlsId=leftNodeNum;
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

                item->setPos(QPointF(pos().x()-40,
                             (pos().y() - outlineRect().height()/2 - item->boundingRect().height())));
               // update();
           }
            else{
                setPos(yuan2->pos().x()+ outlineRect().width()/2 + yuan2->outlineRect().width()/2,
                               yuan2->pos().y());
            }}
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

//=======================TurnNode=========================
TurnNode::TurnNode()
{
    box=new QComboBox;
    speed=0;
    time=0;
    angel=0;

    connect(box,SIGNAL(currentIndexChanged(int)),this,SLOT(setNewText()));
    connect(box,SIGNAL(currentIndexChanged(int)),this,SLOT(setNewIdentifier()));

    identifier="TurnLeftNode";
    rank = 0;
}

TurnNode::~TurnNode()
{
    delete box;
}

void TurnNode::setTime(double t)
{
    time=t;
}

double TurnNode::myTime()
{
    return time;
}

void TurnNode::setSpeed(double s)
{
    speed=s;
}

double TurnNode::mySpeed()
{
    return speed;
}

void TurnNode::setAngel(double a)
{
    angel=a;
}

double TurnNode::myAngel()
{
    return angel;
}

void TurnNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
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

QVariant TurnNode::itemChange(GraphicsItemChange change,
                    const QVariant &value)
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

                item->setPos(QPointF(pos().x()-40,
                             (pos().y() - outlineRect().height()/2 - item->boundingRect().height())));
               // update();
           }
            else{
                setPos(yuan2->pos().x()+ outlineRect().width()/2 + yuan2->outlineRect().width()/2,
                               yuan2->pos().y());
            }}
        return QGraphicsItem::itemChange(change, value);
}

void TurnNode::setNewText()
{
    int index=box->currentIndex();
    if(index==0||index==1)
    {
        setText(tr(" %1  \n %2 s").arg(angel).arg(time));
    }
    else
        setText(tr(" %1 s").arg(time));
}

void TurnNode::setDirection()
{
    int index=box->currentIndex();
    switch (index) {
    case 0:
    {
        direction="TurnLeft";
        break;
    }
    case 1:
    {
        direction="TurnRight";
        break;
    }
    default:
        break;
    }
}

//=========================HoverNode=======================
HoverNode::HoverNode()
{
    speed=0;
    time=0;
    angel=0;

    identifier="Hover";
    rank = 0;
}

HoverNode::~HoverNode()
{
    ;
}

void HoverNode::setTime(double t)
{
    time=t;
}

double HoverNode::myTime()
{
    return time;
}

void HoverNode::setSpeed(double s)
{
    speed=s;
}

double HoverNode::mySpeed()
{
    return speed;
}

void HoverNode::setAngel(double a)
{
    angel=a;
}

double HoverNode::myAngel()
{
    return angel;
}

void HoverNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

        double a = QInputDialog::getDouble(event->widget(),
                               tr("Edit Angel"), tr("Enter new angel:"),
                               QLineEdit::Normal, angel);
        double t = QInputDialog::getDouble(event->widget(),
                               tr("Edit Time"), tr("Enter new time:"),
                               QLineEdit::Normal, time);
        angel=a;time=t;
        setText(tr("%2 s").arg(t));
}

QVariant HoverNode::itemChange(GraphicsItemChange change,
                    const QVariant &value)
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

                //item->setPos(QPointF(pos().x()-40,
                             //(pos().y() - outlineRect().height()/2 - item->boundingRect().height())));
               // update();
           }
            else{
                setPos(yuan2->pos().x()+ outlineRect().width()/2 + yuan2->outlineRect().width()/2,
                               yuan2->pos().y());
            }}
        return QGraphicsItem::itemChange(change, value);
}

void HoverNode::setNewText()
{
    setText(tr(" %1  \n %2 s").arg(angel).arg(time));
}

//===============================DelayNode===========================
DelayNode::DelayNode()
{
    speed=0;
    time=0;
    angel=0;
    rank = 0;
    identifier="Delay";
}

DelayNode::~DelayNode()
{
    ;
}

void DelayNode::setTime(double t)
{
    time=t;
}

double DelayNode::myTime()
{
    return time;
}

void DelayNode::setSpeed(double s)
{
    speed=s;
}

double DelayNode::mySpeed()
{
    return speed;
}

void DelayNode::setAngel(double a)
{
    angel=a;
}

double DelayNode::myAngel()
{
    return angel;
}

void DelayNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

        double a = QInputDialog::getDouble(event->widget(),
                               tr("Edit Angel"), tr("Enter new angel:"),
                               QLineEdit::Normal, angel);
        double t = QInputDialog::getDouble(event->widget(),
                               tr("Edit Time"), tr("Enter new time:"),
                               QLineEdit::Normal, time);
        angel=a;time=t;
        setText(tr(" %1  \n %2 s").arg(a).arg(t));
}

QVariant DelayNode::itemChange(GraphicsItemChange change,
                    const QVariant &value)
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

                //item->setPos(QPointF(pos().x()-40,
                             //(pos().y() - outlineRect().height()/2 - item->boundingRect().height())));
               // update();
           }
            else{
                setPos(yuan2->pos().x()+ outlineRect().width()/2 + yuan2->outlineRect().width()/2,
                               yuan2->pos().y());
            }}
        return QGraphicsItem::itemChange(change, value);
}

void DelayNode::setNewText()
{
    setText(tr(" %1  \n %2 s").arg(angel).arg(time));
}




