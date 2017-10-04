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
#include "ionode.h"


TakeOffNode::TakeOffNode()
{
    identifier="TakeOff";
    rank = 0;
    altitude = 0;
}

void TakeOffNode::setAltitude(double a)
{
    if(isSelected()&&a!=myAltitude())
    {
        altitude=a;
        setText(tr("take off\n %1 m").arg(altitude));
    }
}

double TakeOffNode::myAltitude()
{
    return altitude;
}

void TakeOffNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    double t = QInputDialog::getDouble(event->widget(),
                           tr("Edit Text"), tr("Enter new text:"),
                           QLineEdit::Normal, altitude);
    altitude = t;
    setText(tr("take off\n %1 m").arg(t));
    emit altitudeChanged(altitude);

}

void TakeOffNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem *> items = this->collidingItems();
    int itemsCount = items.count();
    qDebug()<<itemsCount;
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
                    &&!dynamic_cast<QGraphicsProxyWidget*>(items[i])&&!dynamic_cast<ComputeSmallNode*>(items[i]))
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


// /////////////////////////////////////////////////////////////////////////////////////////////////////

LandNode::LandNode()
{
    identifier="Land";
    rank = 0;
}




void LandNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    ;
}

QVariant LandNode::itemChange(GraphicsItemChange change,
                    const QVariant &value)
{
    if (change & ItemPositionHasChanged){
         //if(this->collidingItems().isEmpty()||(this->collidingItems().count()==1&&dynamic_cast<Rec *>(this->collidingItems().first())!=0) )
       //{
            yuan2->setPos(pos().x(),
                         pos().y() - outlineRect().height()/2-yuan->boundingRect().height()/2);
            foreach (Link *link, yuan2->myLinks)
            {link->trackYuans();update();}
            update();
       /*}
        else{
            setPos(yuan2->pos().x(),
                           yuan2->pos().y()+outlineRect().height()/2 +yuan->boundingRect().height()/2);*/
        //}
    }
    return QGraphicsItem::itemChange(change, value);
    /*if (change & ItemPositionHasChanged) {
        yuan2->setPos(pos().x(),
                     pos().y() - outlineRect().height()/2-yuan->boundingRect().height()/2);
        foreach (Link *link, yuan2->myLinks)
        {link->trackYuans();update();}
    }
    return QGraphicsItem::itemChange(change, value);*/
}


void LandNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem *> items = this->collidingItems();
    int itemsCount = items.count();
    qDebug()<<itemsCount;
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
                    &&!dynamic_cast<QGraphicsProxyWidget*>(items[i])&&!dynamic_cast<ComputeSmallNode*>(items[i]))
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

// /////////////////////////////////////////////////////////////////////////////////////////////////////
GoNode::GoNode()
{
    box=new QComboBox;
    Time=0;
    groundspeed = 0.0;

    identifier="Go";
    rank = 0;
    connect(box,SIGNAL(currentIndexChanged(int)),this,SLOT(setDirection()));
}

GoNode::~GoNode()
{
    delete  box;
}

void GoNode::setTime(double t)
{
    if(isSelected()&&t!=myTime())
    {
        Time=t;
        setText(tr("%1 s").arg(Time));
    }
}

double GoNode::myTime()
{
    return Time;
}

void GoNode::setGroundSpeed(double s)
{
    if(isSelected()&&s!=myGroundSpeed())
    {
        groundspeed=s;
        setText(tr("%1 m/s").arg(groundspeed));
    }
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
    double t = QInputDialog::getDouble(event->widget(),
                           tr("Edit fly time"), tr("Enter new time:"),
                           QLineEdit::Normal, groundspeed);
    groundspeed=gs;
    Time = t;
    setText(tr(" %1 m/s \n %2 s").arg(gs).arg(t));
    emit groundSpeedChanged(groundspeed);
}

QVariant GoNode::itemChange(GraphicsItemChange change,
                    const QVariant &value)
{
    if (change & ItemPositionHasChanged){
            //if(this->collidingItems().isEmpty()||(this->collidingItems().count()==1&&dynamic_cast<Rec *>(this->collidingItems().first())!=0) )
           //{
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

           /*}
            else{
                setPos(yuan2->pos().x()+ outlineRect().width()/2 + yuan2->outlineRect().width()/2,
                               yuan2->pos().y());
            }*/
    }
        return QGraphicsItem::itemChange(change, value);
}

void GoNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem *> items = this->collidingItems();
    int itemsCount = items.count();
    qDebug()<<itemsCount;
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
                    &&!dynamic_cast<QGraphicsProxyWidget*>(items[i])&&!dynamic_cast<ComputeSmallNode*>(items[i]))
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

