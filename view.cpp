/*******************************************************************
 * File:view.cpp
 * Author: Zhou Hengcheng
 * Description: set a new directed line when a triYuan is being clicked
******************************************************************/
#include <QtGui>

#include "view.h"
#include "yuan.h"
#include "link.h"

newview::newview()
{
    new_yuan=new Yuan;
    new_yuan->setOutlineColor(Qt::white);
    new_yuan->setBackgroundColor(Qt::white);

}

newview::~newview()
{
   delete new_yuan;
}

void newview::mouseMoveEvent(QMouseEvent *event)
{
   if((this->scene()->selectedItems().count()==1)&&dynamic_cast<triYuan *>(this->scene()->selectedItems().first())!=0)
   {
         new_yuan->setPos(event->x()-75,event->y());

         if(new_yuan->myLinks.size()>0)
         {
             foreach (Link* link,new_yuan->myLinks)
                 delete link;
         }
        Link* new_link=new Link(dynamic_cast<triYuan *>(this->scene()->selectedItems(). first()),new_yuan);
        new_link->setZValue(100);
        this->scene()->addItem(new_link);
        this->scene()->addItem(new_yuan);
   update();
   }
    QGraphicsView::mouseMoveEvent(event);
}

void newview::mouseReleaseEvent(QMouseEvent* event)
{
    if(new_yuan->myLinks.size()!=0)
    {
        foreach (Link* link,new_yuan->myLinks)
         delete link;

        if((this->scene()->selectedItems().count()==1)&&dynamic_cast<triYuan *>(this->scene()->selectedItems().first())!=0)
        {
           if(new_yuan->collidingItems().count()==1&&dynamic_cast<Yuan *>(new_yuan->collidingItems().first())!=0)
           {
               Link* new_link=new Link(dynamic_cast<triYuan *>(this->scene()->selectedItems(). first()),
                                       dynamic_cast<Yuan *>(new_yuan->collidingItems().first()));
               new_link->setZValue(100);
               this->scene()->addItem(new_link);

           }
        }

    }

    QGraphicsView::mouseReleaseEvent(event);
}