void GoNode::setDirection()
{
    int index=box->currentIndex();
    switch (index) {
    case 0:
    {
        direction="GoUp";
        break;
    }
    case 1:
    {
        direction="GoDown";
        break;
    }
    case 2:
    {
        direction="Forward";
        break;
    }
    case 3:
    {
        direction="Backward";
        break;
    }
    case 4:
    {
        direction="GoRight";
        break;
    }
    case 5:
    {
        direction="GoLeft";
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
    Angel=0;

    connect(box,SIGNAL(currentIndexChanged(int)),this,SLOT(setDirection()));

    identifier="TurnLeftNode";
    rank = 0;
}

TurnNode::~TurnNode()
{
    delete box;
}

void TurnNode::setAngel(double a)
{
    Angel=a;
}

double TurnNode::myAngel()
{
    return Angel;
}

void TurnNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    double a = QInputDialog::getDouble(event->widget(),
                           tr("Edit Angel"), tr("Enter new angel:"),
                           QLineEdit::Normal, Angel);
    setText(tr(" %1 ").arg(a));
    Angel=a;
}

QVariant TurnNode::itemChange(GraphicsItemChange change,
                    const QVariant &value)
{
    if (change & ItemPositionHasChanged){
         //if(this->collidingItems().isEmpty()||(this->collidingItems().count()==1&&dynamic_cast<Rec *>(this->collidingItems().first())!=0) )
         //  {
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
           /*}
            else{
                setPos(yuan2->pos().x()+ outlineRect().width()/2 + yuan2->outlineRect().width()/2,
                               yuan2->pos().y());
            }*/
    }
        return QGraphicsItem::itemChange(change, value);
}

void TurnNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem *> items = this->collidingItems();
    int itemsCount = items.count();
    qDebug()<<itemsCount;
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
                    &&!dynamic_cast<QGraphicsProxyWidget*>(items[i])&&!dynamic_cast<ComputeSmallNode*>(items[i]))
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
    time=0;
    identifier="Hover";
    rank = 0;
}

HoverNode::~HoverNode()
{
    ;
}

void HoverNode::setTime(double t)
{
    if(isSelected()&&t!=myTime())
    {
        time=t;
        setText(tr(" Hover \n %1 s").arg(time));
    }
}

double HoverNode::myTime()
{
    return time;
}

void HoverNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
        double t = QInputDialog::getDouble(event->widget(),
                               tr("Edit Time"), tr("Enter new time:"),
                               QLineEdit::Normal, time);
        time=t;
        setText(tr(" Hover \n %1 s").arg(t));
        emit timeChanged(time);
}

QVariant HoverNode::itemChange(GraphicsItemChange change,
                    const QVariant &value)
{
    if (change & ItemPositionHasChanged){
         //if(this->collidingItems().isEmpty()||(this->collidingItems().count()==1&&dynamic_cast<Rec *>(this->collidingItems().first())!=0) )
         //  {
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
           /*}
            else{
                setPos(yuan2->pos().x()+ outlineRect().width()/2 + yuan2->outlineRect().width()/2,
                               yuan2->pos().y());
            }*/
    }
        return QGraphicsItem::itemChange(change, value);
}

void HoverNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem *> items = this->collidingItems();
    int itemsCount = items.count();
    qDebug()<<itemsCount;
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
                    &&!dynamic_cast<QGraphicsProxyWidget*>(items[i])&&!dynamic_cast<ComputeSmallNode*>(items[i]))
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


//===============================DelayNode===========================
DelayNode::DelayNode()
{
    time=0;
    rank = 0;
    identifier="Delay";
}

DelayNode::~DelayNode()
{
    ;
}

void DelayNode::setTime(double t)
{
    if(isSelected()&&t!=myTime())
    {
        time=t;
        setText(tr(" Delay \n %1 s").arg(time));
    }
    time=t;
}

double DelayNode::myTime()
{
    return time;
}

void DelayNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
        double t = QInputDialog::getDouble(event->widget(),
                               tr("Edit Time"), tr("Enter new time:"),
                               QLineEdit::Normal, time);
        time=t;
        setText(tr(" Delay \n %1 s").arg(t));
        emit timeChanged(t);
}

QVariant DelayNode::itemChange(GraphicsItemChange change,
                    const QVariant &value)
{
    if (change & ItemPositionHasChanged){
         //if(this->collidingItems().isEmpty()||(this->collidingItems().count()==1&&dynamic_cast<Rec *>(this->collidingItems().first())!=0) )
         //  {
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
           /*}
            else{
                setPos(yuan2->pos().x()+ outlineRect().width()/2 + yuan2->outlineRect().width()/2,
                               yuan2->pos().y());
            }*/
    }
        return QGraphicsItem::itemChange(change, value);
}


void DelayNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem *> items = this->collidingItems();
    int itemsCount = items.count();
    qDebug()<<itemsCount;
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
                    &&!dynamic_cast<QGraphicsProxyWidget*>(items[i])&&!dynamic_cast<ComputeSmallNode*>(items[i]))
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

